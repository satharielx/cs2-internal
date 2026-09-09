#pragma once
#include <array>
#include <Windows.h>
#include <mutex>

namespace config {
    inline std::recursive_mutex mutex;
    // ESP Settings
    namespace esp {
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

    // Aimbot Settings
    namespace aimbot {
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

    // RCS (Recoil Control System) — only active while shooting
    namespace rcs {
        inline bool enabled = false;
        inline float strength = 1.0f;  // 0.0 – 1.0
    }

    // Misc Settings
    namespace misc {
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