#pragma once
#include "entity.h"
#include <vector>
#include <cstring>
namespace sdk::world {
    struct Entity {
        uintptr_t entity = 0;
        uint32_t handle = UINT32_MAX;
        char name[96]{};
    };
    inline bool ReadName(uintptr_t address, char (&name)[96]) {
        if (!address) return false;
        for (size_t i = 0; i < sizeof(name); ++i) {
            if (!read_memory(address + i, name[i])) return false;
            if (!name[i]) return true;
        }
        return false;
    }
    inline bool IsProjectile(const char* name) { return std::strstr(name, "projectile") != nullptr; }
    inline bool IsSunLight(const char* name) {
        return !std::strcmp(name,"light_environment") || !std::strcmp(name,"light_directional");
    }
    inline bool IsTracked(const char* name) {
        return IsSunLight(name) || !std::strncmp(name, "weapon_", 7) || IsProjectile(name) ||
            !std::strcmp(name, "planted_c4") || !std::strcmp(name, "item_defuser");
    }
    inline std::vector<Entity> Discover(uintptr_t system, int highest) {
        std::vector<Entity> result;
        if (!system) return result;
        // Scan the normal entity range even if the highest-index offset is unavailable.
        const auto limit = std::clamp(highest, 2047, 0x7FFE);
        for (int index = 1; index <= limit; ++index) {
            Entity item; item.entity = entity_at(system, index);
            if (!item.entity) continue;
            const auto identity = read_value<uintptr_t>(item.entity + 0x10);
            if (!identity || !read_memory(identity + 0x10, item.handle) ||
                entity_from_handle(system, item.handle) != item.entity) continue;
            const auto name = read_value<uintptr_t>(identity + off::CEntityIdentity::m_designerName);
            if (ReadName(name, item.name) && IsTracked(item.name)) result.push_back(item);
        }
        return result;
    }
}
