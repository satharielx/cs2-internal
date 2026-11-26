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
#include <cstring>

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

    std::uint8_t* find_pattern_in_image(std::uint8_t* image, std::size_t mapped_size, const char* pattern,
        bool require_unique) {
        const auto base = reinterpret_cast<uintptr_t>(image);
        auto contains = [&](std::size_t offset, std::size_t count) {
            return image && mapped_size <= UINTPTR_MAX - base && offset <= mapped_size &&
                count <= mapped_size - offset && is_readable_range(base + offset, count);
        };
        if (!contains(0, sizeof(IMAGE_DOS_HEADER))) return nullptr;
        IMAGE_DOS_HEADER dos{};
        std::memcpy(&dos, image, sizeof(dos));
        if (dos.e_magic != IMAGE_DOS_SIGNATURE || dos.e_lfanew < 0) return nullptr;
        const auto nt_offset = static_cast<std::size_t>(dos.e_lfanew);
        if (!contains(nt_offset, sizeof(IMAGE_NT_HEADERS))) return nullptr;
        IMAGE_NT_HEADERS nt{};
        std::memcpy(&nt, image + nt_offset, sizeof(nt));
        constexpr WORD machine = sizeof(void*) == 8 ? IMAGE_FILE_MACHINE_AMD64 : IMAGE_FILE_MACHINE_I386;
        if (nt.Signature != IMAGE_NT_SIGNATURE || nt.FileHeader.Machine != machine ||
            nt.OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR_MAGIC ||
            nt.FileHeader.SizeOfOptionalHeader < sizeof(IMAGE_OPTIONAL_HEADER) ||
            !nt.FileHeader.NumberOfSections || nt.FileHeader.NumberOfSections > 96 ||
            nt.OptionalHeader.SizeOfImage > mapped_size || !nt.OptionalHeader.SizeOfImage)
            return nullptr;
        const auto table = nt_offset + offsetof(IMAGE_NT_HEADERS, OptionalHeader) + nt.FileHeader.SizeOfOptionalHeader;
        const auto table_size = nt.FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER);
        if (!contains(table, table_size) || table > nt.OptionalHeader.SizeOfImage ||
            table_size > nt.OptionalHeader.SizeOfImage - table) return nullptr;
        // Validate all section extents before accepting any match.
        std::uint8_t* result = nullptr;
        for (unsigned i = 0; i < nt.FileHeader.NumberOfSections; ++i) {
            IMAGE_SECTION_HEADER section{};
            std::memcpy(&section, image + table + i * sizeof(section), sizeof(section));
            const auto size = section.Misc.VirtualSize ? section.Misc.VirtualSize : section.SizeOfRawData;
            if (section.VirtualAddress > nt.OptionalHeader.SizeOfImage ||
                size > nt.OptionalHeader.SizeOfImage - section.VirtualAddress) return nullptr;
        }
        for (unsigned i = 0; i < nt.FileHeader.NumberOfSections; ++i) {
            IMAGE_SECTION_HEADER section{};
            std::memcpy(&section, image + table + i * sizeof(section), sizeof(section));
            if (!(section.Characteristics & IMAGE_SCN_MEM_EXECUTE)) continue;
            const auto size = section.Misc.VirtualSize ? section.Misc.VirtualSize : section.SizeOfRawData;
            auto* begin = image + section.VirtualAddress;
            if (auto* match = find_pattern_in_range(begin, size, pattern)) {
                if (!require_unique) return match;
                if (result) return nullptr;
                result = match;
                const auto consumed = static_cast<std::size_t>(match - begin) + 1;
                if (find_pattern_in_range(match + 1, size - consumed, pattern)) return nullptr;
            }
        }
        return result;
    }

    static std::uint8_t* scan_loaded_image(const char* module_name, const char* pattern, bool unique) {
        if (!module_name || !*module_name) return nullptr;
        auto* module = reinterpret_cast<std::uint8_t*>(GetModuleHandleA(module_name));
        if (!module) return nullptr;
        const auto base = reinterpret_cast<uintptr_t>(module);
        auto end = base;
        // Derive the actual allocation extent instead of trusting SizeOfImage.
        for (;;) {
            MEMORY_BASIC_INFORMATION info{};
            if (!VirtualQuery(reinterpret_cast<void*>(end), &info, sizeof(info)) ||
                info.AllocationBase != module) break;
            const auto region = reinterpret_cast<uintptr_t>(info.BaseAddress);
            if (info.RegionSize > UINTPTR_MAX - region || region + info.RegionSize <= end) return nullptr;
            end = region + info.RegionSize;
        }
        return find_pattern_in_image(module, end - base, pattern, unique);
    }

    std::uint8_t* find_pattern(const char* module_name, const char* pattern) {
        return scan_loaded_image(module_name, pattern, false);
    }
    std::uint8_t* find_unique_pattern(const char* module_name, const char* pattern) {
        return scan_loaded_image(module_name, pattern, true);
    }

    std::uint8_t* resolve_absolute_rip_address(std::uint8_t* instruction,
        std::size_t offset_to_displacement,
        std::size_t instruction_size) {
        const auto address = reinterpret_cast<uintptr_t>(instruction);
        if (!address || instruction_size > UINTPTR_MAX - address ||
            offset_to_displacement > instruction_size ||
            sizeof(std::int32_t) > instruction_size - offset_to_displacement) return nullptr;
        std::int32_t displacement = 0;
        SIZE_T transferred = 0;
        if (!ProfileRead(GetCurrentProcess(), reinterpret_cast<void*>(address + offset_to_displacement),
            &displacement, sizeof(displacement), &transferred) || transferred != sizeof(displacement)) return nullptr;
        const auto next = address + instruction_size;
        const auto magnitude = displacement < 0 ? static_cast<uintptr_t>(-static_cast<int64_t>(displacement)) :
            static_cast<uintptr_t>(displacement);
        if (displacement < 0 ? magnitude > next : magnitude > UINTPTR_MAX - next) return nullptr;
        return reinterpret_cast<std::uint8_t*>(displacement < 0 ? next - magnitude : next + magnitude);
    }
}
