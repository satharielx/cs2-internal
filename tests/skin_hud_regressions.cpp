#include "../manager/core/skin_hud.h"
#include <array>
#include <cassert>
#include <iostream>
int main() {
    std::array<unsigned char,0x200> instructions{};
    assert(!skin_hud::LayoutVerified(reinterpret_cast<uintptr_t>(instructions.data())));
    constexpr unsigned char primary[]={0x48,0x8d,0xab,0x68,0xff,0xff,0xff};
    std::memcpy(instructions.data()+0x149,primary,sizeof(primary));
    assert(skin_hud::LayoutVerified(reinterpret_cast<uintptr_t>(instructions.data())));
    std::array<unsigned char, 0x200> hud{};
    const auto selection = reinterpret_cast<uintptr_t>(hud.data());
    const auto tick = reinterpret_cast<uintptr_t>(hud.data()) + 0x98;
    static unsigned calls = 0;
    auto clear = +[](void* object, int index)->int {
        assert(index == 0);
        auto address = reinterpret_cast<uintptr_t>(object) + 0x50;
        sdk::write_memory(address, sdk::read_value<int>(address) - 1);
        ++calls;
        return -1;
    };
    assert(skin_hud::Refresh(tick, clear) && calls == 0);
    assert(sdk::read_value<uint32_t>(tick + 0x50) == UINT32_MAX);
    sdk::write_memory(selection + 0x50, 3);
    sdk::write_memory(tick + 0x50, uint32_t{123});
    sdk::write_memory(tick + 0x44, 10.0f);
    assert(skin_hud::Refresh(tick, clear) && calls == 0);
    assert(sdk::read_value<int>(selection + 0x50) == 3);
    assert(sdk::read_value<uint32_t>(tick + 0x50) == UINT32_MAX);
    assert(sdk::read_value<float>(tick + 0x44) == 0);
    sdk::write_memory(selection + 0x50, 65);
    assert(!skin_hud::Refresh(tick, clear) && calls == 0);
    sdk::write_memory(selection + 0x50, 1);
    assert(skin_hud::Refresh(tick, +[](void*, int)->int { assert(false);return -1; }));
    assert(skin_hud::Refresh(tick,nullptr));
    assert(!skin_hud::Refresh(0, clear));
    skin_hud::RefreshTracker tracker;
    assert(tracker.Pending(tick, 3)); tracker.Acknowledge(3);
    assert(!tracker.Pending(tick,3) && tracker.Pending(tick,4));
    assert(tracker.Pending(tick+8,3)); // New HUD object must receive the current request.
    std::cout << "Weapon HUD regressions passed\n";
}
