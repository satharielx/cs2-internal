#pragma once
#include "../skins.h"

namespace menu_advanced::ui_constants {
    // Weapon categories
    inline constexpr int rifles[] = {
        skins::WEAPON_AK47, skins::WEAPON_M4A1, skins::WEAPON_M4A1_SILENCER,
        skins::WEAPON_AWP, skins::WEAPON_FAMAS, skins::WEAPON_GALILAR,
        skins::WEAPON_AUG, skins::WEAPON_SG556, skins::WEAPON_SSG08,
        skins::WEAPON_SCAR20, skins::WEAPON_G3SG1
    };
    inline constexpr int pistols[] = {
        skins::WEAPON_DEAGLE, skins::WEAPON_GLOCK, skins::WEAPON_USP_SILENCER,
        skins::WEAPON_HKP2000, skins::WEAPON_P250, skins::WEAPON_FIVESEVEN,
        skins::WEAPON_TEC9, skins::WEAPON_CZ75A, skins::WEAPON_REVOLVER,
        skins::WEAPON_ELITE
    };
    inline constexpr int smg_heavy[] = {
        skins::WEAPON_MAC10, skins::WEAPON_MP9, skins::WEAPON_MP7,
        skins::WEAPON_MP5SD, skins::WEAPON_UMP45, skins::WEAPON_P90,
        skins::WEAPON_BIZON, skins::WEAPON_NOVA, skins::WEAPON_XM1014,
        skins::WEAPON_SAWEDOFF, skins::WEAPON_MAG7, skins::WEAPON_M249,
        skins::WEAPON_NEGEV
    };

    inline constexpr const char* page_names[] = {
        "Aimbot", "Triggerbot", "Players ESP", "Items ESP",
        "View & Camera", "HUD Overlays", "General", "Movement",
        "Skin Changer", "Grenade Helper", "Configs", "Overview"
    };
}
