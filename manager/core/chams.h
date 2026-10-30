#pragma once
#include <Windows.h>
#include <atomic>
#include <cstdint>

namespace chams {
    // Four register arguments observed in the installed implementation. Preserve
    // its opaque RAX return instead of assigning semantic meaning to it.
    using Generate = uintptr_t(__fastcall*)(uintptr_t,uintptr_t,uintptr_t,uintptr_t);
    inline std::atomic<const char*> status{"Not initialized"};
    inline std::atomic<bool> hook_ready{false};
    inline std::atomic<unsigned> tracked{0}, applied{0}, material_builds{0};
    inline std::atomic<unsigned> holder_writes{0}, slot_updates{0}, conflicts{0}, writes_per_second{0};
    inline std::atomic<unsigned long long> callbacks{0}, matched{0};
    uintptr_t Initialize();
    void OnFrameStage(int stage);
    uintptr_t Draw(Generate original,uintptr_t desc,uintptr_t object,uintptr_t context,uintptr_t output);
    void BeforeRemove(uintptr_t system,uintptr_t entity,uint32_t handle);
    void Shutdown();
}
