#pragma once
#include "../sdk/entity.h"
#include "../sdk/usercmd.h"
#include <d3d11.h>
#include <vector>

using namespace sdk;

namespace features {
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