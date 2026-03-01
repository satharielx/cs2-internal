#include "skin_menu.h"
#include "skins.h"
#include "config.h"
#include "../external/imgui/imgui.h"
#include "debug_console.h"
#include "game_state.h"
#include <algorithm>
#include <set>

namespace skin_menu {
    static char search_buffer[128] = "";
    static int selected_weapon_idx = 0;
    static int selected_skin_idx = 0;
    static bool show_knife_menu = false;
    static bool show_glove_menu = false;
    static bool show_debug_panel = false;
    static bool api_loaded = false;
    static bool loading_api = false;

    // Weapon list for selection (cleaned up - no duplicates)
    static const int weapon_list[] = {
        skins::WEAPON_AK47,
        skins::WEAPON_M4A1,
        skins::WEAPON_M4A1_SILENCER,
        skins::WEAPON_AWP,
        skins::WEAPON_DEAGLE,
        skins::WEAPON_GLOCK,
        skins::WEAPON_USP_SILENCER,
        skins::WEAPON_P250,
        skins::WEAPON_FIVESEVEN,
        skins::WEAPON_TEC9,
        skins::WEAPON_CZ75A,
        skins::WEAPON_REVOLVER,
        skins::WEAPON_ELITE,
        skins::WEAPON_NOVA,
        skins::WEAPON_XM1014,
        skins::WEAPON_SAWEDOFF,
        skins::WEAPON_MAG7,
        skins::WEAPON_M249,
        skins::WEAPON_NEGEV,
        skins::WEAPON_MAC10,
        skins::WEAPON_MP9,
        skins::WEAPON_MP7,
        skins::WEAPON_UMP45,
        skins::WEAPON_P90,
        skins::WEAPON_BIZON,
        skins::WEAPON_FAMAS,
        skins::WEAPON_GALILAR,
        skins::WEAPON_AUG,
        skins::WEAPON_SG556,
        skins::WEAPON_SSG08,
        skins::WEAPON_SCAR20,
        skins::WEAPON_G3SG1
    };

    static const int weapon_count = sizeof(weapon_list) / sizeof(weapon_list[0]);

    void Initialize() {
        if (!api_loaded && !loading_api) {
            loading_api = true;
            debug_console::Console::Get().Info("Initializing skin database...");

            // Load API in background (this might take a few seconds)
            skins::LoadSkinsFromAPI();

            // If API load failed, use fallback
            if (!skins::IsDatabaseLoaded()) {
                debug_console::Console::Get().Warning("API load failed, using fallback database");
                skins::InitializeSkinDatabase();
            }

            debug_console::Console::Get().Success("Skin database initialized with %zu skins", skins::skin_database.size());
            api_loaded = true;
            loading_api = false;
        }
    }

