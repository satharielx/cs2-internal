#pragma once
namespace runtime_patterns::chams {
    // Verified against the 2026-10-23 installed client; no absolute addresses.
    inline constexpr char arena_construct[] = "48 89 5C 24 08 57 48 83 EC 30 8B DA 48 8B F9 48 8B D1 E8 ? ? ? ? 0F B6 87 30 11 00 00";
    inline constexpr char arena_root[] = "48 83 EC 28 F6 81 30 11 00 00 04 75 ? 48 8D 15 ? ? ? ? 48 8D 0D ? ? ? ? FF 15 ? ? ? ? CC 48 8D 41 78";
    // Resolve the relative cleanup call at +7, not the enclosing caller.
    inline constexpr char arena_cleanup_call[] = "48 8D 8D 90 00 00 00 E8 ? ? ? ? 40 84 F6";
    inline constexpr char material_create[] = "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 81 EC 10 01 00 00 48 8B 05 ? ? ? ? 48 8B F2 BA E0 07 00 00";
}
