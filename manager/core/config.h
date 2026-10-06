#pragma once
#include <array>
#include <Windows.h>
#include <mutex>

namespace config {
    inline std::recursive_mutex mutex;
    namespace lighting {
        inline bool night_mode = false;
        inline float night_brightness = .25f;
        inline bool enabled = false;
        inline float color[3] = {1.0f, 1.0f, 1.0f};
        inline float brightness = 3.0f;
    }
    namespace viewmodel {
        inline bool offsets_enabled = false;
        inline float offset[3] = {};
        inline bool camera_fov_enabled = false;
        inline float camera_fov = 90.0f;
        inline bool fov_enabled = false;
        inline float fov = 68.0f;
        inline bool no_aim_punch = false;
    }
    namespace skybox {
        inline bool color_enabled = false;
        inline float color[3] = {1.0f, 1.0f, 1.0f};
        inline float brightness = 3.0f;
        inline bool enabled = false;
        inline char material[256] = "materials/skybox/sky_day02_01.vmat";
    }
    // ESP Settings
    namespace esp {
        inline bool glow = false;
        inline float glow_color[4] = {1, .2f, .1f, .8f};
        inline bool preview = true;
        inline bool armor_bar = false, weapon_text = false, bomb = false, defuse_kit = false;
        inline bool flashed = false, scoped = false, planting = false, defusing = false, hostage = false;
        inline bool out_of_fov_arrows = false;
        inline float scale = 1.0f, alpha = 1.0f;
        inline bool enabled = false;
        inline bool box = true;
        inline bool skeleton = true;
        inline bool health_bar = true;
        inline bool name = true;
        inline bool distance = true;
        inline bool snaplines = false;
        inline bool team_check = true;
        inline float box_color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };  // Red
        inline float team_color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };  // Green
        inline float skeleton_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };  // White
        inline float max_distance = 300.0f;
    }

    namespace world {
        inline bool items = false, bomb = false, grenade_warning = false, smoke_color_enabled = false;
        inline float smoke_color[3] = {1, 1, 1};
    }

    // Aimbot Settings
    namespace aimbot {
        // Head, neck, chest and pelvis; hitscan chooses the nearest enabled bone in FOV.
        inline bool hitboxes[4] = {true, false, false, false};
        inline bool hitscan = false, lock_target = false, draw_fov = false, autoscope = false;
        inline bool disable_flashed = false, disable_airborne = false, disable_scoped = false;
        inline float shot_delay = 0.0f, kill_delay = 0.0f;
        inline float fov_color[4] = {1, 1, 1, .65f};
        inline bool enabled = false;
        inline bool team_check = true;
        inline bool visible_check = true;
        inline bool auto_shoot = false;
        inline int pause_key = VK_XBUTTON2;  // Hold to pause aimbot (Mouse5)
        inline float max_distance = 10000.0f;  // Very high limit for long-range (in game units)
        inline float fov = 10.0f;  // FOV limit for aimlock (degrees) - increased for long-range
        inline float smoothing = 0.3f;  // Smoothing factor (0.0 = instant, 1.0 = no aim)
        inline bool silent_aim = true; // Enable instant silent aim (micro-flick)
    }

    // RCS (Recoil Control System) � only active while shooting
    namespace rcs {
        inline bool enabled = false;
        inline float strength = 1.0f;  // 0.0 � 1.0
    }

    // Misc Settings
    namespace misc {
        inline bool show_money = false, spectator_list = false;
        inline bool hit_marker = false, hit_effect = false, kill_effect = false;
        inline bool knife_range = false, taser_range = false;
        inline bool external_radar = false;
        inline float radar_scale = 1.0f, radar_alpha = .8f;
        inline bool bunny_hop = false;
        inline bool no_flash = false;
        inline bool radar_hack = false;
        inline bool trigger_bot = false;
        inline int trigger_key = VK_SHIFT;
        inline float trigger_delay = 0.0f;
    }

    // Skin Changer Settings
    namespace skin_changer {
        inline bool enabled = false;
        inline bool show_window = false;
    }

    // Colors
    namespace colors {
        inline float menu_accent[4] = { 0.26f, 0.59f, 0.98f, 1.0f };  // Blue
    }
}
