#pragma once
#include "../sdk/entity.h"
#include "../sdk/usercmd.h"
#include <d3d11.h>
#include <vector>
#include <mutex>

using namespace sdk;

namespace features {
    inline std::atomic<const char*> normal_aim_status{"Not evaluated"};
    inline std::atomic<const char*> silent_aim_status{"No callback received"};
    inline std::atomic<unsigned> silent_callbacks{0}, normal_aim_writes{0}, silent_aim_writes{0};

    inline std::atomic<unsigned> aim_controllers{0}, aim_resolved_pawns{0};
    inline std::atomic<unsigned> silent_history_writes{0};
    inline std::atomic<unsigned> silent_target_scans{0}, silent_cache_hits{0};
    inline std::atomic<unsigned> silent_settings_skips{0}, silent_geometry_updates{0};
    struct SilentAimDebug {
        uintptr_t source = 0;
        DWORD raw_angles[3]{};
        unsigned alive = 0, enemies = 0, spotted = 0, bones = 0, in_range = 0, in_fov = 0;
        uintptr_t target_pawn = 0;
        int target_health = 0, target_team = 0;
        char target_name[128]{};
        sdk::Vector3 input{}, eye{}, target{}, output{};
        bool input_valid = false, eye_valid = false, target_valid = false, written = false;
        const char* status = "No sample yet";
    };
    inline std::mutex silent_debug_mutex;
    inline SilentAimDebug silent_debug_snapshot{};
    inline SilentAimDebug GetSilentAimDebug() {
        std::lock_guard<std::mutex> lock(silent_debug_mutex);
        return silent_debug_snapshot;
    }
    void RunNormalAimTick();
    // Math helpers
    bool WorldToScreen(const sdk::Vector3& world, sdk::Vector2& screen, const sdk::ViewMatrix& matrix, int screen_width, int screen_height);
    sdk::Vector2 CalcAngle(const sdk::Vector3& src, const sdk::Vector3& dst);
    float GetFov(const sdk::Vector2& view_angles, const sdk::Vector2& aim_angles);
	float GetFovA(const QAngle& view_angles, const QAngle& aim_angles);
    void ClampAnglesA(QAngle& ang);

    // Screen resolution getter
    void GetScreenResolution(int& width, int& height);

    // ESP Functions
    void DrawBox(const sdk::Vector2& top, const sdk::Vector2& bottom, float width, const float color[4]);
    void DrawLine(const sdk::Vector2& from, const sdk::Vector2& to, const float color[4], float thickness = 1.0f);
    void DrawText(const sdk::Vector2& pos, const char* text, const float color[4]);
    void DrawFilledRect(const sdk::Vector2& pos, const sdk::Vector2& size, const float color[4]);
    void DrawHealthBar(const sdk::Vector2& top, const sdk::Vector2& bottom, int health, int max_health);
    void DrawSkeleton(sdk::C_CSPlayerPawn* player, const sdk::ViewMatrix& view_matrix, int screen_width, int screen_height, const float color[4]);
    void RenderESP();
	void RunSilentAim(CUserCmd* cmd);
    void RunSilentAimSubTick(DWORD* a1, C_CSPlayerPawn* localPawn);

    // Aimbot — runs on its own thread
    void StartAimbotThread();
    void StopAimbotThread();
    void SetInputBlocked(bool blocked);
    void ReleaseInputs();

    // Misc Functions
    void BunnyHop();
    void NoFlash();
    void TriggerBot();
    void RadarHack();

    // Entity list management
    std::vector<sdk::C_CSPlayerPawn*> GetPlayerList();
    sdk::C_CSPlayerPawn* GetLocalPlayer();
}