#pragma once

// These patterns depend on the game build.
namespace runtime_patterns::hooks {
    inline constexpr char create_move[] = "85 D2 0F 85 ? ? ? ? 48 8B C4 44 88 40 18";
    inline constexpr char entity_added[] = "48 89 74 24 ? 57 48 83 EC ? 41 B9 ? ? ? ? 41 8B C0 41 23 C1 48 8B F2 41 83 F8 ? 48 8B F9 44 0F 45 C8 41 81 F9 ? ? ? ? 73 ? FF 81";
    inline constexpr char entity_removed[] = "48 89 74 24 ? 57 48 83 EC ? 41 B9 ? ? ? ? 41 8B C0 41 23 C1 48 8B F2 41 83 F8 ? 48 8B F9 44 0F 45 C8 41 81 F9 ? ? ? ? 73 ? FF 89";
    inline constexpr char present[] = "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 41 8B F0";
    inline constexpr char resize_buffers[] = "40 53 55 56 57 41 54 41 56 41 57 48 83 EC ? 44 8B E2";
    inline constexpr char create_swap_chain[] = "40 53 55 56 57 48 83 EC ? 48 8B F9 49 8B F1 48 8D 0D ? ? ? ? 49 8B D8 48 8B EA E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B 05 ? ? ? ? 4C 8B CE 4C 8B C3 48 8B D5 48 8B CF FF D0 8B D8 85 C0 78 ? 48 85 F6 74 ? 48 83 3E ? 74 ? 48 8B D5 48 8B CE E8 ? ? ? ? 8B C3 48 83 C4 ? 5F 5E 5D 5B C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC";
    inline constexpr char weapon_hud_tick[] = "40 53 48 83 EC 30 48 8B 05 ? ? ? ? 48 8B D9 F3 0F 10 40 30 0F 2F 41 44";
    inline constexpr char clear_weapon_hud[] = "4C 8B DC 55 57 48 83 EC 48 48 63 41 68 48 8B F9 48 63 EA";
    inline constexpr char override_view[] = "4C 8B DC 49 89 4B 08 53 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 41 0F 29 73 A8 48 8B F1";
    inline constexpr char get_view_angles[] = "4C 8B C1 85 D2 74 ? 48 8D 05";
    inline constexpr char input_reference[] = "41 8B 55 00 48 8B 0D ? ? ? ? E8 ? ? ? ? 41 8B 85 80 00 00 00";
    inline constexpr char subtick[] = "48 89 5C 24 ? 55 57 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 8B 01 48 8B F9";
    inline constexpr char sky_draw[] = "45 85 C9 0F 8E ? ? ? ? 4C 8B DC 55";
    inline constexpr char light_update[] = "48 89 54 24 ? 55 57 41 56 48 83 EC";
}
