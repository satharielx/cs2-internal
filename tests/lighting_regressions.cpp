#include <Windows.h>
static unsigned queries = 0, reads = 0, writes = 0;
static SIZE_T CountQuery(LPCVOID address, PMEMORY_BASIC_INFORMATION info, SIZE_T size) {
    ++queries; return VirtualQuery(address, info, size);
}
static BOOL CountRead(HANDLE process, LPCVOID address, LPVOID target, SIZE_T size, SIZE_T* count) {
    ++reads; return ReadProcessMemory(process, address, target, size, count);
}
static BOOL CountWrite(HANDLE process, LPVOID address, LPCVOID source, SIZE_T size, SIZE_T* count) {
    ++writes; return WriteProcessMemory(process, address, source, size, count);
}
#define VirtualQuery CountQuery
#define ReadProcessMemory CountRead
#define WriteProcessMemory CountWrite
#include "../manager/core/lighting.h"
#undef VirtualQuery
#undef ReadProcessMemory
#undef WriteProcessMemory
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
    config::lighting::enabled = false;
    config::lighting::night_mode = true;
    config::lighting::night_brightness = .25f;
    config::lighting::color[0] = std::numeric_limits<float>::quiet_NaN();
    config::lighting::brightness = std::numeric_limits<float>::quiet_NaN();
    {
        lighting::ScopedColor night(object);
        const auto rgb = sdk::read_value<sdk::Vector3>(object + lighting::color_offset);
        assert(rgb.x == 1 && rgb.y == 1.25f && rgb.z == 1.5f);
    }
    assert(light == original);
    config::lighting::night_mode = false;
    config::lighting::enabled = true;
    config::lighting::color[0] = .2f;
    config::lighting::brightness = std::numeric_limits<float>::quiet_NaN();
    { lighting::ScopedColor invalid(object); assert(light == original); }
    config::lighting::brightness = 0;
    {
        lighting::ScopedColor black(object);
        assert(sdk::read_value<sdk::Vector3>(object + lighting::color_offset).Length() == 0);
    }
    assert(light == original);
    { lighting::ScopedColor null_light(0); lighting::ScopedColor overflow(UINTPTR_MAX); }
    const auto settings = lighting::CaptureSettings();
    queries = reads = writes = 0;
    for (int i = 0; i < 10000; ++i) { lighting::ScopedColor cached(object, settings); }
    assert(queries == 10000 && reads == 0 && writes == 0);
    assert(light == original);
    auto* readOnly = VirtualAlloc(nullptr, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    assert(readOnly);
    std::memcpy(readOnly, original.data(), original.size());
    DWORD previous = 0;
    assert(VirtualProtect(readOnly, 4096, PAGE_READONLY, &previous));
    { lighting::ScopedColor protectedLight(reinterpret_cast<uintptr_t>(readOnly), settings); }
    assert(std::memcmp(readOnly, original.data(), original.size()) == 0);
    VirtualFree(readOnly, 0, MEM_RELEASE);
    queries = 0;
    { lighting::ScopedColor disabled(object, lighting::Settings{}); }
    assert(queries == 0);
    std::cout << "Lighting regressions passed\n";
}
