#include "skins.h"
#include "interfaces.h"
#include "game_state.h"
#include "../sdk/mem.h"
#include <Windows.h>
#include <wininet.h>
#include <sstream>
#include <algorithm>
#include "../external/offsets/offsets.hpp"
#include "../external/offsets/client_dll.hpp"
#include "debug_console.h"

#pragma comment(lib, "wininet.lib")

namespace skins {
// HTTP GET request implementation
    static std::string HTTPGet(const std::string& url) {
        std::string response;

        HINTERNET hInternet = InternetOpenA(
            "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36",
            INTERNET_OPEN_TYPE_PRECONFIG,
            NULL,
            NULL,
            0
        );

        if (!hInternet) return "";

        HINTERNET hConnect = InternetOpenUrlA(
            hInternet,
            url.c_str(),
            NULL,
            0,
            INTERNET_FLAG_RELOAD,
            0
        );

        if (!hConnect) {
            InternetCloseHandle(hInternet);
            return "";
        }

        char buffer[8192];
        DWORD bytesRead = 0;

        while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
            response.append(buffer, bytesRead);
        }

        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return response;
    }

    // Helper functions for JSON parsing
    static int ExtractIntValue(const std::string& obj, const std::string& key) {
        std::string search = "\"" + key + "\":";
        size_t pos = obj.find(search);
        if (pos == std::string::npos) return 0;

        pos += search.length();
        while (pos < obj.length() && (obj[pos] == ' ' || obj[pos] == '\t')) pos++;

        size_t end = pos;
        while (end < obj.length() && std::isdigit(obj[end])) end++;

        if (end > pos) {
            try {
                return std::stoi(obj.substr(pos, end - pos));
            }
            catch (...) {}
        }
        return 0;
    }

    static std::string ExtractStringValue(const std::string& obj, const std::string& key) {
        std::string search = "\"" + key + "\":\"";
        size_t pos = obj.find(search);
        if (pos == std::string::npos) return "";

        pos += search.length();
        size_t end = obj.find("\"", pos);
        if (end != std::string::npos) {
            return obj.substr(pos, end - pos);
        }
        return "";
    }

    static bool ExtractBoolValue(const std::string& obj, const std::string& key) {
        std::string search = "\"" + key + "\":";
        size_t pos = obj.find(search);
        if (pos == std::string::npos) return false;

        pos += search.length();
        while (pos < obj.length() && (obj[pos] == ' ' || obj[pos] == '\t')) pos++;

        return obj.substr(pos, 4) == "true";
    }

    // Map weapon names to IDs
    static int MapWeaponNameToID(const std::string& weapon) {
        if (weapon.find("AK-47") != std::string::npos) return WEAPON_AK47;
        if (weapon.find("M4A4") != std::string::npos) return WEAPON_M4A1;
        if (weapon.find("M4A1-S") != std::string::npos) return WEAPON_M4A1_SILENCER;
        if (weapon.find("AWP") != std::string::npos) return WEAPON_AWP;
        if (weapon.find("Desert Eagle") != std::string::npos) return WEAPON_DEAGLE;
        if (weapon.find("Glock") != std::string::npos) return WEAPON_GLOCK;
        if (weapon.find("USP-S") != std::string::npos) return WEAPON_USP_SILENCER;
        if (weapon.find("P250") != std::string::npos) return WEAPON_P250;
        if (weapon.find("Five-SeveN") != std::string::npos) return WEAPON_FIVESEVEN;
        if (weapon.find("CZ75") != std::string::npos) return WEAPON_CZ75A;
        if (weapon.find("R8") != std::string::npos) return WEAPON_REVOLVER;
        if (weapon.find("MAC-10") != std::string::npos) return WEAPON_MAC10;
        if (weapon.find("MP9") != std::string::npos) return WEAPON_MP9;
        if (weapon.find("MP7") != std::string::npos) return WEAPON_MP7;
        if (weapon.find("UMP-45") != std::string::npos) return WEAPON_UMP45;
        if (weapon.find("P90") != std::string::npos) return WEAPON_P90;
        if (weapon.find("Bizon") != std::string::npos) return WEAPON_BIZON;
        if (weapon.find("Nova") != std::string::npos) return WEAPON_NOVA;
        if (weapon.find("XM1014") != std::string::npos) return WEAPON_XM1014;
        if (weapon.find("Sawed-Off") != std::string::npos) return WEAPON_SAWEDOFF;
        if (weapon.find("MAG-7") != std::string::npos) return WEAPON_MAG7;
        if (weapon.find("M249") != std::string::npos) return WEAPON_M249;
        if (weapon.find("Negev") != std::string::npos) return WEAPON_NEGEV;
        if (weapon.find("FAMAS") != std::string::npos) return WEAPON_FAMAS;
        if (weapon.find("Galil") != std::string::npos) return WEAPON_GALILAR;
        if (weapon.find("AUG") != std::string::npos) return WEAPON_AUG;
        if (weapon.find("SG 553") != std::string::npos) return WEAPON_SG556;
        if (weapon.find("SSG 08") != std::string::npos) return WEAPON_SSG08;
        if (weapon.find("SCAR-20") != std::string::npos) return WEAPON_SCAR20;
        if (weapon.find("G3SG1") != std::string::npos) return WEAPON_G3SG1;
        if (weapon.find("MP5-SD") != std::string::npos) return WEAPON_MP5SD;
        return 0;
    }

    static int MapKnifeNameToID(const std::string& knife) {
        if (knife.find("Bayonet") != std::string::npos && knife.find("M9") == std::string::npos) return WEAPON_KNIFE_BAYONET;
        if (knife.find("Classic") != std::string::npos) return WEAPON_KNIFE_CLASSIC;
        if (knife.find("Flip") != std::string::npos) return WEAPON_KNIFE_FLIP;
        if (knife.find("Gut") != std::string::npos) return WEAPON_KNIFE_GUT;
        if (knife.find("Karambit") != std::string::npos) return WEAPON_KNIFE_KARAMBIT;
        if (knife.find("M9") != std::string::npos || knife.find("M9 Bayonet") != std::string::npos) return WEAPON_KNIFE_M9_BAYONET;
        if (knife.find("Huntsman") != std::string::npos) return WEAPON_KNIFE_TACTICAL;
        if (knife.find("Falchion") != std::string::npos) return WEAPON_KNIFE_FALCHION;
        if (knife.find("Bowie") != std::string::npos) return WEAPON_KNIFE_SURVIVAL_BOWIE;
        if (knife.find("Butterfly") != std::string::npos) return WEAPON_KNIFE_BUTTERFLY;
        if (knife.find("Shadow") != std::string::npos || knife.find("Daggers") != std::string::npos) return WEAPON_KNIFE_PUSH;
        if (knife.find("Paracord") != std::string::npos) return WEAPON_KNIFE_CORD;
        if (knife.find("Survival") != std::string::npos && knife.find("Bowie") == std::string::npos) return WEAPON_KNIFE_CANIS;
        if (knife.find("Ursus") != std::string::npos) return WEAPON_KNIFE_URSUS;
        if (knife.find("Navaja") != std::string::npos) return WEAPON_KNIFE_GYPSY_JACKKNIFE;
        if (knife.find("Nomad") != std::string::npos) return WEAPON_KNIFE_OUTDOOR;
        if (knife.find("Stiletto") != std::string::npos) return WEAPON_KNIFE_STILETTO;
        if (knife.find("Talon") != std::string::npos) return WEAPON_KNIFE_WIDOWMAKER;
        if (knife.find("Skeleton") != std::string::npos) return WEAPON_KNIFE_SKELETON;
        return 0;
    }

    // Load skins from official APIs
    void LoadSkinsFromAPI() {
        skin_database.clear();
        skins_by_weapon.clear();

        try {
            // Load weapon skins
            std::string weapons_url = "https://raw.githubusercontent.com/ByMykel/CSGO-API/main/public/api/en/skins.json";
            std::string weapons_data = HTTPGet(weapons_url);

            if (!weapons_data.empty()) {
                size_t pos = 0;
                while ((pos = weapons_data.find("{", pos)) != std::string::npos) {
                    try {
                        size_t end = weapons_data.find("}", pos);
                        if (end == std::string::npos) break;

                        std::string obj = weapons_data.substr(pos, end - pos + 1);
                        
                        int paint_kit = ExtractIntValue(obj, "id");
                        std::string name = ExtractStringValue(obj, "name");
                        std::string weapon = ExtractStringValue(obj, "weapon");
                        std::string rarity_str = ExtractStringValue(obj, "rarity");
                        bool has_stattrak = ExtractBoolValue(obj, "stattrak");

                        if (paint_kit > 0 && !name.empty() && !weapon.empty()) {
                            SkinRarity rarity = RARITY_COMMON;
                            if (rarity_str.find("Contraband") != std::string::npos) rarity = RARITY_CONTRABAND;
                            else if (rarity_str.find("Covert") != std::string::npos) rarity = RARITY_ANCIENT;
                            else if (rarity_str.find("Classified") != std::string::npos) rarity = RARITY_LEGENDARY;
                            else if (rarity_str.find("Restricted") != std::string::npos) rarity = RARITY_MYTHICAL;
                            else if (rarity_str.find("Mil-Spec") != std::string::npos) rarity = RARITY_RARE;
                            else if (rarity_str.find("Industrial") != std::string::npos) rarity = RARITY_UNCOMMON;

                            int weapon_id = MapWeaponNameToID(weapon);
                            if (weapon_id > 0) {
                                SkinInfo skin(paint_kit, name, weapon, weapon_id, rarity, has_stattrak);
                                skin_database.push_back(skin);
                                skins_by_weapon[weapon].push_back(skin);
                            }
                        }

                        pos = end + 1;
                    }
                    catch (...) {
                        pos++;
                    }
                }
            }

            // Load knife skins
            std::string knives_url = "https://raw.githubusercontent.com/ByMykel/CSGO-API/main/public/api/en/knives.json";
            std::string knives_data = HTTPGet(knives_url);

            if (!knives_data.empty()) {
                size_t pos = 0;
                while ((pos = knives_data.find("{", pos)) != std::string::npos) {
                    try {
                        size_t end = knives_data.find("}", pos);
                        if (end == std::string::npos) break;

                        std::string obj = knives_data.substr(pos, end - pos + 1);
                        
                        int paint_kit = ExtractIntValue(obj, "id");
                        std::string name = ExtractStringValue(obj, "name");
                        std::string knife_type = ExtractStringValue(obj, "type");

                        if (paint_kit > 0 && !name.empty()) {
                            int weapon_id = MapKnifeNameToID(knife_type);
                            if (weapon_id > 0) {
                                SkinInfo skin(paint_kit, name, knife_type, weapon_id, RARITY_ANCIENT, false);
                                skin_database.push_back(skin);
                                skins_by_weapon[knife_type].push_back(skin);
                            }
                        }

                        pos = end + 1;
                    }
                    catch (...) {
                        pos++;
                    }
                }
            }

            // Load glove skins
            std::string gloves_url = "https://raw.githubusercontent.com/ByMykel/CSGO-API/main/public/api/en/gloves.json";
            std::string gloves_data = HTTPGet(gloves_url);

            if (!gloves_data.empty()) {
                size_t pos = 0;
                while ((pos = gloves_data.find("{", pos)) != std::string::npos) {
                    try {
                        size_t end = gloves_data.find("}", pos);
                        if (end == std::string::npos) break;

                        std::string obj = gloves_data.substr(pos, end - pos + 1);
                        
                        int paint_kit = ExtractIntValue(obj, "id");
                        std::string name = ExtractStringValue(obj, "name");

                        if (paint_kit > 0 && !name.empty()) {
                            GloveInfo glove(paint_kit, name);
                            glove_database.push_back(glove);
                        }

                        pos = end + 1;
                    }
                    catch (...) {
                        pos++;
                    }
                }
            }

            // Use fallback if nothing loaded
            if (skin_database.empty() && glove_database.empty()) {
                InitializeSkinDatabase();
            }
        }
        catch (...) {
            InitializeSkinDatabase();
        }
    }

    void InitializeSkinDatabase() {
        // Fallback database with popular skins - AK-47
        skin_database.push_back(SkinInfo(44, "Redline", "AK-47", WEAPON_AK47, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(180, "Vulcan", "AK-47", WEAPON_AK47, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(490, "Aquamarine Revenge", "AK-47", WEAPON_AK47, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(524, "Neon Revolution", "AK-47", WEAPON_AK47, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(675, "Bloodsport", "AK-47", WEAPON_AK47, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(730, "Empress", "AK-47", WEAPON_AK47, RARITY_ANCIENT, true));

        // AWP skins
        skin_database.push_back(SkinInfo(10, "Boom", "AWP", WEAPON_AWP, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(344, "Asiimov", "AWP", WEAPON_AWP, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(365, "Dragon Lore", "AWP", WEAPON_AWP, RARITY_CONTRABAND, true));
        skin_database.push_back(SkinInfo(486, "Hyper Beast", "AWP", WEAPON_AWP, RARITY_ANCIENT, true));

        // M4A4 skins
        skin_database.push_back(SkinInfo(309, "Asiimov", "M4A4", WEAPON_M4A1, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(360, "Howl", "M4A4", WEAPON_M4A1, RARITY_CONTRABAND, true));
        skin_database.push_back(SkinInfo(605, "Desolate Space", "M4A4", WEAPON_M4A1, RARITY_ANCIENT, true));

        // M4A1-S skins
        skin_database.push_back(SkinInfo(278, "Cyrex", "M4A1-S", WEAPON_M4A1_SILENCER, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(619, "Golden Coil", "M4A1-S", WEAPON_M4A1_SILENCER, RARITY_LEGENDARY, true));

        // Desert Eagle skins
        skin_database.push_back(SkinInfo(37, "Hypnotic", "Desert Eagle", WEAPON_DEAGLE, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(351, "Blaze", "Desert Eagle", WEAPON_DEAGLE, RARITY_ANCIENT, false));

        // USP-S skins
        skin_database.push_back(SkinInfo(277, "Orion", "USP-S", WEAPON_USP_SILENCER, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(504, "Kill Confirmed", "USP-S", WEAPON_USP_SILENCER, RARITY_LEGENDARY, true));

        // Glock skins
        skin_database.push_back(SkinInfo(38, "Fade", "Glock-18", WEAPON_GLOCK, RARITY_ANCIENT, false));
        skin_database.push_back(SkinInfo(350, "Water Elemental", "Glock-18", WEAPON_GLOCK, RARITY_LEGENDARY, true));

        // Organize by weapon
        for (const auto& skin : skin_database) {
            skins_by_weapon[skin.weapon_name].push_back(skin);
        }

        // Add gloves - popular ones
        glove_database.push_back(GloveInfo(10006, "Superconductor"));
        glove_database.push_back(GloveInfo(10007, "Arid"));
        glove_database.push_back(GloveInfo(10015, "Pandora's Box"));
        glove_database.push_back(GloveInfo(10016, "Foundation"));
        glove_database.push_back(GloveInfo(10018, "Vice"));
        glove_database.push_back(GloveInfo(10024, "Emerald Web"));
    }

    bool IsDatabaseLoaded() {
        return !skin_database.empty() || !glove_database.empty();
    }

    // Get weapon entities from local player
    static std::vector<uintptr_t> GetPlayerWeapons() {
        std::vector<uintptr_t> weapons;

        if (!game_state::IsInGame()) return weapons;

        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return weapons;

        try {
            // Get local pawn directly via offset
            uintptr_t local_pawn = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
            if (!local_pawn || !sdk::is_valid_ptr(local_pawn)) return weapons;

            // Get entity list
            uintptr_t entity_list = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwEntityList);
            if (!entity_list || !sdk::is_valid_ptr(entity_list)) return weapons;

            // Get weapon services
            uintptr_t weapon_services = *(uintptr_t*)(local_pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pWeaponServices);
            if (!weapon_services || !sdk::is_valid_ptr(weapon_services)) return weapons;

            // Get my weapons array (m_hMyWeapons - CS2 engine internal offset, not in schema)
            // This is CPlayer_WeaponServices::m_hMyWeapons array offset
            uintptr_t weapons_array = weapon_services + cs2_dumper::schemas::client_dll::CPlayer_WeaponServices::m_hMyWeapons;

            // Iterate weapons (max 64)
            for (int i = 0; i < 64; i++) {
                uint32_t weapon_handle = *(uint32_t*)(weapons_array + (i * 0x4));
                if (!weapon_handle || weapon_handle == 0xFFFFFFFF) continue;

                // Resolve weapon entity through entity list (proper two-level traversal)
                // These offsets (0x8, 16, 112) are CS2 entity system internals, not in schema
                uintptr_t list_entry = *(uintptr_t*)(entity_list + 0x8 * ((weapon_handle & 0x7FFF) >> 9) + 16);
                if (!list_entry || !sdk::is_valid_ptr(list_entry)) continue;

                uintptr_t weapon = *(uintptr_t*)(list_entry + 112 * (weapon_handle & 0x1FF));
                if (weapon && sdk::is_valid_ptr(weapon)) {
                    weapons.push_back(weapon);
                }
            }
        }
        catch (...) {
        }

        return weapons;
    }

   bool ApplyReUpdate() {
        uintptr_t engine = (uintptr_t)GetModuleHandleA("engine2.dll");
        auto net_client = *(uintptr_t*)(engine + cs2_dumper::offsets::engine2_dll::dwNetworkGameClient);
        if (!net_client) {
            auto deltaTick = *(int*)(net_client + cs2_dumper::offsets::engine2_dll::dwNetworkGameClient_deltaTick);
            if (deltaTick) {
                *(int*)(net_client + cs2_dumper::offsets::engine2_dll::dwNetworkGameClient_deltaTick) = -1;
                debug_console::Console::Get().Info("Applied network re-update!");
                debug_console::Console::Get().Pointer("dwNetworkGameClient", engine + cs2_dumper::offsets::engine2_dll::dwNetworkGameClient, net_client, true);
                debug_console::Console::Get().Pointer("dwNetworkGameClient_deltaTick", net_client + cs2_dumper::offsets::engine2_dll::dwNetworkGameClient_deltaTick, deltaTick, true);
            }
                
            else {
                debug_console::Console::Get().Error("Failed getting deltaTick pointer in dwNetworkGameClient!");
                return false;
            }
                
        }
        else {
            debug_console::Console::Get().Error("Failed getting dwNetworkGameClient pointer in engine2.dll!");
            return false;
        }
    }

    void ApplySkin(void* weapon, int weapon_id) {
        if (!weapon) return;

        auto it = user_skins.find(weapon_id);
        if (it == user_skins.end()) return;

        PlayerSkinConfig& config = it->second;

        try {
            uintptr_t weapon_addr = reinterpret_cast<uintptr_t>(weapon);

            // ========== CS2 SKIN CHANGER OFFSETS ==========
            // All offsets are from C_EconEntity which inherits from C_BaseAttributableItem
            
            // ========== CRITICAL: DISABLE SOC RESTRICTION ==========
            // This allows the client to apply custom skins without server validation
            // Without this, skins will not be applied properly
            *reinterpret_cast<bool*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconItemView::m_bDisallowSOC) = false;

            // Apply paint kit (skin ID)
            *reinterpret_cast<int*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackPaintKit) = config.paint_kit;

            // Apply seed (pattern variation - important for Case Hardened, Fade, etc.)
            *reinterpret_cast<int*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackSeed) = config.seed;

            // Apply wear (float value: 0.0 = Factory New, 1.0 = Battle-Scarred)
            *reinterpret_cast<float*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconEntity::m_flFallbackWear) = config.wear;

            // Apply StatTrak
            if (config.stattrak) {
                *reinterpret_cast<int*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackStatTrak) = config.stattrak_count;
                *reinterpret_cast<int*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconItemView::m_iEntityQuality) = 9; // 9 = Strange (StatTrak)
            }
            else {
                *reinterpret_cast<int*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackStatTrak) = -1;
                *reinterpret_cast<int*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconItemView::m_iEntityQuality) = 4; // 4 = Unique (Standard)
            }

            // Set custom name tag if provided
            if (!config.name_tag.empty()) {
                char* name_ptr = reinterpret_cast<char*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconItemView::m_szCustomName);
                strncpy_s(name_ptr, 32, config.name_tag.c_str(), _TRUNCATE);
            }

            // Mark as custom/fallback item (required for skins to work)
            *reinterpret_cast<int*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh) = -1;
            
            // Set account ID to 0 for fallback items
            *reinterpret_cast<int*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconItemView::m_iAccountID) = 0;

            // Force client update
            ApplyReUpdate();

            debug_console::Console::Get().Success("[SKIN] Applied to %s | Paint: %d | Seed: %d | Wear: %.3f | StatTrak: %s",
                GetWeaponName(weapon_id), config.paint_kit, config.seed, config.wear, 
                config.stattrak ? "YES" : "NO");
        }
        catch (const std::exception& e) {
            debug_console::Console::Get().Error("[SKIN] Exception in ApplySkin: %s", e.what());
        }
        catch (...) {
            debug_console::Console::Get().Error("[SKIN] Unknown exception in ApplySkin");
        }
    }

    void ApplyAllSkins() {
        try {
            auto weapons = GetPlayerWeapons();

            for (auto weapon_addr : weapons) {
                if (!weapon_addr) continue;

                // Get weapon definition index
                int weapon_id = *(int*)(weapon_addr + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex);

                // Apply skin if configured
                if (user_skins.find(weapon_id) != user_skins.end()) {
                    ApplySkin(reinterpret_cast<void*>(weapon_addr), weapon_id);
                }
            }
        }
        catch (...) {
        }
    }

    void ApplyKnife() {
        if (!game_state::IsInGame()) return;

        try {
            auto weapons = GetPlayerWeapons();
            bool knife_changed = false;

            for (auto weapon_addr : weapons) {
                if (!weapon_addr) continue;

                // Get current weapon definition index
                int weapon_id = *reinterpret_cast<short*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex);

                // Check if it's a default knife (CT or T)
                if (weapon_id == WEAPON_KNIFE_CT || weapon_id == WEAPON_KNIFE_T) {
                    debug_console::Console::Get().Info("[KNIFE] Found default knife (ID: %d), changing to %s (ID: %d)",
                        weapon_id, GetWeaponName(selected_knife_id), selected_knife_id);

                    // ========== CRITICAL: DISABLE SOC RESTRICTION ==========
                    // This allows the client to apply custom knife models without server validation
                    *reinterpret_cast<bool*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconItemView::m_bDisallowSOC) = false;

                    // ========== CHANGE KNIFE MODEL ==========
                    // This changes the actual 3D model of the knife
                    *reinterpret_cast<short*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex) = static_cast<short>(selected_knife_id);

                    // Set entity quality for knife (12 = unusual for knives/gloves)
                    *reinterpret_cast<int*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconItemView::m_iEntityQuality) = 3; // 3 = Vintage for knives

                    // Mark as initialized
                    *reinterpret_cast<int*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh) = -1;
                    *reinterpret_cast<int*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconItemView::m_iAccountID) = 0;

                    // ========== APPLY KNIFE SKIN ==========
                    // Apply configured skin to the new knife model
                    if (user_skins.find(selected_knife_id) != user_skins.end()) {
                        ApplySkin(reinterpret_cast<void*>(weapon_addr), selected_knife_id);
                    } else {
                        // Apply default skin properties if no custom config
                        *reinterpret_cast<int*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackPaintKit) = 0;
                        *reinterpret_cast<int*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackSeed) = 0;
                        *reinterpret_cast<float*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconEntity::m_flFallbackWear) = 0.0f;
                        *reinterpret_cast<int*>(weapon_addr + cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackStatTrak) = -1;
                    }

                    // ========== FORCE MODEL REFRESH ==========
                    // Multiple methods to ensure the model updates visually
                    
                    // Method 1: Set viewmodel index to force re-cache
                    ApplyReUpdate();
                    
                    // Method 2: Invalidate physics (forces model rebuild)
                    *reinterpret_cast<int*>(weapon_addr + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth) = 100;
                    
                    knife_changed = true;
                }
            }

            if (knife_changed) {
                // Force network update to sync with server
                ApplyReUpdate();
                debug_console::Console::Get().Success("[KNIFE] Knife model changed successfully!");
            }
        }
        catch (const std::exception& e) {
            debug_console::Console::Get().Error("[KNIFE] Exception: %s", e.what());
        }
        catch (...) {
            debug_console::Console::Get().Error("[KNIFE] Unknown exception");
        }
    }

    void ApplyGloves() {
        if (!game_state::IsInGame()) return;

        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return;

        try {
            // Get local pawn directly via offset
            uintptr_t local_pawn = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
            if (!local_pawn || !sdk::is_valid_ptr(local_pawn)) return;

            // Get entity list
            uintptr_t entity_list = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwEntityList);
            if (!entity_list || !sdk::is_valid_ptr(entity_list)) return;

            // Get glove handle
            uint32_t glove_handle = *(uint32_t*)(local_pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_EconGloves);
            if (!glove_handle || glove_handle == 0xFFFFFFFF) return;

            // Resolve glove entity through entity list (proper two-level traversal)
            // These offsets (0x8, 16, 112) are CS2 entity system internals, not in schema
            uintptr_t list_entry = *(uintptr_t*)(entity_list + 0x8 * ((glove_handle & 0x7FFF) >> 9) + 16);
            if (!list_entry || !sdk::is_valid_ptr(list_entry)) return;

            uintptr_t glove = *(uintptr_t*)(list_entry + 112 * (glove_handle & 0x1FF));

            if (glove && sdk::is_valid_ptr(glove) && selected_glove_kit > 0) {
                // ========== CRITICAL: DISABLE SOC RESTRICTION ==========
                // This allows the client to apply custom glove skins without server validation
                *reinterpret_cast<bool*>(glove + cs2_dumper::schemas::client_dll::C_EconItemView::m_bDisallowSOC) = false;

                // Apply glove skin (C_EconItemView offsets for gloves)
                *reinterpret_cast<int*>(glove + cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackPaintKit) = selected_glove_kit;
                *reinterpret_cast<float*>(glove + cs2_dumper::schemas::client_dll::C_EconEntity::m_flFallbackWear) = 0.01f; // Factory New
                *reinterpret_cast<int*>(glove + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh) = -1; // Force update flag
                *reinterpret_cast<int*>(glove + cs2_dumper::schemas::client_dll::C_EconItemView::m_iAccountID) = 0;

                debug_console::Console::Get().Success("[GLOVES] Applied glove skin (ID: %d)", selected_glove_kit);
            }
        }
        catch (...) {
        }
    }

    const char* GetWeaponName(int weapon_id) {
        switch (weapon_id) {
        case WEAPON_AK47: return "AK-47";
        case WEAPON_M4A1: return "M4A4";
        case WEAPON_M4A1_SILENCER: return "M4A1-S";
        case WEAPON_AWP: return "AWP";
        case WEAPON_DEAGLE: return "Desert Eagle";
        case WEAPON_GLOCK: return "Glock-18";
        case WEAPON_USP_SILENCER: return "USP-S";
        case WEAPON_P250: return "P250";
        case WEAPON_FIVESEVEN: return "Five-SeveN";
        case WEAPON_TEC9: return "Tec-9";
        case WEAPON_CZ75A: return "CZ75-Auto";
        case WEAPON_REVOLVER: return "R8 Revolver";
        case WEAPON_KNIFE_BAYONET: return "Bayonet";
        case WEAPON_KNIFE_CLASSIC: return "Classic Knife";
        case WEAPON_KNIFE_FLIP: return "Flip Knife";
        case WEAPON_KNIFE_GUT: return "Gut Knife";
        case WEAPON_KNIFE_KARAMBIT: return "Karambit";
        case WEAPON_KNIFE_M9_BAYONET: return "M9 Bayonet";
        case WEAPON_KNIFE_TACTICAL: return "Huntsman Knife";
        case WEAPON_KNIFE_FALCHION: return "Falchion Knife";
        case WEAPON_KNIFE_SURVIVAL_BOWIE: return "Bowie Knife";
        case WEAPON_KNIFE_BUTTERFLY: return "Butterfly Knife";
        case WEAPON_KNIFE_PUSH: return "Shadow Daggers";
        case WEAPON_KNIFE_CORD: return "Paracord Knife";
        case WEAPON_KNIFE_CANIS: return "Survival Knife";
        case WEAPON_KNIFE_URSUS: return "Ursus Knife";
        case WEAPON_KNIFE_GYPSY_JACKKNIFE: return "Navaja Knife";
        case WEAPON_KNIFE_OUTDOOR: return "Nomad Knife";
        case WEAPON_KNIFE_STILETTO: return "Stiletto Knife";
        case WEAPON_KNIFE_WIDOWMAKER: return "Talon Knife";
        case WEAPON_KNIFE_SKELETON: return "Skeleton Knife";
        default: return "Unknown";
        }
    }

    const char* GetRarityName(SkinRarity rarity) {
        switch (rarity) {
        case RARITY_COMMON: return "Consumer Grade";
        case RARITY_UNCOMMON: return "Industrial Grade";
        case RARITY_RARE: return "Mil-Spec Grade";
        case RARITY_MYTHICAL: return "Restricted";
        case RARITY_LEGENDARY: return "Classified";
        case RARITY_ANCIENT: return "Covert";
        case RARITY_CONTRABAND: return "Contraband";
        default: return "Unknown";
        }
    }

    const float* GetRarityColor(SkinRarity rarity) {
        static float colors[][4] = {
            {0.7f, 0.7f, 0.7f, 1.0f},   // Common
            {0.4f, 0.6f, 0.9f, 1.0f},   // Uncommon
            {0.3f, 0.4f, 0.8f, 1.0f},   // Rare
            {0.5f, 0.3f, 0.8f, 1.0f},   // Mythical
            {0.8f, 0.3f, 0.6f, 1.0f},   // Legendary
            {0.9f, 0.2f, 0.2f, 1.0f},   // Ancient
            {0.95f, 0.8f, 0.1f, 1.0f}   // Contraband
        };

        if (rarity >= 0 && rarity <= RARITY_CONTRABAND) {
            return colors[rarity];
        }
        return colors[0];
    }
}
