#include "chams.h"
#include "config.h"
#include "game_state.h"
#include "entity_events.h"
#include "constants/chams_signatures.h"
#include "../sdk/pe_rtti.h"
#include <array>
#include <memory>
#include <mutex>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <shared_mutex>

namespace chams {
namespace detail {
    constexpr uintptr_t holder_offset=0xA8, array_offset=0x3E8, count_offset=0x3F0;
    constexpr size_t maximum_materials=64;
    struct Api {
        void* system=nullptr;
        using Construct=void*(__fastcall*)(void*,int);
        using Root=void*(__fastcall*)(void*);
        using Cleanup=void(__fastcall*)(void*);
        struct Format {const char* name; uint64_t first,second;};
        using Load=bool(__cdecl*)(void*,void*,const char*,const Format&,const char*,unsigned);
        using Create=void*(__fastcall*)(void*,void***,const char*,void*,uint32_t,bool);
        Construct construct=nullptr; Root root=nullptr; Cleanup cleanup=nullptr;
        Load load=nullptr; Create create=nullptr;
        bool Ready() const {return system && construct && root && cleanup && load && create;}
    };
    Api api;
    uintptr_t desc_table=0, object_table=0;
    std::atomic<bool> stopping{true}, faulted{false};
    std::mutex registry_mutex;
    std::mutex stage_mutex;
    std::shared_mutex generation_mutex;
    thread_local unsigned generation_depth=0;
    struct GenerationRead {
        std::shared_lock<std::shared_mutex> lock{generation_mutex,std::defer_lock};
        GenerationRead() {if(generation_depth++==0) lock.lock();}
        ~GenerationRead() {--generation_depth;}
    };
    struct Settings {
        bool enabled=false, through=false, team_check=true;
        std::array<unsigned char,3> color{0,190,255};
    };
    struct Material {
        std::array<unsigned char,3> color{};
        bool through=false;
        void** binding=nullptr; // Engine-owned registered resource; never delete as a C++ object.
        void* object=nullptr;
    };
    std::vector<Material> materials;
    Settings last_settings{}, pending_settings{};
    ULONGLONG pending_since=0;
    bool has_pending=false;

    // The only pointer supplied to the engine points into process-heap storage,
    // never DLL globals. A failed restoration retains the backing allocation rather
    // than leaving an engine pointer to freed/unmapped DLL memory on unload.
    struct Record {
        uintptr_t system=0,pawn=0,object=0,holder=0;
        uint32_t handle=UINT32_MAX;
        uintptr_t original_array=0;
        int original_count=0;
        void** slot=nullptr;
        std::atomic<void*> material{nullptr};
        std::atomic<bool> published{false};
        std::atomic<bool> active{true};
        std::atomic<bool> conflicted{false};
        std::mutex mutex;
        // One active entry, with capacity for every accepted original count. A
        // reentrant lifecycle callback can clear an override while another draw
        // still holds its old base/count; all backing entries remain valid.
        Record() {slot=static_cast<void**>(HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,256*sizeof(void*)));}
        ~Record() {if(slot && !published) HeapFree(GetProcessHeap(),0,slot);}
    };
    using Records=std::vector<std::shared_ptr<Record>>;
    std::atomic<std::shared_ptr<const Records>> snapshot{std::make_shared<const Records>()};
    Records records;

    bool Capture(Settings& settings) {
        std::unique_lock<std::recursive_mutex> lock(config::mutex,std::try_to_lock);
        if(!lock.owns_lock()) return false;
        settings.enabled=config::esp::chams;
        settings.through=config::esp::chams_through_walls;
        settings.team_check=config::esp::team_check;
        for(int i=0;i<3;++i) {
            const float value=config::esp::chams_color[i];
            if(!std::isfinite(value)) {settings.enabled=false;return true;}
            settings.color[i]=static_cast<unsigned char>(std::clamp(value,0.f,1.f)*255.f+.5f);
        }
        return true;
    }

