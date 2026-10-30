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
        static thread_local Settings previous;
        Settings result;
        std::unique_lock<std::recursive_mutex> lock(config::mutex, std::try_to_lock);
        // Contention must not switch individual lights back to daylight.
        if (!lock.owns_lock()) return previous;
        previous = {};
        if (!config::lighting::enabled && !config::lighting::night_mode) return previous;
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
        previous = result;
        return previous;
    }

    inline bool WritableLight(uintptr_t object) {
        // Light updates arrive in batches. Reuse the last region for at most 8 ms;
        // actual copies remain guarded against protection changes and stale pointers.
        static thread_local MEMORY_BASIC_INFORMATION cached{};
        static thread_local ULONGLONG expires = 0;
        const auto now = GetTickCount64();
        const auto address = object + color_offset;
        const auto end = object + alpha_offset + sizeof(float);
        auto begin = reinterpret_cast<uintptr_t>(cached.BaseAddress);
        if (now >= expires || address < begin || cached.RegionSize > UINTPTR_MAX - begin ||
            end > begin + cached.RegionSize) {
            cached = {};
            if (!sdk::ProfileQuery(reinterpret_cast<void*>(address), &cached, sizeof(cached))) return false;
            expires = now + 8;
            begin = reinterpret_cast<uintptr_t>(cached.BaseAddress);
        }
        constexpr DWORD writable = PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
        return cached.State == MEM_COMMIT && !(cached.Protect & (PAGE_GUARD | PAGE_NOACCESS)) &&
            (cached.Protect & writable) && cached.RegionSize <= UINTPTR_MAX - begin &&
            end <= begin + cached.RegionSize;
    }

    class ScopedColor {
        uintptr_t object_ = 0;
        sdk::Vector3 original_{};
        float alpha_ = 0;
        ScopedColor* parent_ = nullptr;
        inline static thread_local ScopedColor* current_ = nullptr;
    public:
        explicit ScopedColor(uintptr_t object) : ScopedColor(object, CaptureSettings()) {}
        ScopedColor(uintptr_t object, const Settings& settings) {
            if (!settings.active || !object || object > UINTPTR_MAX - alpha_offset - sizeof(float)) return;
            for (auto* scope = current_; scope; scope = scope->parent_)
                if (scope->object_ == object) return; // Already tinted by an outer callback.
            if (!WritableLight(object)) return;
            if (!sdk::CopyLocalMemory(object + color_offset, &original_, sizeof(original_)) || !sdk::finite(original_) ||
                !sdk::CopyLocalMemory(object + alpha_offset, &alpha_, sizeof(alpha_)) || !std::isfinite(alpha_)) return;
            const auto color = settings.use_original ? original_ * settings.night : settings.color;
            if (!sdk::CopyLocalMemory(reinterpret_cast<uintptr_t>(&color), reinterpret_cast<void*>(object + color_offset), sizeof(color))) return;
            const float alpha = settings.use_original ? alpha_ : 1;
            if (!sdk::CopyLocalMemory(reinterpret_cast<uintptr_t>(&alpha), reinterpret_cast<void*>(object + alpha_offset), sizeof(alpha))) {
                sdk::CopyLocalMemory(reinterpret_cast<uintptr_t>(&original_), reinterpret_cast<void*>(object + color_offset), sizeof(original_));
                return;
            }
            object_ = object;
            parent_ = current_;
            current_ = this;
        }
        ~ScopedColor() {
            if (!object_) return;
            current_ = parent_;
            sdk::CopyLocalMemory(reinterpret_cast<uintptr_t>(&original_), reinterpret_cast<void*>(object_ + color_offset), sizeof(original_));
            sdk::CopyLocalMemory(reinterpret_cast<uintptr_t>(&alpha_), reinterpret_cast<void*>(object_ + alpha_offset), sizeof(alpha_));
        }
        ScopedColor(const ScopedColor&) = delete;
        ScopedColor& operator=(const ScopedColor&) = delete;
    };
}
