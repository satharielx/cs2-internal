#pragma once
#include "mem.h"
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstring>
#include <type_traits>

namespace sdk {
    template<class T> bool read_memory(uintptr_t address, T& value) {
        static_assert(std::is_trivially_copyable_v<T>);
        T copy{};
        SIZE_T transferred = 0;
        if (!address || !ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<const void*>(address),
            &copy, sizeof(copy), &transferred) || transferred != sizeof(copy)) return false;
        std::memcpy(&value, &copy, sizeof(value));
        return true;
    }
    template<class T> T read_value(uintptr_t address) {
        T value{};
        read_memory(address, value);
        return value;
    }
    template<class T> bool write_memory(uintptr_t address, const T& value) {
        static_assert(std::is_trivially_copyable_v<T>);
        MEMORY_BASIC_INFORMATION info{};
        if (!VirtualQuery(reinterpret_cast<void*>(address), &info, sizeof(info)) ||
            info.State != MEM_COMMIT || (info.Protect & (PAGE_NOACCESS | PAGE_GUARD)) ||
            !(info.Protect & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) return false;
        const uintptr_t end = reinterpret_cast<uintptr_t>(info.BaseAddress) + info.RegionSize;
        if (address > end || sizeof(value) > end - address) return false;
        SIZE_T transferred = 0;
        return WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(address), &value,
            sizeof(value), &transferred) && transferred == sizeof(value);
    }

    struct VectorView { int count = 0; uintptr_t data = 0; };
    inline bool read_vector(uintptr_t address, VectorView& view, int limit) {
        view = {};
        int count = 0, capacity = 0;
        uintptr_t data = 0;
        if (!address || !read_memory(address, count) ||
            !read_memory(address + 8, data) || !read_memory(address + 16, capacity) ||
            count < 0 || count > limit || capacity < count || (count && !data)) return false;
        view = {count, data};
        return true;
    }

    using EntityLookup = void* (__fastcall*)(void*, int);
    inline std::atomic<EntityLookup> entity_lookup{nullptr};
    inline uintptr_t entity_at(uintptr_t system, int index) {
        if (!system || index < 0 || index >= 0x7FFF) return 0;
        if (auto lookup = entity_lookup.load())
            return reinterpret_cast<uintptr_t>(lookup(reinterpret_cast<void*>(system), index));
        const auto chunk = read_value<uintptr_t>(system + 0x10 + sizeof(uintptr_t) * (index >> 9));
        return chunk ? read_value<uintptr_t>(chunk + 0x70 * (index & 0x1FF)) : 0;
    }
    inline uintptr_t entity_from_handle(uintptr_t system, uint32_t handle) {
        if (!system || handle == UINT32_MAX) return 0;
        const uintptr_t entity = entity_at(system, handle & 0x7FFF);
        const uintptr_t identity = entity ? read_value<uintptr_t>(entity + 0x10) : 0;
        uint32_t current = UINT32_MAX;
        return identity && read_memory(identity + 0x10, current) && current == handle ? entity : 0;
    }
    inline bool spotted_by(const uint32_t (&mask)[2], int controller_index) {
        if (controller_index < 1 || controller_index > 64) return false;
        const unsigned bit = static_cast<unsigned>(controller_index - 1);
        return (mask[bit / 32] & (uint32_t{1} << (bit % 32))) != 0;
    }
    inline float normalize_yaw(float angle) {
        return std::isfinite(angle) ? std::remainder(angle, 360.0f) : 0.0f;
    }
    inline float clamp_pitch(float angle) {
        return std::isfinite(angle) ? (std::clamp)(angle, -89.0f, 89.0f) : 0.0f;
    }
}
