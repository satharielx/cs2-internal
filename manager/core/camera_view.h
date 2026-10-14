#pragma once
#include "../sdk/entity.h"
#include <atomic>

namespace camera_view {
    // Installed client SHA-256 7081d87f...: camera CalcView receives this
    // address as its angle output. The old 0x4F8 is an integer output now.
    inline constexpr uintptr_t angle_offset = 0x4B8;
    using GetAngles = const sdk::Vector3*(__fastcall*)(void*, int);
    inline GetAngles get_angles = nullptr;
    inline uintptr_t input_slot = 0;
    inline std::atomic<const char*> status{"Not initialized"};

    inline bool Apply(uintptr_t setup, uintptr_t input, GetAngles getter) {
        if (!setup || !input || !getter || setup > UINTPTR_MAX - angle_offset - sizeof(sdk::Vector3)) return false;
        const auto source = getter(reinterpret_cast<void*>(input), 0);
        sdk::ScopedLocalReads reads;
        sdk::Vector3 angles{};
        if (!source || !sdk::read_memory(reinterpret_cast<uintptr_t>(source), angles) || !sdk::finite(angles)) return false;
        const auto destination = setup + angle_offset;
        sdk::Vector3 current{};
        if (!sdk::read_memory(destination, current)) return false;
        if (current.x == angles.x && current.y == angles.y && current.z == angles.z) return true;
        MEMORY_BASIC_INFORMATION region{};
        constexpr DWORD writable = PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
        if (!sdk::ProfileQuery(reinterpret_cast<void*>(destination), &region, sizeof(region)) ||
            region.State != MEM_COMMIT || (region.Protect & (PAGE_GUARD | PAGE_NOACCESS)) || !(region.Protect & writable)) return false;
        const auto begin = reinterpret_cast<uintptr_t>(region.BaseAddress);
        if (region.RegionSize > UINTPTR_MAX - begin || destination + sizeof(angles) > begin + region.RegionSize) return false;
        return sdk::CopyLocalMemory(reinterpret_cast<uintptr_t>(&angles), reinterpret_cast<void*>(destination), sizeof(angles));
    }
    inline bool GuardedApply(uintptr_t setup, uintptr_t input) {
        __try { return Apply(setup, input, get_angles); }
        __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ||
            GetExceptionCode() == EXCEPTION_IN_PAGE_ERROR ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
            return false;
        }
    }
}
