#include "inventory.h"
#include "runtime_inspector.h"
#include "skins/rarity.h"
#include "../sdk/pe_rtti.h"
#include <atomic>

namespace inventory {
namespace native {
    constexpr char factory_pattern[]="48 83 EC 28 B9 48 00 00 00 E8 ? ? ? ? 48 85 C0 74 ? 48 8D 0D ? ? ? ? C7 40 32 00 00 FF 00";
    constexpr char manager_pattern[]="48 89 5C 24 08 57 48 83 EC 20 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 05 ? ? ? ? BF E8 00 00 00";
    using Create=uintptr_t(__fastcall*)();
    using Add=bool(__fastcall*)(uintptr_t,uintptr_t);
    using Remove=void(__fastcall*)(uintptr_t,uintptr_t);
    using Notify=void(__fastcall*)(uintptr_t,const void*,uintptr_t,int);
    using Destroy=void*(__fastcall*)(uintptr_t,unsigned);
    using SetAttribute=void(__fastcall*)(uintptr_t,uintptr_t,uintptr_t,const std::string&);
    using SetNumeric=void(__fastcall*)(uintptr_t,uintptr_t,const uint32_t*);
    struct Api {uintptr_t manager=0,manager_table=0,inventory_table=0,cache_table=0,item_table=0,view_table=0,schema=0;
        uintptr_t numeric_setter=0,string_setter=0;SetNumeric set_numeric=nullptr;
        uint32_t inventory_offset=0;Create create=nullptr;Add add=nullptr;Remove remove=nullptr;
        Notify created=nullptr,destroyed=nullptr;Destroy destroy=nullptr;};
    Api api;
    std::atomic<bool> closing{false};
    bool initialized=false,faulted=false;
    const char* operation="resolver";
    uintptr_t exception_address=0;
    int CaptureFault(EXCEPTION_POINTERS* info) {
        const auto code=info->ExceptionRecord->ExceptionCode;
        if(code!=EXCEPTION_ACCESS_VIOLATION&&code!=EXCEPTION_IN_PAGE_ERROR) return EXCEPTION_CONTINUE_SEARCH;
        faulted=true;exception_address=reinterpret_cast<uintptr_t>(info->ExceptionRecord->ExceptionAddress);
        return EXCEPTION_EXECUTE_HANDLER;
    }
    std::string FaultMessage() {
        std::ostringstream out;out<<"Native inventory fault at "<<operation<<" (address 0x"<<std::hex
            <<exception_address<<"); restart game before retrying";
        return out.str();
    }
    thread_local bool processing=false;
    struct Context {uintptr_t inventory=0,cache=0;std::array<unsigned char,16> owner{};};
    bool Resolve(std::string& error) {
        if(faulted) {error=FaultMessage();return false;}
        if(initialized) return true;
        const auto catalog=runtime_inspector::Catalog();
        const auto baseline=std::find_if(catalog.begin(),catalog.end(),[](const auto& b){return std::strcmp(b.name,"client.dll")==0;});
        if(baseline==catalog.end()) {error="Client research identity unavailable";return false;}
        const auto identity=runtime_inspector::Capture(std::span<const runtime_inspector::Baseline>(&*baseline,1));
        if(identity.modules.empty()||identity.modules[0].identity!=runtime_inspector::Identity::Match) {
            error="Native inventory requires the researched client identity; local inventory remains available";return false;
        }
        sdk::rtti::Image image(GetModuleHandleA("client.dll"));
        const auto base=reinterpret_cast<uintptr_t>(GetModuleHandleA("client.dll"));
        // Build/PDB gated: schema accessor's RIP load, not a call into a lazy initializer.
        unsigned char schema_code[11]{};
        constexpr unsigned char schema_prefix[]={0x48,0x83,0xec,0x28,0x48,0x8b,0x05};
        if(!sdk::read_memory(base+0x1189b30,schema_code)||std::memcmp(schema_code,schema_prefix,7)) {
            error="Econ schema accessor validation failed";return false;
        }
        int32_t schema_disp=0;std::memcpy(&schema_disp,schema_code+7,4);
        const auto system=sdk::read_value<uintptr_t>(base+0x1189b30+11+schema_disp);
        api.schema=sdk::read_value<uintptr_t>(system+8);
        api.numeric_setter=base+0x11547a0;api.string_setter=base+0x1154860;
        api.set_numeric=reinterpret_cast<SetNumeric>(base+0x10df990);
        if(!api.schema||!image.Contains(api.numeric_setter)||!sdk::rtti::Executable(api.numeric_setter)||
            !image.Contains(api.string_setter)||!sdk::rtti::Executable(api.string_setter)||
            !image.Contains(reinterpret_cast<uintptr_t>(api.set_numeric))||!sdk::rtti::Executable(reinterpret_cast<uintptr_t>(api.set_numeric))) {
            error="Econ schema is not ready";return false;
        }
        api.manager_table=image.Vtable(".?AVCCSInventoryManager@@");
        api.inventory_table=image.Vtable(".?AVCCSPlayerInventory@@");
        api.cache_table=image.Vtable(".?AVCGCClientSharedObjectCache@GCSDK@@");
        api.item_table=image.Vtable(".?AVCEconItem@@");
        api.view_table=image.Vtable(".?AVC_EconItemView@@");
        const auto factory=sdk::find_unique_pattern("client.dll",factory_pattern);
        const auto constructor=sdk::find_unique_pattern("client.dll",manager_pattern);
        if(!factory||!constructor||!api.manager_table||!api.inventory_table||!api.cache_table||!api.item_table||!api.view_table) {
            error="Native inventory RTTI or unique constructors unavailable";return false;
        }
        api.manager=reinterpret_cast<uintptr_t>(sdk::resolve_absolute_rip_address(constructor+10,3,7));
        const auto accessor=sdk::read_value<uintptr_t>(api.manager_table+72*sizeof(void*));
        unsigned char getter[8]{};
        if(!sdk::read_memory(accessor,getter)||getter[0]!=0x48||getter[1]!=0x8b||getter[2]!=0x81||getter[7]!=0xc3||
            sdk::read_value<uintptr_t>(api.manager)!=api.manager_table) {error="Inventory manager accessor/layout validation failed";return false;}
        std::memcpy(&api.inventory_offset,getter+3,4);
        if(api.inventory_offset>0x100000) {error="Inventory accessor displacement invalid";return false;}
        api.create=reinterpret_cast<Create>(factory);
        api.add=sdk::read_value<Add>(api.cache_table+2*sizeof(void*));
        api.remove=sdk::read_value<Remove>(api.cache_table+3*sizeof(void*));
        api.created=sdk::read_value<Notify>(api.inventory_table);
        api.destroyed=sdk::read_value<Notify>(api.inventory_table+2*sizeof(void*));
        api.destroy=sdk::read_value<Destroy>(api.item_table);
        for(auto address:{reinterpret_cast<uintptr_t>(api.add),reinterpret_cast<uintptr_t>(api.remove),
            reinterpret_cast<uintptr_t>(api.created),reinterpret_cast<uintptr_t>(api.destroyed),reinterpret_cast<uintptr_t>(api.destroy)})
            if(!image.Contains(address)||!sdk::rtti::Executable(address)) {error="Native inventory method outside executable client code";return false;}
        initialized=true;return true;
    }
    bool GetContext(Context& context) {
        context.inventory=sdk::read_value<uintptr_t>(api.manager+api.inventory_offset);
        if(sdk::read_value<uintptr_t>(context.inventory)!=api.inventory_table||
            !sdk::read_memory(context.inventory+0x10,context.owner)) return false;
        uint64_t owner=0;std::memcpy(&owner,context.owner.data(),8);if(!owner) return false;
        context.cache=sdk::read_value<uintptr_t>(context.inventory+0x68);
        return sdk::read_value<uintptr_t>(context.cache)==api.cache_table;
    }
    uintptr_t FindView(const Context& context,uint64_t id,uint64_t* maximum_id=nullptr,uint32_t* maximum_position=nullptr,bool* valid=nullptr) {
        if(valid) *valid=false;
        sdk::VectorView views{};if(!sdk::read_vector(context.inventory+0x20,views,100000)) return 0;
        uintptr_t result=0;
        for(int n=0;n<views.count;++n) {
            const auto view=sdk::read_value<uintptr_t>(views.data+n*sizeof(void*));
            if(!view) continue;
            if(sdk::read_value<uintptr_t>(view)!=api.view_table) return 0;
            using namespace cs2_dumper::schemas::client_dll;
            const auto item_id=sdk::read_value<uint64_t>(view+C_EconItemView::m_iItemID);
            // 0xF... identifies default/loadout markers, not owned econ IDs.
            if(item_id && (item_id>>60)!=0xf) {
                if(maximum_id) *maximum_id=(std::max)(*maximum_id,item_id);
                if(maximum_position) *maximum_position=(std::max)(*maximum_position,
                    sdk::read_value<uint32_t>(view+C_EconItemView::m_iInventoryPosition)&0x3fffffffU);
            }
            if(id&&item_id==id) result=view;
        }
        if(maximum_id||maximum_position) {
            // Include cached objects that have not acquired a view yet.
            sdk::VectorView types{};
            if(!sdk::read_vector(context.cache+0x10,types,256)) return 0;
            for(int t=0;t<types.count;++t) {
                const auto type=sdk::read_value<uintptr_t>(types.data+t*8);
                const auto count=sdk::read_value<int>(type+8);
                const auto objects=sdk::read_value<uintptr_t>(type+0x10);
                if(count<0||count>100000||(!objects&&count)) return 0;
                for(int n=0;n<count;++n) {
                    const auto item=sdk::read_value<uintptr_t>(objects+n*8);
                    if(sdk::read_value<uintptr_t>(item)!=api.item_table) continue;
                    const auto item_id=sdk::read_value<uint64_t>(item+0x10);
                    if(!item_id||(item_id>>60)==0xf) continue;
                    if(maximum_id) *maximum_id=(std::max)(*maximum_id,item_id);
                    if(maximum_position) *maximum_position=(std::max)(*maximum_position,
                        sdk::read_value<uint32_t>(item+0x2c)&0x3fffffffU);
                }
            }
        }
        if(valid) *valid=true;
        return result;
    }
    bool SetAttributeValue(uintptr_t item,unsigned index,const std::string& value,bool text=false,uint32_t bits=0) {
        const auto count=sdk::read_value<int>(api.schema+0x150);
        const auto definitions=sdk::read_value<uintptr_t>(api.schema+0x158);
        if(count<=0||count>65536||index>=static_cast<unsigned>(count)) return false;
        const auto definition=sdk::read_value<uintptr_t>(definitions+index*8);
        const auto type=sdk::read_value<uintptr_t>(definition+0x18);
        const auto table=sdk::read_value<uintptr_t>(type);
        const auto setter=sdk::read_value<uintptr_t>(table+0x30);
        if(!definition||!type||setter!=(text?api.string_setter:api.numeric_setter)) return false;
        if(text) reinterpret_cast<SetAttribute>(setter)(type,item,definition,value);
        else {
            if(!api.set_numeric) return false;
            api.set_numeric(item,definition,&bits);
        }
        const auto blob=sdk::read_value<uintptr_t>(item+0x20);
        const auto entries=sdk::read_value<uint8_t>(blob+2);
        for(unsigned n=0;n<entries;++n)
            if(sdk::read_value<uint16_t>(blob+4+n*16)==index)
                return text||sdk::read_value<uint32_t>(blob+12+n*16)==bits;
        return false;
    }
    bool SetCosmetics(uintptr_t item,const skins::PlayerSkinConfig& skin) {
        // Numeric attributes share a type object, but not their representation:
        // preserve float bits for finishes and raw uint bits for counters.
        const float paint=static_cast<float>(skin.paint_kit),seed=static_cast<float>(skin.seed);
        uint32_t paint_bits=0,seed_bits=0,wear_bits=0;
        std::memcpy(&paint_bits,&paint,4);std::memcpy(&seed_bits,&seed,4);std::memcpy(&wear_bits,&skin.wear,4);
        return SetAttributeValue(item,6,"",false,paint_bits)&&
            SetAttributeValue(item,7,"",false,seed_bits)&&
            SetAttributeValue(item,8,"",false,wear_bits)&&
            (!skin.stattrak||(SetAttributeValue(item,80,"",false,static_cast<uint32_t>(skin.stattrak_count))&&SetAttributeValue(item,81,"")))&&
            (skin.name_tag.empty()||SetAttributeValue(item,111,skin.name_tag,true));
    }
    bool CreateGuarded(const Context& context,int definition,uint64_t id,uint32_t position,Item& record) {
        __try {
            operation="econ factory";
            const auto item=api.create();if(!item||sdk::read_value<uintptr_t>(item)!=api.item_table) return false;
            record.native_object=item;record.native_inventory=context.inventory;record.native_cache=context.cache;record.native_id=id;
            uint32_t account=0;std::memcpy(&account,context.owner.data(),4);
            operation="econ identity fields";
            if(!sdk::write_memory(item+0x10,id)||!sdk::write_memory(item+0x18,uint64_t{0})||
                !sdk::write_memory(item+0x28,account)||!sdk::write_memory(item+0x2c,position)||
                !sdk::write_memory(item+0x30,static_cast<uint16_t>(definition))) {
                api.destroy(item,1);record.native_object=0;record.native_id=0;return false;
            }
            operation="econ attributes";
            if(!SetCosmetics(item,record.skin)) {api.destroy(item,1);record.native_object=0;record.native_id=0;return false;}
            operation="econ finish rarity";
            if(record.skin.paint_kit) {
                const int rarity=skins::rarity::Native(definition,record.skin.paint_kit);
                const auto packed=sdk::read_value<uint16_t>(item+0x32);
                if(rarity<0||!sdk::write_memory(item+0x32,static_cast<uint16_t>((packed&~0x7800U)|(rarity<<11)))) {
                    api.destroy(item,1);record.native_object=0;record.native_id=0;return false;
                }
            }
            operation="shared-object cache add";
            if(!api.add(context.cache,item)) {api.destroy(item,1);record.native_object=0;record.native_id=0;return false;}
            operation="inventory SOCreated";
            api.created(context.inventory,context.owner.data(),item,4);
            return true;
        } __except(CaptureFault(GetExceptionInformation())) {return false;}
    }
    bool RemoveGuarded(const Context& context,Item& record) {
        if(context.inventory!=record.native_inventory||context.cache!=record.native_cache||
            sdk::read_value<uintptr_t>(record.native_object)!=api.item_table||
            sdk::read_value<uint64_t>(record.native_object+0x10)!=record.native_id||
            sdk::read_value<uint32_t>(record.native_object+0x28)!=sdk::read_value<uint32_t>(reinterpret_cast<uintptr_t>(context.owner.data()))) return false;
        __try {
            operation="shared-object cache remove";
            api.remove(context.cache,record.native_object);
            operation="inventory SODestroyed";
            api.destroyed(context.inventory,context.owner.data(),record.native_object,4);
            operation="econ destructor";
            api.destroy(record.native_object,1);record.native_object=0;record.native_id=0;
            return true;
        } __except(CaptureFault(GetExceptionInformation())) {return false;}
    }
}
void OnFrameStage(int stage) {
    if(stage<0||stage>12||native::processing) return;
    struct Processing {Processing(){native::processing=true;}~Processing(){native::processing=false;}} processing;
    std::unique_lock<std::recursive_mutex> lock(config::mutex,std::try_to_lock);if(!lock.owns_lock()) return;
    for(auto it=items.begin();it!=items.end();++it) {
        if(native::closing&&it->native_object) it->native=NativeState::RemovePending;
        if(it->native!=NativeState::AddPending&&it->native!=NativeState::RemovePending) continue;
        if(!native::Resolve(it->message)) {it->native=NativeState::Failed;return;}
        native::Context context;
        if(!native::GetContext(context)) {it->message="Waiting for native inventory and shared-object cache";return;}
        if(it->native==NativeState::RemovePending) {
            if(native::RemoveGuarded(context,*it)) {items.erase(it);status="Native and local item removed";}
            else {it->native=NativeState::Failed;it->message=native::faulted?native::FaultMessage():"Native removal could not be confirmed; item ownership retained";}
            return;
        }
        if(native::closing) {it->native=NativeState::Local;return;}
        const bool known=std::strcmp(skins::GetWeaponName(it->skin.weapon_id),"Unknown")!=0||
            std::any_of(skins::glove_database.begin(),skins::glove_database.end(),[&](const auto& g){return g.weapon_id==it->skin.weapon_id;});
        if(!known) {it->native=NativeState::Failed;it->message="Native publication requires a recognized weapon or loaded glove definition";return;}
        uint64_t id=0;uint32_t position=0;bool valid=false;native::FindView(context,0,&id,&position,&valid);
        if(!valid) {it->native=NativeState::Failed;it->message="Native item-view vector validation failed";return;}
        for(const auto& existing:items) if((existing.native_id>>60)!=0xf) id=(std::max)(id,existing.native_id);
        if(id>=0xefffffffffffffffULL||position>=0x3fffffffU) {it->native=NativeState::Failed;it->message="Native inventory identifiers exhausted";return;}
        if(native::CreateGuarded(context,it->skin.weapon_id,id+1,position+1,*it)) {
            const auto view=native::FindView(context,it->native_id);
            it->native=view?NativeState::Published:NativeState::Failed;
            it->message=view?"Client-cache item and native item view confirmed":"Native cache object exists but inventory view is missing; remove before retrying";
        } else {it->native=NativeState::Failed;it->message=native::faulted?native::FaultMessage():
            std::string("Native item creation rejected at ")+native::operation+"; ownership retained if allocated";}
        return; // One native operation per stage; no work in draw callbacks.
    }
}
void Shutdown() {
    native::closing=true;
    const auto deadline=GetTickCount64()+1500;
    while(GetTickCount64()<deadline) {
        {std::lock_guard<std::recursive_mutex> lock(config::mutex);
            if(std::none_of(items.begin(),items.end(),[](const Item& i){return i.native_object!=0;})) break;}
        Sleep(1);
    }
    // Any retained objects use engine vtables and engine-owned allocations only.
}
}
