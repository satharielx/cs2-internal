#include "../menu_advanced.h"
#include "widgets.h"
#include "../config_store.h"
#include "../camera_view.h"
#include "../lighting.h"
#include "../skybox.h"
#include "../scene_visuals.h"
#include "../engine_trace.h"
#include <cstdio>

namespace menu_advanced {
    // Render triggerbot tab.
    void RenderTriggerBotTab() {
        SectionHeader("Triggerbot Auto-Fire", ICON_FA_BOLT);

        BeginCard("##TriggerCard");
        ImGui::Indent(10);
        ModernToggle("Enable Triggerbot", &config::misc::trigger_bot);
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::Text("Trigger Keybind: Left Shift");
        ImGui::Text("Fire Delay (Seconds):");
        ImGui::SliderFloat("##TriggerDelay", &config::misc::trigger_delay, 0.0f, 0.5f, "%.2f seconds");
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::TextColored(ImVec4(0.4f, 0.9f, 0.5f, 1.0f), "Automatically fires weapon when crosshair hovers over enemy player.");
        ImGui::Unindent(10);
        EndCard();
    }

    // Render items tab.
    void RenderItemsTab() {
        SectionHeader("World & Item ESP", ICON_FA_CUBE);
        BeginCard("##ItemsCard");
        ImGui::Indent(10);
        ModernToggle("Dropped Items / Weapons on Ground", &config::world::items);
        ModernToggle("Planted Bomb C4", &config::world::bomb);
        ModernToggle("Active Grenade Projectile Warning", &config::world::grenade_warning);
        ImGui::Dummy(ImVec2(0, 8));
        ImGui::TextWrapped("World ESP displays item labels, planted C4 detonation status, and active grenade locations.");
        ImGui::Unindent(10);
        EndCard();
    }

    // Render view tab.
    void RenderViewTab() {
        SectionHeader("View & Camera Settings", ICON_FA_EYE);
        BeginCard("##ViewCard");
        ImGui::Indent(10);
        ModernToggle("Camera FOV Changer", &config::viewmodel::camera_fov_enabled);
        ImGui::BeginDisabled(!config::viewmodel::camera_fov_enabled);
        ImGui::SliderFloat("Camera Field of View", &config::viewmodel::camera_fov, 40.0f, 140.0f, "%.0f deg");
        ImGui::EndDisabled();

        ImGui::Dummy(ImVec2(0, 6));
        ModernToggle("Viewmodel Offset Editor", &config::viewmodel::offsets_enabled);
        ImGui::BeginDisabled(!config::viewmodel::offsets_enabled);
        ImGui::SliderFloat3("Viewmodel Offset X / Y / Z", config::viewmodel::offset, -20.0f, 20.0f);
        ImGui::EndDisabled();

        ImGui::Dummy(ImVec2(0, 6));
        ModernToggle("Custom Viewmodel FOV", &config::viewmodel::fov_enabled);
        ImGui::BeginDisabled(!config::viewmodel::fov_enabled);
        ImGui::SliderFloat("Viewmodel FOV", &config::viewmodel::fov, 40.0f, 120.0f, "%.0f deg");
        ImGui::EndDisabled();

        ImGui::Dummy(ImVec2(0, 6));
        ModernToggle("No Visual Aim Punch (Zero Camera Shake)", &config::viewmodel::no_aim_punch);
        ImGui::TextDisabled("Uses input angles for the rendered camera; recoil compensation is controlled separately.");
        ImGui::TextWrapped("%s", camera_view::status.load());

        ImGui::Separator();
        ModernToggle("Night Mode Lighting", &config::lighting::night_mode);
        ModernToggle("Remove Sun", &config::lighting::remove_sun);
        ModernToggle("Remove Clouds", &config::skybox::remove_clouds);
        ModernToggle("Hide Sky Background", &config::skybox::hide_background);
        ImGui::TextWrapped("Removes sun/cloud geometry by material name. Hide the sky background to remove a sun baked into its texture.");
        ImGui::TextWrapped("%s",scene_visuals::status.load());
        ImGui::SliderFloat("Night Brightness", &config::lighting::night_brightness, .02f, 1.0f, "%.2f");
        ImGui::TextWrapped("%s", lighting::status.load());
        ModernToggle("Custom Light Color", &config::lighting::enabled);
        ImGui::BeginDisabled(!config::lighting::enabled);
        ImGui::ColorEdit3("Light Color", config::lighting::color);
        ImGui::SliderFloat("Light Brightness", &config::lighting::brightness, 0.0f, 5.0f, "%.2f");
        ImGui::EndDisabled();

        ImGui::Separator();
        ModernToggle("Custom Sky Color", &config::skybox::color_enabled);
        ImGui::ColorEdit3("Sky Color", config::skybox::color);
        ImGui::SliderFloat("Sky Brightness", &config::skybox::brightness, 0.0f, 5.0f, "%.2f");
        ModernToggle("Custom Skybox Material", &config::skybox::enabled);
        static char sky_material[256] = "materials/skybox/sky_day02_01.vmat";
        ImGui::InputText("Sky Material Path", sky_material, sizeof(sky_material));
        if (ImGui::Button("Apply Skybox Material", ImVec2(180, 26))) {
            strcpy_s(config::skybox::material, sky_material);
            config::skybox::enabled = true;
            ++skybox::world_revision;
        }
        ImGui::SameLine();
        if (ImGui::Button("Restore Default Map Sky", ImVec2(180, 26))) config::skybox::enabled = false;

        ImGui::Separator();
        ModernToggle("No Flashbang Effect", &config::misc::no_flash);
        ModernToggle("Radar Hack (Show Enemies on Game Radar)", &config::misc::radar_hack);
        ImGui::Unindent(10);
        EndCard();
    }

