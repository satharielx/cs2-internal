#include "menu_advanced.h"
#include "config.h"
#include "skins.h"
#include "features.h"
#include "interfaces.h"
#include "../sdk/entity.h"

namespace menu_advanced {
    // Knife selector state
    static int selected_knife = 507; // Karambit by default
    static bool show_knife_preview = true;
    static int knife_filter = 0; // 0 = all, 1 = expensive, 2 = cheap
    static bool show_knife_stats = true;
    static int last_applied_knife = -1;

    // Weapon selector state
    static int selected_weapon_tab = 0;
    static int selected_weapon_skin = -1;
    static std::vector<skins::SkinInfo> current_weapon_skins;

    // Glove state
    static int selected_glove = 0;
    static const int glove_ids[] = { 10006, 10007, 10015, 10016, 10018, 10024 };
    static const char* glove_names[] = { "Superconductor", "Arid", "Pandora's Box", "Foundation", "Vice", "Emerald Web" };

    struct KnifeInfo {
        int id;
        const char* name;
        int price; // Relative price: 1 = cheap, 2 = medium, 3 = expensive
    };

    const KnifeInfo knives[] = {
        { 500, "Bayonet", 2 },
        { 505, "Flip Knife", 1 },
        { 506, "Gut Knife", 1 },
        { 507, "Karambit", 3 },
        { 508, "M9 Bayonet", 2 },
        { 509, "Tactical Knife", 1 },
        { 512, "Falchion", 1 },
        { 514, "Survival Bowie", 2 },
        { 515, "Butterfly", 2 },
        { 516, "Bowie Knife", 2 },
        { 519, "Ursus Knife", 2 },
        { 520, "Gypsy Jackknife", 1 },
        { 522, "Stiletto", 2 },
        { 523, "Widowmaker", 3 },
        { 525, "Skeleton Knife", 3 }
    };

    const char* GetKnifeName(int knife_id) {
        for (int i = 0; i < 15; i++) {
            if (knives[i].id == knife_id) {
                return knives[i].name;
            }
        }
        return "Unknown";
    }

    const char* GetKnifePrice(int knife_id) {
        for (int i = 0; i < 15; i++) {
            if (knives[i].id == knife_id) {
                switch (knives[i].price) {
                    case 1: return "Budget";
                    case 2: return "Mid-Tier";
                    case 3: return "Premium";
                    default: return "Unknown";
                }
            }
        }
        return "Unknown";
    }

