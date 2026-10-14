#pragma once

// These patterns depend on the game build.
namespace runtime_patterns::game_state {
    inline constexpr char local_pawn_getter[] = "48 83 EC ? 83 F9 ? 75 ? 48 8B 0D ? ? ? ? 48 8D 54 24 ? ? ? ? FF 90 ? ? ? ? ? ? 48 63 C1 4C 8D 05";
    inline constexpr char local_controller[] = "48 8B 05 ? ? ? ? 41 89 BE";
}
