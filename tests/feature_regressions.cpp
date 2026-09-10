#include <cassert>
#include <iostream>
#include <limits>
#include <Windows.h>
static SHORT test_keys[256]{};
static ULONGLONG test_time = 1000;
static unsigned mouse_downs = 0, mouse_ups = 0;
static HMODULE test_module = nullptr;
static HMODULE TestModule(const char*) { return test_module; }
static SHORT TestKey(int key) { return key >= 0 && key < 256 ? test_keys[key] : 0; }
static ULONGLONG TestTime() { return test_time; }
static void TestMouse(DWORD flags, DWORD, DWORD, DWORD, ULONG_PTR) {
    if (flags & MOUSEEVENTF_LEFTDOWN) ++mouse_downs;
    if (flags & MOUSEEVENTF_LEFTUP) ++mouse_ups;
}
#define GetAsyncKeyState TestKey
#define GetTickCount64 TestTime
#define mouse_event TestMouse
#define GetModuleHandleA TestModule
#include "../manager/core/features.cpp"
#undef GetAsyncKeyState
#undef GetTickCount64
#undef mouse_event
#undef GetModuleHandleA
#include "../manager/core/skins.cpp"

// Exercise production feature code against owned buffers; no game or input injection.
namespace game_state {
Snapshot test_state{};
Snapshot GetSnapshot() { return test_state; }
bool IsInGame() { return test_state.in_game; }
uintptr_t GetEntityList() { return test_state.entity_list; }
uintptr_t GetLocalController() { return test_state.controller; }
uintptr_t GetLocalPawnRaw() { return test_state.pawn; }
sdk::C_CSPlayerPawn* GetLocalPawn() { return reinterpret_cast<sdk::C_CSPlayerPawn*>(test_state.pawn); }
}
namespace sdk { std::uint8_t* find_pattern(const char*, const char*) { return nullptr; } }
namespace debug_console {
Console& Console::Get() { static Console instance; return instance; }
void Console::Info(const char*, ...) {}
void Console::Success(const char*, ...) {}
void Console::Warning(const char*, ...) {}
void Console::Error(const char*, ...) {}
void Console::Debug(const char*, ...) {}
}

struct Buffer {
    std::vector<unsigned char> bytes;
    explicit Buffer(size_t n) : bytes(n) {}
    uintptr_t addr() { return reinterpret_cast<uintptr_t>(bytes.data()); }
    template<class T> void put(size_t offset, T value) {
        assert(offset + sizeof(T) <= bytes.size());
        std::memcpy(bytes.data() + offset, &value, sizeof(value));
    }
};
static unsigned refreshes = 0, attributes = 0;
static uintptr_t last_refresh_owner = 0;
static void __fastcall Refresh(void* owner, bool) { ++refreshes; last_refresh_owner = reinterpret_cast<uintptr_t>(owner); }
static void __fastcall Attribute(void*, const char*, float) { ++attributes; }
static bool AlmostEqual(float a, float b) { return std::abs(a-b) < 0.001f; }

