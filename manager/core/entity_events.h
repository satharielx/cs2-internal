#pragma once
#include <array>
#include <vector>
#include "../sdk/entity.h"
#include <mutex>
#include <atomic>
#include "../sdk/feature_support.h"
namespace entity_events {
    struct Entry { uintptr_t pointer=0; uint32_t handle=UINT32_MAX; };
    inline std::mutex mutex;
    inline std::array<Entry,65> controllers{};
    inline std::array<Entry,32768> entities{};
    inline uintptr_t owner=0;
    inline bool seeded=false;
    inline std::atomic<bool> enabled{false};
    inline std::atomic<unsigned long long> revision{0}, hits{0}, rebuilds{0};
    inline void Reset() {
        enabled=false;
        std::lock_guard<std::mutex> lock(mutex);
        controllers={}; entities={}; owner=0; seeded=false; ++revision;
    }
    inline void Added(uintptr_t system,uintptr_t entity,uint32_t handle) {
        if(!enabled || !system || !entity || handle==UINT32_MAX) return;
        std::lock_guard<std::mutex> lock(mutex);
        if(!owner) owner=system;
        if(system!=owner) return;
        const unsigned index=handle&0x7FFF;
        entities[index]={entity,handle};
        if(index>=1 && index<=64) controllers[index]={entity,handle};
        ++revision; // Pawn creation also invalidates target geometry/health.
    }
    inline void Removed(uintptr_t system,uintptr_t entity,uint32_t handle) {
        if(!enabled || !system || handle==UINT32_MAX) return;
        std::lock_guard<std::mutex> lock(mutex);
        if(system!=owner) return;
        const unsigned index=handle&0x7FFF;
        if(index>=1 && index<=64 && controllers[index].pointer==entity && controllers[index].handle==handle)
            controllers[index]={};
        if(entities[index].pointer==entity && entities[index].handle==handle) entities[index]={};
        ++revision;
    }
    inline uintptr_t Resolve(uintptr_t system,uint32_t handle) {
        if(!enabled || handle==UINT32_MAX) return 0;
        std::lock_guard<std::mutex> lock(mutex);
        if(owner!=system) return 0;
        const auto& entry=entities[handle&0x7FFF];
        return entry.handle==handle ? entry.pointer : 0;
    }
    inline bool Snapshot(uintptr_t system,std::array<Entry,65>& result) {
        if(!enabled || !system) return false;
        unsigned long long version;
        {
            std::unique_lock<std::mutex> lock(mutex,std::try_to_lock);
            if(!lock.owns_lock()) return false;
            if(owner!=system) { owner=system; controllers={}; entities={}; seeded=false; ++revision; }
            if(seeded) { result=controllers; ++hits; return true; }
            version=revision.load();
        }
        // One bootstrap for entities that predate hook activation. No registry
        // lock is held during engine lookup (it may invoke lifecycle callbacks).
        std::array<Entry,65> fresh{};
        std::vector<Entry> bootstrap;
        auto remember=[&](uint32_t h) {
            const auto ptr=sdk::entity_from_handle(system,h);
            if(ptr) bootstrap.push_back({ptr,h});
            return ptr;
        };
        for(int i=1;i<=64;++i) {
            const auto entity=sdk::entity_at(system,i);
            const auto identity=entity?sdk::read_value<uintptr_t>(entity+0x10):0;
            uint32_t handle=UINT32_MAX;
            if(identity && sdk::read_memory(identity+0x10,handle) && handle!=UINT32_MAX && (handle&0x7FFF)==static_cast<unsigned>(i))
            {
                fresh[i]={entity,handle}; bootstrap.push_back(fresh[i]);
                const auto pawn=remember(sdk::read_value<uint32_t>(entity+sdk::off::CCSPlayerController::m_hPlayerPawn));
                const auto services=pawn?sdk::read_value<uintptr_t>(pawn+sdk::off::C_BasePlayerPawn::m_pWeaponServices):0;
                if(services) remember(sdk::read_value<uint32_t>(services+sdk::off::CPlayer_WeaponServices::m_hActiveWeapon));
            }
        }
        std::unique_lock<std::mutex> lock(mutex,std::try_to_lock);
        if(!lock.owns_lock() || owner!=system || version!=revision.load()) return false;
        for(const auto& entry:bootstrap) entities[entry.handle&0x7FFF]=entry;
        controllers=fresh; seeded=true;
        result=fresh; ++rebuilds;
        return true;
    }
}
