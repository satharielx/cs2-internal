#pragma once
#include "hooks_signatures.h"

// These patterns depend on the game build.
namespace runtime_patterns::pattern_resolver {
    inline constexpr char entity_system[] = "48 89 0D ? ? ? ? E9 ? ? ? ? CC";
    inline constexpr char view_matrix[] = "48 8D 0D ? ? ? ? 48 C1 E0 06";
    inline constexpr char local_controller[] = "48 8B 05 ? ? ? ? 41 89 BE";
    inline constexpr char view_angles_legacy[] = "F3 0F 11 86 ? ? ? ? F3 0F 10 44 24";
    inline constexpr char global_vars_legacy[] = "48 8B 05 ? ? ? ? 8B 38 E8 00";
    inline constexpr char csgo_input[] = "48 8B 0D ? ? ? ? 8B 10 E8 ? ? ? ? 45 32 FF";
    inline constexpr auto& create_move = runtime_patterns::hooks::create_move;
    inline constexpr char network_game_client[] = "48 89 3D ? ? ? ? FF 87";
}
