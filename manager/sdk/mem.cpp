/*
Module Name: Memory pattern scanning and RIP address resolution
Authors: sathariel, martinmarinov
Product: Nephilimgate Multicheat
Tools used: imgui, a2x-cs2dumper
© 2026 sathariel & martinmarinov
*/

#include "mem.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <stdint.h>

#include <vector>
#include <stdexcept>
#include <format>
#include <algorithm>
#include <cctype>

namespace sdk {
    static int hex_digit(unsigned char c) {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        return -1;
    }

    static std::vector<int> ida_pattern_to_bytes(const char* pattern) {
        std::vector<int> bytes;
        if (!pattern) return bytes;
        while (*pattern) {
            while (std::isspace(static_cast<unsigned char>(*pattern))) ++pattern;
            if (!*pattern) break;
            if (*pattern == '?') {
                ++pattern;
                if (*pattern == '?') ++pattern;
                bytes.push_back(-1);
            }
            else {
                const int high = hex_digit(static_cast<unsigned char>(*pattern++));
                if (high < 0 || !*pattern) return {};
                const int low = hex_digit(static_cast<unsigned char>(*pattern++));
                if (low < 0) return {};
                bytes.push_back((high << 4) | low);
            }
            if (*pattern && !std::isspace(static_cast<unsigned char>(*pattern))) return {};
        }
        return bytes;
    }

    std::uint8_t* find_pattern_in_range(std::uint8_t* data, std::size_t size, const char* pattern) {
        const auto bytes = ida_pattern_to_bytes(pattern);
        const uintptr_t begin = reinterpret_cast<uintptr_t>(data);
        if (!data || bytes.empty() || bytes.size() > size || size > UINTPTR_MAX - begin)
            return nullptr;
        const uintptr_t end = begin + size;
        const uintptr_t last = end - bytes.size();
        uintptr_t cursor = begin;
        while (cursor <= last) {
            MEMORY_BASIC_INFORMATION info{};
            if (!VirtualQuery(reinterpret_cast<void*>(cursor), &info, sizeof(info))) break;
            const uintptr_t base = reinterpret_cast<uintptr_t>(info.BaseAddress);
            if (info.RegionSize > UINTPTR_MAX - base) break;
            const uintptr_t region_end = (std::min)(end, base + info.RegionSize);
            if (region_end <= cursor) break;
            if (is_readable_range(cursor, 1)) {
                for (uintptr_t candidate = cursor; candidate < region_end && candidate <= last; ++candidate) {
                    // A match may straddle readable pages with different protections.
                    if (bytes.size() > region_end - candidate &&
                        !is_readable_range(candidate, bytes.size())) break;
                    auto* address = reinterpret_cast<std::uint8_t*>(candidate);
                    std::size_t i = 0;
                    for (; i < bytes.size(); ++i) {
                        if (bytes[i] != -1 && address[i] != bytes[i]) break;
                    }
                    if (i == bytes.size()) return address;
                }
            }
            cursor = region_end;
        }
        return nullptr;
    }

    std::uint8_t* find_pattern(const char* module_name, const char* pattern) {
        if (!module_name || !*module_name) return nullptr;
        const HMODULE module = GetModuleHandleA(module_name);
        const uintptr_t base = reinterpret_cast<uintptr_t>(module);
        if (!is_readable_range(base, sizeof(IMAGE_DOS_HEADER))) return nullptr;
        const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
        if (dos->e_magic != IMAGE_DOS_SIGNATURE || dos->e_lfanew < 0 ||
            static_cast<uintptr_t>(dos->e_lfanew) > UINTPTR_MAX - base) return nullptr;
        const uintptr_t nt_address = base + dos->e_lfanew;
        if (!is_readable_range(nt_address, sizeof(IMAGE_NT_HEADERS))) return nullptr;
        const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(nt_address);
        if (nt->Signature != IMAGE_NT_SIGNATURE ||
            nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR_MAGIC) return nullptr;
        return find_pattern_in_range(reinterpret_cast<std::uint8_t*>(module),
            nt->OptionalHeader.SizeOfImage, pattern);
    }

    std::uint8_t* resolve_absolute_rip_address(std::uint8_t* instruction,
        std::size_t offset_to_displacement,
        std::size_t instruction_size) {
        if (!instruction) {
            throw std::runtime_error("Null instruction pointer in RIP resolution");
        }

        auto displacement = *reinterpret_cast<const std::int32_t*>(
            reinterpret_cast<std::uintptr_t>(instruction) + offset_to_displacement);

        return instruction + instruction_size + displacement;
    }
}
