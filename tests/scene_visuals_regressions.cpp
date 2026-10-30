#include "../manager/core/scene_visuals.h"
#include <cassert>
#include <iostream>
#include <vector>
struct Material {uintptr_t table;const char* name;};
static unsigned name_calls=0;
static const char* __fastcall Name(uintptr_t material) {++name_calls;return reinterpret_cast<Material*>(material)->name;}
struct Call {uintptr_t list;int count;};
static std::vector<Call> calls;
static void __fastcall Forward(uintptr_t desc,uintptr_t context,uintptr_t list,int count,uintptr_t view,uintptr_t layer,uintptr_t unknown) {
    assert(desc==11 && context==22 && view==33 && layer==44 && unknown==55);
    calls.push_back({list,count});
}
int main() {
    using namespace scene_visuals;
    assert(ClassifyName("materials/sky/SUN_disc.vmat")==Sun);
    assert(ClassifyName("materials/sky/sky_clouds01.vmat") & Clouds);
    assert(ClassifyName("materials/sun_map/brick.vmat")==0);
    assert(ClassifyName("materials/de_sun/sunset_wall.vmat")==0);
    assert(ClassifyName("materials/world/cloudy_window.vmat")==0);
    uintptr_t vtable[]={reinterpret_cast<uintptr_t>(&Name)};
    Material wall{reinterpret_cast<uintptr_t>(vtable),"materials/world/brick.vmat"};
    Material sun{reinterpret_cast<uintptr_t>(vtable),"materials/sky/sun_disc.vmat"};
    Material cloud{reinterpret_cast<uintptr_t>(vtable),"materials/sky/clouds.vmat"};
    std::array<unsigned char,stride*80> entries{};
    const auto list=reinterpret_cast<uintptr_t>(entries.data());
    auto put=[&](int index,Material* material) {std::memcpy(entries.data()+index*stride+material_offset,&material,sizeof(material));};
    put(0,&wall);put(1,&sun);put(2,&wall);put(3,&cloud);put(4,&sun);put(5,&wall);
    const auto original=entries;
    auto draw=[&](unsigned options,int count=6) {flags=options;calls.clear();Draw(Forward,11,22,list,count,33,44,55);assert(entries==original);};
    draw(0);assert(calls.size()==1 && calls[0].list==list && calls[0].count==6 && name_calls==0);
    draw(Sun);assert(calls.size()==3 && calls[0].count==1 && calls[1].list==list+2*stride && calls[1].count==2 && calls[2].list==list+5*stride);
    const auto cached_names=name_calls;draw(Sun);assert(name_calls==cached_names);
    draw(Clouds);assert(calls.size()==2 && calls[0].count==3 && calls[1].list==list+4*stride && calls[1].count==2);
    draw(Sun|Clouds);assert(calls.size()==3 && calls[0].count==1 && calls[1].count==1 && calls[2].count==1);
    draw(Sky);assert(calls.size()==1 && calls[0].count==6); // Sky pass is handled separately.
    draw(Sun,-1);assert(calls.size()==1 && calls[0].count==-1);
    draw(Sun,4097);assert(calls.size()==1 && calls[0].count==4097);
    // Unknown/unreadable material names pass through; no geometry is blackened.
    assert(ReadKind(0)==0 && ReadKind(1)==0);
    put(0,&sun);put(1,&sun);flags=Sun;calls.clear();Draw(Forward,11,22,list,2,33,44,55);assert(calls.empty());
    // Bound additional draw submissions when alternating geometry would split a batch excessively.
    for(int i=0;i<80;++i) put(i,i%2 ? &sun : &wall);
    flags=Sun;calls.clear();Draw(Forward,11,22,list,80,33,44,55);
    assert(calls.size()==1 && calls[0].count==80 && fallbacks==1);
    std::cout<<"Scene filter regressions passed: selective sun/cloud filtering, ordered runs, argument preservation, unchanged engine entries, cache and bounded fallback\n";
}
