#pragma once
#include "../skins.h"
#include <cstdint>

namespace skins::detail {
    // Knife data.
    struct KnifeData { int weapon_id; uint32_t subclass_hash; const char* model_path; };

    inline constexpr KnifeData g_knife_data[] = {
        { WEAPON_KNIFE_BAYONET,         3933374535, "weapons/models/knife/knife_bayonet/weapon_knife_bayonet.vmdl" },
        { WEAPON_KNIFE_CLASSIC,         3787235507, "weapons/models/knife/knife_css/weapon_knife_css.vmdl" },
        { WEAPON_KNIFE_FLIP,            4046390180, "weapons/models/knife/knife_flip/weapon_knife_flip.vmdl" },
        { WEAPON_KNIFE_GUT,             2047704618, "weapons/models/knife/knife_gut/weapon_knife_gut.vmdl" },
        { WEAPON_KNIFE_KARAMBIT,        1731408398, "weapons/models/knife/knife_karambit/weapon_knife_karambit.vmdl" },
        { WEAPON_KNIFE_M9_BAYONET,      1638561588, "weapons/models/knife/knife_m9/weapon_knife_m9.vmdl" },
        { WEAPON_KNIFE_TACTICAL,        2282479884, "weapons/models/knife/knife_tactical/weapon_knife_tactical.vmdl" },
        { WEAPON_KNIFE_FALCHION,        3412259219, "weapons/models/knife/knife_falchion/weapon_knife_falchion.vmdl" },
        { WEAPON_KNIFE_SURVIVAL_BOWIE,  2511498851, "weapons/models/knife/knife_bowie/weapon_knife_bowie.vmdl" },
        { WEAPON_KNIFE_BUTTERFLY,       1353709123, "weapons/models/knife/knife_butterfly/weapon_knife_butterfly.vmdl" },
        { WEAPON_KNIFE_PUSH,            4269888884, "weapons/models/knife/knife_push/weapon_knife_push.vmdl" },
        { WEAPON_KNIFE_CORD,            1105782941, "weapons/models/knife/knife_cord/weapon_knife_cord.vmdl" },
        { WEAPON_KNIFE_CANIS,           275962944,  "weapons/models/knife/knife_canis/weapon_knife_canis.vmdl" },
        { WEAPON_KNIFE_URSUS,           1338637359, "weapons/models/knife/knife_ursus/weapon_knife_ursus.vmdl" },
        { WEAPON_KNIFE_GYPSY_JACKKNIFE, 3230445913, "weapons/models/knife/knife_navaja/weapon_knife_navaja.vmdl" },
        { WEAPON_KNIFE_OUTDOOR,         3206681373, "weapons/models/knife/knife_outdoor/weapon_knife_outdoor.vmdl" },
        { WEAPON_KNIFE_STILETTO,        2595277776, "weapons/models/knife/knife_stiletto/weapon_knife_stiletto.vmdl" },
        { WEAPON_KNIFE_WIDOWMAKER,      4029975521, "weapons/models/knife/knife_talon/weapon_knife_talon.vmdl" },
        { WEAPON_KNIFE_SKELETON,        365028728,  "weapons/models/knife/knife_skeleton/weapon_knife_skeleton.vmdl" },
        { WEAPON_KNIFE_KUKRI,           3845286452, "weapons/models/knife/knife_kukri/weapon_knife_kukri.vmdl" },
    };

    inline const KnifeData* GetKnifeData(int weapon_id) {
        for (const auto& kd : g_knife_data)
            if (kd.weapon_id == weapon_id) return &kd;
        return nullptr;
    }

}
