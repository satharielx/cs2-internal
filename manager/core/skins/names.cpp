#include "../skins.h"

namespace skins {
    // Names / rarities.
    const char* GetWeaponName(int id) {
        switch (id) {
        case WEAPON_AK47:                  return "AK-47";
        case WEAPON_M4A1:                  return "M4A4";
        case WEAPON_M4A1_SILENCER:         return "M4A1-S";
        case WEAPON_AWP:                   return "AWP";
        case WEAPON_DEAGLE:                return "Desert Eagle";
        case WEAPON_GLOCK:                 return "Glock-18";
        case WEAPON_USP_SILENCER:          return "USP-S";
        case WEAPON_P250:                  return "P250";
        case WEAPON_FIVESEVEN:             return "Five-SeveN";
        case WEAPON_TEC9:                  return "Tec-9";
        case WEAPON_CZ75A:                 return "CZ75-Auto";
        case WEAPON_REVOLVER:              return "R8 Revolver";
        case WEAPON_ELITE:                 return "Dual Berettas";
        case WEAPON_HKP2000:               return "P2000";
        case WEAPON_NOVA:                  return "Nova";
        case WEAPON_XM1014:                return "XM1014";
        case WEAPON_SAWEDOFF:              return "Sawed-Off";
        case WEAPON_MAG7:                  return "MAG-7";
        case WEAPON_M249:                  return "M249";
        case WEAPON_NEGEV:                 return "Negev";
        case WEAPON_MAC10:                 return "MAC-10";
        case WEAPON_MP9:                   return "MP9";
        case WEAPON_MP7:                   return "MP7";
        case WEAPON_MP5SD:                 return "MP5-SD";
        case WEAPON_UMP45:                 return "UMP-45";
        case WEAPON_P90:                   return "P90";
        case WEAPON_BIZON:                 return "PP-Bizon";
        case WEAPON_FAMAS:                 return "FAMAS";
        case WEAPON_GALILAR:               return "Galil AR";
        case WEAPON_AUG:                   return "AUG";
        case WEAPON_SG556:                 return "SG 553";
        case WEAPON_SSG08:                 return "SSG 08";
        case WEAPON_SCAR20:                return "SCAR-20";
        case WEAPON_G3SG1:                 return "G3SG1";
        case WEAPON_KNIFE_BAYONET:         return "Bayonet";
        case WEAPON_KNIFE_CLASSIC:         return "Classic Knife";
        case WEAPON_KNIFE_FLIP:            return "Flip Knife";
        case WEAPON_KNIFE_GUT:             return "Gut Knife";
        case WEAPON_KNIFE_KARAMBIT:        return "Karambit";
        case WEAPON_KNIFE_M9_BAYONET:      return "M9 Bayonet";
        case WEAPON_KNIFE_TACTICAL:        return "Huntsman Knife";
        case WEAPON_KNIFE_FALCHION:        return "Falchion Knife";
        case WEAPON_KNIFE_SURVIVAL_BOWIE:  return "Bowie Knife";
        case WEAPON_KNIFE_BUTTERFLY:       return "Butterfly Knife";
        case WEAPON_KNIFE_PUSH:            return "Shadow Daggers";
        case WEAPON_KNIFE_CORD:            return "Paracord Knife";
        case WEAPON_KNIFE_CANIS:           return "Survival Knife";
        case WEAPON_KNIFE_URSUS:           return "Ursus Knife";
        case WEAPON_KNIFE_GYPSY_JACKKNIFE: return "Navaja Knife";
        case WEAPON_KNIFE_OUTDOOR:         return "Nomad Knife";
        case WEAPON_KNIFE_STILETTO:        return "Stiletto Knife";
        case WEAPON_KNIFE_WIDOWMAKER:      return "Talon Knife";
        case WEAPON_KNIFE_SKELETON:        return "Skeleton Knife";
        case WEAPON_KNIFE_KUKRI:           return "Kukri Knife";
        case WEAPON_KNIFE_CT:              return "CT Knife";
        case WEAPON_KNIFE_T:               return "T Knife";
        case WEAPON_TASER:                 return "Zeus x27";
        default:                           return "Unknown";
        }
    }

    const char* GetRarityName(SkinRarity r) {
        switch (r) {
        case RARITY_COMMON:     return "Consumer Grade";
        case RARITY_UNCOMMON:   return "Industrial Grade";
        case RARITY_RARE:       return "Mil-Spec Grade";
        case RARITY_MYTHICAL:   return "Restricted";
        case RARITY_LEGENDARY:  return "Classified";
        case RARITY_ANCIENT:    return "Covert";
        case RARITY_CONTRABAND: return "Contraband";
        default:                return "Unknown";
        }
    }

    const float* GetRarityColor(SkinRarity r) {
        static float c[][4] = {
            {0.7f,0.7f,0.7f,1.f},  {0.4f,0.6f,0.9f,1.f}, {0.3f,0.4f,0.8f,1.f},
            {0.5f,0.3f,0.8f,1.f},  {0.8f,0.3f,0.6f,1.f}, {0.9f,0.2f,0.2f,1.f},
            {0.95f,0.8f,0.1f,1.f}
        };
        return (r >= 0 && r <= RARITY_CONTRABAND) ? c[r] : c[0];
    }

}