    // Engine calls are isolated from C++ RAII objects so SEH can reject a bad ABI
    // without crossing destructors. Constructors/root layout are pattern-gated.
    bool BuildGuarded(const Api& functions,const char* text,const char* name,void*** binding) {
        alignas(16) unsigned char arena[0x1400]{};
        bool constructed=false,ok=false;
        __try {
            functions.construct(arena,0); constructed=true;
            auto* root=functions.root(arena);
            const Api::Format format{"generic",0x469806E97412167CULL,0xE73790B53EE6F2AFULL};
            if(root && functions.load(root,nullptr,text,format,nullptr,0)) {
                functions.create(functions.system,binding,name,root,0,true);
                ok=*binding && sdk::read_value<void*>(reinterpret_cast<uintptr_t>(*binding));
            }
        } __except(GetExceptionCode()==EXCEPTION_ACCESS_VIOLATION || GetExceptionCode()==EXCEPTION_IN_PAGE_ERROR
            ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {faulted=true;}
        if(constructed) {
            __try {functions.cleanup(arena);}
            __except(GetExceptionCode()==EXCEPTION_ACCESS_VIOLATION || GetExceptionCode()==EXCEPTION_IN_PAGE_ERROR
                ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {faulted=true;ok=false;}
        }
        return ok && !faulted;
    }

    void* SelectMaterial(const Settings& settings) {
        for(auto& material:materials)
            if(material.color==settings.color && material.through==settings.through) {
                const auto current=sdk::read_value<void*>(reinterpret_cast<uintptr_t>(material.binding));
                if(!current || !sdk::is_valid_ptr(reinterpret_cast<uintptr_t>(current))) {
                    status="Cached material binding unavailable";return nullptr;
                }
                material.object=current;
                return current;
            }
        if(materials.size()>=maximum_materials) {status="Material cache full; reuse a previous color or reload"; return nullptr;}
        char text[1536]{};
        const int length=snprintf(text,sizeof(text),
            "<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} "
            "format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->\n"
            "{ shader = \"csgo_character.vfx\"\n%s"
            "g_vColorTint = [%.3f, %.3f, %.3f, 1.0]\n"
            "g_bFogEnabled = 0\ng_flMetalness = 0.0\n"
            "g_tMetalness = resource:\"materials/default/default_metal_tga_8fbc2820.vtex\"\n"
            "g_tAmbientOcclusion = resource:\"materials/default/default_ao_tga_79a2e0d0.vtex\"\n"
            "g_tColor = resource:\"materials/dev/primary_white_color_tga_21186c76.vtex\"\n"
            "g_tNormal = resource:\"materials/default/default_normal_tga_1b833b2a.vtex\"\n}",
            settings.through ? "F_DISABLE_Z_BUFFERING = 1\nF_DISABLE_Z_PREPASS = 1\nF_DISABLE_Z_WRITE = 1\n" : "",
            settings.color[0]/255.f,settings.color[1]/255.f,settings.color[2]/255.f);
        if(length<0 || length>=sizeof(text)) {status="Material definition overflow";return nullptr;}
        Material material{settings.color,settings.through};
        const char* name=settings.through ? "materials/dev/glowproperty_hidden.vmat" : "materials/dev/glowproperty.vmat";
        if(!BuildGuarded(api,text,name,&material.binding)) {
            status=faulted ? "Material engine call faulted; disabled until reload" : "Engine rejected the chams material";
            return nullptr;
        }
        material.object=sdk::read_value<void*>(reinterpret_cast<uintptr_t>(material.binding));
        if(!material.object || !sdk::is_valid_ptr(material.object ? reinterpret_cast<uintptr_t>(material.object) : 0)) {
            status="Material binding has no readable material";return nullptr;
        }
        materials.push_back(material); ++material_builds;
        return material.object;
    }

    bool Identity(const Record& record) {
        return sdk::entity_from_handle(record.system,record.handle)==record.pawn &&
            sdk::read_value<uintptr_t>(record.object)==object_table &&
            sdk::read_value<uintptr_t>(record.object+holder_offset)==record.holder;
    }
    template<class T> bool WriteHolder(uintptr_t address,const T& value) {
        if(!sdk::write_memory(address,value)) return false;
        holder_writes.fetch_add(1,std::memory_order_relaxed);
        return true;
    }
    bool Restore(Record& record) {
        std::lock_guard<std::mutex> lock(record.mutex);
        if(!record.published) return true;
        uintptr_t current=0;
        if(!sdk::read_memory(record.holder+array_offset,current)) return false;
        if(current!=reinterpret_cast<uintptr_t>(record.slot)) {record.published=false;return true;}
        // A recycled holder must never receive an old engine-owned array.
        const bool same=Identity(record);
        const uintptr_t original=same ? record.original_array : 0;
        const int count=same ? record.original_count : 0;
        int current_count=0;
        if(!sdk::read_memory(record.holder+count_offset,current_count)) return false;
        if((current_count!=count && !WriteHolder(record.holder+count_offset,count)) ||
            !WriteHolder(record.holder+array_offset,original)) return false;
        record.published=false;
        return true;
    }
    bool Apply(Record& record,void* material) {
        std::lock_guard<std::mutex> lock(record.mutex);
        if(!record.active || record.conflicted || !record.slot || !material) return false;
        uintptr_t base=0; int count=0;
        if(!sdk::read_memory(record.holder+array_offset,base) || !sdk::read_memory(record.holder+count_offset,count)) return false;
        const auto ours=reinterpret_cast<uintptr_t>(record.slot);
        if(record.published && base!=ours) {
            // Stop fighting an engine/renderer replacement on every frame.
            record.published=false;record.conflicted=true;
            conflicts.fetch_add(1,std::memory_order_relaxed);
            return false;
        }
        if(base==ours && count==1 && record.material==material) return true;
        if(!record.published) {
            if(count<0 || count>256 || (base==0)!=(count==0)) return false;
            if(base && !sdk::is_readable_range(base,static_cast<size_t>(count)*sizeof(void*))) return false;
            record.original_array=base; record.original_count=count;
        }
        // Apply is called under the exclusive draw barrier. Ordinary stores are
        // sufficient here; only a new material requires touching the backing array.
        if(record.material!=material) {
            std::fill_n(record.slot,256,material);
            record.material=material;
            slot_updates.fetch_add(1,std::memory_order_relaxed);
        }
        if(base!=ours) {
            if(!WriteHolder(record.holder+array_offset,ours)) return false;
            record.published=true;
        }
        if(count!=1 && !WriteHolder(record.holder+count_offset,1)) {
            // Keep ownership recorded so retirement can retry restoration.
            return false;
        }
        record.published=true;
        return true;
    }
    void RetireAll() {
        snapshot.store(std::make_shared<const Records>());
        for(auto& record:records) {record->active=false;Restore(*record);}
        records.clear(); tracked=0; applied=0;
    }
}

uintptr_t Initialize() {
    using namespace detail;
    stopping=false; faulted=false; hook_ready=false;
    status="Unavailable: chams RTTI or material helpers did not resolve";
    const auto scene_module=GetModuleHandleA("scenesystem.dll");
    const auto material_module=GetModuleHandleA("materialsystem2.dll");
    const auto tier=GetModuleHandleA("tier0.dll");
    sdk::rtti::Image scene(scene_module),material_image(material_module);
    desc_table=scene.Vtable(".?AVCAnimatableSceneObjectDesc@@");
    const auto base_table=scene.Vtable(".?AVCBaseSceneObjectDesc@@");
    object_table=scene.Vtable(".?AVCSceneAnimatableObject@@");
    const auto system_table=material_image.Vtable(".?AVCMaterialSystem2@@");
    const auto target=sdk::read_value<uintptr_t>(desc_table+4*sizeof(void*));
    if(!desc_table || !base_table || !object_table || !system_table || !scene.Contains(target) ||
        target!=sdk::read_value<uintptr_t>(base_table+4*sizeof(void*)) || !sdk::rtti::Executable(target)) return 0;
    unsigned char entry[128]{};
    SIZE_T transferred=0;
    if(!sdk::ProfileRead(GetCurrentProcess(),reinterpret_cast<void*>(target),entry,sizeof(entry),&transferred) || transferred!=sizeof(entry)) return 0;
    const unsigned char holder[]={0x48,0x8B,0x82,0xA8,0,0,0};
    const unsigned char array[]={0x4C,0x8B,0x80,0xE8,3,0,0};
    const unsigned char count[]={0x8B,0x88,0xF0,3,0,0};
    if(std::search(std::begin(entry),std::end(entry),std::begin(holder),std::end(holder))==std::end(entry) ||
        std::search(std::begin(entry),std::end(entry),std::begin(array),std::end(array))==std::end(entry) ||
        std::search(std::begin(entry),std::end(entry),std::begin(count),std::end(count))==std::end(entry)) return 0;
    const auto factory=reinterpret_cast<void*(__cdecl*)(const char*,int*)>(GetProcAddress(material_module,"CreateInterface"));
    api={};
    if(factory) api.system=factory("VMaterialSystem2_001",nullptr);
    if(!api.system || sdk::read_value<uintptr_t>(reinterpret_cast<uintptr_t>(api.system))!=system_table) return 0;
    api.construct=reinterpret_cast<Api::Construct>(sdk::find_pattern("client.dll",runtime_patterns::chams::arena_construct));
    api.root=reinterpret_cast<Api::Root>(sdk::find_pattern("client.dll",runtime_patterns::chams::arena_root));
    const auto cleanup=sdk::find_pattern("client.dll",runtime_patterns::chams::arena_cleanup_call);
    if(cleanup) {
        int32_t delta=0;
        if(sdk::read_memory(reinterpret_cast<uintptr_t>(cleanup)+8,delta))
            api.cleanup=reinterpret_cast<Api::Cleanup>(reinterpret_cast<uintptr_t>(cleanup)+12+delta);
    }
    api.load=reinterpret_cast<Api::Load>(GetProcAddress(tier,"?LoadKV3@@YA_NPEAVKeyValues3@@PEAVCUtlString@@PEBDAEBUKV3ID_t@@2I@Z"));
    const auto create=sdk::read_value<uintptr_t>(system_table+29*sizeof(void*));
    // Validate the inspected implementation family, including the allocation size.
    const auto material_ctor=sdk::find_pattern("materialsystem2.dll",runtime_patterns::chams::material_create);
    if(create!=reinterpret_cast<uintptr_t>(material_ctor)) return 0;
    api.create=reinterpret_cast<Api::Create>(create);
    if(!api.Ready() || !sdk::rtti::Executable(reinterpret_cast<uintptr_t>(api.cleanup))) return 0;
    status="Chams prepared; waiting for hook activation";
    return target;
}

void OnFrameStage(int stage) {
    using namespace detail;
    if(stage!=1 || stopping || !hook_ready) return;
    std::lock_guard<std::mutex> stage_lock(stage_mutex);
    if(stopping) return;
    static ULONGLONG rate_at=0;
    static unsigned rate_total=0;
    const auto sample_time=GetTickCount64();
    if(!rate_at || sample_time<rate_at) {rate_at=sample_time;rate_total=holder_writes.load(std::memory_order_relaxed);}
    else if(sample_time-rate_at>=1000) {
        const auto total=holder_writes.load(std::memory_order_relaxed);
        writes_per_second=static_cast<unsigned>((total-rate_total)*1000ULL/(sample_time-rate_at));
        rate_total=total;rate_at=sample_time;
    }
    Settings settings=last_settings;
    if(Capture(settings)) last_settings=settings; // Keep the previous snapshot during menu/config contention.
    const auto state=game_state::GetSnapshot();
    if(!settings.enabled || !state.in_game || !state.pawn || !state.entity_list || faulted) {
        if(tracked.load(std::memory_order_relaxed)) {
            std::unique_lock<std::shared_mutex> draws(generation_mutex);
            std::lock_guard<std::mutex> lock(registry_mutex); RetireAll();
        }
        if(!faulted) status=settings.enabled ? "Waiting for a game session" : "Disabled";
        return;
    }
    const auto now=GetTickCount64();
    if(!has_pending || settings.color!=pending_settings.color || settings.through!=pending_settings.through) {
        pending_settings=settings; pending_since=now; has_pending=true;
    }
    // Build once after a color edit settles, never once per player/draw.
    if(!materials.empty() && now-pending_since<250) return;
    if(!api.Ready()) {status="Material pipeline unavailable";return;}
    void* material=SelectMaterial(settings);
    if(!material) {
        std::unique_lock<std::shared_mutex> draws(generation_mutex);
        std::lock_guard<std::mutex> lock(registry_mutex); RetireAll(); return;
    }
    // Enumerate and validate outside the draw barrier. Draws continue while the
    // frame thread reads entities; only actual override changes require a writer.
    Records previous;
    {std::lock_guard<std::mutex> lock(registry_mutex); previous=records;}
    sdk::ScopedLocalReads reads;
    Records next;
    next.reserve(64);
    const auto team=sdk::read_value<uint8_t>(state.pawn+sdk::off::C_BaseEntity::m_iTeamNum);
    bool needs_write=false;
    for(int index=1;index<=64;++index) {
        const auto controller=sdk::entity_at(state.entity_list,index);
        const auto handle=controller ? sdk::read_value<uint32_t>(controller+sdk::off::CCSPlayerController::m_hPlayerPawn) : UINT32_MAX;
        const auto pawn=controller ? sdk::entity_from_handle(state.entity_list,handle) : 0;
        if(!pawn || pawn==state.pawn || sdk::read_value<int>(pawn+sdk::off::C_BaseEntity::m_iHealth)<=0 ||
            sdk::read_value<uint8_t>(pawn+sdk::off::C_BaseEntity::m_lifeState)!=0 ||
            (settings.team_check && sdk::read_value<uint8_t>(pawn+sdk::off::C_BaseEntity::m_iTeamNum)==team)) continue;
        const auto node=sdk::read_value<uintptr_t>(pawn+sdk::off::C_BaseEntity::m_pGameSceneNode);
        const auto link=node ? sdk::read_value<uintptr_t>(node+sdk::off::CSkeletonInstance::m_modelState+0xD8) : 0;
        const auto object=link ? sdk::read_value<uintptr_t>(link+0x20) : 0;
        if(!object || sdk::read_value<uintptr_t>(object)!=object_table) continue;
        const auto holder=sdk::read_value<uintptr_t>(object+holder_offset);
        if(!holder) continue;
        std::shared_ptr<Record> record;
        for(const auto& old:previous) if(old->active && old->system==state.entity_list && old->pawn==pawn && old->handle==handle && old->object==object && old->holder==holder) {record=old;break;}
        if(!record) {
            record=std::make_shared<Record>(); record->system=state.entity_list;record->pawn=pawn;
            record->handle=handle;record->object=object;record->holder=holder;
        }
        needs_write |= !record->conflicted && (!record->published.load() || record->material.load()!=material ||
            sdk::read_value<uintptr_t>(holder+array_offset)!=reinterpret_cast<uintptr_t>(record->slot) ||
            sdk::read_value<int>(holder+count_offset)!=1);
        next.push_back(std::move(record));
    }
    if(!needs_write && next==previous) {
        applied=static_cast<unsigned>(std::count_if(next.begin(),next.end(),[](const auto& r){return r->active.load() && r->published.load() && !r->conflicted.load();}));
        if(next.empty()) status="No eligible player scene objects / writable holders";
        return;
    }
    std::unique_lock<std::shared_mutex> draws(generation_mutex);
    std::lock_guard<std::mutex> lock(registry_mutex);
    unsigned installed=0;
    next.erase(std::remove_if(next.begin(),next.end(),[&](const auto& record) {
        if(!record->active || !Identity(*record) || sdk::read_value<int>(record->pawn+sdk::off::C_BaseEntity::m_iHealth)<=0) return true;
        if(Apply(*record,material)) ++installed;
        return false;
    }),next.end());
    for(const auto& old:records) if(std::find(next.begin(),next.end(),old)==next.end()) {old->active=false;Restore(*old);}
    records=std::move(next);
    snapshot.store(std::make_shared<const Records>(records));
    tracked=static_cast<unsigned>(records.size()); applied=installed;
    status=installed ? "Player material overrides active" :
        std::any_of(records.begin(),records.end(),[](const auto& r){return r->conflicted.load();}) ?
            "Override replaced externally; reapplication paused until chams is toggled" :
            "No eligible player scene objects / writable holders";
}

uintptr_t Draw(Generate original,uintptr_t desc,uintptr_t object,uintptr_t context,uintptr_t output) {
    detail::GenerationRead generation;
    // No process-memory reads, shared_ptr atomics or membership scans on this
    // engine-wide hot path. The read guard already pins the writer-owned records.
    static thread_local unsigned pending_callbacks=0;
    if(++pending_callbacks==64) {callbacks.fetch_add(64,std::memory_order_relaxed);pending_callbacks=0;}
    return original(desc,object,context,output);
}

void BeforeRemove(uintptr_t system,uintptr_t entity,uint32_t handle) {
    using namespace detail;
    std::unique_lock<std::shared_mutex> draws(generation_mutex,std::defer_lock);
    if(!generation_depth) draws.lock();
    std::lock_guard<std::mutex> lock(registry_mutex);
    for(const auto& record:records) if(record->system==system && record->pawn==entity && record->handle==handle) {
        record->active=false; Restore(*record);
    }
}

void Shutdown() {
    detail::stopping=true; hook_ready=false;
    std::lock_guard<std::mutex> stage_lock(detail::stage_mutex);
    std::unique_lock<std::shared_mutex> draws(detail::generation_mutex);
    std::lock_guard<std::mutex> lock(detail::registry_mutex);
    detail::RetireAll();
    detail::materials.clear(); detail::api={};
    detail::has_pending=false; detail::last_settings={};
    status="Stopped; owned overrides restored where writable";
}
}
