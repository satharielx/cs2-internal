#include <Windows.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <vector>
#include <atomic>
static std::atomic<unsigned long long> read_calls{0},write_calls{0},query_calls{0};
static BOOL CountRead(HANDLE process,LPCVOID address,LPVOID target,SIZE_T bytes,SIZE_T* count) {
    ++read_calls;return ReadProcessMemory(process,address,target,bytes,count);
}
static BOOL CountWrite(HANDLE process,LPVOID address,LPCVOID source,SIZE_T bytes,SIZE_T* count) {
    ++write_calls;return WriteProcessMemory(process,address,source,bytes,count);
}
static SIZE_T CountQuery(LPCVOID address,PMEMORY_BASIC_INFORMATION info,SIZE_T size) {
    ++query_calls;return VirtualQuery(address,info,size);
}
static ULONGLONG clock_value=1000;
static ULONGLONG TestClock() {return clock_value;}
#define GetTickCount64 TestClock
#define ReadProcessMemory CountRead
#define WriteProcessMemory CountWrite
#define VirtualQuery CountQuery
#include "../manager/core/chams.cpp"
#undef GetTickCount64
#undef ReadProcessMemory
#undef WriteProcessMemory
#undef VirtualQuery

namespace game_state {
Snapshot test_state{};
Snapshot GetSnapshot() {return test_state;}
}
namespace sdk {std::uint8_t* find_pattern(const char*,const char*) {return nullptr;}}
struct Buffer {
    std::vector<unsigned char> bytes;
    explicit Buffer(size_t size):bytes(size) {}
    uintptr_t addr() {return reinterpret_cast<uintptr_t>(bytes.data());}
    template<class T> void put(size_t offset,T value) {assert(offset+sizeof(T)<=bytes.size());std::memcpy(bytes.data()+offset,&value,sizeof(value));}
};
static unsigned creates=0,cleans=0;
static bool hidden=false, load_success=true;
static uintptr_t material_storage[8]{};
static void* material_binding=material_storage;
static std::string definition,resource;
static void* __fastcall Construct(void* arena,int mode) {assert(mode==0);return arena;}
static void* __fastcall Root(void* arena) {return static_cast<unsigned char*>(arena)+0x78;}
static void __fastcall Cleanup(void*) {++cleans;}
static bool __cdecl Load(void*,void*,const char* text,const chams::detail::Api::Format& format,const char*,unsigned flags) {
    definition=text;
    assert(flags==0 && std::strcmp(format.name,"generic")==0 && format.first==0x469806E97412167CULL);
    assert(definition.find("csgo_character.vfx")!=std::string::npos);
    assert(definition.find("F_TRANSLUCENT")==std::string::npos);
    hidden=definition.find("F_DISABLE_Z_BUFFERING")!=std::string::npos;
    return load_success;
}
static void* __fastcall Create(void*,void*** output,const char* name,void*,uint32_t flags,bool immediate) {
    assert(flags==0 && immediate);resource=name;++creates;*output=&material_binding;return output;
}
static uintptr_t __fastcall Forward(uintptr_t a,uintptr_t b,uintptr_t c,uintptr_t d) {
    assert(a==1 && b==2 && c==3 && d==4);return 0x123456789ABCDEF0ULL;
}
static std::promise<void>* draw_entered=nullptr;
static std::shared_future<void> draw_release;
static uintptr_t __fastcall BlockingDraw(uintptr_t,uintptr_t,uintptr_t,uintptr_t) {
    draw_entered->set_value();draw_release.wait();return 42;
}
static void TestRtti() {
    auto* memory=static_cast<unsigned char*>(VirtualAlloc(nullptr,0x4000,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));
    assert(memory);
    const auto address=reinterpret_cast<uintptr_t>(memory);
    auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(memory);dos->e_magic=IMAGE_DOS_SIGNATURE;dos->e_lfanew=0x80;
    auto* nt=reinterpret_cast<IMAGE_NT_HEADERS64*>(memory+0x80);
    nt->Signature=IMAGE_NT_SIGNATURE;nt->FileHeader.NumberOfSections=1;
    nt->FileHeader.SizeOfOptionalHeader=sizeof(IMAGE_OPTIONAL_HEADER64);
    nt->OptionalHeader.Magic=IMAGE_NT_OPTIONAL_HDR64_MAGIC;nt->OptionalHeader.SizeOfImage=0x4000;
    auto* section=IMAGE_FIRST_SECTION(nt);
    section->VirtualAddress=0x1000;section->SizeOfRawData=0x1000;section->Misc.VirtualSize=0x1000;
    section->Characteristics=IMAGE_SCN_CNT_INITIALIZED_DATA;
    const char type[]=".?AVRegressionDesc@@";
    std::memcpy(memory+0x1110,type,sizeof(type));
    uint32_t locator[]={1,0,0,0x1100,0,0x1200};std::memcpy(memory+0x1200,locator,sizeof(locator));
    const uintptr_t pointer=address+0x1200;std::memcpy(memory+0x1300,&pointer,8);
    sdk::rtti::Image image(reinterpret_cast<HMODULE>(memory));
    assert(image.Vtable(type)==address+0x1308);
    assert(image.Vtable(".?AVMissing@@")==0 && image.Contains(address+0x1000) && !image.Contains(address+0x4000));
    VirtualFree(memory,0,MEM_RELEASE);
}
int main() {
    using namespace chams;
    using namespace chams::detail;
    TestRtti();
    assert(Initialize()==0); // Missing game modules must leave startup optional.
    api={reinterpret_cast<void*>(1),Construct,Root,Cleanup,Load,Create};
    stopping=false;faulted=false;hook_ready=true;
    assert(Draw(Forward,1,2,3,4)==0x123456789ABCDEF0ULL);
    const auto old_reads=read_calls.load(),old_writes=write_calls.load(),old_queries=query_calls.load();
    const auto draw_start=std::chrono::steady_clock::now();
    for(int i=0;i<20000;++i) assert(Draw(Forward,1,2,3,4)==0x123456789ABCDEF0ULL);
    assert(read_calls==old_reads && write_calls==old_writes && query_calls==old_queries);
    std::cout<<"Chams 20,000 draw callbacks: "<<std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-draw_start).count()
        <<" ms, zero memory API calls\n";
    Buffer system(0x100),chunk(512*0x70),local(0x5000),controller(0x1000),pawn(0x5000),identity(0x80);
    Buffer node(0x400),link(0x80),object(0x200),holder(0x500),desc(16);
    const uint32_t handle=128|(7u<<15);
    system.put(0x10,chunk.addr());chunk.put(0x70,controller.addr());chunk.put(128*0x70,pawn.addr());
    controller.put(sdk::off::CCSPlayerController::m_hPlayerPawn,handle);
    pawn.put(0x10,identity.addr());identity.put(0x10,handle);
    pawn.put(sdk::off::C_BaseEntity::m_iHealth,100);
    pawn.put<uint8_t>(sdk::off::C_BaseEntity::m_iTeamNum,3);
    local.put<uint8_t>(sdk::off::C_BaseEntity::m_iTeamNum,2);
    pawn.put(sdk::off::C_BaseEntity::m_pGameSceneNode,node.addr());
    node.put(sdk::off::CSkeletonInstance::m_modelState+0xD8,link.addr());link.put(0x20,object.addr());
    object_table=0x1000;desc_table=0x2000;
    object.put(0,object_table);object.put(holder_offset,holder.addr());desc.put(0,desc_table);
    game_state::test_state={true,system.addr(),controller.addr(),local.addr(),"test"};
    config::esp::chams=true;config::esp::team_check=true;
    auto base=[&]{return sdk::read_value<uintptr_t>(holder.addr()+array_offset);};
    auto count=[&]{return sdk::read_value<int>(holder.addr()+count_offset);};
    OnFrameStage(0);assert(creates==0 && base()==0);
    OnFrameStage(1);assert(creates==1 && cleans==1 && tracked==1 && applied==1 && count()==1);
    assert(resource=="materials/dev/glowproperty.vmat" && !hidden);
    const auto owned=base();assert(sdk::read_value<void*>(owned)==material_storage);
    OnFrameStage(1);assert(creates==1 && base()==owned); // Cached material and persistent array.
    clock_value+=300;OnFrameStage(1); // Finish the color-settle window before testing steady frames.
    const auto steady_snapshot=snapshot.load();const auto steady_writes=write_calls.load();
    const auto stable_holder_writes=holder_writes.load(),stable_slot_updates=slot_updates.load();
    for(int i=0;i<20;++i) OnFrameStage(1);
    assert(snapshot.load()==steady_snapshot && write_calls==steady_writes);
    assert(holder_writes==stable_holder_writes && slot_updates==stable_slot_updates);
    // A count-only repair must not refill the backing array or rewrite its base.
    holder.put(count_offset,0);OnFrameStage(1);
    assert(count()==1 && slot_updates==stable_slot_updates && holder_writes==stable_holder_writes+1);
    // Readers pin old snapshots across multiple writer publications.
    auto previous=snapshot.load();
    config::esp::chams=false;OnFrameStage(1);assert(base()==0 && count()==0 && previous->size()==1);
    assert((*previous)[0]->slot && !(*previous)[0]->active);previous.reset();
    config::esp::chams=true;clock_value+=300;OnFrameStage(1);assert(count()==1);
    pawn.put(sdk::off::C_BaseEntity::m_iHealth,0);OnFrameStage(1);assert(base()==0 && tracked==0);
    pawn.put(sdk::off::C_BaseEntity::m_iHealth,100);
    local.put<uint8_t>(sdk::off::C_BaseEntity::m_iTeamNum,3);OnFrameStage(1);assert(base()==0);
    local.put<uint8_t>(sdk::off::C_BaseEntity::m_iTeamNum,2);
    identity.put<uint32_t>(0x10,handle+(1u<<15));OnFrameStage(1);assert(base()==0);
    identity.put(0x10,handle);OnFrameStage(1);assert(count()==1);
    // Removal restores before the original lifecycle callback can free the pawn.
    BeforeRemove(system.addr(),pawn.addr(),handle);assert(base()==0 && count()==0);
    OnFrameStage(1); // Inactive records must not be reused after retirement.
    assert(count()==1);
    config::esp::chams=false;OnFrameStage(1);
    // Preserve pre-existing engine arrays; never modify their elements.
    uintptr_t original_array[2]={0x1111,0x2222};
    holder.put(array_offset,reinterpret_cast<uintptr_t>(original_array));holder.put(count_offset,2);
    config::esp::chams=true;OnFrameStage(1);assert(count()==1 && original_array[0]==0x1111);
    config::esp::chams=false;OnFrameStage(1);
    assert(base()==reinterpret_cast<uintptr_t>(original_array) && count()==2);
    holder.put<uintptr_t>(array_offset,0);holder.put(count_offset,0);
    // Another renderer replacing our array owns its replacement; disabling chams
    // must not overwrite that replacement with our saved engine pointer.
    config::esp::chams=true;OnFrameStage(1);assert(count()==1);
    holder.put(array_offset,reinterpret_cast<uintptr_t>(original_array));holder.put(count_offset,2);
    const auto old_conflicts=conflicts.load();OnFrameStage(1);
    assert(conflicts==old_conflicts+1 && applied==0);
    const auto conflict_snapshot=snapshot.load();const auto conflict_writes=holder_writes.load();
    for(int i=0;i<10;++i) OnFrameStage(1);
    assert(snapshot.load()==conflict_snapshot && holder_writes==conflict_writes && conflicts==old_conflicts+1);
    config::esp::chams=false;OnFrameStage(1);
    assert(base()==reinterpret_cast<uintptr_t>(original_array) && count()==2);
    holder.put<uintptr_t>(array_offset,0);holder.put(count_offset,0);
    auto* protected_holder=VirtualAlloc(nullptr,4096,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
    assert(protected_holder);
    DWORD protection=0;assert(VirtualProtect(protected_holder,4096,PAGE_READONLY,&protection));
    object.put(holder_offset,reinterpret_cast<uintptr_t>(protected_holder));
    config::esp::chams=true;OnFrameStage(1);assert(applied==0);
    assert(sdk::read_value<uintptr_t>(reinterpret_cast<uintptr_t>(protected_holder)+array_offset)==0);
    config::esp::chams=false;OnFrameStage(1);
    object.put(holder_offset,holder.addr());VirtualFree(protected_holder,0,MEM_RELEASE);
    config::esp::chams=true;config::esp::chams_through_walls=true;
    OnFrameStage(1);clock_value+=300;OnFrameStage(1);
    assert(hidden && resource=="materials/dev/glowproperty_hidden.vmat" && creates==2);
    // Disconnect clears membership and our supplied array, despite unchanged config.
    game_state::test_state.in_game=false;OnFrameStage(1);assert(base()==0 && count()==0);
    game_state::test_state.in_game=true;OnFrameStage(1);
    std::promise<void> entered,leave;
    auto release=leave.get_future().share();
    std::thread reader([&]{auto owned_snapshot=snapshot.load();entered.set_value();release.wait();
        assert(owned_snapshot->size()==1 && (*owned_snapshot)[0]->slot);});
    entered.get_future().wait();
    config::esp::chams=false;OnFrameStage(1);leave.set_value();reader.join();
    assert(base()==0);
    // A writer cannot restore/free a published array while an original engine
    // draw holds the read-side guard, even if its snapshot was published earlier.
    config::esp::chams=true;OnFrameStage(1);assert(count()==1);
    std::promise<void> draw_started,allow_draw;
    draw_entered=&draw_started;draw_release=allow_draw.get_future().share();
    std::thread drawing([&]{assert(Draw(BlockingDraw,desc.addr(),object.addr(),3,4)==42);});
    draw_started.get_future().wait();
    config::esp::chams=false;
    auto writer=std::async(std::launch::async,[]{OnFrameStage(1);});
    assert(writer.wait_for(std::chrono::milliseconds(10))==std::future_status::timeout);
    allow_draw.set_value();drawing.join();writer.get();assert(base()==0 && count()==0);
    load_success=false;config::esp::chams_color[0]=.5f;config::esp::chams=true;
    OnFrameStage(1);clock_value+=300;OnFrameStage(1);assert(base()==0 && creates==2 && cleans==3);
    Shutdown();assert(!hook_ready && snapshot.load()->empty());
    std::cout<<"Chams regressions passed: material construction/cleanup, persistent overrides, restoration, filters, serials, lifecycle, concurrent snapshots and failed loads\n";
}
