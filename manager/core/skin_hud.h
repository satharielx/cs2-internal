#pragma once
#include "../sdk/entity.h"
#include <atomic>

namespace skin_hud {
    using Clear = int(__fastcall*)(void*, int);
    inline Clear clear = nullptr;
    inline std::atomic<unsigned> revision{0};
    inline std::atomic<unsigned> ticks{0}, clear_calls{0};
    inline std::atomic<const char*> status{"Not initialized"};
    inline void Request() { ++revision; }
    // HUD tick receives the +0x98 secondary base. Native cleanup receives
    // primary base +0x30 (tick this -0x68), with count +0x50.
    inline bool Refresh(uintptr_t tickThis, Clear fn) {
        if (!fn || tickThis < 0x68) return false;
        const auto selection = tickThis - 0x68;
        int count = 0;
        if (!sdk::read_memory(selection + 0x50, count) || count < 0 || count > 64) return false;
        if (count == 0) { status = "Waiting for populated weapon HUD entries"; return false; }
        for (int remaining = count; remaining > 0; --remaining) {
            fn(reinterpret_cast<void*>(selection), 0);
            ++clear_calls;
            int next = 0;
            if (!sdk::read_memory(selection + 0x50, next) || next != remaining - 1) return false;
        }
        // Force the normal tick's active-weapon/name update and bypass its timer.
        return sdk::write_memory(tickThis + 0x50, uint32_t{UINT32_MAX})
            && sdk::write_memory(tickThis + 0x44, 0.0f);
    }
    inline bool GuardedRefresh(uintptr_t tickThis) {
        __try { return Refresh(tickThis, clear); }
        __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ||
            GetExceptionCode() == EXCEPTION_IN_PAGE_ERROR ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
            clear = nullptr;
            status = "Weapon HUD refresh faulted; disabled until reload";
            return false;
        }
    }
}
