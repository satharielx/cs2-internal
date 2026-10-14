#include <Windows.h>
#include <cassert>
static unsigned recoilWrites=0;
static BOOL RecoilWrite(HANDLE process, LPVOID address, LPCVOID data, SIZE_T size, SIZE_T* written) {
    ++recoilWrites; return WriteProcessMemory(process,address,data,size,written);
}
#define WriteProcessMemory RecoilWrite
#define main FeatureRegressionMain
#include "feature_regressions.cpp"
#undef main
#undef WriteProcessMemory
int main() {
    Buffer client(cs2_dumper::offsets::client_dll::dwViewAngles+64), pawn(0x5000), punch(0x1000), nextPawn(0x5000);
    test_module=reinterpret_cast<HMODULE>(client.addr());
    const auto view=cs2_dumper::offsets::client_dll::dwViewAngles;
    game_state::test_state={}; game_state::test_state.in_game=true; game_state::test_state.pawn=pawn.addr();
    config::aimbot::enabled=false; config::rcs::enabled=true; config::rcs::strength=1;
    pawn.put(sdk::off::C_BaseEntity::m_iHealth,100);
    pawn.put(sdk::off::C_CSPlayerPawn::m_pAimPunchServices,punch.addr());
    pawn.put(features::rcs_m_iShotsFired,1);
    punch.put(sdk::off::CCSPlayer_AimPunchServices::m_predictableBaseAngle,sdk::Vector3{1,2,0});
    client.put(view,sdk::Vector2{10,20});
    // No entity list exists: standalone RCS must never enumerate other players.
    recoilWrites=0; features::RunNormalAimTick();
    auto angles=sdk::read_value<sdk::Vector2>(client.addr()+view);
    assert(angles.x==8 && angles.y==16 && recoilWrites==1);
    recoilWrites=0; for(int i=0;i<1000;++i) features::RunNormalAimTick();
    assert(recoilWrites==0);
    punch.put(sdk::off::CCSPlayer_AimPunchServices::m_predictableBaseAngle,sdk::Vector3{1.5f,3,0});
    features::RunNormalAimTick(); angles=sdk::read_value<sdk::Vector2>(client.addr()+view);
    assert(angles.x==7 && angles.y==14);
    config::rcs::enabled=false; features::RunNormalAimTick();
    config::rcs::enabled=true; client.put(view,sdk::Vector2{10,20});
    features::RunNormalAimTick(); angles=sdk::read_value<sdk::Vector2>(client.addr()+view);
    assert(angles.x==7 && angles.y==14); // Enabling starts a new recoil baseline.
    nextPawn.put(sdk::off::C_BaseEntity::m_iHealth,100);
    nextPawn.put(sdk::off::C_CSPlayerPawn::m_pAimPunchServices,punch.addr());
    nextPawn.put(features::rcs_m_iShotsFired,1);
    game_state::test_state.pawn=nextPawn.addr(); client.put(view,sdk::Vector2{10,20});
    features::RunNormalAimTick(); angles=sdk::read_value<sdk::Vector2>(client.addr()+view);
    assert(angles.x==7 && angles.y==14);
    nextPawn.put(features::rcs_m_iShotsFired,0); recoilWrites=0; features::RunNormalAimTick(); assert(recoilWrites==0);
    std::cout<<"RCS regressions passed: no target enumeration, 1000 unchanged updates produce zero writes, baseline reset\n";
}