    // Render hud tab.
    void RenderHudTab() {
        SectionHeader("HUD & Overlays", ICON_FA_SLIDERS_H);
        BeginCard("##HudCard");
        ImGui::Indent(10);
        ModernToggle("Spectator List Window", &config::misc::spectator_list);
        ModernToggle("Hit Marker Crosshair", &config::misc::hit_marker);
        ModernToggle("Hit Impact Ring Effect", &config::misc::hit_effect);
        ModernToggle("Kill Flash Ring Effect", &config::misc::kill_effect);
        ModernToggle("Knife Attack Range Indicator", &config::misc::knife_range);
        ModernToggle("Zeus Taser Range Indicator", &config::misc::taser_range);
        ModernToggle("External 2D Radar Overlay", &config::misc::external_radar);
        ImGui::SliderFloat("Radar Scale", &config::misc::radar_scale, .25f, 4.0f, "%.2fx");
        ImGui::SliderFloat("Radar Opacity", &config::misc::radar_alpha, 0.0f, 1.0f, "%.2f");
        ImGui::Unindent(10);
        EndCard();
    }

    // Render main tab.
    void RenderMainTab() {
        SectionHeader("General & Misc Settings", ICON_FA_HOME);
        BeginCard("##MainMiscCard");
        ImGui::Indent(10);
        ModernToggle("Auto Bunny Hop", &config::misc::bunny_hop);
        ModernToggle("Auto Strafe", &config::misc::auto_strafe);
        ModernToggle("Show Player Money in ESP", &config::misc::show_money);
        ModernToggle("No Flashbang Blindness", &config::misc::no_flash);
        ModernToggle("Engine Radar Hack", &config::misc::radar_hack);
        ImGui::Unindent(10);
        EndCard();
    }

    // Render movement tab.
    void RenderMovementTab() {
        SectionHeader("Movement Assistant", ICON_FA_RUNNING);
        BeginCard("##MovementCard");
        ImGui::Indent(10);
        ModernToggle("Automatic Bunny Hop (BHop)", &config::misc::bunny_hop);
        ImGui::TextDisabled("Hold spacebar to continuously jump at peak velocity.");
        ModernToggle("Auto Strafe", &config::misc::auto_strafe);
        ImGui::TextWrapped("Hold Space in the air and turn the mouse to strafe. A/D takes priority; disabled on ladders and in noclip.");
        ModernToggle("Strafe Assist", &config::misc::strafe_assist);
        ImGui::TextWrapped("Maintains lateral air movement relative to your velocity while holding Space. A/D takes priority.");
        ModernToggle("Quick Stop", &config::misc::quick_stop);
        ImGui::TextWrapped("Counter-strafes on the ground when you release WASD.");
        ModernToggle("Edge Jump", &config::misc::edge_jump);
        ImGui::TextWrapped("Hold the edge key to jump before stepping off a ledge. Requires working collision traces.");
        ImGui::SliderInt("Edge key (VK)", &config::misc::edge_jump_key, 1, 255);
        ImGui::TextDisabled("Default: Alt (18)");
        ImGui::Unindent(10);
        EndCard();
    }

