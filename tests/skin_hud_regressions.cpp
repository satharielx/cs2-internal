#include "../manager/core/skin_hud.h"
#include <array>
#include <cassert>
#include <iostream>
int main() {
    std::array<unsigned char, 0x200> hud{};
    const auto selection = reinterpret_cast<uintptr_t>(hud.data()) + 0x30;
    const auto tick = reinterpret_cast<uintptr_t>(hud.data()) + 0x98;
    static unsigned calls = 0;
    auto clear = +[](void* object, int index)->int {
        assert(index == 0);
        auto address = reinterpret_cast<uintptr_t>(object) + 0x50;
        sdk::write_memory(address, sdk::read_value<int>(address) - 1);
        ++calls;
        return -1;
    };
    assert(!skin_hud::Refresh(tick, clear) && calls == 0);
    sdk::write_memory(selection + 0x50, 3);
    sdk::write_memory(tick + 0x50, uint32_t{123});
    sdk::write_memory(tick + 0x44, 10.0f);
    assert(skin_hud::Refresh(tick, clear) && calls == 3);
    assert(sdk::read_value<int>(selection + 0x50) == 0);
    assert(sdk::read_value<uint32_t>(tick + 0x50) == UINT32_MAX);
    assert(sdk::read_value<float>(tick + 0x44) == 0);
    sdk::write_memory(selection + 0x50, 65);
    assert(!skin_hud::Refresh(tick, clear) && calls == 3);
    sdk::write_memory(selection + 0x50, 1);
    assert(!skin_hud::Refresh(tick, +[](void*, int)->int { return -1; }));
    assert(!skin_hud::Refresh(0, clear));
    std::cout << "Weapon HUD regressions passed\n";
}
