#include "features.h"
#include "config.h"
#include "interfaces.h"
#include "debug_console.h"
#include "game_state.h"
#include "../external/imgui/imgui.h"
#include "../external/offsets/offsets.hpp"
#include "../sdk/mem.h"
#include "../sdk/source2sdk_offsets.h"
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <algorithm>
#include <cmath>
#include <thread>
#include <atomic>

namespace features {
// Per-frame cached values (read from game_state atomics once per frame)
static sdk::C_CSPlayerPawn* g_local_player = nullptr;
static sdk::ViewMatrix g_view_matrix = {};
static int g_screen_width = 1920;
static int g_screen_height = 1080;
    
// Track resolution changes
static int g_last_screen_width = 0;
static int g_last_screen_height = 0;

static std::vector<uintptr_t> playerPawns;

    // Aimbot thread control
    static std::atomic<bool> s_aimbot_running{ false };
    static std::thread s_aimbot_thread;

    // Get screen dimensions from multiple sources with fallback priority
    static void UpdateScreenDimensions() {
        bool success = false;
        
        // METHOD 1: Get from DX11 swap chain back buffer (most reliable for DX11 games)
        if (interfaces::swap_chain_dx11 && interfaces::swap_chain_dx11->swap_chain) {
            try {
                IDXGISwapChain* swap_chain = interfaces::swap_chain_dx11->swap_chain;
                if (swap_chain) {
                    DXGI_SWAP_CHAIN_DESC desc;
                    if (SUCCEEDED(swap_chain->GetDesc(&desc))) {
                        g_screen_width = desc.BufferDesc.Width;
                        g_screen_height = desc.BufferDesc.Height;
                        success = true;
                        
                        static bool logged_once = false;
                        if (!logged_once) {
                            debug_console::Console::Get().Success("[Resolution] Swap chain: %dx%d", 
                                g_screen_width, g_screen_height);
                            logged_once = true;
                        }
                    }
                }
            }
            catch (...) {
                // Fall through to next method
            }
        }
        
        // METHOD 2: Get from window client rect (fallback)
        if (!success && interfaces::hwnd) {
            RECT rect;
            if (GetClientRect(interfaces::hwnd, &rect)) {
                int width = rect.right - rect.left;
                int height = rect.bottom - rect.top;
                
                // Only update if dimensions are valid
                if (width > 0 && height > 0) {
                    g_screen_width = width;
                    g_screen_height = height;
                    success = true;
                    
                    static bool logged_once = false;
                    if (!logged_once) {
                        debug_console::Console::Get().Info("[Resolution] Window rect: %dx%d", 
                            g_screen_width, g_screen_height);
                        logged_once = true;
                    }
                }
            }
        }
        
        // METHOD 3: Read from CS2 game memory (additional verification)
        // CS2 stores screen resolution in memory, can be used to verify
        // This is optional but provides extra validation
        try {
            uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
            if (client) {
                // These offsets may need updating - verify with cs2-dumper
                // Typically around dwViewMatrix area
                // int* screen_width = (int*)(client + some_offset);
                // int* screen_height = (int*)(client + some_offset);
                // Can add validation here if offsets are known
            }
        }
        catch (...) {
            // Silent fail
        }
        
        // Ensure we have valid dimensions (default to 1920x1080 if all methods fail)
        if (g_screen_width <= 0 || g_screen_height <= 0) {
            g_screen_width = 1920;
            g_screen_height = 1080;
            
            static bool logged_default = false;
            if (!logged_default) {
                debug_console::Console::Get().Warning("[Resolution] Using default: %dx%d", 
                    g_screen_width, g_screen_height);
                logged_default = true;
            }
        }
        
        // Detect and log resolution changes (windowed/fullscreen switches, Alt+Tab, etc.)
        if (g_last_screen_width != g_screen_width || g_last_screen_height != g_screen_height) {
            if (g_last_screen_width != 0 && g_last_screen_height != 0) {
                debug_console::Console::Get().Info("[Resolution] Changed: %dx%d -> %dx%d", 
                    g_last_screen_width, g_last_screen_height,
                    g_screen_width, g_screen_height);
            }
            g_last_screen_width = g_screen_width;
            g_last_screen_height = g_screen_height;
        }
    }