    // Render grenades tab.
    void RenderGrenadesTab() {
        SectionHeader("Grenade Helper & Physics Predictor", ICON_FA_BOMB);
        BeginCard("##GrenadeCard");
        ImGui::Indent(10);
        ModernToggle("Grenade Trajectory Physics Prediction", &config::grenade::trajectory);
        ModernToggle("Thrown Grenade Trail", &config::grenade::thrown_trail);
        ModernToggle("Surface Pointing & Bounce Calculation Inspector", &config::grenade::surface_info);
        ImGui::ColorEdit4("Trajectory Color", config::grenade::color);
        ImGui::ColorEdit4("Predicted Landing Spot Color", config::grenade::landing_color);
        ImGui::Separator();
        ModernToggle("Active Grenade Projectile Warning", &config::world::grenade_warning);
        ModernToggle("Custom Smoke Grenade Color", &config::world::smoke_color_enabled);
        ImGui::ColorEdit3("Smoke Tint", config::world::smoke_color);
        ImGui::Dummy(ImVec2(0, 6));
        ImGui::TextWrapped("%s", engine_trace::faulted.load()
            ? "Collision trace faulted; reload to retry."
            : engine_trace::functions.Ready() ? "Collision traces ready. Hold a grenade to preview its path."
            : engine_trace::MissingStatus());
        ImGui::Unindent(10);
        EndCard();
    }

    // Render configs tab.
    void RenderConfigsTab() {
        SectionHeader("Configuration Presets & Management", ICON_FA_SAVE);
        BeginCard("##ConfigCard");
        ImGui::Indent(10);
        ImGui::Text("Quick Config Presets:");
        if (ImGui::Button("Legit Preset", ImVec2(130, 32))) {
            config::aimbot::hitboxes[0] = true;
            config::aimbot::enabled = true; config::aimbot::fov = 2.5f; config::aimbot::smoothing = 0.5f;
            config::rcs::enabled = true; config::rcs::strength = 0.75f;
            config::esp::enabled = true; config::esp::box = true; config::esp::skeleton = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Rage Preset", ImVec2(130, 32))) {
            config::aimbot::hitboxes[0] = true;
            config::aimbot::enabled = true; config::aimbot::silent_aim = true; config::aimbot::fov = 15.0f;
            config::aimbot::smoothing = 0.0f; config::rcs::enabled = true; config::rcs::strength = 1.0f;
            config::esp::enabled = true; config::esp::box = true; config::esp::skeleton = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Visuals Only", ImVec2(130, 32))) {
            config::aimbot::enabled = false; config::esp::enabled = true;
            config::esp::box = true; config::esp::skeleton = true; config::esp::glow = true;
        }
        ImGui::Separator();
        static char configPath[512] = "";
        if (!configPath[0]) {
            wchar_t appData[MAX_PATH]{};
            const auto length = GetEnvironmentVariableW(L"LOCALAPPDATA", appData, MAX_PATH);
            const auto folder = (length && length < MAX_PATH ? std::filesystem::path(appData) : std::filesystem::temp_directory_path()) / "Nephilim" / "Configs";
            std::error_code error; std::filesystem::create_directories(folder, error);
            snprintf(configPath, sizeof(configPath), "%s", (folder / "default.cfg").string().c_str());
        }
        ImGui::InputText("Configuration file", configPath, sizeof(configPath));
        if (ImGui::Button("Save Configuration", ImVec2(170, 32))) {
            config_store::Save(std::filesystem::path(configPath));
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Configuration", ImVec2(170, 32))) {
            config_store::Load(std::filesystem::path(configPath));
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset to Defaults", ImVec2(170, 32))) {
            config_store::Reset();
        }
        ImGui::TextWrapped("%s", config_store::status.c_str());
        ImGui::Unindent(10);
        EndCard();
    }

}