    void RenderWeaponSkinSelector() {
        ImGui::BeginChild("WeaponList", ImVec2(200, 0), true);
        ImGui::Text("Weapons");
        ImGui::Separator();

        for (int i = 0; i < weapon_count; i++) {
            int weapon_id = weapon_list[i];
            const char* weapon_name = skins::GetWeaponName(weapon_id);

            bool is_configured = skins::user_skins.find(weapon_id) != skins::user_skins.end();
            if (is_configured) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 1.0f, 0.3f, 1.0f));
            }

            if (ImGui::Selectable(weapon_name, selected_weapon_idx == i)) {
                selected_weapon_idx = i;
                selected_skin_idx = 0;
            }

            if (is_configured) {
                ImGui::PopStyleColor();
            }
        }

        ImGui::EndChild();
    }

    void RenderSkinList() {
        ImGui::BeginChild("SkinList", ImVec2(300, 0), true);

        int selected_weapon = weapon_list[selected_weapon_idx];
        const char* weapon_name = skins::GetWeaponName(selected_weapon);

        ImGui::Text("Skins for %s", weapon_name);
        ImGui::Separator();

        // Search bar
        ImGui::InputText("Search##skin_search", search_buffer, sizeof(search_buffer));
        ImGui::Separator();

        // Get skins for this weapon - remove duplicates
        std::vector<skins::SkinInfo> available_skins;
        std::set<int> seen_paint_kits;

        for (const auto& skin : skins::skin_database) {
            if (skin.weapon_id == selected_weapon) {
                // Skip duplicates
                if (seen_paint_kits.find(skin.paint_kit) != seen_paint_kits.end()) {
                    continue;
                }
                seen_paint_kits.insert(skin.paint_kit);

                // Skip empty names
                if (skin.name.empty()) {
                    debug_console::Console::Get().Warning("Skipping skin with empty name (Paint Kit: %d)", skin.paint_kit);
                    continue;
                }

                // Apply search filter
                if (search_buffer[0] != '\0') {
                    std::string skin_name_lower = skin.name;
                    std::string search_lower = search_buffer;
                    std::transform(skin_name_lower.begin(), skin_name_lower.end(), skin_name_lower.begin(), ::tolower);
                    std::transform(search_lower.begin(), search_lower.end(), search_lower.begin(), ::tolower);

                    if (skin_name_lower.find(search_lower) == std::string::npos) {
                        continue;
                    }
                }
                available_skins.push_back(skin);
            }
        }

        // Display skins
        for (size_t i = 0; i < available_skins.size(); i++) {
            const auto& skin = available_skins[i];

            // Color by rarity
            const float* color = skins::GetRarityColor(skin.rarity);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color[0], color[1], color[2], color[3]));

            char label[256];
            sprintf_s(label, "%s##%d", skin.name.c_str(), skin.paint_kit);

            if (ImGui::Selectable(label, selected_skin_idx == (int)i)) {
                selected_skin_idx = (int)i;
                debug_console::Console::Get().Debug("Selected skin: %s", skin.name.c_str());

                // Apply this skin
                skins::PlayerSkinConfig config;
                config.weapon_id = selected_weapon;
                config.paint_kit = skin.paint_kit;
                config.seed = 0;
                config.wear = 0.01f;
                config.stattrak = false;
                config.stattrak_count = 0;

                skins::user_skins[selected_weapon] = config;
            }

            ImGui::PopStyleColor();

            // Show rarity on hover
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("%s", skins::GetRarityName(skin.rarity));
            }
        }

        if (available_skins.empty()) {
            ImGui::TextDisabled("No skins available");
            if (loading_api) {
                ImGui::TextDisabled("Loading from API...");
            }
        }

        ImGui::EndChild();
    }

    void RenderSkinConfig() {
        ImGui::BeginChild("SkinConfig", ImVec2(0, 0), true);

        int selected_weapon = weapon_list[selected_weapon_idx];
        auto it = skins::user_skins.find(selected_weapon);

        if (it != skins::user_skins.end()) {
            skins::PlayerSkinConfig& config = it->second;

            ImGui::Text("Skin Configuration");
            ImGui::Separator();

            // Find selected skin info
            const skins::SkinInfo* skin_info = nullptr;
            for (const auto& skin : skins::skin_database) {
                if (skin.weapon_id == selected_weapon && skin.paint_kit == config.paint_kit) {
                    skin_info = &skin;
                    break;
                }
            }

            if (skin_info) {
                const float* color = skins::GetRarityColor(skin_info->rarity);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color[0], color[1], color[2], color[3]));
                ImGui::Text("%s", skin_info->name.c_str());
                ImGui::PopStyleColor();
                ImGui::Text("Rarity: %s", skins::GetRarityName(skin_info->rarity));
                ImGui::Separator();
            }

            // Pattern seed
            ImGui::SliderInt("Pattern Seed", &config.seed, 0, 1000);
            ImGui::SameLine();
            if (ImGui::Button("Random##Seed")) {
                config.seed = rand() % 1000;
                debug_console::Console::Get().Debug("Randomized pattern seed: %d", config.seed);
            }

            // Wear (float value)
            ImGui::SliderFloat("Float Value", &config.wear, 0.0f, 1.0f, "%.4f");

            // Float presets
            if (ImGui::Button("Factory New")) { 
                config.wear = 0.01f;
                debug_console::Console::Get().Debug("Set wear to Factory New (0.01)");
            }
            ImGui::SameLine();
            if (ImGui::Button("Minimal Wear")) { 
                config.wear = 0.08f;
                debug_console::Console::Get().Debug("Set wear to Minimal Wear (0.08)");
            }
            ImGui::SameLine();
            if (ImGui::Button("Field-Tested")) { 
                config.wear = 0.38f;
                debug_console::Console::Get().Debug("Set wear to Field-Tested (0.38)");
            }
            ImGui::SameLine();
            if (ImGui::Button("Well-Worn")) { 
                config.wear = 0.45f;
                debug_console::Console::Get().Debug("Set wear to Well-Worn (0.45)");
            }
            ImGui::SameLine();
            if (ImGui::Button("Battle-Scarred")) { 
                config.wear = 0.80f;
                debug_console::Console::Get().Debug("Set wear to Battle-Scarred (0.80)");
            }

            ImGui::Separator();

            // StatTrak
            if (skin_info && skin_info->stattrak_available) {
                ImGui::Checkbox("StatTrak™", &config.stattrak);

                if (config.stattrak) {
                    ImGui::SliderInt("StatTrak Counter", &config.stattrak_count, 0, 99999);
                }
            }
            else {
                ImGui::TextDisabled("StatTrak™ not available for this skin");
            }

            ImGui::Separator();

            // Name tag
            char name_tag_buffer[32];
            strncpy_s(name_tag_buffer, config.name_tag.c_str(), sizeof(name_tag_buffer) - 1);
            if (ImGui::InputText("Name Tag", name_tag_buffer, sizeof(name_tag_buffer))) {
                config.name_tag = name_tag_buffer;
            }

            ImGui::Separator();

            // Apply button
            if (ImGui::Button("Apply Skin Now", ImVec2(-1, 30))) {
                debug_console::Console::Get().Info("Applying skin configuration...");
                skins::ApplyAllSkins();
                debug_console::Console::Get().Success("Skins applied! Configuration will auto-apply on weapon pickup.");
                ImGui::OpenPopup("Applied");
            }

            if (ImGui::BeginPopup("Applied")) {
                ImGui::Text("Skin applied! Will auto-apply on next weapon pickup!");
                ImGui::EndPopup();
            }

            // Remove skin button
            if (ImGui::Button("Remove Skin", ImVec2(-1, 0))) {
                const char* weapon_name = skins::GetWeaponName(selected_weapon);
                debug_console::Console::Get().Warning("Removed skin configuration for %s", weapon_name);
                skins::user_skins.erase(selected_weapon);
            }

        }
        else {
            ImGui::TextDisabled("Select a skin from the list");
        }

        ImGui::EndChild();
    }

    void RenderKnifeMenu() {
        ImGui::Begin("Knife Changer", &show_knife_menu, ImGuiWindowFlags_AlwaysAutoResize);

        // Define knife types (no duplicates)
        static const struct {
            const char* name;
            int weapon_id;
        } knife_types[] = {
            {"Bayonet", skins::WEAPON_KNIFE_BAYONET},
            {"Flip Knife", skins::WEAPON_KNIFE_FLIP},
            {"Gut Knife", skins::WEAPON_KNIFE_GUT},
            {"Karambit", skins::WEAPON_KNIFE_KARAMBIT},
            {"M9 Bayonet", skins::WEAPON_KNIFE_M9_BAYONET},
            {"Huntsman Knife", skins::WEAPON_KNIFE_TACTICAL},
            {"Falchion Knife", skins::WEAPON_KNIFE_FALCHION},
            {"Bowie Knife", skins::WEAPON_KNIFE_SURVIVAL_BOWIE},
            {"Butterfly Knife", skins::WEAPON_KNIFE_BUTTERFLY},
            {"Shadow Daggers", skins::WEAPON_KNIFE_PUSH},
            {"Ursus Knife", skins::WEAPON_KNIFE_URSUS},
            {"Navaja Knife", skins::WEAPON_KNIFE_GYPSY_JACKKNIFE},
            {"Stiletto Knife", skins::WEAPON_KNIFE_STILETTO},
            {"Talon Knife", skins::WEAPON_KNIFE_WIDOWMAKER},
            {"Skeleton Knife", skins::WEAPON_KNIFE_SKELETON}
        };

        static int selected_knife = 0;
        
        // Build combo label
        const char* current_knife = knife_types[selected_knife].name;
        if (ImGui::BeginCombo("Knife Type##knife_selector", current_knife)) {
            for (int i = 0; i < IM_ARRAYSIZE(knife_types); i++) {
                bool is_selected = (selected_knife == i);
                if (ImGui::Selectable(knife_types[i].name, is_selected)) {
                    selected_knife = i;
                    debug_console::Console::Get().Info("Selected knife: %s", knife_types[i].name);
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        skins::selected_knife_id = knife_types[selected_knife].weapon_id;

        ImGui::Separator();

        // Get knife skins - filter by selected knife type only
        static int knife_skin_idx = 0;
        std::vector<skins::SkinInfo> knife_skins;
        
        for (const auto& skin : skins::skin_database) {
            if (skin.weapon_id == skins::selected_knife_id && !skin.name.empty()) {
                knife_skins.push_back(skin);
            }
        }

        // Remove duplicates
        std::sort(knife_skins.begin(), knife_skins.end(), 
            [](const skins::SkinInfo& a, const skins::SkinInfo& b) {
                return a.paint_kit < b.paint_kit;
            });
        knife_skins.erase(std::unique(knife_skins.begin(), knife_skins.end(),
            [](const skins::SkinInfo& a, const skins::SkinInfo& b) {
                return a.paint_kit == b.paint_kit;
            }), knife_skins.end());

        if (!knife_skins.empty()) {
            ImGui::Text("Available Skins: %zu", knife_skins.size());
            
            if (ImGui::BeginCombo("Knife Skin##knife_skin_selector", 
                knife_skin_idx < (int)knife_skins.size() ? knife_skins[knife_skin_idx].name.c_str() : "Select Skin")) {
                
                for (size_t i = 0; i < knife_skins.size(); i++) {
                    const auto& skin = knife_skins[i];
                    bool is_selected = (knife_skin_idx == (int)i);
                    
                    if (ImGui::Selectable(skin.name.c_str(), is_selected)) {
                        knife_skin_idx = (int)i;
                        debug_console::Console::Get().Debug("Selected knife skin: %s (Paint Kit: %d)", 
                            skin.name.c_str(), skin.paint_kit);
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        } else {
            ImGui::TextDisabled("No skins available for this knife");
        }

        ImGui::Separator();

        // Knife skin configuration
        if (!knife_skins.empty() && knife_skin_idx < (int)knife_skins.size()) {
            const auto& selected_skin = knife_skins[knife_skin_idx];
            
            ImGui::Text("Knife Skin: %s", selected_skin.name.c_str());
            
            // Configure skin for the selected knife
            skins::PlayerSkinConfig knife_config;
            knife_config.weapon_id = skins::selected_knife_id;
            knife_config.paint_kit = selected_skin.paint_kit;
            knife_config.seed = 0;
            knife_config.wear = 0.01f;
            knife_config.stattrak = false;
            knife_config.stattrak_count = 0;
            
            // Check if config exists, update it
            auto it = skins::user_skins.find(skins::selected_knife_id);
            if (it != skins::user_skins.end()) {
                knife_config.seed = it->second.seed;
                knife_config.wear = it->second.wear;
                knife_config.stattrak = it->second.stattrak;
                knife_config.stattrak_count = it->second.stattrak_count;
            }
            
            ImGui::SliderInt("Pattern Seed##knife", &knife_config.seed, 0, 1000);
            ImGui::SliderFloat("Wear##knife", &knife_config.wear, 0.0f, 1.0f, "%.4f");
            ImGui::Checkbox("StatTrak™##knife", &knife_config.stattrak);
            
            if (knife_config.stattrak) {
                ImGui::SliderInt("Kills##knife", &knife_config.stattrak_count, 0, 99999);
            }
            
            // Save configuration
            skins::user_skins[skins::selected_knife_id] = knife_config;
        }

        ImGui::Separator();

        if (ImGui::Button("Apply Knife Now", ImVec2(-1, 0))) {
            debug_console::Console::Get().Info("Applying knife changer...");
            skins::ApplyKnife();
            debug_console::Console::Get().Success("Knife changer applied! Will auto-apply on respawn.");
        }

        ImGui::End();
    }

    void RenderGloveMenu() {
        ImGui::Begin("Glove Changer", &show_glove_menu, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("Select Gloves");
        ImGui::Separator();

        // Create clean glove list without duplicates
        std::vector<skins::GloveInfo> unique_gloves;
        std::set<int> seen_paint_kits;

        for (const auto& glove : skins::glove_database) {
            if (seen_paint_kits.find(glove.paint_kit) == seen_paint_kits.end() && !glove.name.empty()) {
                unique_gloves.push_back(glove);
                seen_paint_kits.insert(glove.paint_kit);
            }
        }

        if (!unique_gloves.empty()) {
            ImGui::Text("Available Gloves: %zu", unique_gloves.size());
            
            if (ImGui::BeginCombo("Glove Skin##glove_selector", 
                skins::selected_glove_kit > 0 ? "Selected" : "Select Gloves")) {
                
                for (size_t i = 0; i < unique_gloves.size(); i++) {
                    const auto& glove = unique_gloves[i];

                    bool is_selected = (skins::selected_glove_kit == glove.paint_kit);
                    if (ImGui::Selectable(glove.name.c_str(), is_selected)) {
                        skins::selected_glove_kit = glove.paint_kit;
                        debug_console::Console::Get().Debug("Selected glove: %s (Paint Kit: %d)", 
                            glove.name.c_str(), glove.paint_kit);
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        } else {
            ImGui::TextDisabled("No gloves available");
        }

        ImGui::Separator();

        // Glove wear configuration
        static float glove_wear = 0.01f;
        ImGui::SliderFloat("Glove Wear", &glove_wear, 0.0f, 1.0f, "%.4f");
        
        if (ImGui::Button("Factory New##glove")) glove_wear = 0.01f;
        ImGui::SameLine();
        if (ImGui::Button("Minimal Wear##glove")) glove_wear = 0.08f;
        ImGui::SameLine();
        if (ImGui::Button("Field-Tested##glove")) glove_wear = 0.38f;

        ImGui::Separator();

        if (ImGui::Button("Apply Gloves Now", ImVec2(-1, 0))) {
            debug_console::Console::Get().Info("Applying glove changer (Wear: %.4f)...", glove_wear);
            skins::ApplyGloves();
            debug_console::Console::Get().Success("Glove changer applied! Will auto-apply on respawn.");
        }

        ImGui::End();
    }

    void RenderDebugPanel() {
        ImGui::SetNextWindowSize(ImVec2(750, 500), ImGuiCond_FirstUseEver);
        ImGui::Begin("Skin Changer Debug", &show_debug_panel, ImGuiWindowFlags_NoCollapse);

        if (!game_state::IsInGame()) {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Not in game - no weapon data available");
            ImGui::End();
            return;
        }

        auto weapons = skins::GetCurrentWeaponsDebugInfo();

        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Live Weapon Inventory (%zu weapons)", weapons.size());
        ImGui::Separator();

        // Configured skins summary
        ImGui::Text("Configured skins: %zu | Selected knife: %s | Glove kit: %d",
            skins::user_skins.size(),
            skins::GetWeaponName(skins::selected_knife_id),
            skins::selected_glove_kit);
        ImGui::Separator();

        if (weapons.empty()) {
            ImGui::TextDisabled("No weapons detected in inventory");
            ImGui::End();
            return;
        }

        // Table of weapons
        if (ImGui::BeginTable("WeaponDebugTable", 7,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
            ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp)) {

            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("Weapon", ImGuiTableColumnFlags_None, 120.0f);
            ImGui::TableSetupColumn("DefIdx", ImGuiTableColumnFlags_None, 50.0f);
            ImGui::TableSetupColumn("Paint Kit", ImGuiTableColumnFlags_None, 65.0f);
            ImGui::TableSetupColumn("Seed", ImGuiTableColumnFlags_None, 45.0f);
            ImGui::TableSetupColumn("Wear", ImGuiTableColumnFlags_None, 70.0f);
            ImGui::TableSetupColumn("StatTrak", ImGuiTableColumnFlags_None, 60.0f);
            ImGui::TableSetupColumn("SOC/Flags", ImGuiTableColumnFlags_None, 120.0f);
            ImGui::TableHeadersRow();

            for (const auto& w : weapons) {
                ImGui::TableNextRow();

                // Weapon name column
                ImGui::TableNextColumn();
                const char* name = skins::GetWeaponName(w.def_index);
                if (w.is_active) {
                    ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "%s *", name);
                } else {
                    ImGui::Text("%s", name);
                }

                // Def index
                ImGui::TableNextColumn();
                ImGui::Text("%d", w.def_index);

                // Paint kit
                ImGui::TableNextColumn();
                if (w.fallback_paint_kit > 0) {
                    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "%d", w.fallback_paint_kit);
                } else {
                    ImGui::TextDisabled("0");
                }

                // Seed
                ImGui::TableNextColumn();
                ImGui::Text("%d", w.fallback_seed);

                // Wear
                ImGui::TableNextColumn();
                if (w.fallback_wear <= 0.07f)
                    ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.2f, 1.0f), "%.4f", w.fallback_wear);
                else if (w.fallback_wear <= 0.15f)
                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 0.4f, 1.0f), "%.4f", w.fallback_wear);
                else if (w.fallback_wear <= 0.38f)
                    ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.3f, 1.0f), "%.4f", w.fallback_wear);
                else if (w.fallback_wear <= 0.45f)
                    ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.2f, 1.0f), "%.4f", w.fallback_wear);
                else
                    ImGui::TextColored(ImVec4(0.9f, 0.3f, 0.3f, 1.0f), "%.4f", w.fallback_wear);

                // StatTrak
                ImGui::TableNextColumn();
                if (w.fallback_stattrak >= 0) {
                    ImGui::TextColored(ImVec4(0.9f, 0.5f, 0.1f, 1.0f), "%d", w.fallback_stattrak);
                } else {
                    ImGui::TextDisabled("OFF");
                }

                // SOC / flags
                ImGui::TableNextColumn();
                ImGui::Text("SOC:%s Mat:%s Q:%d",
                    w.disallow_soc ? "Y" : "N",
                    w.restore_material ? "Y" : "N",
                    w.entity_quality);
            }

            ImGui::EndTable();
        }

        ImGui::Separator();

        // Expanded detail for each weapon on hover/click
        if (ImGui::CollapsingHeader("Detailed View (per weapon)")) {
            for (size_t i = 0; i < weapons.size(); i++) {
                const auto& w = weapons[i];
                const char* name = skins::GetWeaponName(w.def_index);

                char header[128];
                sprintf_s(header, "%s%s [%d]##detail_%zu", name, w.is_active ? " (ACTIVE)" : "", w.def_index, i);

                if (ImGui::TreeNode(header)) {
                    ImGui::Text("Address:        0x%p", (void*)w.address);
                    ImGui::Text("Def Index:      %d", w.def_index);
                    ImGui::Text("Entity Quality: %d (%s)",
                        w.entity_quality,
                        w.entity_quality == 3 ? "Knife" :
                        w.entity_quality == 9 ? "StatTrak" :
                        w.entity_quality == 4 ? "Unique" : "Other");
                    ImGui::Text("Item ID High:   %d%s", w.item_id_high, w.item_id_high == -1 ? " (FALLBACK)" : "");
                    ImGui::Text("Account ID:     %u", w.account_id);
                    ImGui::Text("Owner XUID Low: %u", w.owner_xuid_low);
                    ImGui::Separator();
                    ImGui::Text("Paint Kit:      %d", w.fallback_paint_kit);
                    ImGui::Text("Seed:           %d", w.fallback_seed);
                    ImGui::Text("Wear:           %.6f", w.fallback_wear);
                    ImGui::Text("StatTrak:       %d%s", w.fallback_stattrak, w.fallback_stattrak == -1 ? " (disabled)" : "");
                    ImGui::Separator();
                    ImGui::Text("DisallowSOC:    %s", w.disallow_soc ? "TRUE" : "FALSE");
                    ImGui::Text("RestoreMat:     %s", w.restore_material ? "TRUE" : "FALSE");
                    if (w.custom_name[0] != '\0')
                        ImGui::Text("Name Tag:       \"%s\"", w.custom_name);

                    // Show configured vs live comparison
                    auto cfg_it = skins::user_skins.find(w.def_index);
                    if (cfg_it != skins::user_skins.end()) {
                        ImGui::Separator();
                        ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "Configured Skin:");
                        ImGui::Text("  Paint Kit: %d %s", cfg_it->second.paint_kit,
                            cfg_it->second.paint_kit == w.fallback_paint_kit ? "(MATCH)" : "(MISMATCH!)");
                        ImGui::Text("  Seed:      %d %s", cfg_it->second.seed,
                            cfg_it->second.seed == w.fallback_seed ? "(MATCH)" : "(MISMATCH!)");
                        ImGui::Text("  Wear:      %.4f %s", cfg_it->second.wear,
                            (cfg_it->second.wear == w.fallback_wear) ? "(MATCH)" : "(MISMATCH!)");
                    } else {
                        ImGui::Separator();
                        ImGui::TextDisabled("No skin configured for this weapon");
                    }

                    ImGui::TreePop();
                }
            }
        }

        ImGui::End();
    }

    void RenderSkinChangerWindow() {
        ImGui::SetNextWindowSize(ImVec2(900, 600), ImGuiCond_FirstUseEver);
        ImGui::Begin("Skin Changer", nullptr, ImGuiWindowFlags_NoCollapse);

        // Initialize on first render
        if (!api_loaded && !loading_api) {
            debug_console::Console::Get().Info("Initializing skin changer...");
            Initialize();
        }

        if (loading_api) {
            ImGui::Text("Loading skins from API...");
        }
        else {
            ImGui::Text("Loaded %zu skins from database", skins::skin_database.size());
        }

        ImGui::Separator();

        // Auto-apply status and controls
        ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "SOC Skin Changer Active");
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Server Object Cache (SOC) bypass enabled\nSkins apply without server validation");
        }
        ImGui::Text("Skins will auto-apply when picking up weapons");
        ImGui::Separator();

        // Apply all button
        if (ImGui::Button("Apply All Skins Now", ImVec2(200, 30))) {
            debug_console::Console::Get().Info("Applying all configured skins...");
            skins::ApplyAllSkins();
            debug_console::Console::Get().Success("All skins applied!");
        }
        ImGui::SameLine();
        if (ImGui::Button("Apply Knife", ImVec2(120, 30))) {
            skins::ApplyKnife();
            debug_console::Console::Get().Success("Knife applied!");
        }
        ImGui::SameLine();
        if (ImGui::Button("Apply Gloves", ImVec2(120, 30))) {
            skins::ApplyGloves();
            debug_console::Console::Get().Success("Gloves applied!");
        }

        ImGui::Separator();

        // Quick access buttons
        if (ImGui::Button("Knife Changer")) {
            show_knife_menu = !show_knife_menu;
            debug_console::Console::Get().Debug("Knife menu toggled: %s", show_knife_menu ? "ON" : "OFF");
        }
        ImGui::SameLine();
        if (ImGui::Button("Glove Changer")) {
            show_glove_menu = !show_glove_menu;
            debug_console::Console::Get().Debug("Glove menu toggled: %s", show_glove_menu ? "ON" : "OFF");
        }
        ImGui::SameLine();
        if (ImGui::Button("Reload Skins")) {
            api_loaded = false;
            debug_console::Console::Get().Info("Reloading skins...");
            Initialize();
        }
        ImGui::SameLine();
        if (ImGui::Button(show_debug_panel ? "Hide Debug" : "Show Debug")) {
            show_debug_panel = !show_debug_panel;
            debug_console::Console::Get().Debug("Debug panel toggled: %s", show_debug_panel ? "ON" : "OFF");
        }

        ImGui::Separator();

        // Main layout: 3 columns
        ImGui::BeginChild("MainLayout");

        ImGui::Columns(3, "SkinColumns");
        ImGui::SetColumnWidth(0, 220);
        ImGui::SetColumnWidth(1, 320);

        // Column 1: Weapon list
        RenderWeaponSkinSelector();

        ImGui::NextColumn();

        // Column 2: Skin list
        RenderSkinList();

        ImGui::NextColumn();

        // Column 3: Configuration
        RenderSkinConfig();

        ImGui::Columns(1);
        ImGui::EndChild();

        ImGui::End();

        // Render popup windows
        if (show_knife_menu) {
            RenderKnifeMenu();
        }

        if (show_glove_menu) {
            RenderGloveMenu();
        }

        if (show_debug_panel) {
            RenderDebugPanel();
        }
    }

    void RenderQuickAccessTab() {
        ImGui::Text("Quick Skin Changer");
        ImGui::Separator();

        ImGui::Text("Configured Skins: %zu", skins::user_skins.size());

        if (ImGui::Button("Open Full Skin Changer", ImVec2(-1, 40))) {
            config::skin_changer::show_window = true;
        }

        ImGui::Separator();

        // Quick toggles
        ImGui::Checkbox("Enable Skin Changer", &config::skin_changer::enabled);

        if (config::skin_changer::enabled) {
            ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "Skins will be applied on weapon pickup");
        }

        ImGui::Separator();

        // Recently configured weapons
        if (!skins::user_skins.empty()) {
            ImGui::Text("Recently Configured:");
            int count = 0;
            for (const auto& pair : skins::user_skins) {
                if (count++ >= 5) break;

                const char* weapon_name = skins::GetWeaponName(pair.first);
                ImGui::BulletText("%s", weapon_name);
            }
        }
    }
}