    // World to screen projection
    bool WorldToScreen(const sdk::Vector3& world, sdk::Vector2& screen, const sdk::ViewMatrix& matrix, int screen_width, int screen_height) {
        float w = matrix.matrix[3][0] * world.x + matrix.matrix[3][1] * world.y +
            matrix.matrix[3][2] * world.z + matrix.matrix[3][3];

        if (w < 0.001f) return false;

        float x = matrix.matrix[0][0] * world.x + matrix.matrix[0][1] * world.y +
            matrix.matrix[0][2] * world.z + matrix.matrix[0][3];
        float y = matrix.matrix[1][0] * world.x + matrix.matrix[1][1] * world.y +
            matrix.matrix[1][2] * world.z + matrix.matrix[1][3];

        x /= w;
        y /= w;

        screen.x = (screen_width / 2.0f) + (x * screen_width / 2.0f);
        screen.y = (screen_height / 2.0f) - (y * screen_height / 2.0f);

        return true;
    }

    // Calculate angle between two positions
    sdk::Vector2 CalcAngle(const sdk::Vector3& src, const sdk::Vector3& dst) {
        sdk::Vector3 delta = src - dst;
        float hyp = sqrtf(delta.x * delta.x + delta.y * delta.y);

        sdk::Vector2 angles;
        angles.x = atanf(delta.z / hyp) * (180.0f / 3.14159265f);  // Pitch
        angles.y = atanf(delta.y / delta.x) * (180.0f / 3.14159265f);  // Yaw

        if (delta.x >= 0.0f) {
            angles.y += 180.0f;
        }

        // Normalize angles to [-180, 180]
        while (angles.x > 180.0f) angles.x -= 360.0f;
        while (angles.x < -180.0f) angles.x += 360.0f;
        while (angles.y > 180.0f) angles.y -= 360.0f;
        while (angles.y < -180.0f) angles.y += 360.0f;

        return angles;
    }

    // Get current screen resolution (updates every frame)
    void GetScreenResolution(int& width, int& height) {
        width = g_screen_width;
        height = g_screen_height;
    }

    // Normalize angle delta
    static float NormalizeAngle(float angle) {
        while (angle > 180.0f) angle -= 360.0f;
        while (angle < -180.0f) angle += 360.0f;
        return angle;
    }

    // Calculate FOV distance between current view and aim angles
    float GetFov(const sdk::Vector2& view_angles, const sdk::Vector2& aim_angles) {
        sdk::Vector2 delta;
        delta.x = NormalizeAngle(aim_angles.x - view_angles.x);
        delta.y = NormalizeAngle(aim_angles.y - view_angles.y);
        return sqrtf(delta.x * delta.x + delta.y * delta.y);
    }

