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

namespace sdk {
    static std::vector<int> ida_pattern_to_bytes(const char* pattern) {
        std::vector<int> bytes = {};

        char* start = const_cast<char*>(pattern);
        const char* end = const_cast<char*>(pattern) + std::strlen(pattern);

        for (char* current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;
                if (*current == '?') {
                    ++current;
                }
                bytes.push_back(-1);
            }
            else if (*current == ' ') {
                continue;
            }
            else {
                bytes.push_back(std::strtoul(current, &current, 16));
            }
        }

        return bytes;
    }


    std::uint8_t* find_pattern(const char* module_name, const char* pattern) {
        const HMODULE module_handle = GetModuleHandleA(module_name);
        bool globalresult = false;
        if (module_handle == nullptr) {
            throw std::runtime_error(
                std::format("Module '{}' is not loaded", module_name));
        }

        const auto* dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
        if (dos_header->e_magic != IMAGE_DOS_SIGNATURE) {
            throw std::runtime_error(
                std::format("Invalid DOS signature in module '{}'", module_name));
        }

        const auto* nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(
            reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

        if (nt_headers->Signature != IMAGE_NT_SIGNATURE) {
            throw std::runtime_error(
                std::format("Invalid NT signature in module '{}'", module_name));
        }

        const DWORD image_size = nt_headers->OptionalHeader.SizeOfImage;
        auto* image_data = reinterpret_cast<std::uint8_t*>(module_handle);

        const std::vector<int> bytes = ida_pattern_to_bytes(pattern);
        const std::size_t pattern_size = bytes.size();
        const int* pattern_data = bytes.data();
        std::uint8_t* address = nullptr;
        for (std::size_t i = 0ul; i < image_size - pattern_size; ++i) {
            bool found = true;

            for (std::size_t j = 0ul; j < pattern_size; ++j) {
                if (image_data[i + j] != pattern_data[j] && pattern_data[j] != -1) {
                    found = false;
                    break;
                }
            }

            if (found) {
                globalresult = true;
                address = &image_data[i];
                break;
            }
        }

        if(!globalresult) throw std::runtime_error(
            std::format("Pattern '{}' not found in module '{}'", pattern, module_name));
        else {
            return address;
        }
       
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
