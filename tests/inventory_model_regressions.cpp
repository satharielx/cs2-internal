#include "../manager/core/inventory.cpp"
#include "../manager/core/runtime_inspector.cpp"
#include "../manager/core/model_inspector.h"
#include "../manager/core/ui/model_preview.h"
#include "imgui_preview.h"
#include <cassert>
#include <iostream>

namespace sdk {
std::uint8_t* find_unique_pattern(const char*,const char*) {return nullptr;}
std::uint8_t* resolve_absolute_rip_address(std::uint8_t*,size_t,size_t) {return nullptr;}
}
namespace skins {
void ApplyKnife() {}
const char* GetWeaponName(int id) {return id==7?"AK-47":"Unknown";}
bool DecorateInventoryView(uintptr_t,const PlayerSkinConfig&) {return true;}
}
struct Buffer {
    std::vector<unsigned char> bytes;
    explicit Buffer(size_t size):bytes(size) {}
    uintptr_t address() {return reinterpret_cast<uintptr_t>(bytes.data());}
    template<class T> void Put(size_t offset,T value) {assert(offset+sizeof(T)<=bytes.size());std::memcpy(bytes.data()+offset,&value,sizeof(value));}
};
static uintptr_t new_view=0,view_array=0,inventory_address=0;
static unsigned created=0,added=0,removed=0,destroyed=0;
static bool reject_add=false,in_cache=false,omit_view=false,fault_factory=false;
static unsigned attributes=0;
static void __fastcall SetTestAttribute(uintptr_t,uintptr_t item,uintptr_t definition,const std::string& value) {
    const auto index=sdk::read_value<uint16_t>(definition);
    auto blob=sdk::read_value<uintptr_t>(item+0x20);
    if(!blob) {blob=reinterpret_cast<uintptr_t>(HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,4+16*8));sdk::write_memory(item+0x20,blob);}
    auto count=sdk::read_value<uint8_t>(blob+2);
    sdk::write_memory(blob+4+count*16,index);
    if(index==111) assert(value=="Inventory test");
    else if(index<=8) sdk::write_memory(blob+12+count*16,std::stof(value));
    else sdk::write_memory(blob+12+count*16,static_cast<uint32_t>(std::stoul(value)));
    sdk::write_memory(blob+2,static_cast<uint8_t>(count+1));++attributes;
}
static void __fastcall SetTestNumeric(uintptr_t item,uintptr_t definition,const uint32_t* bits) {
    SetTestAttribute(0,item,definition,"0");
    const auto blob=sdk::read_value<uintptr_t>(item+0x20);
    const auto count=sdk::read_value<uint8_t>(blob+2);
    sdk::write_memory(blob+12+(count-1)*16,*bits);
}
static uintptr_t __fastcall Create() {
    ++created;if(fault_factory) RaiseException(EXCEPTION_ACCESS_VIOLATION,0,0,nullptr);
    auto* item=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,0x48);
    sdk::write_memory(reinterpret_cast<uintptr_t>(item),inventory::native::api.item_table);
    sdk::write_memory(reinterpret_cast<uintptr_t>(item)+0x32,uint16_t{0x87ff});
    return reinterpret_cast<uintptr_t>(item);
}
static bool __fastcall Add(uintptr_t cache,uintptr_t item) {
    assert(cache&&item&&attributes>=6);
    assert(sdk::read_value<uint16_t>(item+0x32)==(0x87ff|(5<<11)));
    const auto blob=sdk::read_value<uintptr_t>(item+0x20);
    assert(sdk::read_value<uint8_t>(blob+2)==6);
    assert(sdk::read_value<float>(blob+12)==282.f&&sdk::read_value<float>(blob+44)==.1f);
    assert(sdk::read_value<uint32_t>(blob+60)==90&&sdk::read_value<uint32_t>(blob+76)==0);
    ++added;in_cache=!reject_add;return in_cache;
}
static void __fastcall Remove(uintptr_t,uintptr_t) {assert(in_cache);in_cache=false;++removed;}
static void __fastcall NotifyCreated(uintptr_t object,const void* owner,uintptr_t item,int event) {
    using namespace cs2_dumper::schemas::client_dll;
    assert(object==inventory_address&&event==4&&sdk::read_value<uint32_t>(reinterpret_cast<uintptr_t>(owner))==0x42);
    assert(sdk::read_value<uint32_t>(item+0x28)==0x42&&sdk::read_value<uint16_t>(item+0x30)==7);
    assert(sdk::read_value<uint64_t>(item+0x10)==1001&&sdk::read_value<uint32_t>(item+0x2c)==81);
    if(omit_view) return;
    sdk::write_memory(new_view,inventory::native::api.view_table);
    sdk::write_memory(new_view+C_EconItemView::m_iItemID,sdk::read_value<uint64_t>(item+0x10));
    sdk::write_memory(new_view+C_EconItemView::m_iInventoryPosition,uint32_t{81});
    sdk::write_memory(view_array+8,new_view);sdk::write_memory(inventory_address+0x20,2);
    inventory::OnFrameStage(6); // Reentrant notifications must not publish twice.
}
static void __fastcall NotifyDestroyed(uintptr_t object,const void*,uintptr_t,int event) {
    assert(object==inventory_address&&event==4&&!in_cache);
    sdk::write_memory(view_array+8,uintptr_t{0});sdk::write_memory(object+0x20,1);
}
static void* __fastcall Destroy(uintptr_t item,unsigned flags) {
    assert(flags==1&&!in_cache);++destroyed;
    HeapFree(GetProcessHeap(),0,reinterpret_cast<void*>(sdk::read_value<uintptr_t>(item+0x20)));
    HeapFree(GetProcessHeap(),0,reinterpret_cast<void*>(item));return nullptr;
}
static void NativeChecks() {
    using namespace inventory;using namespace cs2_dumper::schemas::client_dll;
    assert(skins::rarity::Native(9,803)==6); // AWP Neo-Noir, Covert.
    assert(skins::rarity::Native(1,962)==6); // Desert Eagle Printstream, Covert.
    assert(skins::rarity::Native(7,282)==5); // AK-47 Redline, Classified.
    assert(skins::rarity::Native(1,37)==4); // Desert Eagle Blaze, Restricted.
    assert(skins::rarity::Native(16,309)==7); // M4A4 Howl, Contraband.
    assert(skins::rarity::Native(500,38)==6&&skins::rarity::Native(500,0)==6);
    assert(skins::rarity::Native(9,99999)==-1&&skins::rarity::Native(65535,803)==-1);
    for(size_t i=1;i<std::size(skins::rarity::paint_rarities);++i)
        assert(skins::rarity::paint_rarities[i-1].id<skins::rarity::paint_rarities[i].id);
    Buffer manager(0x100),player(0x100),cache(0x100),old_view(0x1000),view(0x1000),views(32),marker(0x1000);
    Buffer schema(0x200),definitions(112*8),attribute_definitions(112*0x20),type(8),table(0x38);
    native::api={};native::api.manager=manager.address();native::api.inventory_offset=8;
    native::api.manager_table=0x11111;native::api.inventory_table=0x22222;native::api.cache_table=0x33333;
    native::api.item_table=0x44444;native::api.view_table=0x55555;
    native::api.create=Create;native::api.add=Add;native::api.remove=Remove;
    native::api.created=NotifyCreated;native::api.destroyed=NotifyDestroyed;native::api.destroy=Destroy;
    native::api.schema=schema.address();native::api.numeric_setter=native::api.string_setter=reinterpret_cast<uintptr_t>(SetTestAttribute);
    native::api.set_numeric=SetTestNumeric;
    schema.Put(0x150,112);schema.Put(0x158,definitions.address());type.Put(0,table.address());table.Put(0x30,native::api.numeric_setter);
    for(unsigned index:{6,7,8,80,81,111}) {
        definitions.Put(index*8,attribute_definitions.address()+index*0x20);
        attribute_definitions.Put<uint16_t>(index*0x20,static_cast<uint16_t>(index));
        attribute_definitions.Put(index*0x20+0x18,type.address());
    }
    native::initialized=true;native::faulted=false;native::closing=false;
    manager.Put(8,player.address());player.Put(0,native::api.inventory_table);
    player.Put<uint64_t>(0x10,0x0110000100000042ULL);player.Put<uint32_t>(0x18,1);
    player.Put(0x68,cache.address());cache.Put(0,native::api.cache_table);
    player.Put(0x20,3);player.Put(0x28,views.address());player.Put(0x30,4);
    marker.Put(0,native::api.view_table);marker.Put<uint64_t>(C_EconItemView::m_iItemID,0xf000000000000007ULL);
    marker.Put<uint32_t>(C_EconItemView::m_iInventoryPosition,0xc000ffff);views.Put(16,marker.address());
    old_view.Put(0,native::api.view_table);old_view.Put<uint64_t>(C_EconItemView::m_iItemID,1000);
    old_view.Put<uint32_t>(C_EconItemView::m_iInventoryPosition,0xc0000050);views.Put(0,old_view.address());
    new_view=view.address();view_array=views.address();inventory_address=player.address();
    items.clear();next_id=1;skins::PlayerSkinConfig skin;skin.weapon_id=7;skin.paint_kit=282;skin.wear=.1f;skin.name_tag="Inventory test";skin.stattrak=true;skin.stattrak_count=90;
    const auto id=inventory::Add(skin);assert(id&&inventory::Equip(id)&&skins::user_skins[7].paint_kit==282);
    Publish(id);OnFrameStage(6);
    assert(items[0].native==NativeState::Published&&items[0].native_id==1001&&in_cache&&created==1&&added==1);
    assert(inventory::Save("native.inv")&&!inventory::Load("native.inv"));
    inventory::Remove(id);OnFrameStage(6);assert(items.empty()&&!in_cache&&removed==1&&destroyed==1);
    reject_add=true;const auto failed=inventory::Add(skin);Publish(failed);OnFrameStage(6);
    assert(items[0].native==NativeState::Failed&&!items[0].native_object&&!items[0].native_id&&destroyed==2);
    reject_add=false;inventory::Remove(failed);
    omit_view=true;const auto missing=inventory::Add(skin);Publish(missing);OnFrameStage(6);
    assert(items[0].native==NativeState::Failed&&items[0].native_object&&in_cache);
    inventory::Remove(missing);OnFrameStage(6);assert(items.empty()&&!in_cache);omit_view=false;
    fault_factory=true;const auto fault=inventory::Add(skin);Publish(fault);OnFrameStage(6);
    assert(items[0].native==NativeState::Failed&&!items[0].native_id&&native::faulted);
    assert(items[0].message.find("econ factory")!=std::string::npos);
    const auto attempts=created;Publish(fault);OnFrameStage(6);assert(created==attempts);
    inventory::Remove(fault);fault_factory=false;native::faulted=false; // Isolated fake engine reset.
    const auto saved=inventory::Add(skin);assert(saved&&inventory::Save("collection.inv"));
    items.clear();assert(inventory::Load("collection.inv")&&items.size()==1&&items[0].skin.name_tag==skin.name_tag&&items[0].native==NativeState::Local);
    {std::ofstream out("bad.inv");out<<"NEPHILIM_INVENTORY 1\n1 7 282 0 0.1 2 0 \"bad bool\"\n";}
    assert(!inventory::Load("bad.inv")&&items.size()==1);
    skin.wear=std::numeric_limits<float>::infinity();assert(!inventory::Add(skin));
    inventory::Shutdown();items.clear();native::api={};
}
static void ModelChecks() {
    using namespace model_inspector;
    auto mesh=Mannequin();assert(!mesh.vertices.empty()&&!mesh.triangles.empty());
    std::string error;
    assert(ParseObj("v 0 0 0\nv 1 0 0\nv 1 1 0\nv 0 1 0\nf -4/1 -3/2 -2/3 -1/4\n",mesh,error));
    assert(mesh.vertices.size()==4&&mesh.triangles.size()==2);
    const auto saved=mesh.vertices.size();
    assert(!LoadObjFile("missing-agent.obj",mesh,error)&&mesh.vertices.size()==saved);
    for(const char* bad:{"v nan 0 0\nf 1 1 1\n","v 0 0 0\nf 0 1 2\n","v 0 0 0\nf 1 2 3\n","v 0 0 0\n"})
        assert(!ParseObj(bad,mesh,error)&&mesh.vertices.size()==saved);
    assert(!ParseObj(std::string(max_bytes+1,'x'),mesh,error));
    assert(ParseObj("v 2e38 0 0\nv 3e38 0 0\nv 2e38 1e38 0\nf 1 2 3\n",mesh,error));
    for(const auto& p:mesh.vertices) assert(std::isfinite(p.x)&&std::isfinite(p.y)&&std::isfinite(p.z));
    const auto point=Rotate({0,0,0},View{});assert(std::isfinite(point.z)&&point.z>0);
    ImGui::CreateContext();auto& io=ImGui::GetIO();io.DisplaySize={1200,900};io.DeltaTime=1.f/60;io.IniFilename=nullptr;
    unsigned char* pixels;int width,height;io.Fonts->GetTexDataAsRGBA32(&pixels,&width,&height);
    const float color[]{.2f,.8f,1.f};
    for(int i=0;i<3;++i) {
        ImGui::NewFrame();ImGui::SetNextWindowPos({20,20});ImGui::SetNextWindowSize({1000,800});ImGui::Begin("Model inspector test");
        ImGui::SetNextItemOpen(true,ImGuiCond_Always);menu_advanced::RenderModelInspector(color);ImGui::End();ImGui::Render();
    }
    assert(ImGui::GetDrawData()->TotalVtxCount>0&&ImGui::GetDrawData()->TotalVtxCount<100000);
    if(std::filesystem::exists(menu_advanced::AgentMeshPath())) {
        Mesh agent;assert(LoadObjFile(menu_advanced::AgentMeshPath(),agent,error));
        assert(agent.vertices.size()>1000&&agent.triangles.size()>1000&&agent.triangles.size()<=10000);
        assert(ImGui::GetDrawData()->TotalVtxCount>10000);
    }
    SaveMenuPreview("model-inspector.bmp",pixels,width,height);ImGui::DestroyContext();
}
int main() {
    NativeChecks();ModelChecks();
    std::cout<<"PASS: local inventory persistence/validation, native insertion/events/removal/rollback/reentrancy, OBJ bounds and production 3D preview\n";
}
