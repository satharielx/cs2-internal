#include "../manager/core/lighting.h"
#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>

int main() {
    std::array<unsigned char, 0x200> light{};
    const auto object = reinterpret_cast<uintptr_t>(light.data());
    sdk::write_memory(object + lighting::color_offset, sdk::Vector3(4, 5, 6));
    sdk::write_memory(object + lighting::alpha_offset, 0.5f);
    const auto original = light;
    { lighting::ScopedColor disabled(object); assert(light == original); }
    config::lighting::enabled = true;
    config::lighting::color[0] = 0.2f;
    config::lighting::color[1] = 0.4f;
    config::lighting::color[2] = 0.8f;
    config::lighting::brightness = 2;
    try {
        lighting::ScopedColor active(object);
        const auto rgb = sdk::read_value<sdk::Vector3>(object + lighting::color_offset);
        assert(rgb.x == 0.4f && rgb.y == 0.8f && rgb.z == 1.6f);
        assert(sdk::read_value<float>(object + lighting::alpha_offset) == 1);
        auto expected = original;
        std::memcpy(expected.data() + lighting::color_offset, &rgb, sizeof(rgb));
        const float alpha = 1;
        std::memcpy(expected.data() + lighting::alpha_offset, &alpha, sizeof(alpha));
        assert(light == expected);
        { lighting::ScopedColor nested(object); }
        assert(light == expected);
        throw 1;
    } catch (int) {}
    assert(light == original);
    config::lighting::brightness = std::numeric_limits<float>::quiet_NaN();
    { lighting::ScopedColor invalid(object); assert(light == original); }
    config::lighting::brightness = 0;
    {
        lighting::ScopedColor black(object);
        assert(sdk::read_value<sdk::Vector3>(object + lighting::color_offset).Length() == 0);
    }
    assert(light == original);
    { lighting::ScopedColor null_light(0); lighting::ScopedColor overflow(UINTPTR_MAX); }
    std::cout << "Lighting regressions passed\n";
}
