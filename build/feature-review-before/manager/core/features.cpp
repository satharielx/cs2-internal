#include "features.h"
#include "config.h"
#include "interfaces.h"
#include "debug_console.h"
#include "game_state.h"
#include "../external/offsets/offsets.hpp"
#include "../external/imgui/imgui.h"
#include "../sdk/mem.h"
#include "../sdk/source2sdk_offsets.h"
#include "../sdk/usercmd.h"
#include "pattern_resolver.h"
#include "skins.h"
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

    static int g_last_screen_width = 0;
    static int g_last_screen_height = 0;

    static std::vector<uintptr_t> playerPawns;

    // Aimbot thread control
    static std::atomic<bool> s_aimbot_running{ false };
    static std::thread s_aimbot_thread;

    // Get screen dimensions from multiple sources with fallback priority
    static void UpdateScreenDimensions() {
        bool success = false;
        if (interfaces::swap_chain) {
            try {
                IDXGISwapChain* swap_chain = interfaces::swap_chain;
                if (swap_chain) {
                    DXGI_SWAP_CHAIN_DESC desc;
                    if (SUCCEEDED(swap_chain->GetDesc(&desc))) {
                        g_screen_width = desc.BufferDesc.Width;
                        g_screen_height = desc.BufferDesc.Height;
                        success = true;
                    }
                }
            }
            catch (...) {}
        }
        if (!success && interfaces::hwnd) {
            RECT rect;
            if (GetClientRect(interfaces::hwnd, &rect)) {
                int width = rect.right - rect.left;
                int height = rect.bottom - rect.top;
                if (width > 0 && height > 0) {
                    g_screen_width = width;
                    g_screen_height = height;
                    success = true;
                }
            }
        }
        if (g_screen_width <= 0 || g_screen_height <= 0) {
            g_screen_width = 1920;
            g_screen_height = 1080;
        }
        if (g_last_screen_width != g_screen_width || g_last_screen_height != g_screen_height) {
            g_last_screen_width = g_screen_width;
            g_last_screen_height = g_screen_height;
        }
    }

    // ======================== MATH / ANGLE HELPERS ========================
    void ClampAnglesA(sdk::QAngle& ang) {
        while (ang.x > 89.0f) ang.x -= 180.0f;
        while (ang.x < -89.0f) ang.x += 180.0f;
        while (ang.y > 180.0f) ang.y -= 360.0f;
        while (ang.y < -180.0f) ang.y += 360.0f;
        ang.z = 0.0f;
    }

    float GetFovA(const sdk::QAngle& viewAngles, const sdk::QAngle& aimAngles) {
        sdk::QAngle delta = aimAngles - viewAngles;
        while (delta.y > 180.0f) delta.y -= 360.0f;
        while (delta.y < -180.0f) delta.y += 360.0f;
        return sqrtf(delta.x * delta.x + delta.y * delta.y);
    }

    sdk::Vector2 CalcAngle(const sdk::Vector3& src, const sdk::Vector3& dst) {
        sdk::Vector3 delta = dst - src;
        float hyp = sqrtf(delta.x * delta.x + delta.y * delta.y);
        sdk::Vector2 angles;
        angles.x = atanf(delta.z / hyp) * (180.0f / 3.14159265f);
        angles.y = atanf(delta.y / delta.x) * (180.0f / 3.14159265f);
        if (delta.x >= 0.0f) angles.y += 180.0f;
        while (angles.x > 180.0f) angles.x -= 360.0f;
        while (angles.x < -180.0f) angles.x += 360.0f;
        while (angles.y > 180.0f) angles.y -= 360.0f;
        while (angles.y < -180.0f) angles.y += 360.0f;
        return angles;
    }

    static float NormalizeAngle(float angle) {
        while (angle > 180.0f) angle -= 360.0f;
        while (angle < -180.0f) angle += 360.0f;
        return angle;
    }

    float GetFov(const sdk::Vector2& view_angles, const sdk::Vector2& aim_angles) {
        sdk::Vector2 delta;
        delta.x = NormalizeAngle(aim_angles.x - view_angles.x);
        delta.y = NormalizeAngle(aim_angles.y - view_angles.y);
        return sqrtf(delta.x * delta.x + delta.y * delta.y);
    }

    void GetScreenResolution(int& width, int& height) {
        width = g_screen_width;
        height = g_screen_height;
    }

    // ======================== BONE / EYE HELPERS (raw pointers) ========================
    static sdk::Vector3 GetBonePositionRaw(uintptr_t pawn, int bone_index) {
        uintptr_t game_scene_node = *(uintptr_t*)(pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode);
        if (!game_scene_node || !sdk::is_valid_ptr(game_scene_node)) return sdk::Vector3();
        uintptr_t bone_array = *(uintptr_t*)(game_scene_node + (cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState + 0x80));
        if (!bone_array || !sdk::is_valid_ptr(bone_array)) return sdk::Vector3();
        return *(sdk::Vector3*)(bone_array + bone_index * 32);
    }

    static sdk::Vector3 GetEyePositionRaw(uintptr_t pawn) {
        sdk::Vector3 origin = *(sdk::Vector3*)(pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
        sdk::Vector3 view_offset = *(sdk::Vector3*)(pawn + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);
        return origin + view_offset;
    }

    // RCS offsets
    constexpr std::ptrdiff_t rcs_m_iShotsFired = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_iShotsFired;
    constexpr std::ptrdiff_t rcs_m_aimPunchAngle = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_aimPunchAngle;

    // ======================== SILENT AIM (CreateMove) -- unchanged but relies on SubTick pattern ========================
    void RunSilentAim(CUserCmd* cmd) {
        if (!cmd || !cmd->csgoUserCmd.pBaseCmd) return;
        if (!config::aimbot::enabled || !config::aimbot::silent_aim) return;
        if (!game_state::IsInGame()) return;
        if (GetAsyncKeyState(config::aimbot::pause_key) & 0x8000) return;

        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return;
        uintptr_t localPawn = game_state::GetLocalPawnRaw();
        if (!localPawn || !sdk::is_valid_ptr(localPawn)) return;
        if (*(int*)(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth) <= 0) return;
        if (!(cmd->csgoUserCmd.pBaseCmd->pInButtonState->nValue & sdk::IN_ATTACK)) return;

        sdk::Vector3 localEye = GetEyePositionRaw(localPawn);
        int localTeam = *(int*)(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
		uintptr_t entity_list = game_state::GetEntityList();
        if (!entity_list || !sdk::is_valid_ptr(entity_list)) return;

        int localIndex = -1;
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
            if (pawn == localPawn) { localIndex = i; break; }
        }

        sdk::C_CSPlayerPawn* bestTarget = nullptr;
        float bestFov = config::aimbot::fov;
        sdk::Vector3 bestPos;

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
            if (!pawn || pawn == localPawn) continue;
            int health = *(int*)(pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);
            if (health <= 0) continue;
            if (config::aimbot::team_check) {
                int playerTeam = *(int*)(pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
                if (localTeam == playerTeam) continue;
            }
            if (config::aimbot::visible_check && localIndex != -1) {
                int32_t spottedMask = *(int32_t*)(pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState + cs2_dumper::schemas::client_dll::EntitySpottedState_t::m_bSpottedByMask);
                if (!(spottedMask & (1 << (localIndex - 1)))) continue;
            }
            sdk::Vector3 headPos = GetBonePositionRaw(pawn, 6);
            float dist = localEye.Distance(headPos);
            if (dist > config::aimbot::max_distance) continue;
            sdk::Vector2 aimAngles2D = CalcAngle(localEye, headPos);
            sdk::QAngle aimAngles(aimAngles2D.x, aimAngles2D.y, 0.0f);
            sdk::QAngle currentAngles = cmd->csgoUserCmd.pBaseCmd->pViewAngles->angValue;
            float fov = GetFovA(currentAngles, aimAngles);
            if (fov < bestFov) {
                bestFov = fov;
                bestTarget = reinterpret_cast<sdk::C_CSPlayerPawn*>(pawn);
                bestPos = headPos;
            }
        }
        if (!bestTarget) return;

        sdk::Vector2 aim2D = CalcAngle(localEye, bestPos);
        sdk::QAngle targetAngles(aim2D.x, aim2D.y, 0.0f);
        sdk::QAngle current = cmd->csgoUserCmd.pBaseCmd->pViewAngles->angValue;
        if (config::aimbot::smoothing > 0.0f) {
            sdk::QAngle delta = targetAngles - current;
            ClampAnglesA(delta);
            targetAngles = current + delta * (1.0f - config::aimbot::smoothing);
        }
        if (config::rcs::enabled) {
            sdk::Vector3 punch = *(sdk::Vector3*)(localPawn + rcs_m_aimPunchAngle);
            targetAngles.x -= punch.x * 2.0f * config::rcs::strength;
            targetAngles.y -= punch.y * 2.0f * config::rcs::strength;
        }
        targetAngles.x = std::clamp(targetAngles.x, -89.0f, 89.0f);
        targetAngles.y = std::fmod(targetAngles.y, 360.0f);
        if (targetAngles.y > 180.0f) targetAngles.y -= 360.0f;
        if (targetAngles.y < -180.0f) targetAngles.y += 360.0f;
        cmd->SetSubTickAngle(targetAngles);
        if (config::aimbot::auto_shoot) {
            cmd->csgoUserCmd.pBaseCmd->pInButtonState->nValue |= sdk::IN_ATTACK;
        }
    }

    void RunSilentAimSubTick(DWORD* a1, sdk::C_CSPlayerPawn* localPawn)
    {
        if (!localPawn || !config::aimbot::enabled || !config::aimbot::silent_aim) return;
        if (!game_state::IsInGame()) return;
        if (GetAsyncKeyState(config::aimbot::pause_key) & 0x8000) return;

        

        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) {
            debug_console::Console::Get().Warning("[SilentAim] client.dll not found");
            return;
        }

        uintptr_t localPawnPtr = reinterpret_cast<uintptr_t>(localPawn);

        int localHealth = *(int*)(localPawnPtr + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);
        if (localHealth <= 0) return;

        sdk::Vector3 localEye = GetEyePositionRaw(localPawnPtr);
        int localTeam = *(int*)(localPawnPtr + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);

         uintptr_t entity_list = game_state::GetEntityList();
        if (!entity_list || !sdk::is_valid_ptr(entity_list)) {
            debug_console::Console::Get().Warning("[SilentAim] Entity list invalid!");
            return;
        }

        // Local index
        int localIndex = -1;
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
            if (pawn == localPawnPtr) {
                localIndex = i;
                break;
            }
        }

        int candidatesFound = 0;
        int validBoneCandidates = 0;
        sdk::C_CSPlayerPawn* bestTarget = nullptr;
        sdk::Vector3 bestHeadPos;
        float bestFov = config::aimbot::fov;

        for (int i = 1; i < 64; i++) {
            uintptr_t listEntry = *(uintptr_t*)(entity_list + (8 * (i & 0x7FFF) >> 9) + 16);
            if (!listEntry || !sdk::is_valid_ptr(listEntry)) continue;

            uintptr_t controller = *(uintptr_t*)(listEntry + 112 * (i & 0x1FF));
            if (!controller) continue;

            uint32_t pawnHandle = *(uint32_t*)(controller + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
            if (!pawnHandle) continue;

            uintptr_t list2 = *(uintptr_t*)(entity_list + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 16);
            if (!list2) continue;

            uintptr_t pawn = *(uintptr_t*)(list2 + 112 * (pawnHandle & 0x1FF));
            if (!pawn || pawn == localPawnPtr) continue;

            int health = *(int*)(pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);
            if (health <= 0) continue;

            int team = *(int*)(pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
            if (config::aimbot::team_check && team == localTeam) continue;

            if (config::aimbot::visible_check && localIndex != -1) {
                int32_t spotted = *(int32_t*)(pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState +
                    cs2_dumper::schemas::client_dll::EntitySpottedState_t::m_bSpottedByMask);
                if (!(spotted & (1 << (localIndex - 1)))) continue;
            }

            sdk::Vector3 headPos = GetBonePositionRaw(pawn, 6);

            candidatesFound++;

            if (headPos.x == 0.0f && headPos.y == 0.0f && headPos.z == 0.0f) {
                continue;
            }

            validBoneCandidates++;

            float dist = localEye.Distance(headPos);
            if (dist > config::aimbot::max_distance) continue;

            sdk::Vector2 aim2D = CalcAngle(localEye, headPos);
            sdk::QAngle aimAngles(aim2D.x, aim2D.y, 0.0f);

            float fov = GetFovA({ *(float*)&a1[4], *(float*)&a1[5], 0.0f }, aimAngles);

            if (fov < bestFov) {
                bestFov = fov;
                bestTarget = reinterpret_cast<sdk::C_CSPlayerPawn*>(pawn);
                bestHeadPos = headPos;
            }
        }

        // ======================== MAX DIAGNOSTIC LOG ========================
        static auto lastLog = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastLog).count() > 200)
        {
            if (!bestTarget) {
                debug_console::Console::Get().Warning("[SilentAim] NO TARGET | Candidates: %d | Valid Bones: %d | LocalTeam: %d | LocalIndex: %d",
                    candidatesFound, validBoneCandidates, localTeam, localIndex);
            }
            else {
                float distMeters = localEye.Distance(bestHeadPos) * 0.0254f;
                int targetTeam = *(int*)((uintptr_t)bestTarget + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
                int targetHP = *(int*)((uintptr_t)bestTarget + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);

                debug_console::Console::Get().Success("[SilentAim] LOCKED ? Team=%d | HP=%d | Dist=%.1fm | FOV=%.2f | BoneValid=Yes",
                    targetTeam, targetHP, distMeters, bestFov);
            }
            lastLog = now;
        }
        // =================================================================

        if (!bestTarget) return;

        // Angle calculation
        sdk::Vector2 target2D = CalcAngle(localEye, bestHeadPos);
        sdk::QAngle targetAngles(target2D.x, target2D.y, 0.0f);

        sdk::QAngle currentAngles = { *(float*)&a1[4], *(float*)&a1[5], 0.0f };

        sdk::QAngle delta;
        delta.x = targetAngles.x - currentAngles.x;
        delta.y = targetAngles.y - currentAngles.y;

        while (delta.y > 180.0f) delta.y -= 360.0f;
        while (delta.y < -180.0f) delta.y += 360.0f;

        if (config::aimbot::smoothing > 0.0f) {
            targetAngles = currentAngles + delta * (1.0f - config::aimbot::smoothing);
        }
        else {
            targetAngles = currentAngles + delta;
        }

        if (config::rcs::enabled) {
            sdk::Vector3 punch = *(sdk::Vector3*)(localPawnPtr + rcs_m_aimPunchAngle);
            targetAngles.x -= punch.x * 2.0f * config::rcs::strength;
            targetAngles.y -= punch.y * 2.0f * config::rcs::strength;
        }

        targetAngles.x = std::clamp(targetAngles.x, -89.0f, 89.0f);
        targetAngles.y = std::fmod(targetAngles.y, 360.0f);
        if (targetAngles.y > 180.0f) targetAngles.y -= 360.0f;
        if (targetAngles.y < -180.0f) targetAngles.y += 360.0f;

        a1[4] = targetAngles.x;
        a1[5] = targetAngles.y;
    }


    static void AimbotLoop() {
        debug_console::Console::Get().Info("[Aimbot] Normal aim thread started");
        int localIndex = -1;
        sdk::Vector2 old_punch = { 0.0f, 0.0f };

        while (s_aimbot_running.load()) {
            if (!config::aimbot::enabled || !game_state::IsInGame()) {
                old_punch = { 0.0f, 0.0f };
                Sleep(50);
                continue;
            }
            if (GetAsyncKeyState(config::aimbot::pause_key) & 0x8000) { Sleep(1); continue; }
            if (config::aimbot::silent_aim) continue;

            try {
                uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
                if (!client) { Sleep(50); continue; }

                uintptr_t localPawn = game_state::GetLocalPawnRaw();
                if (!localPawn || !sdk::is_valid_ptr(localPawn)) { Sleep(1); continue; }
                if (*(int*)(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth) <= 0) { Sleep(1); continue; }

                int local_team = *(int*)(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
                sdk::Vector3 local_eye = GetEyePositionRaw(localPawn);

				uintptr_t entity_list = game_state::GetEntityList();
                if (!entity_list || !sdk::is_valid_ptr(entity_list)) { Sleep(1); continue; }

                localIndex = -1;
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
                    if (pawn == localPawn) { localIndex = i; break; }
                }

                uintptr_t closest_pawn = 0;
                float closest_dist = config::aimbot::max_distance;

                // Use pattern-resolved view angles
                sdk::Vector2* va = nullptr;
                va = reinterpret_cast<sdk::Vector2*>(*(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwViewAngles));
                sdk::Vector2 current_angles = *va;

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
                    if (!pawn || pawn == localPawn) continue;

                    int health = *(int*)(pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);
                    if (health <= 0) continue;
                    if (config::aimbot::team_check) {
                        int team = *(int*)(pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
                        if (team == local_team) continue;
                    }
                    if (config::aimbot::visible_check && localIndex != -1) {
                        int32_t spotted = *(int32_t*)(pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState + cs2_dumper::schemas::client_dll::EntitySpottedState_t::m_bSpottedByMask);
                        if (!(spotted & (1 << (localIndex - 1)))) continue;
                    }
                    sdk::Vector3 head_pos = GetBonePositionRaw(pawn, 6);
                    if (head_pos.x == 0.0f && head_pos.y == 0.0f && head_pos.z == 0.0f) continue;
                    sdk::Vector2 target_angles = CalcAngle(local_eye, head_pos);
                    float fov = GetFov(current_angles, target_angles);
                    if (fov > config::aimbot::fov) continue;
                    float dist = local_eye.Distance(head_pos);
                    if (dist < closest_dist) {
                        closest_dist = dist;
                        closest_pawn = pawn;
                    }
                }

                if (closest_pawn && (GetAsyncKeyState(VK_LBUTTON) & 0x8000) && !skins::IsKnife(skins::GetDefIndex(skins::GetActiveWeapon()))) {
                    sdk::Vector3 head_pos = GetBonePositionRaw(closest_pawn, 6);
                    sdk::Vector2 target_angles = CalcAngle(local_eye, head_pos);
                    sdk::Vector2 delta;
                    delta.x = NormalizeAngle(target_angles.x - current_angles.x);
                    delta.y = NormalizeAngle(target_angles.y - current_angles.y);
                    float smooth_factor = 1.0f - config::aimbot::smoothing;
                    delta.x *= smooth_factor;
                    delta.y *= smooth_factor;
                    sdk::Vector2 new_angles;
                    new_angles.x = current_angles.x + delta.x;
                    new_angles.y = current_angles.y + delta.y;
                    new_angles.x = std::clamp(new_angles.x, -89.0f, 89.0f);
                    while (new_angles.y > 180.0f) new_angles.y -= 360.0f;
                    while (new_angles.y < -180.0f) new_angles.y += 360.0f;
                    *va = new_angles;
                }

                // RCS
                if (config::rcs::enabled) {
                    int shots_fired = *(int*)(localPawn + rcs_m_iShotsFired);
                    if (shots_fired > 0) {
                        sdk::Vector3 punch = *(sdk::Vector3*)(localPawn + rcs_m_aimPunchAngle);
                        sdk::Vector2 cur_punch = { punch.x, punch.y };
                        sdk::Vector2 delta;
                        delta.x = (cur_punch.x - old_punch.x) * 2.0f * config::rcs::strength;
                        delta.y = (cur_punch.y - old_punch.y) * 2.0f * config::rcs::strength;
                        sdk::Vector2 angles = *va;
                        angles.x -= delta.x;
                        angles.y -= delta.y;
                        angles.x = std::clamp(angles.x, -89.0f, 89.0f);
                        while (angles.y > 180.0f) angles.y -= 360.0f;
                        while (angles.y < -180.0f) angles.y += 360.0f;
                        *va = angles;
                        old_punch = cur_punch;
                    }
                    else {
                        old_punch = { 0.0f, 0.0f };
                    }
                }
            }
            catch (...) {}
            Sleep(1);
        }
        debug_console::Console::Get().Info("[Aimbot] Normal aim thread stopped");
    }

    void StartAimbotThread() {
        if (s_aimbot_running.load()) return;
        s_aimbot_running.store(true);
        s_aimbot_thread = std::thread(AimbotLoop);
        debug_console::Console::Get().Success("[Aimbot] Normal aim thread launched");
    }

    void StopAimbotThread() {
        s_aimbot_running.store(false);
        if (s_aimbot_thread.joinable()) s_aimbot_thread.join();
        debug_console::Console::Get().Info("[Aimbot] Normal aim thread joined");
    }

    // ======================== ESP & DRAWING FUNCTIONS ========================
    bool WorldToScreen(const sdk::Vector3& world, sdk::Vector2& screen, const sdk::ViewMatrix& matrix, int screen_width, int screen_height) {
        float w = matrix.matrix[3][0] * world.x + matrix.matrix[3][1] * world.y +
            matrix.matrix[3][2] * world.z + matrix.matrix[3][3];
        if (w < 0.001f) return false;
        float x = matrix.matrix[0][0] * world.x + matrix.matrix[0][1] * world.y +
            matrix.matrix[0][2] * world.z + matrix.matrix[0][3];
        float y = matrix.matrix[1][0] * world.x + matrix.matrix[1][1] * world.y +
            matrix.matrix[1][2] * world.z + matrix.matrix[1][3];
        x /= w; y /= w;
        screen.x = (screen_width / 2.0f) + (x * screen_width / 2.0f);
        screen.y = (screen_height / 2.0f) - (y * screen_height / 2.0f);
        return true;
    }

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
        float bg_color[4] = { 0.1f, 0.1f, 0.1f, 0.8f };
        DrawFilledRect(sdk::Vector2(top.x - 8, top.y), sdk::Vector2(4, height), bg_color);
        float health_percent = health / (float)max_health;
        float hp_color[4] = { 1.0f - health_percent, health_percent, 0.0f, 0.9f };
        DrawFilledRect(sdk::Vector2(top.x - 8, bottom.y - health_height), sdk::Vector2(4, health_height), hp_color);
        if (health < 100) {
            char health_text[8];
            sprintf_s(health_text, "%d", health);
            DrawText(sdk::Vector2(top.x - 18, top.y + (height / 2) - 7), health_text, hp_color);
        }
    }

    void DrawSkeleton(sdk::C_CSPlayerPawn* player, const sdk::ViewMatrix& view_matrix, int screen_width, int screen_height, const float color[4]) {
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

    // ======================== ENTITY LIST MANAGEMENT ========================
    std::vector<sdk::C_CSPlayerPawn*> GetPlayerList() {
        playerPawns.clear();
        std::vector<sdk::C_CSPlayerPawn*> players;
        if (!game_state::IsInGame()) return players;

		uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");

		uintptr_t entity_list = game_state::GetEntityList();
        if (!entity_list || !sdk::is_valid_ptr(entity_list)) return players;

        try {
            for (int i = 1; i < 64; i++) {
                uintptr_t entityList1 = *(uintptr_t*)(entity_list + (8 * (i & 0x7fff) >> 9) + 16);
                if (!entityList1 || !sdk::is_valid_ptr(entityList1)) continue;
                uintptr_t playerController = *(uintptr_t*)(entityList1 + 112 * (i & 0x1FF));
                if (!playerController) continue;
                uint32_t playerPawn = *(uint32_t*)(playerController + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
                if (!playerPawn) continue;
                uintptr_t entityList2 = *(uintptr_t*)(entity_list + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
                if (!entityList2) continue;
                uintptr_t pCSPlayerPawn = *(uintptr_t*)(entityList2 + 112 * (playerPawn & 0x1FF));
                if (!pCSPlayerPawn) continue;
                int health = *(int*)(pCSPlayerPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);
                if (health < 0 || !health) continue;
                playerPawns.push_back(pCSPlayerPawn);
                auto* player = reinterpret_cast<sdk::C_CSPlayerPawn*>(pCSPlayerPawn);
                if (!player->IsAlive()) continue;
                players.push_back(player);
            }
        }
        catch (const std::exception& e) {
            debug_console::Console::Get().Error("Exception in GetPlayerList: %s", e.what());
        }
        return players;
    }

    sdk::C_CSPlayerPawn* GetLocalPlayer() {
        return game_state::GetLocalPawn();
    }

    static DWORD g_last_cache_tick = 0;
    static void UpdateCache() {
        DWORD now = GetTickCount();
        if (now == g_last_cache_tick) return;
        g_last_cache_tick = now;
        GetPlayerList();
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

    void RenderESP() {
        if (!config::esp::enabled) return;
        if (!game_state::IsInGame()) return;
        UpdateCache();

		uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");

		uintptr_t g_local_player2 = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
        if (!g_local_player2) return;

		int local_team = *(int*)(g_local_player2 + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
        uintptr_t local_pawn_addr = g_local_player2;

        for (uintptr_t player : playerPawns) {
            if (!player || player == local_pawn_addr) continue;
            int player_team = *(int*)(player + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
            int health = *(int*)(player + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);
            if (config::esp::team_check && player_team == local_team) continue;

            try {
                sdk::Vector3 origin = *(sdk::Vector3*)(player + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
                sdk::Vector3 head_pos = { origin.x, origin.y, origin.z + 65.0f };
                head_pos.z += 10.0f;
                sdk::Vector3 ourpos = *(sdk::Vector3*)(local_pawn_addr + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
                float distance = ourpos.Distance(origin);
                sdk::Vector2 screen_pos, screen_head;
                if (!WorldToScreen(origin, screen_pos, g_view_matrix, g_screen_width, g_screen_height)) continue;
                if (!WorldToScreen(head_pos, screen_head, g_view_matrix, g_screen_width, g_screen_height)) continue;
                float height = screen_pos.y - screen_head.y;
                float width = height / 2.0f;
                const float* color = (player_team == local_team) ? config::esp::team_color : config::esp::box_color;

                if (config::esp::box) DrawBox(screen_head, screen_pos, width, color);
                if (config::esp::health_bar) DrawHealthBar(sdk::Vector2(screen_head.x - width / 2, screen_head.y), sdk::Vector2(screen_pos.x - width / 2, screen_pos.y), health, 100);
                if (config::esp::skeleton) DrawSkeleton(reinterpret_cast<sdk::C_CSPlayerPawn*>(player), g_view_matrix, g_screen_width, g_screen_height, config::esp::skeleton_color);
                if (config::esp::distance) {
                    char dist_text[32];
                    sprintf_s(dist_text, "%.0fm", distance * 0.0254f);
                    DrawText(sdk::Vector2(screen_pos.x - 15, screen_pos.y + 5), dist_text, color);
                }
                if (config::esp::snaplines) DrawLine(sdk::Vector2(g_screen_width / 2.0f, g_screen_height), screen_pos, color, 1.0f);
            }
            catch (...) { continue; }
        }
    }

    // ======================== MISC FEATURES ========================
    void BunnyHop() {
        if (!config::misc::bunny_hop) return;
        if (!game_state::IsInGame()) return;
        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return;
        uintptr_t localPawn = reinterpret_cast<uintptr_t>(game_state::GetLocalPawn());
        if (!localPawn || !sdk::is_valid_ptr(localPawn)) return;
        try {
            static bool was_in_air = false;
            int flags = *(int*)(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_fFlags);
            bool is_on_ground = (flags & 1) != 0;
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

    void NoFlash() {
        if (!config::misc::no_flash) return;
        if (!game_state::IsInGame()) return;
        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return;
        uintptr_t localPawn = reinterpret_cast<uintptr_t>(game_state::GetLocalPawn());
        if (!localPawn || !sdk::is_valid_ptr(localPawn)) return;
        try {
            float* flash_duration = reinterpret_cast<float*>(localPawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_flFlashDuration);
            if (*flash_duration > 0.0f) *flash_duration = 0.0f;
        }
        catch (...) {}
    }

    void TriggerBot() {
        static DWORD last_shot = 0;
        static bool mouse_down = false;
        static DWORD mouse_down_time = 0;
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
        if (!game_state::IsInGame()) return;
        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return;
        uintptr_t localPawn = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);   
        if (!localPawn || !sdk::is_valid_ptr(localPawn)) return;
        try {
            int entity_index = *(int*)(localPawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_iIDEntIndex);
            if (entity_index > 0) {
                uintptr_t entity_list = game_state::GetEntityList();
                if (!entity_list || !sdk::is_valid_ptr(entity_list)) return;
                uintptr_t list1 = *(uintptr_t*)(entity_list + (0x8 * (entity_index >> 9) + 16));
                if (!list1 || !sdk::is_valid_ptr(list1)) return;
                uintptr_t controller = *(uintptr_t*)(list1 + 112 * (entity_index & 0x1FF));
                if (!controller || !sdk::is_valid_ptr(controller)) return;
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
        catch (...) {}
    }

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
            catch (...) { continue; }
            count++;
        }
    }
}