int main() {
    const float inf = std::numeric_limits<float>::infinity();
    assert(AlmostEqual(features::CalcAngle({}, {1, 0, 0}).y, 0));
    assert(AlmostEqual(features::CalcAngle({}, {0, 1, 0}).y, 90));
    assert(AlmostEqual(std::abs(features::CalcAngle({}, {-1, 0, 0}).y), 180));
    assert(AlmostEqual(features::CalcAngle({}, {0, -1, 0}).y, -90));
    assert(AlmostEqual(features::CalcAngle({}, {1, 0, 1}).x, -45));
    assert(AlmostEqual(features::GetFovA({0, 179, 0}, {0, -179, 0}), 2));
    assert(std::isinf(features::GetFovA({0, inf, 0}, {})));
    sdk::QAngle angle{270, 721, 10}; features::ClampAnglesA(angle);
    assert(angle.x == 89 && AlmostEqual(angle.y, 1) && angle.z == 0);
    sdk::ViewMatrix matrix{}; matrix.matrix[0][0]=matrix.matrix[1][1]=matrix.matrix[3][3]=1;
    sdk::Vector2 screen{};
    assert(features::WorldToScreen({}, screen, matrix, 800, 600));
    assert(screen.x == 400 && screen.y == 300);
    assert(!features::WorldToScreen({inf, 0, 0}, screen, matrix, 800, 600));
    assert(!features::WorldToScreen({}, screen, matrix, 0, 600));
    matrix.matrix[3][3]=-1;
    assert(!features::WorldToScreen({}, screen, matrix, 800, 600));
    uint32_t mask[2]{0x80000000u, 0x80000001u};
    assert(sdk::spotted_by(mask,32) && sdk::spotted_by(mask,33) && sdk::spotted_by(mask,64));
    assert(!sdk::spotted_by(mask,31) && !sdk::spotted_by(mask,0) && !sdk::spotted_by(mask,65));

    Buffer system(0x100), chunk0(0x70*512), chunk1(0x70*512);
    Buffer pawn(0x5000), first(0x5000), second(0x5000), identity1(0x50), identity2(0x50);
    system.put(0x10,chunk0.addr()); system.put(0x18,chunk1.addr());
    chunk0.put(0x70*511,first.addr()); chunk1.put(0,second.addr());
    const uint32_t handle1=511 | (3u<<15), handle2=512 | (4u<<15);
    first.put(0x10,identity1.addr()); second.put(0x10,identity2.addr());
    identity1.put(0x10,handle1); identity2.put(0x10,handle2);
    assert(sdk::entity_from_handle(system.addr(),handle1)==first.addr());
    assert(sdk::entity_from_handle(system.addr(),handle2)==second.addr());
    assert(!sdk::entity_from_handle(system.addr(),handle1+(1u<<15)));
    assert(!sdk::entity_from_handle(system.addr(),UINT32_MAX));
    assert(!sdk::entity_at(system.addr(),-1));

    Buffer services(0x100), handles(8), vec(24);
    handles.put(0,handle1); handles.put(4,handle2);
    vec.put(0,2); vec.put(8,handles.addr()); vec.put(16,2);
    sdk::VectorView view{};
    assert(sdk::read_vector(vec.addr(),view,64) && view.count==2 && view.data==handles.addr());
    vec.put(0,65); assert(!sdk::read_vector(vec.addr(),view,64));
    vec.put(0,2); vec.put(16,1); assert(!sdk::read_vector(vec.addr(),view,64));
    vec.put(16,2); vec.put<uintptr_t>(8,0); assert(!sdk::read_vector(vec.addr(),view,64));

    auto* pages = static_cast<unsigned char*>(VirtualAlloc(nullptr,8192,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));
    assert(pages); DWORD previous=0;
    assert(VirtualProtect(pages+4096,4096,PAGE_NOACCESS,&previous));
    uint64_t sentinel=42;
    assert(!sdk::read_memory(reinterpret_cast<uintptr_t>(pages+4092),sentinel) && sentinel==42);
    assert(!sdk::write_memory(reinterpret_cast<uintptr_t>(pages+4092),sentinel));
    assert(!sdk::write_memory(reinterpret_cast<uintptr_t>(pages+4096),sentinel));
    VirtualFree(pages,0,MEM_RELEASE);

    sdk::CUserCmd cmd{};
    assert(!cmd.GetInputHistoryEntry(-1) && !cmd.GetInputHistoryEntry(0));
    assert(!cmd.IsButtonPressed(sdk::IN_ATTACK));
    cmd.SetSubTickAngle({1,2,0});
    sdk::CMsgQAngle msg{}; sdk::CCSGOInputHistoryEntryPB entry{}; entry.pViewAngles=&msg;
    sdk::RepeatedPtrField_t<sdk::CCSGOInputHistoryEntryPB>::Rep_t rep{};
    rep.nAllocatedSize=1; rep.tElements[0]=&entry;
    cmd.csgoUserCmd.inputHistoryField.pRep=&rep; cmd.csgoUserCmd.inputHistoryField.nCurrentSize=1;
    assert(cmd.GetInputHistoryEntry(0)==&entry && !cmd.GetInputHistoryEntry(1));
    cmd.SetSubTickAngle({3,4,0}); assert(msg.angValue.x==3 && msg.angValue.y==4);
    cmd.SetSubTickAngle({inf,4,0}); assert(msg.angValue.x==3);

    using namespace cs2_dumper::schemas::client_dll;
    pawn.put(cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth,100);
    pawn.put(C_BasePlayerPawn::m_pWeaponServices,services.addr());
    services.put(CPlayer_WeaponServices::m_hMyWeapons,2);
    services.put(CPlayer_WeaponServices::m_hMyWeapons+8,handles.addr());
    services.put(CPlayer_WeaponServices::m_hMyWeapons+16,2);
    services.put(CPlayer_WeaponServices::m_hActiveWeapon,handle1);
    first.put<uint16_t>(skins::OFF_ITEM_DEF_INDEX,7); second.put<uint16_t>(skins::OFF_ITEM_DEF_INDEX,9);
    game_state::test_state={true,system.addr(),0,pawn.addr()};
    skins::g_engine_funcs_resolved=true; skins::s_engineFunctionsResolved=true;
    skins::g_fnUpdateComposite=Refresh; skins::g_compositeOwnerOffset=0x608; skins::g_fnSetAttribute=Attribute;
    skins::PlayerSkinConfig cfg; cfg.paint_kit=282; cfg.seed=12; cfg.wear=0.1f;
    cfg.stattrak=true; cfg.stattrak_count=321; cfg.name_tag="regression";
    skins::user_skins[7]=cfg; skins::user_skins[9]=cfg;
    skins::SetCurrentFrameStage(7); skins::ApplyAllSkins(); assert(refreshes==0);
    skins::SetCurrentFrameStage(6); skins::ApplyAllSkins();
    assert(refreshes==2 && attributes==6); // Includes the inactive weapon.
    assert(skins::GetActiveWeapon()==first.addr());
    auto weapon_reads=skins::GetWeaponReadDiagnostics();
    assert(weapon_reads.services==services.addr() && weapon_reads.count==2 && weapon_reads.active==first.addr());
    pawn.put<uintptr_t>(C_BasePlayerPawn::m_pWeaponServices,0);
    assert(!skins::GetActiveWeapon() && skins::GetAllWeapons().empty());
    assert(std::strstr(skins::GetWeaponReadDiagnostics().status,"Weapon-services pointer"));
    pawn.put(C_BasePlayerPawn::m_pWeaponServices,services.addr());
    services.put(CPlayer_WeaponServices::m_hMyWeapons+16,1);
    assert(skins::GetActiveWeapon()==first.addr()); // Active handle is independent of vector validity.
    assert(std::strstr(skins::GetWeaponReadDiagnostics().status,"invalid bounds"));
    services.put(CPlayer_WeaponServices::m_hMyWeapons+16,2);
    Buffer aim_punch(0x200);
    pawn.put(cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_pAimPunchServices,aim_punch.addr());
    aim_punch.put(CCSPlayer_AimPunchServices::m_predictableBaseAngle,sdk::Vector3(1,2,0));
    assert(features::ReadAimPunch(pawn.addr()).y==2);
    assert(sdk::read_value<int>(first.addr()+skins::OFF_FALLBACK_STATTRAK)==321);
    assert(sdk::read_value<uint32_t>(second.addr()+skins::OFF_ITEM_ID_HIGH)==UINT32_MAX);
    assert(std::strcmp(reinterpret_cast<char*>(first.addr()+skins::OFF_CUSTOM_NAME),"regression")==0);
    skins::ApplyAllSkins(); assert(refreshes==2 && attributes==6);
    skins::user_skins[7].wear=inf; skins::user_skins[7].stattrak=false;
    skins::user_skins[7].name_tag=std::string(200,'x');
    skins::ApplyAllSkins(); assert(refreshes==3);
    assert(last_refresh_owner==first.addr()+0x608);
    assert(sdk::read_value<float>(first.addr()+skins::OFF_FALLBACK_WEAR)==0);
    assert(sdk::read_value<int>(first.addr()+skins::OFF_FALLBACK_STATTRAK)==-1);
    assert(sdk::read_value<char>(first.addr()+skins::OFF_CUSTOM_NAME+160)==0);
    skins::user_skins.erase(7);
    skins::ApplyAllSkins(); assert(refreshes==4);
    assert(sdk::read_value<int>(first.addr()+skins::OFF_FALLBACK_PAINT)==0);
    assert(sdk::read_value<uint32_t>(first.addr()+skins::OFF_ITEM_ID_HIGH)==0);
    assert(sdk::read_value<char>(first.addr()+skins::OFF_CUSTOM_NAME)==0);
    skins::ApplyAllSkins(); assert(refreshes==4);

    skins::glove_database.emplace_back(10018,"Superconductor",5030);
    skins::selected_glove_kit=10018;
    unsigned before=attributes;
    skins::SetCurrentFrameStage(7); skins::ApplyGloves(); assert(attributes==before);
    skins::SetCurrentFrameStage(6); skins::ApplyGloves(); assert(attributes==before+3);
    auto glove=pawn.addr()+cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_EconGloves;
    assert(sdk::read_value<uint16_t>(glove+C_EconItemView::m_iItemDefinitionIndex)==5030);
    skins::ApplyGloves(); assert(attributes==before+3);

    pawn.put<uint8_t>(cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum,2);
    first.put(cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth,150);
    second.put(cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth,100);
    first.put<uint8_t>(cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum,3);
    second.put<uint8_t>(cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum,3);
    pawn.put(cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_iIDEntIndex,511);
    config::misc::trigger_bot=true; config::misc::trigger_key=VK_SHIFT; config::misc::trigger_delay=0.1f;
    features::SetInputBlocked(false);
    features::TriggerBot(); assert(mouse_downs==0); // Key is required.
    test_keys[VK_SHIFT]=static_cast<SHORT>(0x8000);
    features::TriggerBot(); test_time+=90; features::TriggerBot(); assert(mouse_downs==0);
    pawn.put(cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_iIDEntIndex,512);
    features::TriggerBot(); test_time+=20; features::TriggerBot(); assert(mouse_downs==0); // Target change resets delay.
    test_time+=90; features::TriggerBot(); assert(mouse_downs==1);
    features::SetInputBlocked(true); features::TriggerBot(); assert(mouse_ups==1);
    test_time+=200; features::TriggerBot(); assert(mouse_downs==1);

    Buffer controller(0x1000);
    Buffer scene(0x300);
    first.put(cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode,scene.addr());
    controller.put(CCSPlayerController::m_hPlayerPawn,handle1);
    chunk0.put(0x70*64,controller.addr());
    config::esp::enabled=false; config::misc::radar_hack=true;
    features::RadarHack(); // Includes controller 64, independently of ESP.
    assert(sdk::read_value<bool>(first.addr()+cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState+
        EntitySpottedState_t::m_bSpotted));
    config::misc::no_flash=true;
    pawn.put(cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_flFlashDuration,4.0f);
    features::NoFlash();
    assert(sdk::read_value<float>(pawn.addr()+cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_flFlashDuration)==0);

    const auto jumpOffset=cs2_dumper::buttons::jump;
    const auto jumpPage=static_cast<uintptr_t>(jumpOffset)&~uintptr_t{4095};
    auto* module=static_cast<unsigned char*>(VirtualAlloc(nullptr,jumpPage+4096,MEM_RESERVE,PAGE_READWRITE));
    assert(module && VirtualAlloc(module+jumpPage,4096,MEM_COMMIT,PAGE_READWRITE));
    test_module=reinterpret_cast<HMODULE>(module);
    features::SetInputBlocked(false); config::misc::bunny_hop=true;
    test_keys[VK_SPACE]=static_cast<SHORT>(0x8000);
    pawn.put<uint32_t>(cs2_dumper::schemas::client_dll::C_BaseEntity::m_fFlags,1);
    features::BunnyHop(); assert(sdk::read_value<uint32_t>(reinterpret_cast<uintptr_t>(module)+jumpOffset)==65537);
    pawn.put<uint32_t>(cs2_dumper::schemas::client_dll::C_BaseEntity::m_fFlags,0);
    features::BunnyHop(); assert(sdk::read_value<uint32_t>(reinterpret_cast<uintptr_t>(module)+jumpOffset)==256);
    pawn.put<uint32_t>(cs2_dumper::schemas::client_dll::C_BaseEntity::m_fFlags,1);
    features::BunnyHop(); features::ReleaseInputs();
    assert(sdk::read_value<uint32_t>(reinterpret_cast<uintptr_t>(module)+jumpOffset)==256);
    test_module=nullptr; VirtualFree(module,0,MEM_RELEASE);

    Buffer clientMemory(cs2_dumper::offsets::client_dll::dwViewAngles+0x100);
    Buffer localScene(0x300), bones(256*32);
    test_module=reinterpret_cast<HMODULE>(clientMemory.addr());
    pawn.put(cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode,localScene.addr());
    pawn.put(cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset+0x20,64.0f);
    scene.put(CSkeletonInstance::m_modelState+0x80,bones.addr());
    bones.put(6*32,sdk::Vector3(100,100,64));
    config::aimbot::enabled=true; config::aimbot::silent_aim=false;
    config::aimbot::visible_check=false; config::aimbot::team_check=true;
    config::aimbot::fov=90; config::aimbot::max_distance=10000; config::aimbot::smoothing=0;
    config::aimbot::auto_shoot=false; config::rcs::enabled=false;
    test_keys[VK_LBUTTON]=static_cast<SHORT>(0x8000);
    features::RunNormalAimTick();
    auto angles=sdk::read_value<sdk::Vector2>(clientMemory.addr()+cs2_dumper::offsets::client_dll::dwViewAngles);
    assert(std::abs(angles.x)<0.001f && std::abs(angles.y-45.0f)<0.001f);
    assert(features::normal_aim_writes==1);
    test_keys[VK_LBUTTON]=0;
    features::RunNormalAimTick(); assert(features::normal_aim_writes==1);
    config::aimbot::silent_aim=true;
    DWORD history[7]{};
    features::RunSilentAimSubTick(history,reinterpret_cast<sdk::C_CSPlayerPawn*>(pawn.addr()));
    angles=sdk::read_value<sdk::Vector2>(reinterpret_cast<uintptr_t>(history+4));
    assert(std::abs(angles.x)<0.001f && std::abs(angles.y-45.0f)<0.001f);
    assert(features::silent_aim_writes==1);
    test_module=nullptr;

    Buffer inventory(0x200), slots(2*0xC8);
    controller.put(skins::OFF_INV_SERVICES,inventory.addr());
    inventory.put(skins::OFF_LOADOUT_VEC,2);
    inventory.put(skins::OFF_LOADOUT_VEC+8,slots.addr());
    inventory.put(skins::OFF_LOADOUT_VEC+16,2);
    slots.put(0,first.addr()+skins::ECON_ITEM_VIEW_BASE);
    slots.put(0xC8,second.addr()+skins::ECON_ITEM_VIEW_BASE);
    game_state::test_state.controller=controller.addr();
    auto loadout=skins::ReadLoadout();
    assert(loadout.size()==2 && loadout[0].def_index==7 && loadout[1].def_index==9);
    std::cout << "Feature regressions passed: math/projection, masks/handles/vectors, protected memory, user commands, staged weapons/gloves/removal, trigger timing/menu release, radar/no-flash, bunny-hop/release, and loadout stride.\n";
}
