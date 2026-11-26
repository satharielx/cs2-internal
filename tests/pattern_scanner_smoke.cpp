#include "../manager/sdk/mem.h"
#include <cstdio>
#include <cstring>
#include <stdexcept>

static void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

static void image_checks() {
    auto* image = static_cast<std::uint8_t*>(VirtualAlloc(nullptr, 0x4000, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
    check(image != nullptr, "Allocate synthetic mapped image");
    try {
        auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(image);
        dos->e_magic = IMAGE_DOS_SIGNATURE;
        dos->e_lfanew = 0x100;
        auto* nt = reinterpret_cast<IMAGE_NT_HEADERS*>(image + 0x100);
        nt->Signature = IMAGE_NT_SIGNATURE;
        nt->FileHeader.Machine = IMAGE_FILE_MACHINE_AMD64;
        nt->FileHeader.SizeOfOptionalHeader = sizeof(IMAGE_OPTIONAL_HEADER);
        nt->FileHeader.NumberOfSections = 2;
        nt->OptionalHeader.Magic = IMAGE_NT_OPTIONAL_HDR_MAGIC;
        nt->OptionalHeader.SizeOfImage = 0x4000;
        auto* sections = IMAGE_FIRST_SECTION(nt);
        sections[0].VirtualAddress = 0x1000;
        sections[0].Misc.VirtualSize = 0x1000;
        sections[0].Characteristics = IMAGE_SCN_MEM_READ;
        sections[1].VirtualAddress = 0x2000;
        sections[1].Misc.VirtualSize = 0x1000;
        sections[1].Characteristics = IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE;
        std::memcpy(image + 0x1100, "\xDE\xAD\xBE\xEF", 4);
        std::memcpy(image + 0x2100, "\xDE\xAD\xBE\xEF", 4);
        check(sdk::find_pattern_in_image(image, 0x4000, "DE AD BE EF") == image + 0x2100,
            "Ignore identical signature in non-executable data");
        check(sdk::find_pattern_in_image(image, 0x4000, "DE AD BE EF", true) == image + 0x2100,
            "Unique code match ignores duplicate data");
        std::memcpy(image + 0x2200, "\xDE\xAD\xBE\xEF", 4);
        check(!sdk::find_pattern_in_image(image, 0x4000, "DE AD BE EF", true), "Reject ambiguous code matches");
        image[0x2200] = 0;
        sections[0].Characteristics |= IMAGE_SCN_MEM_EXECUTE;
        check(!sdk::find_pattern_in_image(image, 0x4000, "DE AD BE EF", true), "Reject ambiguity across code sections");
        sections[0].Characteristics = IMAGE_SCN_MEM_READ;
        image[0x2100] = 0;
        check(!sdk::find_pattern_in_image(image, 0x4000, "DE AD BE EF"), "Data-only signature cannot become code");
        image[0x2fff] = 0xAB; image[0x3000] = 0xCD;
        check(!sdk::find_pattern_in_image(image, 0x4000, "AB CD"), "Do not cross a section boundary");
        nt->FileHeader.Machine = IMAGE_FILE_MACHINE_I386;
        check(!sdk::find_pattern_in_image(image, 0x4000, "00"), "Reject wrong architecture");
        nt->FileHeader.Machine = IMAGE_FILE_MACHINE_AMD64;
        check(!sdk::find_pattern_in_image(image, 0x2000, "00"), "Image cannot exceed supplied mapping");
        sections[0].Misc.VirtualSize = 0xffffffff;
        check(!sdk::find_pattern_in_image(image, 0x4000, "00"), "Reject invalid non-code section before scanning");
        sections[0].Misc.VirtualSize = 0x1000;
        nt->FileHeader.SizeOfOptionalHeader = 0;
        check(!sdk::find_pattern_in_image(image, 0x4000, "00"), "Reject malformed optional header");
        nt->FileHeader.SizeOfOptionalHeader = sizeof(IMAGE_OPTIONAL_HEADER);
        dos->e_lfanew = 0x3fff;
        check(!sdk::find_pattern_in_image(image, 0x4000, "00"), "Reject truncated NT header");
        check(!sdk::find_pattern_in_image(nullptr, 0x4000, "00"), "Reject null image");
        VirtualFree(image, 0, MEM_RELEASE);
    } catch (...) { VirtualFree(image, 0, MEM_RELEASE); throw; }
}

static void rip_checks() {
    std::uint8_t bytes[64]{};
    auto* instruction = bytes + 16;
    std::int32_t displacement = 9;
    std::memcpy(instruction + 3, &displacement, sizeof(displacement));
    check(sdk::resolve_absolute_rip_address(instruction, 3, 7) == bytes + 32, "Resolve positive displacement");
    displacement = -8;
    std::memcpy(instruction + 3, &displacement, sizeof(displacement));
    check(sdk::resolve_absolute_rip_address(instruction, 3, 7) == bytes + 15, "Resolve negative displacement");
    check(!sdk::resolve_absolute_rip_address(nullptr, 3, 7), "Null RIP input returns null");
    check(!sdk::resolve_absolute_rip_address(instruction, 4, 7), "Displacement must fit instruction");
    check(!sdk::resolve_absolute_rip_address(instruction, SIZE_MAX, 7), "Reject displacement offset overflow");
    check(!sdk::resolve_absolute_rip_address(reinterpret_cast<std::uint8_t*>(UINTPTR_MAX - 2), 0, 7),
        "Reject instruction end overflow before reading memory");
}

int main() {
    std::uint8_t* pages = nullptr;
    try {
        image_checks();
        rip_checks();
        std::uint8_t bytes[]{0x48, 0x8B, 0xC4, 0x55};
        check(sdk::find_pattern_in_range(bytes, sizeof(bytes), "48 8b C4 55") == bytes, "Exact whole-range match");
        check(sdk::find_pattern_in_range(bytes, sizeof(bytes), "C4 55") == bytes + 2, "Last candidate included");
        check(sdk::find_pattern_in_range(bytes, sizeof(bytes), "48 ? ?? ?") == bytes, "Terminal wildcards");
        check(sdk::find_pattern_in_range(bytes, sizeof(bytes), "\t48\n8B C4 55 ") == bytes, "Whitespace separators");
        for (const char* bad : {"", "G1", "4", "48??", "48 ???", "48 8B C4 55 66"})
            check(!sdk::find_pattern_in_range(bytes, sizeof(bytes), bad), "Reject malformed/oversized patterns");
        check(!sdk::find_pattern_in_range(bytes, 0, "48"), "Empty range");
        check(!sdk::find_pattern_in_range(bytes, sizeof(bytes), nullptr), "Null pattern");
        check(!sdk::find_pattern("codex_nonexistent_module.dll", "48"), "Missing module returns null");

        SYSTEM_INFO system{};
        GetSystemInfo(&system);
        const auto page = system.dwPageSize;
        pages = static_cast<std::uint8_t*>(VirtualAlloc(nullptr, page * 3, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
        check(pages != nullptr, "Allocate test pages");
        pages[page - 1] = 0x12;
        pages[page] = 0x34;
        pages[page + 1] = 0x56;
        pages[page * 2] = 0xDE;
        pages[page * 2 + 1] = 0xAD;
        DWORD old = 0;
        check(VirtualProtect(pages + page, page, PAGE_NOACCESS, &old), "Protect middle page");
        check(!sdk::resolve_absolute_rip_address(pages + page - 3, 3, 7), "Unreadable RIP displacement returns null");
        check(!sdk::is_readable_range(reinterpret_cast<uintptr_t>(pages + page - 1), 2), "Reject unreadable crossing");
        check(!sdk::find_pattern_in_range(pages, page * 3, "12 34 56"), "Do not cross inaccessible page");
        check(sdk::find_pattern_in_range(pages, page * 3, "DE AD") == pages + page * 2, "Find after inaccessible page");
        check(VirtualProtect(pages + page, page, PAGE_READONLY, &old), "Set readable page");
        check(sdk::find_pattern_in_range(pages, page * 3, "12 34 56") == pages + page - 1, "Cross readable protection boundary");
        check(VirtualProtect(pages + page, page, PAGE_READWRITE | PAGE_GUARD, &old), "Set guard page");
        check(sdk::find_pattern_in_range(pages, page * 3, "DE AD") == pages + page * 2, "Skip guard page");
        MEMORY_BASIC_INFORMATION info{};
        VirtualQuery(pages + page, &info, sizeof(info));
        check((info.Protect & PAGE_GUARD) != 0, "Guard must not be consumed");
        VirtualFree(pages, 0, MEM_RELEASE);
        std::puts("PASS: PE architecture/sections/bounds, checked RIP resolution, syntax and protected pages");
        return 0;
    }
    catch (const std::exception& e) {
        if (pages) VirtualFree(pages, 0, MEM_RELEASE);
        std::fprintf(stderr, "FAIL: %s\n", e.what());
        return 1;
    }
}
