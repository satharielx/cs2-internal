#pragma once

// These patterns depend on the game build. Keep entry and call-site resolution distinct.
namespace skins::signatures {
    inline constexpr char SET_MODEL_SIGNATURE[] = "40 53 48 83 EC ? 48 8B D9 4C 8B C2 48 8B 0D ? ? ? ? 48 8D 54 24 40";
    inline constexpr char SET_MESH_GROUP_MASK_SIGNATURE[] = "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8D 99";
    inline constexpr char UPDATE_SUBCLASS_SIGNATURE[] = "4C 8B DC 53 48 81 EC ? ? ? ? 48 8B 41 10 48 8B D9 8B 50 30 C1 EA 04";
    inline constexpr char UPDATE_COMPOSITE_MATERIAL_SIGNATURE[] = "E8 ? ? ? ? 48 8D 8B ? ? ? ? 48 89 BC 24";
    inline constexpr char CLEAR_HUD_WEAPON_ICON_SIGNATURE[] = "E8 ? ? ? ? 8B F8 C6 84 24 ? ? ? ? ?";
    inline constexpr char EQUIP_ITEM_IN_LOADOUT_SIGNATURE[] = "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 89 54 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 0F B7 FA";
    inline constexpr char REGEN_WEAPON_SKINS_SIGNATURE[] = "40 55 53 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 44 0F B6 FA 48 8B D9 BA ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ?";
    inline constexpr char find_hud_element[] = "4C 8B DC 53 48 83 EC ? 48 8B 05";
    inline constexpr char build_skin[] = "48 85 C9 0F 84 ? ? ? ? 48 8B C4 48 89 50 10";
    inline constexpr char create_paint_kit[] = "48 89 5C 24 ? 56 48 83 EC ? 48 8B 01 FF 50";
    inline constexpr char set_body_group_call[] = "E8 ? ? ? ? EB 0C 48 8B CF";
    inline constexpr char regenerate_skins[] = "48 83 EC ? E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 48 8B 10";
    inline constexpr char set_attribute_call[] = "E8 ? ? ? ? 66 41 0F 6E D4";
}
