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

    class ScopedColor {
        uintptr_t object_ = 0;
        sdk::Vector3 original_{};
        float alpha_ = 0;
    public:
        explicit ScopedColor(uintptr_t object) {
            if (!object || object > UINTPTR_MAX - alpha_offset - sizeof(float)) return;
            sdk::Vector3 color;
            bool useOriginal = false;
            float night = 1;
            {
                std::unique_lock<std::recursive_mutex> lock(config::mutex, std::try_to_lock);
                if (!lock.owns_lock() || (!config::lighting::enabled && !config::lighting::night_mode)) return;
                useOriginal = !config::lighting::enabled;
                if (config::lighting::night_mode && std::isfinite(config::lighting::night_brightness)) night = std::clamp(config::lighting::night_brightness, .02f, 1.0f);
                color = {config::lighting::color[0], config::lighting::color[1], config::lighting::color[2]};
                if (!sdk::finite(color) || !std::isfinite(config::lighting::brightness)) return;
                const float brightness = std::clamp(config::lighting::brightness, 0.0f, 5.0f);
                color = sdk::Vector3(std::clamp(color.x, 0.0f, 1.0f),
                    std::clamp(color.y, 0.0f, 1.0f), std::clamp(color.z, 0.0f, 1.0f)) * brightness;
            }
            if (!sdk::read_memory(object + color_offset, original_) || !sdk::finite(original_) ||
                !sdk::read_memory(object + alpha_offset, alpha_) || !std::isfinite(alpha_)) return;
            color = (useOriginal ? original_ : color) * night;
            if (!sdk::write_memory(object + color_offset, color)) return;
            if (!sdk::write_memory(object + alpha_offset, 1.0f)) {
                sdk::write_memory(object + color_offset, original_);
                return;
            }
            object_ = object;
        }
        ~ScopedColor() {
            if (!object_) return;
            sdk::write_memory(object_ + color_offset, original_);
            sdk::write_memory(object_ + alpha_offset, alpha_);
        }
        ScopedColor(const ScopedColor&) = delete;
        ScopedColor& operator=(const ScopedColor&) = delete;
    };
}
