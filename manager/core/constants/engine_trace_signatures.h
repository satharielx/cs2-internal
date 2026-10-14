#pragma once

// These patterns depend on the game build.
namespace runtime_patterns::engine_trace {
    inline constexpr char init_data[] = "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8D 79 ? 33 F6 C7 47";
    inline constexpr char init_hit[] = "40 55 41 55 41 57 48 83 EC";
    inline constexpr char init_filter[] = "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 0F B6 41 ? 33 FF 24";
    inline constexpr char create_trace[] = "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 40 F2 0F 10 02 4D 8D 71 08 F2 0F 11 81 F8 1C 00 00";
    inline constexpr char get_hit[] = "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 81 EC 80 00 00 00 48 8B E9 0F 29 74 24 70 48 8B CA 49 8B F9 0F 28 F2 48 8B DA";
}
