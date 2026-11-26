#pragma once
#include "../sdk/entity.h"
#include <atomic>
#include <algorithm>

namespace skin_hud {
    using Clear = int(__fastcall*)(void*, int);
    inline Clear clear = nullptr;
    inline std::atomic<unsigned> revision{0};
    inline std::atomic<unsigned> ticks{0}, clear_calls{0};
    inline std::atomic<unsigned> completed{0};
    inline std::atomic<const char*> status{"Not initialized"};
    inline void Request() { ++revision; }
    struct RefreshTracker {
        uintptr_t owner = 0;
        unsigned applied = 0;
        bool Pending(uintptr_t hud, unsigned requested) {
            if (owner != hud) { owner = hud; applied = requested ? requested-1 : 0; }
            return requested != applied;
        }
        void Acknowledge(unsigned requested) { applied = requested; completed = requested; }
    };
    inline RefreshTracker tracker;
    inline bool Invalidate(uintptr_t tickThis) {
        return sdk::write_memory(tickThis + 0x50, uint32_t{UINT32_MAX})
            && sdk::write_memory(tickThis + 0x44, 0.0f);
    }
    inline bool LayoutVerified(uintptr_t tick_entry) {
        unsigned char code[0x200]{};
        if(!sdk::read_memory(tick_entry,code)) return false;
        constexpr unsigned char primary[]={0x48,0x8d,0xab,0x68,0xff,0xff,0xff};
        return std::search(std::begin(code),std::end(code),std::begin(primary),std::end(primary))!=std::end(code);
    }
    // Build 26598758: the tick passes this -0x98 to the selection updater;
    // that updater passes the same primary base to ClearWeaponHUD. Count +0x50.
    inline bool Refresh(uintptr_t tickThis, Clear fn) {
        if (tickThis < 0x98) return false;
        const auto selection = tickThis - 0x98;
        int count = 0;
        if (!sdk::read_memory(selection + 0x50, count) || count < 0 || count > 64) return false;
        // ClearWeaponHUD removes native weapon/panel ownership. Calling it for
        // a name change makes the selection updater create duplicate entries.
        // Preserve every entry and let the native active-weapon path refresh.
        (void)fn;
        return Invalidate(tickThis);
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
