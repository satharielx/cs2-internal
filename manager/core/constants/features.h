#pragma once
#include "../../sdk/entity.h"
#include <cstdint>
namespace features::constants {
    inline constexpr uint64_t grenade_prediction_interval_ms = 33;
    inline constexpr uint64_t grenade_idle_refresh_ms = 100;
    inline constexpr float radians_to_degrees = 180.0f / 3.14159265358979323846f;
    inline constexpr int aim_bones[] = {sdk::BONE_HEAD, sdk::BONE_NECK, sdk::BONE_CHEST, sdk::BONE_PELVIS};
    inline constexpr uint64_t grenade_solid_mask = 0x1ULL | 0x2ULL | 0x40000ULL;
}
