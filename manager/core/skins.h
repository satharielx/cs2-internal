#pragma once
#include <string>
#include <vector>
#include <map>

namespace skins {
    // Weapon definitions
    enum WeaponID {
        WEAPON_NONE = 0,
        WEAPON_DEAGLE = 1,
        WEAPON_ELITE = 2,
        WEAPON_FIVESEVEN = 3,
        WEAPON_GLOCK = 4,
        WEAPON_AK47 = 7,
        WEAPON_AUG = 8,
        WEAPON_AWP = 9,
        WEAPON_FAMAS = 10,
        WEAPON_G3SG1 = 11,
        WEAPON_GALILAR = 13,
        WEAPON_M249 = 14,
        WEAPON_M4A1 = 16,
        WEAPON_MAC10 = 17,
        WEAPON_P90 = 19,
        WEAPON_MP5SD = 23,
        WEAPON_UMP45 = 24,
        WEAPON_XM1014 = 25,
        WEAPON_BIZON = 26,
        WEAPON_MAG7 = 27,
        WEAPON_NEGEV = 28,
        WEAPON_SAWEDOFF = 29,
        WEAPON_TEC9 = 30,
        WEAPON_TASER = 31,
        WEAPON_HKP2000 = 32,
        WEAPON_MP7 = 33,
        WEAPON_MP9 = 34,
        WEAPON_NOVA = 35,
        WEAPON_P250 = 36,
        WEAPON_SCAR20 = 38,
        WEAPON_SG556 = 39,
        WEAPON_SSG08 = 40,
        WEAPON_KNIFE_CT = 42,
        WEAPON_FLASHBANG = 43,
        WEAPON_HEGRENADE = 44,
        WEAPON_SMOKEGRENADE = 45,
        WEAPON_MOLOTOV = 46,
        WEAPON_DECOY = 47,
        WEAPON_INCGRENADE = 48,
        WEAPON_C4 = 49,
        WEAPON_KNIFE_T = 59,
        WEAPON_M4A1_SILENCER = 60,
        WEAPON_USP_SILENCER = 61,
        WEAPON_CZ75A = 63,
        WEAPON_REVOLVER = 64,
        // All knife types from CS2
        WEAPON_KNIFE_BAYONET = 500,
        WEAPON_KNIFE_CLASSIC = 503,
        WEAPON_KNIFE_FLIP = 505,
        WEAPON_KNIFE_GUT = 506,
        WEAPON_KNIFE_KARAMBIT = 507,
        WEAPON_KNIFE_M9_BAYONET = 508,
        WEAPON_KNIFE_TACTICAL = 509,      // Huntsman
        WEAPON_KNIFE_FALCHION = 512,
        WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
        WEAPON_KNIFE_BUTTERFLY = 515,
        WEAPON_KNIFE_PUSH = 516,           // Shadow Daggers
        WEAPON_KNIFE_CORD = 517,           // Paracord
        WEAPON_KNIFE_CANIS = 518,          // Survival
        WEAPON_KNIFE_URSUS = 519,
        WEAPON_KNIFE_GYPSY_JACKKNIFE = 520, // Navaja
        WEAPON_KNIFE_OUTDOOR = 521,        // Nomad
        WEAPON_KNIFE_STILETTO = 522,
        WEAPON_KNIFE_WIDOWMAKER = 523,      // Talon
        WEAPON_KNIFE_SKELETON = 525
    };

    // Skin rarity
    enum SkinRarity {
        RARITY_COMMON = 0,
        RARITY_UNCOMMON = 1,
        RARITY_RARE = 2,
        RARITY_MYTHICAL = 3,
        RARITY_LEGENDARY = 4,
        RARITY_ANCIENT = 5,
        RARITY_CONTRABAND = 6
    };

    // Skin data structure
    struct SkinInfo {
        int paint_kit;
        std::string name;
        std::string weapon_name;
        int weapon_id;
        SkinRarity rarity;
        bool stattrak_available;

        SkinInfo() : paint_kit(0), weapon_id(0), rarity(RARITY_COMMON), stattrak_available(false) {}
        SkinInfo(int pk, const std::string& n, const std::string& wn, int wid, SkinRarity r, bool st)
            : paint_kit(pk), name(n), weapon_name(wn), weapon_id(wid), rarity(r), stattrak_available(st) {
        }
    };

    // Glove data structure
    struct GloveInfo {
        int paint_kit;
        std::string name;

        GloveInfo() : paint_kit(0) {}
        GloveInfo(int pk, const std::string& n) : paint_kit(pk), name(n) {}
    };

    // Player skin config
    struct PlayerSkinConfig {
        int weapon_id;
        int paint_kit;
        int seed;
        float wear;
        bool stattrak;
        int stattrak_count;
        std::string name_tag;

        PlayerSkinConfig()
            : weapon_id(0), paint_kit(0), seed(0), wear(0.0f),
            stattrak(false), stattrak_count(0), name_tag("") {
        }
    };

    // Global skin database
    inline std::vector<SkinInfo> skin_database;
    inline std::vector<GloveInfo> glove_database;
    inline std::map<std::string, std::vector<SkinInfo>> skins_by_weapon;

    // User's skin configurations per weapon
    inline std::map<int, PlayerSkinConfig> user_skins;
    inline int selected_glove_kit = 0;
    inline int selected_knife_id = WEAPON_KNIFE_BAYONET;

    // Database management
    void InitializeSkinDatabase();
    void LoadSkinsFromAPI();
    bool IsDatabaseLoaded();

    // Skin application
    void ApplySkin(void* weapon, int weapon_id);
    void ApplyAllSkins();
    void ApplyKnife();
    void ApplyGloves();

    // Batch operations
    void ClearAllSkins();
    void ApplyDefaultPreset(); // Apply popular skins to all weapons
    void ApplyFactoryNewAll(); // Set all skins to Factory New (0.0 wear)
    void ApplyRandomSeeds(); // Randomize pattern seeds

    // Utilities
    const char* GetWeaponName(int weapon_id);
    const char* GetRarityName(SkinRarity rarity);
    const float* GetRarityColor(SkinRarity rarity);
    const float* GetRarityColor(SkinRarity rarity);
}