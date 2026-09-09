#include "../manager/sdk/mem.h"
#include <cstdio>
#include <cstring>
#include <stdexcept>

static void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

int main() {
    std::uint8_t* pages = nullptr;
    try {
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
        std::puts("PASS: pattern syntax, boundaries, missing modules, protected pages and readable crossings");
        return 0;
    }
    catch (const std::exception& e) {
        if (pages) VirtualFree(pages, 0, MEM_RELEASE);
        std::fprintf(stderr, "FAIL: %s\n", e.what());
        return 1;
    }
}
