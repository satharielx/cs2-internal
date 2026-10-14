#pragma once
#include "../sdk/entity.h"
#include <map>
#include <vector>
namespace grenade_trails {
    struct Trail { std::vector<sdk::Vector3> points; uint64_t seen = 0, sampled = 0; };
    struct Cache {
        std::map<uint32_t, Trail> trails;
        void Prune(uint64_t now) {
            for (auto it = trails.begin(); it != trails.end();)
                if (now - it->second.seen > 2000) it = trails.erase(it); else ++it;
        }
        void Sample(uint32_t handle, sdk::Vector3 position, uint64_t now) {
            if (!sdk::finite(position) || handle == UINT32_MAX) return;
            if (!trails.count(handle) && trails.size() >= 64) return;
            auto& trail = trails[handle]; trail.seen = now;
            if (!trail.points.empty() && (now - trail.sampled < 16 || trail.points.back().Distance(position) < 1)) return;
            if (trail.points.size() >= 256) trail.points.erase(trail.points.begin());
            trail.points.push_back(position); trail.sampled = now;
        }
    };
}
