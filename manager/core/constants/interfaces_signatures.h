#pragma once

// These patterns depend on the game build.
namespace runtime_patterns::interfaces {
    inline constexpr char entity_system[] = "48 89 0D ? ? ? ? E9 ? ? ? ? CC";
    inline constexpr char entity_lookup[] = "4C 8D 49 10 81 FA ?? ?? 00 00 77 ?? 8B CA C1 F9 09";
    inline constexpr char input_reference[] = "48 8B 0D ? ? ? ? 4C 8D 8F ? ? ? ? 45 33 F6";
    inline constexpr char frame_stage_notify[] = "48 89 5C 24 ? 48 89 6C 24 ? 57 48 83 EC ? 48 8B F9 33 ED";
}
