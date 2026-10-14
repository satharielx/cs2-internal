#pragma once
#include "config.h"
#include "../sdk/entity.h"
#include <algorithm>
#include <atomic>

namespace lighting {
    inline std::atomic<const char*> status{"Not initialized"};

    // UpdateLightObject layout from Al1gn's reply:
    // https://www.unknowncheats.me/forum/counter-strike-2-a/751804-changing-light-color.html#post4673038
    inline constexpr uintptr_t color_offset = 0xE0 + 0x04;
    inline constexpr uintptr_t alpha_offset = 0xE0 + 0xAC;

    struct Settings {
        bool active = false, use_original = true;
        sdk::Vector3 color{};
        float night = 1;
    };
    inline Settings CaptureSettings() {
        Settings result;
        std::unique_lock<std::recursive_mutex> lock(config::mutex, std::try_to_lock);
        if (!lock.owns_lock() || (!config::lighting::enabled && !config::lighting::night_mode)) return result;
        result.use_original = !config::lighting::enabled;
        if (config::lighting::night_mode && std::isfinite(config::lighting::night_brightness))
            result.night = std::clamp(config::lighting::night_brightness, .02f, 1.0f);
        if (!result.use_original) {
            const sdk::Vector3 color{config::lighting::color[0], config::lighting::color[1], config::lighting::color[2]};
            if (!sdk::finite(color) || !std::isfinite(config::lighting::brightness)) return result;
            result.color = sdk::Vector3(std::clamp(color.x, 0.0f, 1.0f),
                std::clamp(color.y, 0.0f, 1.0f), std::clamp(color.z, 0.0f, 1.0f)) *
                (std::clamp(config::lighting::brightness, 0.0f, 5.0f) * result.night);
        }
        result.active = true;
        return result;
    }

    class ScopedColor {
        uintptr_t object_ = 0;
        sdk::Vector3 original_{};
        float alpha_ = 0;
    public:
        explicit ScopedColor(uintptr_t object) : ScopedColor(object, CaptureSettings()) {}
        ScopedColor(uintptr_t object, const Settings& settings) {
            if (!settings.active || !object || object > UINTPTR_MAX - alpha_offset - sizeof(float)) return;
            // Validate both fields once, then use guarded in-process copies.
            MEMORY_BASIC_INFORMATION region{};
            constexpr DWORD writable = PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
            if (!sdk::ProfileQuery(reinterpret_cast<void*>(object + color_offset), &region, sizeof(region)) ||
                region.State != MEM_COMMIT || (region.Protect & (PAGE_GUARD | PAGE_NOACCESS)) || !(region.Protect & writable)) return;
            const auto begin = reinterpret_cast<uintptr_t>(region.BaseAddress);
            if (region.RegionSize > UINTPTR_MAX - begin || object + alpha_offset + sizeof(float) > begin + region.RegionSize) return;
            if (!sdk::CopyLocalMemory(object + color_offset, &original_, sizeof(original_)) || !sdk::finite(original_) ||
                !sdk::CopyLocalMemory(object + alpha_offset, &alpha_, sizeof(alpha_)) || !std::isfinite(alpha_)) return;
            const auto color = settings.use_original ? original_ * settings.night : settings.color;
            if (!sdk::CopyLocalMemory(reinterpret_cast<uintptr_t>(&color), reinterpret_cast<void*>(object + color_offset), sizeof(color))) return;
            const float alpha = 1;
            if (!sdk::CopyLocalMemory(reinterpret_cast<uintptr_t>(&alpha), reinterpret_cast<void*>(object + alpha_offset), sizeof(alpha))) {
                sdk::CopyLocalMemory(reinterpret_cast<uintptr_t>(&original_), reinterpret_cast<void*>(object + color_offset), sizeof(original_));
                return;
            }
            object_ = object;
        }
        ~ScopedColor() {
            if (!object_) return;
            sdk::CopyLocalMemory(reinterpret_cast<uintptr_t>(&original_), reinterpret_cast<void*>(object_ + color_offset), sizeof(original_));
            sdk::CopyLocalMemory(reinterpret_cast<uintptr_t>(&alpha_), reinterpret_cast<void*>(object_ + alpha_offset), sizeof(alpha_));
        }
        ScopedColor(const ScopedColor&) = delete;
        ScopedColor& operator=(const ScopedColor&) = delete;
    };
}
