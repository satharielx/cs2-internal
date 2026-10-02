#pragma once
#include <atomic>
#include <cstdint>

namespace skybox {
    inline std::atomic<const char*> color_status{"Not initialized"};
    class ScopedColor {
        uintptr_t address_ = 0;
        float original_[3]{};
    public:
        ScopedColor(uintptr_t draw_data, int count);
        ~ScopedColor();
        ScopedColor(const ScopedColor&) = delete;
        ScopedColor& operator=(const ScopedColor&) = delete;
    };
    inline std::atomic<unsigned> world_revision{0};
    inline std::atomic<const char*> status{"Not initialized"};
    bool Initialize();
    void OnFrameStage(int stage);
    void RestoreBeforeUnload();
}