    void RenderKnifeTab() {
        ImGui::Columns(2, "KnifeColumns", true);

        // Left side - Knife list
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Available Knives");
        ImGui::Separator();

        // Filter options
        ImGui::Text("Filter:");
        ImGui::RadioButton("All Knives##filter", &knife_filter, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Budget##filter", &knife_filter, 1);
        ImGui::SameLine();
        ImGui::RadioButton("Premium##filter", &knife_filter, 2);

        ImGui::Separator();
        ImGui::Text("Knife Selection:");
        ImGui::BeginChild("KnifeList", ImVec2(0, 300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

        for (int i = 0; i < 15; i++) {
            const KnifeInfo& knife = knives[i];
            int price = knife.price;
            
            // Apply filter
            if (knife_filter == 1 && price != 1) continue; // Budget
            if (knife_filter == 2 && price != 3) continue; // Premium

            bool is_selected = (selected_knife == knife.id);
            ImVec4 color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
            
            if (price == 3) color = ImVec4(1.0f, 0.8f, 0.0f, 1.0f); // Premium - Gold
            else if (price == 2) color = ImVec4(0.7f, 0.9f, 1.0f, 1.0f); // Mid - Blue
            else color = ImVec4(0.7f, 1.0f, 0.7f, 1.0f); // Budget - Green

            ImGui::PushStyleColor(ImGuiCol_Text, color);
            
            std::string label = std::string(knife.name) + " (" + GetKnifePrice(knife.id) + ")";
            
            if (ImGui::Selectable(label.c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(0, 25))) {
                selected_knife = knife.id;
                if (ImGui::IsMouseDoubleClicked(0)) {
                    ApplySelectedKnife(knife.id);
                }
            }
            
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();

        ImGui::NextColumn();

        // Right side - Preview and details
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.8f, 1.0f), "Knife Details");
        ImGui::Separator();

        ImGui::BeginChild("KnifePreview", ImVec2(0, 300), true);
        
        ImGui::Text("Selected: %s", GetKnifeName(selected_knife));
        ImGui::Text("Category: %s", GetKnifePrice(selected_knife));
        ImGui::Separator();

        // Stats
        if (show_knife_stats) {
            ImGui::Text("Rarity: Covert");
            ImGui::Text("Float Range: 0.00 - 1.0");
            ImGui::Text("Available Conditions:");
            ImGui::BulletText("Factory New");
            ImGui::BulletText("Minimal Wear");
            ImGui::BulletText("Field-Tested");
            ImGui::BulletText("Well-Worn");
            ImGui::BulletText("Battle-Scarred");
            ImGui::Separator();
            ImGui::TextWrapped("Double-click a knife to apply immediately.");
        }

        ImGui::EndChild();

        ImGui::Columns(1);
        ImGui::Separator();

        // Action buttons
        if (ImGui::Button("Apply Selected Knife", ImVec2(200, 35))) {
            ApplySelectedKnife(selected_knife);
        }
        ImGui::SameLine();
        if (ImGui::Button("Random Knife", ImVec2(150, 35))) {
            int random_idx = rand() % 15;
            selected_knife = knives[random_idx].id;
            ApplySelectedKnife(selected_knife);
        }

        ImGui::Checkbox("Show Stats##knife", &show_knife_stats);
        ImGui::SameLine();
        ImGui::Checkbox("Auto Preview##knife", &show_knife_preview);

        if (last_applied_knife != -1) {
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Last Applied: %s", GetKnifeName(last_applied_knife));
        }
    }

    void RenderESPTab() {
        auto RenderCard = [](const char* title, auto content_func) {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.15f, 0.8f));
            ImGui::BeginChild(title, ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 1.0f, 0.8f, 1.0f));
            ImGui::Text("%s", title);
            ImGui::PopStyleColor();
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 5));
            
            content_func();
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        };

        // Main ESP Toggle
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.12f, 0.18f, 0.9f));
        ImGui::BeginChild("##ESPMaster", ImVec2(0, 80), true);
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::Indent(15);
        
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
        ImGui::Checkbox("ENABLE ESP SYSTEM", &config::esp::enabled);
        ImGui::PopStyleVar();
        
        ImGui::SameLine(300);
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), 
            "Status: %s", config::esp::enabled ? "ACTIVE" : "DISABLED");
        
        ImGui::Unindent(15);
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::Dummy(ImVec2(0, 10));

        // Two-column layout for cards
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, 420);

        // Left Column - Rendering Features
        RenderCard("RENDERING FEATURES", []() {
            ImGui::Indent(10);
            
            ImGui::Checkbox("Box ESP", &config::esp::box);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Player bounding boxes");
            
            ImGui::Checkbox("Skeleton", &config::esp::skeleton);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Bone structure");
            
            ImGui::Checkbox("Health Bar", &config::esp::health_bar);
            ImGui::SameLine(200);
            ImGui::TextDisabled("HP indicator");
            
            ImGui::Checkbox("Distance", &config::esp::distance);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Distance in meters");
            
            ImGui::Checkbox("Snaplines", &config::esp::snaplines);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Lines to players");
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        });

        ImGui::Dummy(ImVec2(0, 10));

        RenderCard("FILTERS & OPTIONS", []() {
            ImGui::Indent(10);
            
            ImGui::Checkbox("Team Check", &config::esp::team_check);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Ignore teammates");
            
            ImGui::Dummy(ImVec2(0, 10));
            ImGui::Text("Max Distance:");
            ImGui::PushItemWidth(280);
            ImGui::SliderFloat("##MaxDistESP", &config::esp::max_distance, 50.0f, 500.0f, "%.0fm");
            ImGui::PopItemWidth();
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        });

        ImGui::NextColumn();

        // Right Column - Colors
        RenderCard("COLOR CONFIGURATION", []() {
            ImGui::Indent(10);
            
            ImGui::Text("Enemy Color:");
            ImGui::ColorEdit4("##EnemyColor", config::esp::box_color, 
                ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
            
            ImGui::Dummy(ImVec2(0, 5));
            ImGui::Text("Teammate Color:");
            ImGui::ColorEdit4("##TeamColor", config::esp::team_color, 
                ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
            
            ImGui::Dummy(ImVec2(0, 5));
            ImGui::Text("Skeleton Color:");
            ImGui::ColorEdit4("##SkeletonColor", config::esp::skeleton_color, 
                ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        });

        ImGui::Columns(1);
    }

    void RenderAimbotTab() {
        // Two-column layout similar to screenshot
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, 300);

        // LEFT COLUMN - Aimbot Settings
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        ImGui::BeginChild("##AimbotLeft", ImVec2(0, 400), true);
        
        ImGui::Indent(10);
        ImGui::Dummy(ImVec2(0, 5));
        
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Aimbot");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 8));

        ImGui::Checkbox("Enable", &config::aimbot::enabled);
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("Auto-fire");
        ImGui::Checkbox("##AutoFire", &config::aimbot::auto_shoot);
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("Auto-stop");
        ImGui::Checkbox("##AutoStop", &config::aimbot::team_check);
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("Auto-scope");
        bool auto_scope = false;
        ImGui::Checkbox("##AutoScope", &auto_scope);
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Checkbox("Disable aimbot while flashed", &config::aimbot::visible_check);
        ImGui::Checkbox("Disable aimbot through smoke", &config::aimbot::team_check);
        ImGui::Dummy(ImVec2(0, 10));

        ImGui::Text("Switch target delay");
        ImGui::PushItemWidth(230);
        float switch_delay = 0.100f;
        ImGui::SliderFloat("##SwitchDelay", &switch_delay, 0.0f, 1.0f, "%.3f");
        ImGui::PopItemWidth();
        ImGui::Dummy(ImVec2(0, 10));

        ImGui::Text("Multipoint");
        ImGui::Dummy(ImVec2(0, 5));
        const char* multipoint_items[] = { "Medium" };
        static int multipoint_current = 0;
        ImGui::PushItemWidth(230);
        ImGui::Combo("##Multipoint", &multipoint_current, multipoint_items, IM_ARRAYSIZE(multipoint_items));
        ImGui::PopItemWidth();
        ImGui::Dummy(ImVec2(0, 10));

        ImGui::Text("Mouse lock percentage x");
        ImGui::PushItemWidth(230);
        float mouse_lock_x = 1.000f;
        ImGui::SliderFloat("##MouseLockX", &mouse_lock_x, 0.0f, 2.0f, "%.3f");
        ImGui::PopItemWidth();
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("Mouse lock percentage y");
        ImGui::PushItemWidth(230);
        float mouse_lock_y = 1.000f;
        ImGui::SliderFloat("##MouseLockY", &mouse_lock_y, 0.0f, 2.0f, "%.3f");
        ImGui::PopItemWidth();

        ImGui::Unindent(10);
        ImGui::Dummy(ImVec2(0, 5));
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::NextColumn();

        // RIGHT COLUMN - Recoil/Triggerbot
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        ImGui::BeginChild("##TriggerRight", ImVec2(0, 200), true);
        
        ImGui::Indent(10);
        ImGui::Dummy(ImVec2(0, 5));
        
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Triggerbot");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 8));

        ImGui::Checkbox("Enable##Trigger", &config::misc::trigger_bot);
        ImGui::Dummy(ImVec2(0, 10));

        ImGui::Checkbox("Disable triggerbot while flashed", &config::misc::no_flash);
        ImGui::Checkbox("Disable triggerbot through smoke", &config::misc::radar_hack);
        ImGui::Dummy(ImVec2(0, 10));

        ImGui::Unindent(10);
        ImGui::Dummy(ImVec2(0, 5));
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::Dummy(ImVec2(0, 10));

        // Recoil section
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        ImGui::BeginChild("##RecoilRight", ImVec2(0, 190), true);
        
        ImGui::Indent(10);
        ImGui::Dummy(ImVec2(0, 5));
        
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Recoil");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 8));

        ImGui::Checkbox("Enable##Recoil", &config::rcs::enabled);
        ImGui::Dummy(ImVec2(0, 10));

        ImGui::Text("Return crosshair");
        bool return_crosshair = false;
        ImGui::Checkbox("##ReturnCrosshair", &return_crosshair);
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("Start bullet");
        int start_bullet = 1;
        ImGui::PushItemWidth(230);
        ImGui::SliderInt("##StartBullet", &start_bullet, 1, 30);
        ImGui::PopItemWidth();
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("X Axis");
        ImGui::PushItemWidth(230);
        float x_axis = 0.60f;
        ImGui::SliderFloat("##XAxis", &x_axis, 0.0f, 1.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("Y Axis");
        ImGui::PushItemWidth(230);
        float y_axis = 0.60f;
        ImGui::SliderFloat("##YAxis", &y_axis, 0.0f, 1.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("Smooth");
        ImGui::PushItemWidth(230);
        ImGui::SliderFloat("##Smooth", &config::rcs::strength, 0.0f, 2.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::Unindent(10);
        ImGui::Dummy(ImVec2(0, 5));
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::Columns(1);
    }

    void RenderSkinTab() {
        ImGui::BeginChild("Skin Tab", ImVec2(0, 0), false);
        
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Weapon Skins & Knife");
        ImGui::Separator();

        ImGui::Checkbox("Enable Skin Changer##main", &config::skin_changer::enabled);
        ImGui::Separator();

        // Tabs for different categories
        if (ImGui::BeginTabBar("SkinCategories")) {
            if (ImGui::BeginTabItem("Rifles")) {
                ImGui::Text("Primary Rifles:");
                ImGui::BulletText("AK-47 Variants");
                ImGui::BulletText("M4A4 Skins");
                ImGui::BulletText("M4A1-S Skins");
                ImGui::BulletText("AWP Dragon Lore");
                ImGui::Separator();
                if (ImGui::Button("Load Rifle Skins", ImVec2(150, 0))) {
                    // Load rifle skins
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Pistols")) {
                ImGui::Text("Secondary Weapons:");
                ImGui::BulletText("Desert Eagle");
                ImGui::BulletText("USP-S");
                ImGui::BulletText("P250");
                ImGui::BulletText("Glock-18");
                ImGui::Separator();
                if (ImGui::Button("Load Pistol Skins", ImVec2(150, 0))) {
                    // Load pistol skins
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("SMG/Heavy")) {
                ImGui::Text("Other Weapons:");
                ImGui::BulletText("MP7 Skins");
                ImGui::BulletText("UMP-45");
                ImGui::BulletText("Negev");
                ImGui::BulletText("M249");
                ImGui::Separator();
                if (ImGui::Button("Load SMG Skins", ImVec2(150, 0))) {
                    // Load SMG skins
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Knives")) {
                RenderKnifeTab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Gloves")) {
                ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Glove Selection");
                ImGui::Separator();

                ImGui::BeginChild("GloveList", ImVec2(0, 250), true);
                for (int i = 0; i < 6; i++) {
                    bool is_selected = (selected_glove == i);
                    std::string glove_label = std::string(glove_names[i]) + " (ID: " + std::to_string(glove_ids[i]) + ")";
                    
                    if (ImGui::Selectable(glove_label.c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick)) {
                        selected_glove = i;
                        if (ImGui::IsMouseDoubleClicked(0)) {
                            skins::ApplyGloves();
                        }
                    }
                }
                ImGui::EndChild();

                ImGui::Separator();
                ImGui::Text("Selected: %s", glove_names[selected_glove]);
                
                if (ImGui::Button("Apply Gloves", ImVec2(150, 35))) {
                    skins::ApplyGloves();
                }
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::EndChild();
    }

    void RenderUtilitiesTab() {
        auto RenderCard = [](const char* title, auto content_func, ImVec4 title_color = ImVec4(0.5f, 1.0f, 0.5f, 1.0f)) {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.15f, 0.8f));
            ImGui::BeginChild(title, ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
            
            ImGui::PushStyleColor(ImGuiCol_Text, title_color);
            ImGui::Text("%s", title);
            ImGui::PopStyleColor();
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 5));
            
            content_func();
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        };

        // Two-column layout
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, 420);

        // Left Column - Movement & Vision
        RenderCard("MOVEMENT & VISION", []() {
            ImGui::Indent(10);
            
            ImGui::Checkbox("Bunny Hop", &config::misc::bunny_hop);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Auto-jump");
            
            ImGui::Checkbox("No Flash", &config::misc::no_flash);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Remove flashbang");
            
            ImGui::Checkbox("Radar Hack", &config::misc::radar_hack);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Show all on radar");
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        });

        ImGui::NextColumn();

        // Right Column - Combat
        RenderCard("COMBAT ASSIST", []() {
            ImGui::Indent(10);
            
            ImGui::Checkbox("Trigger Bot", &config::misc::trigger_bot);
            
            if (config::misc::trigger_bot) {
                ImGui::Dummy(ImVec2(0, 5));
                ImGui::Indent(20);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
                ImGui::Text("Trigger Key: Left Shift");
                ImGui::PopStyleColor();
                
                ImGui::Dummy(ImVec2(0, 5));
                ImGui::Text("Delay:");
                ImGui::PushItemWidth(230);
                ImGui::SliderFloat("##TriggerDelay", &config::misc::trigger_delay, 0.0f, 0.5f, "%.2fs");
                ImGui::PopItemWidth();
                ImGui::Unindent(20);
            }
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        }, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));

        ImGui::Columns(1);
    }

    void RenderSettingsTab() {
        auto RenderCard = [](const char* title, auto content_func, ImVec4 title_color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f)) {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.15f, 0.8f));
            ImGui::BeginChild(title, ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
            
            ImGui::PushStyleColor(ImGuiCol_Text, title_color);
            ImGui::Text("%s", title);
            ImGui::PopStyleColor();
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 5));
            
            content_func();
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        };

        // Info Banner
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.10f, 0.18f, 0.9f));
        ImGui::BeginChild("##InfoBanner", ImVec2(0, 90), true);
        ImGui::Dummy(ImVec2(0, 15));
        ImGui::Indent(20);
        
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.5f, 1.0f), "CS2 PRO CHEAT");
        ImGui::PopFont();
        
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
        ImGui::Text("Version 2.0 | Latest Offsets: 2025-02-10");
        ImGui::PopStyleColor();
        
        ImGui::Unindent(20);
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::Dummy(ImVec2(0, 10));

        // Two-column layout
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, 420);

        // Left Column
        RenderCard("CONTROLS", []() {
            ImGui::Indent(10);
            
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.8f, 1.0f), "[INSERT]");
            ImGui::SameLine(120);
            ImGui::Text("Toggle Menu");
            
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "[END]");
            ImGui::SameLine(120);
            ImGui::Text("Unload Cheat");
            
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "[MOUSE5]");
            ImGui::SameLine(120);
            ImGui::Text("Pause Aimbot (hold)");
            
            ImGui::TextColored(ImVec4(0.8f, 0.6f, 1.0f, 1.0f), "[L-SHIFT]");
            ImGui::SameLine(120);
            ImGui::Text("Trigger Bot");
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        }, ImVec4(0.6f, 0.8f, 1.0f, 1.0f));

        ImGui::Dummy(ImVec2(0, 10));

        RenderCard("SYSTEM STATUS", []() {
            ImGui::Indent(10);
            
            ImGui::Text("Status:");
            ImGui::SameLine(120);
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "RUNNING");
            
            ImGui::Text("FPS:");
            ImGui::SameLine(120);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), "%.0f", ImGui::GetIO().Framerate);
            
            ImGui::Text("Render:");
            ImGui::SameLine(120);
            ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "DX11");
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        }, ImVec4(0.4f, 1.0f, 0.4f, 1.0f));

        ImGui::NextColumn();

        // Right Column
        RenderCard("QUICK ACTIONS", []() {
            ImGui::Indent(10);
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 0.8f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
            
            if (ImGui::Button("Reset All Settings", ImVec2(340, 40))) {
                config::esp::enabled = false;
                config::aimbot::enabled = false;
                config::misc::bunny_hop = false;
                config::skin_changer::enabled = false;
            }
            
            ImGui::PopStyleColor(3);
            
            ImGui::Dummy(ImVec2(0, 10));
            
            if (ImGui::Button("Disable All Features", ImVec2(165, 35))) {
                config::esp::enabled = false;
                config::aimbot::enabled = false;
                config::rcs::enabled = false;
                config::misc::bunny_hop = false;
                config::misc::no_flash = false;
                config::misc::radar_hack = false;
                config::misc::trigger_bot = false;
            }
            
            ImGui::SameLine();
            
            if (ImGui::Button("Enable Safe Mode", ImVec2(165, 35))) {
                config::esp::enabled = true;
                config::aimbot::enabled = false;
                config::misc::bunny_hop = true;
                config::misc::no_flash = true;
            }
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        }, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));

        ImGui::Columns(1);
    }

    // Current selected tab
    static int selected_tab = 0;

    void RenderMainMenu() {
        static int weapon_tab = 0; // 0 = Globals, 1 = Weapons
        
        ImGui::SetNextWindowSize(ImVec2(850, 600), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(100, 50), ImGuiCond_FirstUseEver);

        // Modern dark theme
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.11f, 0.11f, 0.11f, 0.98f));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.13f, 0.13f, 0.13f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

        if (ImGui::Begin("MIDNIGHT", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
            // ===================== LEFT SIDEBAR =====================
            ImGui::BeginChild("##Sidebar", ImVec2(200, 0), false, ImGuiWindowFlags_NoScrollbar);
            
            // MIDNIGHT logo with icon
            ImGui::Dummy(ImVec2(0, 15));
            ImGui::Indent(20);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
            ImGui::Text("  MIDNIGHT");
            ImGui::PopStyleColor();
            ImGui::Unindent(20);
            
            ImGui::Dummy(ImVec2(0, 25));

            // Category sections
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            ImGui::Indent(20);
            ImGui::Text("Combat");
            ImGui::Unindent(20);
            ImGui::PopStyleColor();
            ImGui::Dummy(ImVec2(0, 5));

            // Tab button helper
            auto TabButton = [](const char* icon, const char* label, int index, int* current_tab) -> bool {
                bool is_selected = (*current_tab == index);
                
                ImGui::Indent(10);
                
                if (is_selected) {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.18f, 0.18f, 0.18f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.20f, 0.20f, 0.20f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                } else {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
                }

                bool clicked = ImGui::Button((std::string(icon) + " " + label).c_str(), ImVec2(180, 30));
                ImGui::PopStyleColor(4);
                ImGui::Unindent(10);

                if (clicked) *current_tab = index;
                return clicked;
            };

            TabButton("", "Aimbot", 0, &selected_tab);
            TabButton("", "Triggerbot", 1, &selected_tab);

            ImGui::Dummy(ImVec2(0, 15));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            ImGui::Indent(20);
            ImGui::Text("Visuals");
            ImGui::Unindent(20);
            ImGui::PopStyleColor();
            ImGui::Dummy(ImVec2(0, 5));

            TabButton("", "Players", 2, &selected_tab);
            TabButton("", "Items", 3, &selected_tab);
            TabButton("", "View", 4, &selected_tab);
            TabButton("", "Hud", 5, &selected_tab);

            ImGui::Dummy(ImVec2(0, 15));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            ImGui::Indent(20);
            ImGui::Text("Misc");
            ImGui::Unindent(20);
            ImGui::PopStyleColor();
            ImGui::Dummy(ImVec2(0, 5));

            TabButton("", "Main", 6, &selected_tab);
            TabButton("", "Movement", 7, &selected_tab);

            ImGui::Dummy(ImVec2(0, 15));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            ImGui::Indent(20);
            ImGui::Text("Cheat");
            ImGui::Unindent(20);
            ImGui::PopStyleColor();
            ImGui::Dummy(ImVec2(0, 5));

            TabButton("", "Inventory", 8, &selected_tab);
            TabButton("", "Grenades", 9, &selected_tab);
            TabButton("", "Configs", 10, &selected_tab);

            ImGui::EndChild();

            // ===================== MAIN CONTENT AREA =====================
            ImGui::SameLine();
            ImGui::BeginChild("##MainContent", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);
            
            // Top bar with tabs
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09f, 0.09f, 0.09f, 1.0f));
            ImGui::BeginChild("##TopBar", ImVec2(0, 50), false, ImGuiWindowFlags_NoScrollbar);
            
            ImGui::Dummy(ImVec2(0, 8));
            ImGui::Indent(15);

            // GLOBALS and WEAPONS tabs
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 8));
            
            if (weapon_tab == 0) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            }
            if (ImGui::Button("GLOBALS")) weapon_tab = 0;
            ImGui::PopStyleColor(2);
            
            ImGui::SameLine();
            
            if (weapon_tab == 1) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            }
            if (ImGui::Button("WEAPONS")) weapon_tab = 1;
            ImGui::PopStyleColor(2);
            
            ImGui::PopStyleVar();

            // Top-right icons (search, settings, user)
            float icon_x = ImGui::GetWindowWidth() - 100;
            ImGui::SetCursorPos(ImVec2(icon_x, 15));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
            ImGui::Text(" ");  // search icon
            ImGui::SameLine();
            ImGui::Text(" ");  // settings icon
            ImGui::SameLine();
            ImGui::Text(" ");  // user icon
            ImGui::PopStyleColor();

            ImGui::Unindent(15);
            ImGui::EndChild();
            ImGui::PopStyleColor();

            // VAC warning banner
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.8f, 0.2f, 0.2f, 0.9f));
            ImGui::BeginChild("##VACWarning", ImVec2(0, 30), false, ImGuiWindowFlags_NoScrollbar);
            ImGui::Dummy(ImVec2(0, 5));
            ImGui::Indent(15);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImGui::Text("Valve has updated its VAC Live neural network anti-cheat system. Be careful when using features such as Aimbot, Triggerbot, and Recoil Control.");
            ImGui::PopStyleColor();
            ImGui::Unindent(15);
            ImGui::EndChild();
            ImGui::PopStyleColor();

            ImGui::Dummy(ImVec2(0, 5));

            // Main scrollable content area
            ImGui::BeginChild("##ContentScroll", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
            
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 15));

            switch (selected_tab) {
                case 0: RenderAimbotTab(); break;
                case 1: RenderTriggerBotTab(); break;
                case 2: RenderESPTab(); break;
                case 3: RenderItemsTab(); break;
                case 4: RenderViewTab(); break;
                case 5: RenderHudTab(); break;
                case 6: RenderMainTab(); break;
                case 7: RenderMovementTab(); break;
                case 8: RenderSkinTab(); break;
                case 9: RenderGrenadesTab(); break;
                case 10: RenderConfigsTab(); break;
            }

            ImGui::PopStyleVar();
            ImGui::EndChild();

            ImGui::EndChild();

            ImGui::End();
        }

        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(5);
    }

    void RenderTriggerBotTab() {
        ImGui::Text("Triggerbot configuration");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 10));

        ImGui::Checkbox("Enable Triggerbot", &config::misc::trigger_bot);
        ImGui::Text("Trigger delay:");
        ImGui::SliderFloat("##TriggerDelay", &config::misc::trigger_delay, 0.0f, 0.5f, "%.2fs");
    }

    void RenderItemsTab() {
        ImGui::Text("Item ESP and visualization");
        ImGui::Separator();
        ImGui::Text("Weapon drops, grenades, and other items");
    }

    void RenderViewTab() {
        ImGui::Text("View settings");
        ImGui::Separator();
        ImGui::Checkbox("No Flash", &config::misc::no_flash);
        ImGui::Checkbox("Third Person (experimental)", &config::misc::radar_hack);
    }

    void RenderHudTab() {
        ImGui::Text("HUD customization");
        ImGui::Separator();
        ImGui::Text("Customize your in-game HUD elements");
    }

    void RenderMainTab() {
        ImGui::Text("Main miscellaneous settings");
        ImGui::Separator();
        ImGui::Checkbox("Bunny Hop", &config::misc::bunny_hop);
        ImGui::Checkbox("Radar Hack", &config::misc::radar_hack);
    }

    void RenderMovementTab() {
        ImGui::Text("Movement enhancements");
        ImGui::Separator();
        ImGui::Checkbox("Auto Strafe", &config::misc::bunny_hop);
    }

    void RenderGrenadesTab() {
        ImGui::Text("Grenade helper");
        ImGui::Separator();
        ImGui::Text("Grenade trajectories and lineups");
    }

    void RenderConfigsTab() {
        ImGui::Text("Configuration management");
        ImGui::Separator();
        
        if (ImGui::Button("Save Config", ImVec2(200, 30))) {
            // Save config logic
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Config", ImVec2(200, 30))) {
            // Load config logic
        }
    }

    void ApplySelectedKnife(int knife_id) {
        try {
            // Get local player
            auto local_player = features::GetLocalPlayer();
            if (!local_player) return;

            // Enable skin changer
            config::skin_changer::enabled = true;
            
            // Store selected knife
            selected_knife = knife_id;
            last_applied_knife = knife_id;

            // Call skins system to apply knife
            skins::ApplyKnife();
        }
        catch (...) {
            // Error applying knife
        }
    }
}