    // ESP Drawing Functions
    void DrawBox(const sdk::Vector2& top, const sdk::Vector2& bottom, float width, const float color[4]) {
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

        float height = bottom.y - top.y;
        ImVec2 top_left(top.x - width / 2, top.y);
        ImVec2 bottom_right(top.x + width / 2, bottom.y);

        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(color[0], color[1], color[2], color[3]));
        draw_list->AddRect(top_left, bottom_right, col, 0.0f, 0, 2.0f);
    }

    void DrawLine(const sdk::Vector2& from, const sdk::Vector2& to, const float color[4], float thickness) {
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(color[0], color[1], color[2], color[3]));
        draw_list->AddLine(ImVec2(from.x, from.y), ImVec2(to.x, to.y), col, thickness);
    }

    void DrawText(const sdk::Vector2& pos, const char* text, const float color[4]) {
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(color[0], color[1], color[2], color[3]));

        // Add text outline for better visibility
        ImU32 outline_col = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.8f));
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                if (x == 0 && y == 0) continue;
                draw_list->AddText(ImVec2(pos.x + x, pos.y + y), outline_col, text);
            }
        }

        draw_list->AddText(ImVec2(pos.x, pos.y), col, text);
    }

    void DrawFilledRect(const sdk::Vector2& pos, const sdk::Vector2& size, const float color[4]) {
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(color[0], color[1], color[2], color[3]));
        draw_list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + size.y), col);
    }

    void DrawHealthBar(const sdk::Vector2& top, const sdk::Vector2& bottom, int health, int max_health) {
        float height = bottom.y - top.y;
        float health_height = (health / (float)max_health) * height;

        // Background (dark)
        float bg_color[4] = { 0.1f, 0.1f, 0.1f, 0.8f };
        DrawFilledRect(sdk::Vector2(top.x - 8, top.y), sdk::Vector2(4, height), bg_color);

        // Health bar (gradient from red to green)
        float health_percent = health / (float)max_health;
        float hp_color[4] = {
            1.0f - health_percent,  // Red component
            health_percent,          // Green component
            0.0f,
            0.9f
        };
        DrawFilledRect(sdk::Vector2(top.x - 8, bottom.y - health_height), sdk::Vector2(4, health_height), hp_color);

        // Health value text
        if (health < 100) {
            char health_text[8];
            sprintf_s(health_text, "%d", health);
            DrawText(sdk::Vector2(top.x - 18, top.y + (height / 2) - 7), health_text, hp_color);
        }
    }

    void DrawSkeleton(sdk::C_CSPlayerPawn* player, const sdk::ViewMatrix& view_matrix, int screen_width, int screen_height, const float color[4]) {
        // Define bone connections for realistic skeleton
        const int bone_connections[][2] = {
            {sdk::BONE_HEAD, sdk::BONE_NECK},
            {sdk::BONE_NECK, sdk::BONE_CHEST},
            {sdk::BONE_CHEST, sdk::BONE_PELVIS},
            {sdk::BONE_NECK, sdk::BONE_LEFT_SHOULDER},
            {sdk::BONE_LEFT_SHOULDER, sdk::BONE_LEFT_ELBOW},
            {sdk::BONE_LEFT_ELBOW, sdk::BONE_LEFT_HAND},
            {sdk::BONE_NECK, sdk::BONE_RIGHT_SHOULDER},
            {sdk::BONE_RIGHT_SHOULDER, sdk::BONE_RIGHT_ELBOW},
            {sdk::BONE_RIGHT_ELBOW, sdk::BONE_RIGHT_HAND},
            {sdk::BONE_PELVIS, sdk::BONE_LEFT_HIP},
            {sdk::BONE_LEFT_HIP, sdk::BONE_LEFT_KNEE},
            {sdk::BONE_LEFT_KNEE, sdk::BONE_LEFT_FOOT},
            {sdk::BONE_PELVIS, sdk::BONE_RIGHT_HIP},
            {sdk::BONE_RIGHT_HIP, sdk::BONE_RIGHT_KNEE},
            {sdk::BONE_RIGHT_KNEE, sdk::BONE_RIGHT_FOOT}
        };

        for (const auto& connection : bone_connections) {
            sdk::Vector3 bone1_pos = player->GetBonePosition(connection[0]);
            sdk::Vector3 bone2_pos = player->GetBonePosition(connection[1]);

            sdk::Vector2 bone1_screen, bone2_screen;
            if (WorldToScreen(bone1_pos, bone1_screen, view_matrix, screen_width, screen_height) &&
                WorldToScreen(bone2_pos, bone2_screen, view_matrix, screen_width, screen_height)) {
                DrawLine(bone1_screen, bone2_screen, color, 2.0f);
            }
        }
    }

    // Get all players from entity list
    std::vector<sdk::C_CSPlayerPawn*> GetPlayerList() {
        playerPawns.clear();
        std::vector<sdk::C_CSPlayerPawn*> players;

        if (!game_state::IsInGame()) return players;

		uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");

	    auto entity_list = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwEntityList);
        if (!entity_list || !sdk::is_valid_ptr(entity_list)) return players;

        try {
            for (int i = 1; i < 64; i++) {
                uintptr_t entityList1 = *(uintptr_t*)(entity_list + (8 * (i & 0x7fff) >> 9) + 16);
                if (!entityList1 || !sdk::is_valid_ptr(entityList1)) continue;

                uintptr_t playerController = *(uintptr_t*)(entityList1 + 112 * (i & 0x1FF));
                if (!playerController) continue;

                uint32_t playerPawn = *(uint32_t*)(playerController 
                    + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
                if (!playerPawn) continue;

				uintptr_t entityList2 = *(uintptr_t*)(entity_list + 0x8 
                    * ((playerPawn & 0x7FFF) >> 9) + 16);
				if (!entityList2) continue;

				uintptr_t pCSPlayerPawn = *(uintptr_t*)(entityList2 + 112 * (playerPawn & 0x1FF));
				if (!pCSPlayerPawn) continue;

				int health = *(int*)(pCSPlayerPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);
                if (health < 0 || !health) continue;

                playerPawns.push_back(pCSPlayerPawn);

                auto* player = reinterpret_cast<sdk::C_CSPlayerPawn*>(pCSPlayerPawn);
                if (player->IsDormant()) continue;
                if (!player->IsAlive()) continue;

                players.push_back(player);

				
            }
        }
        catch (const std::exception &e) {
			debug_console::Console::Get().Error("Exception in GetPlayerList: %s", e.what());
        }

        return players;
    }

    // Get local player
    sdk::C_CSPlayerPawn* GetLocalPlayer() {
        return game_state::GetLocalPawn();
    }

    // Per-frame cache update — refreshes playerPawns + view matrix at most once per tick
    static DWORD g_last_cache_tick = 0;

    static void UpdateCache() {
        DWORD now = GetTickCount();
        if (now == g_last_cache_tick) return; // already ran this tick
        g_last_cache_tick = now;

        // Refresh player list every frame
        GetPlayerList();

        // Refresh view matrix every frame
        HMODULE client_mod = GetModuleHandleA("client.dll");
        if (client_mod) {
            uintptr_t base = reinterpret_cast<uintptr_t>(client_mod);
            auto* matrix_ptr = reinterpret_cast<sdk::ViewMatrix*>(base + cs2_dumper::offsets::client_dll::dwViewMatrix);
            if (matrix_ptr) {
                g_view_matrix = *matrix_ptr;
            }
        }

        UpdateScreenDimensions();
    }

    // Main ESP rendering
    void RenderESP() {
        if (!config::esp::enabled) return;
        if (!game_state::IsInGame()) return;

		UpdateCache();

		uintptr_t client = (uintptr_t)GetModuleHandle(L"client.dll");

		auto localPawnHandle = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);



		g_local_player = GetLocalPlayer();
        if (!g_local_player) return;

        // Debug logging - Display local player info
        static int debug_log_frame = 0;
        if (debug_log_frame % 300 == 0) {  // Every 5 seconds
            try {
				int local_team = localPawnHandle ? *reinterpret_cast<int*>(localPawnHandle + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum) : -1;  // m_iTeamNum
				int local_health = localPawnHandle ? *reinterpret_cast<int*>(localPawnHandle + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth) : -1;  // m_iHealth

                sdk::Vector3 pos = *(sdk::Vector3*)(localPawnHandle + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);

                
                debug_console::Console::Get().Info("=== LOCAL PLAYER INFO ===");
                debug_console::Console::Get().Info("Address: 0x%llX", &localPawnHandle);
                debug_console::Console::Get().Info("Team: %d (1=Spectator, 2=Terrorist, 3=CT)", local_team);
                debug_console::Console::Get().Info("Health: %d / 100", local_health);
                debug_console::Console::Get().Info("Position: X=%.2f Y=%.2f Z=%.2f", 
                    pos.x, 
                    pos.y, 
                    pos.z);
                debug_console::Console::Get().Info("Alive: %s", local_health > 0 ? "YES" : "NO");
                //debug_console::Console::Get().Info("Dormant: %s", g_local_player->IsDormant() ? "YES" : "NO");

                // Log all players in entity list
                debug_console::Console::Get().Info("=== ENTITY LIST ===");
                std::vector<sdk::C_CSPlayerPawn*> players = GetPlayerList();
                debug_console::Console::Get().Info("Total players: %zu", playerPawns.size());
                
                for (int i = 0; i < playerPawns.size(); i++) {
                    auto player = playerPawns[i];
                    if (!player) continue;
                    
                    try {
						int team = *(int*)(player + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
						int health = *(int*)(player + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);
                        bool is_self = player == localPawnHandle;
                        
                        const char* team_str = "Unknown";
                        if (team == 1) team_str = "Spectator";
                        else if (team == 2) team_str = "Terrorist";
                        else if (team == 3) team_str = "CT";
                        
                        debug_console::Console::Get().Debug(
                            "  [%zu] Addr:0x%llX | Team:%s | Health:%d | Alive:%s%s",
                            i,
                            (player),
                            team_str,
                            health,
                            health > 0 ? "YES" : "NO",
                            is_self ? " (LOCAL)" : ""
                        );
                    }
                    catch (...) {
                        debug_console::Console::Get().Error("Failed to read player %zu info", i);
                    }
                }
                debug_console::Console::Get().Info("======================");
            }
            catch (const std::exception& e) {
                debug_console::Console::Get().Error("Exception in debug logging: %s", e.what());
            }
            catch (...) {
                debug_console::Console::Get().Error("Unknown exception in debug logging");
            }
        }
        debug_log_frame++;

        int local_team = *(int*)(localPawnHandle + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);

        for (uintptr_t player : playerPawns) {
            if (!player || player == localPawnHandle) continue;

            int player_team = *(int*)(player + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
			int health = *(int*)(player + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);
            if (config::esp::team_check && player_team == local_team) continue;

            try {
				sdk::Vector3 origin = *(sdk::Vector3*)(player + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
                sdk::Vector3 head_pos = { origin.x, origin.y, origin.z + 65.0f };
                head_pos.z += 10.0f;

				sdk::Vector3 ourpos = *(sdk::Vector3*)(localPawnHandle + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);

                float distance = ourpos.Distance(origin);
                //if (distance > config::esp::max_distance) continue;

                sdk::Vector2 screen_pos, screen_head;
                if (!WorldToScreen(origin, screen_pos, g_view_matrix, g_screen_width, g_screen_height)) continue;
                if (!WorldToScreen(head_pos, screen_head, g_view_matrix, g_screen_width, g_screen_height)) continue;

                float height = screen_pos.y - screen_head.y;
                float width = height / 2.0f;

                const float* color = (player_team == local_team) ? config::esp::team_color : config::esp::box_color;

                if (config::esp::box) {
                    DrawBox(screen_head, screen_pos, width, color);
                }

                if (config::esp::health_bar) {
                    DrawHealthBar(sdk::Vector2(screen_head.x - width / 2, screen_head.y),
                        sdk::Vector2(screen_pos.x - width / 2, screen_pos.y),
                        health, 100);
                }

                if (config::esp::skeleton) {
                    //DrawSkeleton(player, g_view_matrix, g_screen_width, g_screen_height, config::esp::skeleton_color);
                }

                if (config::esp::distance) {
                    char dist_text[32];
                    sprintf_s(dist_text, "%.0fm", distance * 0.0254f);
                    DrawText(sdk::Vector2(screen_pos.x - 15, screen_pos.y + 5), dist_text, color);
                }

                if (config::esp::snaplines) {
                    DrawLine(sdk::Vector2(g_screen_width / 2.0f, g_screen_height), screen_pos, color, 1.0f);
                }
            }
            catch (...) {
                continue;
            }
        }
    }

    // =========================================================================
    // Aimbot — runs on its own thread, reads game memory independently of ESP
    // =========================================================================

    // Helper: get bone position from raw pawn address
    static sdk::Vector3 GetBonePositionRaw(uintptr_t pawn, int bone_index) {
        uintptr_t game_scene_node = *(uintptr_t*)(pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode);
        if (!game_scene_node || !sdk::is_valid_ptr(game_scene_node)) return sdk::Vector3();

        uintptr_t bone_array = *(uintptr_t*)(game_scene_node + (cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState + 0x80));
        if (!bone_array || !sdk::is_valid_ptr(bone_array)) return sdk::Vector3();

        return *(sdk::Vector3*)(bone_array + bone_index * 32);
    }

    // Helper: get eye position from raw pawn address
    static sdk::Vector3 GetEyePositionRaw(uintptr_t pawn) {
        sdk::Vector3 origin = *(sdk::Vector3*)(pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
        sdk::Vector3 view_offset = *(sdk::Vector3*)(pawn + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);
        return origin + view_offset;
    }

    // RCS offsets — fill these in from cs2-dumper
    // C_CSPlayerPawn::m_iShotsFired
    constexpr std::ptrdiff_t rcs_m_iShotsFired = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_iShotsFired;      // TODO: find m_iShotsFired
    // C_CSPlayerPawn::m_aimPunchAngle
    constexpr std::ptrdiff_t rcs_m_aimPunchAngle = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_aimPunchAngle;     // TODO: find m_aimPunchAngle

    // Aimbot thread loop — human-like aim toward closest enemy head
    static void AimbotLoop() {
        debug_console::Console::Get().Info("[Aimbot] Thread started");
        int localIndex = -1;

        // RCS state — tracks previous punch for delta calculation
        sdk::Vector2 old_punch = { 0.0f, 0.0f };

        while (s_aimbot_running.load()) {
            // Sleep only when idle
            if ((!config::aimbot::enabled && !config::rcs::enabled) || !game_state::IsInGame()) {
                old_punch = { 0.0f, 0.0f };
                Sleep(50);
                continue;
            }

            // Hold pause key to temporarily disable
            if (GetAsyncKeyState(config::aimbot::pause_key) & 0x8000) {
                Sleep(1);
                continue;
            }

            try {
                uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
                if (!client) { Sleep(50); continue; }

                uintptr_t localPawn = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
                if (!localPawn || !sdk::is_valid_ptr(localPawn)) { Sleep(1); continue; }

                int local_team = *(int*)(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
                sdk::Vector3 local_eye = GetEyePositionRaw(localPawn);

                // Walk entity list directly (independent of ESP's playerPawns)
                uintptr_t entity_list = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwEntityList);
                if (!entity_list || !sdk::is_valid_ptr(entity_list)) { Sleep(1); continue; }

                uintptr_t closest_pawn = 0;
                float closest_dist = 9999;

                for (int i = 1; i < 64; i++) {
                    uintptr_t list1 = *(uintptr_t*)(entity_list + (8 * (i & 0x7FFF) >> 9) + 16);
                    if (!list1 || !sdk::is_valid_ptr(list1)) continue;

                    uintptr_t controller = *(uintptr_t*)(list1 + 112 * (i & 0x1FF));
                    if (!controller) continue;

                    uint32_t pawnHandle = *(uint32_t*)(controller + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
                    if (!pawnHandle) continue;

                    uintptr_t list2 = *(uintptr_t*)(entity_list + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 16);
                    if (!list2) continue;

                    uintptr_t pawn = *(uintptr_t*)(list2 + 112 * (pawnHandle & 0x1FF));
                    if (!pawn) continue;
                    if (pawn == localPawn) { localIndex = i; continue; }

                    int health = *(int*)(pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);
                    if (health <= 0) continue;

                    int team = *(int*)(pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
                    if (config::aimbot::team_check && team == local_team) continue;

                    if (config::aimbot::visible_check) {
                        int32_t spotted = *(int32_t*)(pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState + cs2_dumper::schemas::client_dll::EntitySpottedState_t::m_bSpottedByMask);
                        if (!(spotted & ((1 << localIndex) - 1))) continue;
                    }

                    // Calculate distance to HEAD position for accurate long-range targeting
                    sdk::Vector3 head_pos = GetBonePositionRaw(pawn, 6);
                    if (head_pos.x == 0.0f && head_pos.y == 0.0f && head_pos.z == 0.0f) continue; // Invalid bone
                    
                    float dist = local_eye.Distance(head_pos);
                    
                    // Apply max distance check
                    if (dist > config::aimbot::max_distance) continue;
                    
                    if (dist < closest_dist) {
                        closest_dist = dist;
                        closest_pawn = pawn;
                    }
                }

                // ── Silent Aim: instant micro-flick only during shot ──
                if (config::aimbot::enabled && closest_pawn) {
                    int shots_fired = *(int*)(localPawn + rcs_m_iShotsFired);
                    
                    // Only aim when actively shooting
                    if (shots_fired > 0) {
                        sdk::Vector3 head_pos = GetBonePositionRaw(closest_pawn, 6);
                        sdk::Vector2 target_angles = CalcAngle(local_eye, head_pos);
                        
                        // Get current view angles
                        sdk::Vector2* va = reinterpret_cast<sdk::Vector2*>(
                            client + cs2_dumper::offsets::client_dll::dwViewAngles);
                        sdk::Vector2 current_angles = *va;
                        
                        // Check if target is within FOV
                        float fov_to_target = GetFov(current_angles, target_angles);
                        
                        if (fov_to_target <= config::aimbot::fov) {
                            if (config::aimbot::silent_aim) {
                                // Silent aim mode: instant micro-flick (external approach)
                                // Save current angles for restoration
                                static sdk::Vector2 saved_angles = current_angles;
                                static bool angles_modified = false;
                                static DWORD last_shot_time = 0;
                                DWORD current_time = GetTickCount64();
                                
                                // Apply instant angle on first shot frame
                                if (!angles_modified || (current_time - last_shot_time) > 100) {
                                    saved_angles = current_angles;
                                    
                                    // Instant snap to target
                                    target_angles.x = std::clamp(target_angles.x, -89.0f, 89.0f);
                                    while (target_angles.y > 180.0f) target_angles.y -= 360.0f;
                                    while (target_angles.y < -180.0f) target_angles.y += 360.0f;
                                    
                                    *va = target_angles;
                                    angles_modified = true;
                                    last_shot_time = current_time;
                                } 
                                // Restore after 2 frames (~16ms)
                                else if (angles_modified && (current_time - last_shot_time) >= 16) {
                                    *va = saved_angles;
                                    angles_modified = false;
                                }
                            } else {
                                // Smooth aim mode
                                sdk::Vector2 delta;
                                delta.x = NormalizeAngle(target_angles.x - current_angles.x);
                                delta.y = NormalizeAngle(target_angles.y - current_angles.y);
                                
                                // Apply smoothing
                                float smooth_factor = 1.0f - config::aimbot::smoothing;
                                delta.x *= smooth_factor;
                                delta.y *= smooth_factor;
                                
                                // Calculate new angles
                                sdk::Vector2 new_angles;
                                new_angles.x = current_angles.x + delta.x;
                                new_angles.y = current_angles.y + delta.y;
                                
                                // Clamp and normalize
                                new_angles.x = std::clamp(new_angles.x, -89.0f, 89.0f);
                                while (new_angles.y > 180.0f) new_angles.y -= 360.0f;
                                while (new_angles.y < -180.0f) new_angles.y += 360.0f;
                                
                                *va = new_angles;
                            }
                        }
                    }
                }

                // ── RCS: compensate recoil only while actively shooting ──
                if (config::rcs::enabled) {
                    int shots_fired = *(int*)(localPawn + rcs_m_iShotsFired);

                    if (shots_fired > 1) {
                        sdk::Vector3 punch = *(sdk::Vector3*)(localPawn + rcs_m_aimPunchAngle);
                        sdk::Vector2 cur_punch = { punch.x, punch.y };

                        sdk::Vector2 delta;
                        delta.x = (cur_punch.x - old_punch.x) * 2.0f * config::rcs::strength;
                        delta.y = (cur_punch.y - old_punch.y) * 2.0f * config::rcs::strength;

                        sdk::Vector2* va = reinterpret_cast<sdk::Vector2*>(
                            client + cs2_dumper::offsets::client_dll::dwViewAngles);
                        sdk::Vector2 angles = *va;

                        angles.x -= delta.x;
                        angles.y -= delta.y;

                        angles.x = std::clamp(angles.x, -89.0f, 89.0f);
                        while (angles.y > 180.0f) angles.y -= 360.0f;
                        while (angles.y < -180.0f) angles.y += 360.0f;

                        *va = angles;
                        old_punch = cur_punch;
                    } else {
                        old_punch = { 0.0f, 0.0f };
                    }
                }
            }
            catch (...) {
                // Silently recover
            }

            // ~500 Hz tick — matches typical mouse polling, gives smooth visual movement
            Sleep(1);
        }

        debug_console::Console::Get().Info("[Aimbot] Thread stopped");
    }

    void StartAimbotThread() {
        if (s_aimbot_running.load()) return;
        s_aimbot_running.store(true);
        s_aimbot_thread = std::thread(AimbotLoop);
        debug_console::Console::Get().Success("[Aimbot] Thread launched");
    }

    void StopAimbotThread() {
        s_aimbot_running.store(false);
        if (s_aimbot_thread.joinable()) {
            s_aimbot_thread.join();
        }
        debug_console::Console::Get().Info("[Aimbot] Thread joined");
    }

    // Bunny hop
    void BunnyHop() {
        if (!config::misc::bunny_hop) return;
        if (!game_state::IsInGame()) return;

        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return;

        uintptr_t localPawn = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
        if (!localPawn || !sdk::is_valid_ptr(localPawn)) return;

        try {
            static bool was_in_air = false;
            int flags = *(int*)(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_fFlags); // m_fFlags
            bool is_on_ground = (flags & 1) != 0; // FL_ONGROUND

            if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                if (is_on_ground && !was_in_air) {
                    int* buttons = reinterpret_cast<int*>(localPawn + 0x1730);
                    *buttons |= (1 << 0);
                }
            }

            was_in_air = !is_on_ground;
        }
        catch (...) {}
    }

    // No flash
    void NoFlash() {
        if (!config::misc::no_flash) return;
        if (!game_state::IsInGame()) return;

        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return;

        uintptr_t localPawn = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
        if (!localPawn || !sdk::is_valid_ptr(localPawn)) return;

        try {
            float* flash_duration = reinterpret_cast<float*>(localPawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_flFlashDuration); // m_flFlashDuration
            if (*flash_duration > 0.0f) {
                *flash_duration = 0.0f;
            }
        }
        catch (...) {}
    }

    // Trigger bot — fires mouse_event when crosshair passes over an enemy
    void TriggerBot() {
        static DWORD last_shot = 0;
        static bool mouse_down = false;
        static DWORD mouse_down_time = 0;

        // Release mouse if we pressed it on a previous frame
        if (mouse_down) {
            DWORD now = GetTickCount64();
            if (now - mouse_down_time >= 10) {
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                mouse_down = false;
                last_shot = now;
            }
            return;
        }

        if (!config::misc::trigger_bot) return;
        //if (!(GetAsyncKeyState(config::misc::trigger_key) & 0x8000)) return;
        if (!game_state::IsInGame()) return;

        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return;

        uintptr_t localPawn = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
        if (!localPawn || !sdk::is_valid_ptr(localPawn)) return;

        try {
            int entity_index = *(int*)(localPawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_iIDEntIndex);

            if (entity_index > 0) {
                uintptr_t entity_list = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwEntityList);
                if (!entity_list || !sdk::is_valid_ptr(entity_list)) return;

                uintptr_t list1 = *(uintptr_t*)(entity_list + (0x8 * (entity_index >> 9) + 16));
                if (!list1 || !sdk::is_valid_ptr(list1)) return;

                uintptr_t controller = *(uintptr_t*)(list1 + 112 * (entity_index & 0x1FF));
                if (!controller || !sdk::is_valid_ptr(controller)) return;

                /*uint32_t pawnHandle = *(uint32_t*)(controller + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
                if (!pawnHandle) return;

                uintptr_t list2 = *(uintptr_t*)(entity_list + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 16);
                if (!list2 || !sdk::is_valid_ptr(list2)) return;

                uintptr_t pawn = *(uintptr_t*)(list2 + 112 * (pawnHandle & 0x1FF));
                if (!pawn || !sdk::is_valid_ptr(pawn)) return;*/

                int target_team = *(int*)(controller + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
                int local_team = *(int*)(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
				int local_health = *(int*)(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);   
                int target_health = *(int*)(controller + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);

                if (target_team != local_team && target_health > 0 && local_health > 0) {
                    DWORD now = GetTickCount();
                    DWORD delay = (DWORD)(config::misc::trigger_delay * 1000.0f);

                    if (now - last_shot >= delay) {
                        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                        mouse_down = true;
                        mouse_down_time = now;
                        
                    }
                }
            }
        }
        catch (...) {
        }
    }

    // Radar hack - sets m_bSpotted on all enemy players so they appear on radar
    void RadarHack() {
        if (!config::misc::radar_hack) return;
		if (!game_state::IsInGame()) return;
		int localIndex = -1;
		int count = 1;
		uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
		if (!client) return;

		uintptr_t localPawn = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
		if (!localPawn || !sdk::is_valid_ptr(localPawn)) return;

		int local_team = *(int*)(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);

		for (uintptr_t player : playerPawns) {
            if (!player || player == localPawn) continue;

            try {
                int player_team = *(int*)(player + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
                if (player_team == local_team) {
                    localIndex = count;
                    count++;
                    continue;
                } 

                int32_t spotted = *(int32_t*)(player + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState + cs2_dumper::schemas::client_dll::EntitySpottedState_t::m_bSpottedByMask);
                bool* spottedOriginal = (bool*)(player + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState + cs2_dumper::schemas::client_dll::EntitySpottedState_t::m_bSpotted);
                if (!(spotted & ((1 << localIndex) - 1))) *spottedOriginal = true;
            }
            catch (...) {
                continue;
            }
            count++;
        }
    }
}