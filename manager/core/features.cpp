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
#include <unordered_map>
#include <cstring>
#include "../external/offsets/buttons.hpp"

namespace features {
    // Per-frame cached values from the game-state monitor
    static sdk::C_CSPlayerPawn* g_local_player = nullptr;
    static sdk::ViewMatrix g_view_matrix = {};
    static int g_screen_width = 1920;
    static int g_screen_height = 1080;

    static int g_last_screen_width = 0;
    static int g_last_screen_height = 0;

    static std::vector<uintptr_t> playerPawns;
    static std::unordered_map<uintptr_t, std::string> player_names;
    static std::atomic<bool> s_input_blocked{false};
    static bool s_mouse_down = false;
    static ULONGLONG s_mouse_down_time = 0;
    static uintptr_t s_jump_address = 0;
    void SetInputBlocked(bool blocked) { s_input_blocked = blocked; }

    // Aimbot thread control
    static std::atomic<bool> s_aimbot_running{ false };
    static std::thread s_aimbot_thread;
    static std::mutex s_aimbot_lifecycle;

    // Get screen dimensions from multiple sources with fallback priority
    static void UpdateScreenDimensions() {
        if (ImGui::GetCurrentContext()) {
            const auto size = ImGui::GetIO().DisplaySize;
            if (std::isfinite(size.x) && std::isfinite(size.y) && size.x > 0 && size.y > 0) {
                g_screen_width = static_cast<int>(size.x);
                g_screen_height = static_cast<int>(size.y);
                return;
            }
        }
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
        ang.x = sdk::clamp_pitch(ang.x);
        ang.y = sdk::normalize_yaw(ang.y);
        ang.z = 0.0f;
    }

    float GetFovA(const sdk::QAngle& viewAngles, const sdk::QAngle& aimAngles) {
        sdk::QAngle delta = aimAngles - viewAngles;
        if (!std::isfinite(delta.x) || !std::isfinite(delta.y)) return INFINITY;
        delta.y = sdk::normalize_yaw(delta.y);
        return sqrtf(delta.x * delta.x + delta.y * delta.y);
    }

    sdk::Vector2 CalcAngle(const sdk::Vector3& src, const sdk::Vector3& dst) {
        const auto delta = dst - src;
        if (!sdk::finite(delta) || delta.Length() == 0.0f) return {};
        constexpr float degrees = 180.0f / 3.14159265358979323846f;
        return {-std::atan2(delta.z, std::hypot(delta.x, delta.y)) * degrees,
            std::atan2(delta.y, delta.x) * degrees};
    }

    static float NormalizeAngle(float angle) { return sdk::normalize_yaw(angle); }

    float GetFov(const sdk::Vector2& view_angles, const sdk::Vector2& aim_angles) {
        return GetFovA({view_angles.x, view_angles.y, 0}, {aim_angles.x, aim_angles.y, 0});
    }

    void GetScreenResolution(int& width, int& height) {
        width = g_screen_width;
        height = g_screen_height;
    }

    // ======================== BONE / EYE HELPERS (raw pointers) ========================
    static sdk::Vector3 GetBonePositionRaw(uintptr_t pawn, int bone_index) {
        return pawn ? reinterpret_cast<sdk::C_CSPlayerPawn*>(pawn)->GetBonePosition(bone_index) : sdk::Vector3{};
    }
    static sdk::Vector3 GetEyePositionRaw(uintptr_t pawn) {
        return pawn ? reinterpret_cast<sdk::C_CSPlayerPawn*>(pawn)->GetEyePosition() : sdk::Vector3{};
    }
    static bool IsSpottedBy(uintptr_t pawn, int local_index) {
        uint32_t mask[2]{};
        return sdk::read_memory(pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState +
            cs2_dumper::schemas::client_dll::EntitySpottedState_t::m_bSpottedByMask, mask) && sdk::spotted_by(mask, local_index);
    }

    // RCS offsets
    constexpr std::ptrdiff_t rcs_m_iShotsFired = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_iShotsFired;
    static sdk::Vector3 ReadAimPunch(uintptr_t pawn) {
        const auto services = sdk::read_value<uintptr_t>(pawn + sdk::off::C_CSPlayerPawn::m_pAimPunchServices);
        if (!services) return {};
        const auto angle = sdk::read_value<sdk::Vector3>(services + sdk::off::CCSPlayer_AimPunchServices::m_predictableBaseAngle);
        return sdk::finite(angle) ? angle : sdk::Vector3{};
    }

    // ======================== SILENT AIM (CreateMove) -- unchanged but relies on SubTick pattern ========================
    void RunSilentAim(CUserCmd* cmd) {
        if (!cmd || !cmd->csgoUserCmd.pBaseCmd || !cmd->csgoUserCmd.pBaseCmd->pViewAngles || !cmd->csgoUserCmd.pBaseCmd->pInButtonState) return;
        if (!config::aimbot::enabled || !config::aimbot::silent_aim) return;
        if (!game_state::IsInGame()) return;
        if (GetAsyncKeyState(config::aimbot::pause_key) & 0x8000) return;

        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return;
        uintptr_t localPawn = game_state::GetLocalPawnRaw();
        if (!localPawn || !sdk::is_valid_ptr(localPawn)) return;
        if (sdk::read_value<int>(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth) <= 0) return;
        if (!(cmd->csgoUserCmd.pBaseCmd->pInButtonState->nValue & sdk::IN_ATTACK)) return;

        sdk::Vector3 localEye = GetEyePositionRaw(localPawn);
        int localTeam = sdk::read_value<uint8_t>(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
		uintptr_t entity_list = game_state::GetEntityList();
        if (!entity_list || !sdk::is_valid_ptr(entity_list)) return;

        int localIndex = -1;
        for (int i = 1; i <= 64; i++) {
            uintptr_t list1 = entity_list;
            if (!list1 || !sdk::is_valid_ptr(list1)) continue;
            uintptr_t controller = sdk::entity_at(entity_list, i);
            if (!controller) continue;
            uint32_t pawnHandle = sdk::read_value<uint32_t>(controller + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
            if (!pawnHandle) continue;
            uintptr_t list2 = entity_list;
            if (!list2) continue;
            uintptr_t pawn = sdk::entity_from_handle(entity_list, pawnHandle);
            if (pawn == localPawn) { localIndex = i; break; }
        }

        sdk::C_CSPlayerPawn* bestTarget = nullptr;
        float bestFov = config::aimbot::fov;
        sdk::Vector3 bestPos;

        for (int i = 1; i <= 64; i++) {
            uintptr_t list1 = entity_list;
            if (!list1 || !sdk::is_valid_ptr(list1)) continue;
            uintptr_t controller = sdk::entity_at(entity_list, i);
            if (!controller) continue;
            uint32_t pawnHandle = sdk::read_value<uint32_t>(controller + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
            if (!pawnHandle) continue;
            uintptr_t list2 = entity_list;
            if (!list2) continue;
            uintptr_t pawn = sdk::entity_from_handle(entity_list, pawnHandle);
            if (!pawn || pawn == localPawn) continue;
            int health = sdk::read_value<int>(pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);
            if (health <= 0) continue;
            if (config::aimbot::team_check) {
                int playerTeam = sdk::read_value<uint8_t>(pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
                if (localTeam == playerTeam) continue;
            }
            if (config::aimbot::visible_check && !IsSpottedBy(pawn, localIndex)) continue;
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
        if (!bestTarget) { silent_aim_status = "No target passed team/spotted/bone/distance/FOV checks"; return; }

        sdk::Vector2 aim2D = CalcAngle(localEye, bestPos);
        sdk::QAngle targetAngles(aim2D.x, aim2D.y, 0.0f);
        sdk::QAngle current = cmd->csgoUserCmd.pBaseCmd->pViewAngles->angValue;
        if (config::aimbot::smoothing > 0.0f) {
            sdk::QAngle delta = targetAngles - current;
            ClampAnglesA(delta);
            targetAngles = current + delta * (1.0f - config::aimbot::smoothing);
        }
        if (config::rcs::enabled) {
            sdk::Vector3 punch = ReadAimPunch(localPawn);
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

    struct AimPlayers {
        uintptr_t pawns[65]{};
        int local_index = -1;
    };
    static AimPlayers ReadAimPlayers(uintptr_t system, uintptr_t local) {
        AimPlayers result{};
        unsigned controllers = 0, resolved = 0;
        for (int i = 1; i <= 64; ++i) {
            const auto controller = sdk::entity_at(system, i);
            if (!controller) continue;
            ++controllers;
            uint32_t handle = UINT32_MAX;
            if (!sdk::read_memory(controller + sdk::off::CCSPlayerController::m_hPlayerPawn, handle)) continue;
            const auto pawn = sdk::entity_from_handle(system, handle);
            // Enumeration is shared by normal and silent aim. Apply health and
            // optional team filtering in the callers, without duplicate reads.
            result.pawns[i] = pawn;
            if (pawn) ++resolved;
            if (pawn == local) result.local_index = i;
        }
        aim_controllers = controllers;
        aim_resolved_pawns = resolved;
        return result;
    }
    struct SilentTargetCache {
        struct Target {
            uintptr_t pawn = 0; uint32_t handle = UINT32_MAX;
            int health = 0, team = 0; bool spotted = false;
            sdk::Vector3 head{}; sdk::Vector2 angle{};
            float distance_squared = 0;
        };
        Target targets[65]{};
        uintptr_t system = 0, local = 0, weapon = 0;
        int local_index = -1;
        ULONGLONG expires = 0;
        sdk::Vector3 eye{};
        bool geometry_valid = false;
    };
    static SilentTargetCache& ReadSilentTargets(uintptr_t system, uintptr_t local, uintptr_t weapon) {
        static thread_local SilentTargetCache cache{};
        const auto now = GetTickCount64();
        if (cache.system == system && cache.local == local && cache.weapon == weapon && now < cache.expires) {
            ++silent_cache_hits;
            return cache;
        }
        cache = {};
        cache.system = system; cache.local = local; 
        cache.weapon = weapon; 
        cache.expires = now + 8;
        const auto players = ReadAimPlayers(system, local);
        cache.local_index = players.local_index;
        for (int i = 1; i <= 64; ++i) {
            auto& item = cache.targets[i];
            item.pawn = players.pawns[i];
            if (!item.pawn || item.pawn == local) continue;
            item.health = sdk::read_value<int>(item.pawn + sdk::off::C_BaseEntity::m_iHealth);
            if (item.health <= 0) continue;
            const auto identity = sdk::read_value<uintptr_t>(item.pawn + 0x10);
            if (identity) item.handle = sdk::read_value<uint32_t>(identity + 0x10);
            item.team = sdk::read_value<uint8_t>(item.pawn + sdk::off::C_BaseEntity::m_iTeamNum);
            item.spotted = IsSpottedBy(item.pawn, cache.local_index);
            item.head = GetBonePositionRaw(item.pawn, 6);
        }
        ++silent_target_scans;
        return cache;
    }
    static void PrepareSilentGeometry(SilentTargetCache& cache, const sdk::Vector3& eye) {
        if (cache.geometry_valid && cache.eye.x == eye.x && cache.eye.y == eye.y && cache.eye.z == eye.z) return;
        cache.eye = eye;
        cache.geometry_valid = true;
        for (auto& target : cache.targets) {
            if (!target.pawn || target.pawn == cache.local || target.health <= 0) continue;
            const auto delta = target.head - eye;
            target.distance_squared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
            target.angle = CalcAngle(eye, target.head);
        }
        ++silent_geometry_updates;
    }
    void RunSilentAimSubTick(DWORD* a1, sdk::C_CSPlayerPawn* localPawn)
    {
        struct DebugSample {
            SilentAimDebug data{};
            bool publish = false;
            DebugSample() {
                static std::atomic<ULONGLONG> next_sample{0};
                const auto now = GetTickCount64();
                auto next = next_sample.load();
                publish = now >= next && next_sample.compare_exchange_strong(next, now + 250);
            }
            ~DebugSample() {
                if (!publish) return;
                data.status = silent_aim_status.load();
                std::lock_guard<std::mutex> lock(silent_debug_mutex);
                silent_debug_snapshot = data;
            }
        } sample;
        sample.data.source = reinterpret_cast<uintptr_t>(a1);
        DWORD words[7]{};
        if (!a1 || !sdk::read_memory(reinterpret_cast<uintptr_t>(a1), words)) {
            silent_aim_status = "Subtick input unreadable"; return;
        }
        // These DWORD slots contain IEEE-754 float bits, not numeric DWORD values.
        sdk::Vector3 sourceAngles{};
        static_assert(sizeof(sourceAngles) == 3 * sizeof(DWORD));
        std::memcpy(&sourceAngles, words + 4, sizeof(sourceAngles));
        std::memcpy(sample.data.raw_angles, words + 4, sizeof(sample.data.raw_angles));
        sample.data.input = sourceAngles;
        if (!sdk::finite(sourceAngles)) { silent_aim_status = "Input angles contain NaN/Inf"; return; }
        sample.data.input_valid = true;
        if (!localPawn) { silent_aim_status = "Local pawn unavailable"; return; }
        if (!config::aimbot::enabled || !config::aimbot::silent_aim) { silent_aim_status = "Silent aim disabled"; return; }
        const auto state = game_state::GetSnapshot();
        if (!state.in_game) { silent_aim_status = "Local game state unavailable"; return; }
        if (GetAsyncKeyState(config::aimbot::pause_key) & 0x8000) { silent_aim_status = "Pause key held"; return; }



        // The state monitor already resolves client.dll. Reuse one coherent
        // snapshot instead of taking its mutex repeatedly or the loader lock.
        const uintptr_t entity_list = state.entity_list;
        if (!entity_list) {
            silent_aim_status = "Entity list unavailable";
            return;
        }

        uintptr_t localPawnPtr = reinterpret_cast<uintptr_t>(localPawn);

        const auto services = sdk::read_value<uintptr_t>(localPawnPtr + sdk::off::C_BasePlayerPawn::m_pWeaponServices);
        uint32_t activeHandle = UINT32_MAX;
        if (services) sdk::read_memory(services + sdk::off::CPlayer_WeaponServices::m_hActiveWeapon, activeHandle);
        const auto activeWeapon = sdk::entity_from_handle(entity_list, activeHandle);
        if (!activeWeapon) { silent_aim_status = "Active weapon unresolved"; return; }
        if (skins::IsKnife(skins::GetDefIndex(activeWeapon))) { silent_aim_status = "Knife equipped"; return; }
        int localHealth = sdk::read_value<int>(localPawnPtr + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);
        if (localHealth <= 0) { silent_aim_status = "Local pawn is dead"; return; }

        sdk::Vector3 localEye = GetEyePositionRaw(localPawnPtr);
        sample.data.eye = localEye;
        sample.data.eye_valid = sdk::finite(localEye);
        if (!sample.data.eye_valid) { silent_aim_status = "Eye position contains NaN/Inf"; return; }
        int localTeam = sdk::read_value<uint8_t>(localPawnPtr + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);

        auto& players = ReadSilentTargets(entity_list, localPawnPtr, activeWeapon);
        PrepareSilentGeometry(players, localEye);
        const int localIndex = players.local_index;
        sdk::C_CSPlayerPawn* bestTarget = nullptr;
        sdk::Vector3 bestHeadPos;
        const float maxFov = config::aimbot::fov;
        const float maxDistance = config::aimbot::max_distance;
        if (!std::isfinite(maxFov) || maxFov < 0 || !std::isfinite(maxDistance) || maxDistance < 0) return;
        const float maxFovSquared = maxFov * maxFov;
        const float maxDistanceSquared = maxDistance * maxDistance;
        float bestFovSquared = maxFovSquared;
        int bestControllerIndex = -1;

        for (int i = 1; i <= 64; i++) {
            const auto& candidate = players.targets[i];
            uintptr_t pawn = candidate.pawn;
            if (!pawn || pawn == localPawnPtr) continue;

            int health = candidate.health;
            if (health <= 0) continue;
            ++sample.data.alive;

            int team = candidate.team;
            if (config::aimbot::team_check && team == localTeam) continue;
            ++sample.data.enemies;

            if (config::aimbot::visible_check && !candidate.spotted) continue;
            ++sample.data.spotted;

            sdk::Vector3 headPos = candidate.head;



            if (!sdk::finite(headPos) || (headPos.x == 0.0f && headPos.y == 0.0f && headPos.z == 0.0f)) {
                continue;
            }



            ++sample.data.bones;
            if (candidate.distance_squared > maxDistanceSquared) continue;
            ++sample.data.in_range;

            const float pitch = candidate.angle.x - sourceAngles.x;
            const float yaw = sdk::normalize_yaw(candidate.angle.y - sourceAngles.y);
            const float fovSquared = pitch * pitch + yaw * yaw;

            if (fovSquared <= maxFovSquared) ++sample.data.in_fov;
            if (fovSquared <= bestFovSquared) {
                bestFovSquared = fovSquared;
                bestTarget = reinterpret_cast<sdk::C_CSPlayerPawn*>(pawn);
                bestHeadPos = headPos;
                bestControllerIndex = i;
                sample.data.target_pawn = pawn;
                sample.data.target_health = health;
                sample.data.target_team = team;
            }
        }

        if (!bestTarget) {
            silent_aim_status = !sample.data.alive ? "No living remote pawns resolved" :
                !sample.data.enemies ? "All living players rejected by team check" :
                !sample.data.spotted ? (localIndex < 1 ? "Spotted check: local controller index missing" : "No enemies spotted by local player") :
                !sample.data.bones ? "Enemy head positions unavailable" :
                !sample.data.in_range ? "Enemies outside maximum distance" : "Enemies outside aim FOV";
            return;
        }
        // Cached positions live for at most 8 ms. Revalidate the selected
        // identity and health before writing, so a despawn/death is not reused.
        const auto& selected = players.targets[bestControllerIndex];
        if (sdk::entity_from_handle(entity_list, selected.handle) != selected.pawn ||
            sdk::read_value<int>(selected.pawn + sdk::off::C_BaseEntity::m_iHealth) <= 0) {
            silent_aim_status = "Selected target despawned or died"; return;
        }

        if (sample.publish) {
            const auto controller = sdk::entity_at(entity_list, bestControllerIndex);
            if (controller) sdk::read_memory(controller + sdk::off::CBasePlayerController::m_iszPlayerName, sample.data.target_name);
            sample.data.target_name[sizeof(sample.data.target_name) - 1] = '\0';
        }
        sample.data.target = bestHeadPos;
        sample.data.target_valid = true;
        // Angle calculation
        sdk::Vector2 target2D = selected.angle;
        sdk::QAngle targetAngles(target2D.x, target2D.y, 0.0f);

        sdk::QAngle currentAngles = {sourceAngles.x, sourceAngles.y, sourceAngles.z};

        sdk::QAngle delta;
        delta.x = targetAngles.x - currentAngles.x;
        delta.y = targetAngles.y - currentAngles.y;

        if (!std::isfinite(delta.x) || !std::isfinite(delta.y)) return;
        delta.y = sdk::normalize_yaw(delta.y);

        if (config::aimbot::smoothing > 0.0f) {
            targetAngles = currentAngles + delta * (1.0f - config::aimbot::smoothing);
        }
        else {
            targetAngles = currentAngles + delta;
        }

        if (config::rcs::enabled) {
            sdk::Vector3 punch = ReadAimPunch(localPawnPtr);
            targetAngles.x -= punch.x * 2.0f * config::rcs::strength;
            targetAngles.y -= punch.y * 2.0f * config::rcs::strength;
        }

        targetAngles.x = std::clamp(targetAngles.x, -89.0f, 89.0f);
        targetAngles.y = std::fmod(targetAngles.y, 360.0f);
        if (targetAngles.y > 180.0f) targetAngles.y -= 360.0f;
        if (targetAngles.y < -180.0f) targetAngles.y += 360.0f;

        const sdk::Vector3 output{targetAngles.x, targetAngles.y, 0.0f};
        sample.data.output = output;
        if (sdk::finite(output) && sdk::write_memory(reinterpret_cast<uintptr_t>(&a1[4]), output)) {
            sample.data.written = true;
            silent_aim_status = "Subtick angles written";
            ++silent_aim_writes;
        } else silent_aim_status = "Subtick angle write failed";
    }


    void RunNormalAimTick() {

        int localIndex = -1;
        static sdk::Vector2 old_punch = { 0.0f, 0.0f };

        do {

            std::lock_guard<std::recursive_mutex> settings_lock(config::mutex);
            if (s_input_blocked || (!config::aimbot::enabled && !config::rcs::enabled) || !game_state::IsInGame()) {
                normal_aim_status = s_input_blocked ? "Menu/console blocks input" : "Aim/RCS disabled or local pawn unavailable";
                old_punch = { 0.0f, 0.0f };

                continue;
            }
            if (GetAsyncKeyState(config::aimbot::pause_key) & 0x8000) { normal_aim_status = "Pause key held"; old_punch = {}; continue; }
            if (config::aimbot::enabled && config::aimbot::silent_aim) { normal_aim_status = "Silent mode selected"; continue; }

            try {
                uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
                if (!client) { normal_aim_status = "client.dll unavailable"; continue; }

                uintptr_t localPawn = game_state::GetLocalPawnRaw();
                if (!localPawn || !sdk::is_valid_ptr(localPawn)) { normal_aim_status = "Local pawn unavailable"; continue; }
                if (sdk::read_value<int>(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth) <= 0) { continue; }

                int local_team = sdk::read_value<uint8_t>(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
                sdk::Vector3 local_eye = GetEyePositionRaw(localPawn);

				uintptr_t entity_list = game_state::GetEntityList();
                if (!entity_list || !sdk::is_valid_ptr(entity_list)) { normal_aim_status = "Entity list unavailable"; continue; }

                const auto players = ReadAimPlayers(entity_list, localPawn);
                localIndex = players.local_index;

                uintptr_t closest_pawn = 0;
                float closest_dist = config::aimbot::max_distance;

                // Use pattern-resolved view angles
                sdk::Vector2* va = nullptr;
                va = reinterpret_cast<sdk::Vector2*>(client + cs2_dumper::offsets::client_dll::dwViewAngles);
                sdk::Vector2 current_angles{}; if (!sdk::read_memory(reinterpret_cast<uintptr_t>(va), current_angles)) { normal_aim_status = "View-angle read failed"; continue; }

                for (int i = 1; i <= 64; i++) {
                    uintptr_t pawn = players.pawns[i];
                    if (!pawn || pawn == localPawn) continue;

                    int health = sdk::read_value<int>(pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);
                    if (health <= 0) continue;
                    if (config::aimbot::team_check) {
                        int team = sdk::read_value<uint8_t>(pawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
                        if (team == local_team) continue;
                    }
                    if (config::aimbot::visible_check && !IsSpottedBy(pawn, localIndex)) continue;
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

                normal_aim_status = closest_pawn ? "Target selected; hold left mouse or enable Auto Shoot" :
                    (config::aimbot::visible_check && localIndex < 1 ? "Spotted check: local controller index unresolved" :
                        "No target passed team/spotted/bone/distance/FOV checks");
                const auto active_weapon = skins::GetActiveWeapon();
                if (closest_pawn && !active_weapon) normal_aim_status = "Active weapon unresolved";
                else if (closest_pawn && skins::IsKnife(skins::GetDefIndex(active_weapon))) normal_aim_status = "Knife equipped";
                bool aim_adjusted = false;
                if (config::aimbot::enabled && closest_pawn && active_weapon && ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) || config::aimbot::auto_shoot) && !skins::IsKnife(skins::GetDefIndex(active_weapon))) {
                    sdk::Vector3 head_pos = GetBonePositionRaw(closest_pawn, 6);
                    sdk::Vector2 target_angles = CalcAngle(local_eye, head_pos);
                    if (config::rcs::enabled) {
                        const auto punch = ReadAimPunch(localPawn);
                        target_angles.x -= punch.x * 2.0f * config::rcs::strength;
                        target_angles.y -= punch.y * 2.0f * config::rcs::strength;
                    }
                    sdk::Vector2 delta;
                    delta.x = NormalizeAngle(target_angles.x - current_angles.x);
                    delta.y = NormalizeAngle(target_angles.y - current_angles.y);
                    float smooth_factor = 1.0f - config::aimbot::smoothing;
                    delta.x *= smooth_factor;
                    delta.y *= smooth_factor;
                    sdk::Vector2 new_angles;
                    new_angles.x = current_angles.x + delta.x;
                    new_angles.y = current_angles.y + delta.y;
                    new_angles.x = sdk::clamp_pitch(new_angles.x);
                    new_angles.y = sdk::normalize_yaw(new_angles.y);

                    aim_adjusted = sdk::write_memory(reinterpret_cast<uintptr_t>(va), new_angles);
                    normal_aim_status = aim_adjusted ? "View angles written" : "View-angle write failed";
                    if (aim_adjusted) ++normal_aim_writes;
                }

                // RCS
                if (config::rcs::enabled) {
                    int shots_fired = sdk::read_value<int>(localPawn + rcs_m_iShotsFired);
                    if (shots_fired > 0) {
                        sdk::Vector3 punch = ReadAimPunch(localPawn);
                        sdk::Vector2 cur_punch = { punch.x, punch.y };
                        sdk::Vector2 delta;
                        delta.x = (cur_punch.x - old_punch.x) * 2.0f * config::rcs::strength;
                        delta.y = (cur_punch.y - old_punch.y) * 2.0f * config::rcs::strength;
                        sdk::Vector2 angles = sdk::read_value<sdk::Vector2>(reinterpret_cast<uintptr_t>(va));
                        angles.x -= delta.x;
                        angles.y -= delta.y;
                        angles.x = sdk::clamp_pitch(angles.x);
                        angles.y = sdk::normalize_yaw(angles.y);

                        if (!aim_adjusted) sdk::write_memory(reinterpret_cast<uintptr_t>(va), angles);
                        old_punch = cur_punch;
                    }
                    else {
                        old_punch = { 0.0f, 0.0f };
                    }
                }
            }
            catch (...) {}

        } while (false);

    }

    static void AimbotLoop() {
        while (s_aimbot_running.load()) {
            RunNormalAimTick();
            Sleep(4);
        }
    }
    void StartAimbotThread() {
        std::lock_guard<std::mutex> lock(s_aimbot_lifecycle);
        if (s_aimbot_running.load()) return;
        s_aimbot_running.store(true);
        try { s_aimbot_thread = std::thread(AimbotLoop); } catch (...) { s_aimbot_running = false; throw; }
        debug_console::Console::Get().Success("[Aimbot] Normal aim thread launched");
    }

    void StopAimbotThread() {
        std::lock_guard<std::mutex> lock(s_aimbot_lifecycle);
        s_aimbot_running.store(false);
        if (s_aimbot_thread.joinable()) s_aimbot_thread.join();
        debug_console::Console::Get().Info("[Aimbot] Normal aim thread joined");
    }

    // ======================== ESP & DRAWING FUNCTIONS ========================
    bool WorldToScreen(const sdk::Vector3& world, sdk::Vector2& screen, const sdk::ViewMatrix& matrix, int screen_width, int screen_height) {
        float w = matrix.matrix[3][0] * world.x + matrix.matrix[3][1] * world.y +
            matrix.matrix[3][2] * world.z + matrix.matrix[3][3];
        if (!sdk::finite(world) || screen_width <= 0 || screen_height <= 0 || !std::isfinite(w) || w < 0.001f) return false;
        float x = matrix.matrix[0][0] * world.x + matrix.matrix[0][1] * world.y +
            matrix.matrix[0][2] * world.z + matrix.matrix[0][3];
        float y = matrix.matrix[1][0] * world.x + matrix.matrix[1][1] * world.y +
            matrix.matrix[1][2] * world.z + matrix.matrix[1][3];
        x /= w; y /= w; if (!std::isfinite(x) || !std::isfinite(y)) return false;
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
        // One shadow pass instead of eight full copies of every glyph.
        draw_list->AddText(ImVec2(pos.x + 1, pos.y + 1), outline_col, text);
        draw_list->AddText(ImVec2(pos.x, pos.y), col, text);
    }

    void DrawFilledRect(const sdk::Vector2& pos, const sdk::Vector2& size, const float color[4]) {
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(color[0], color[1], color[2], color[3]));
        draw_list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + size.y), col);
    }

    void DrawHealthBar(const sdk::Vector2& top, const sdk::Vector2& bottom, int health, int max_health) {
        if (max_health <= 0) return;
        health = std::clamp(health, 0, max_health);
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

    struct EspBones {
        struct Bone { sdk::Vector3 position; unsigned char padding[20]; };
        static_assert(sizeof(Bone) == 32);
        Bone bones[sdk::BONE_RIGHT_FOOT + 1]{};
    };
    static bool ReadEspBones(uintptr_t scene, EspBones& bones) {
        const auto array = sdk::read_value<uintptr_t>(scene + sdk::off::CSkeletonInstance::m_modelState + 0x80);
        // One bounded read includes every skeleton joint, including the head.
        return array && sdk::read_memory(array, bones);
    }
    static void DrawEspSkeleton(const EspBones& bones, const sdk::ViewMatrix& view_matrix, int screen_width, int screen_height, const float color[4]) {
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
        sdk::Vector2 projected[sdk::BONE_RIGHT_FOOT + 1]{};
        bool visited[sdk::BONE_RIGHT_FOOT + 1]{}, valid[sdk::BONE_RIGHT_FOOT + 1]{};
        auto* draw = ImGui::GetBackgroundDrawList();
        const auto packed = ImGui::ColorConvertFloat4ToU32({color[0],color[1],color[2],color[3]});
        for (const auto& connection : bone_connections) {
            for (int joint : connection) {
                if (visited[joint]) continue;
                visited[joint] = true;
                const auto& position = bones.bones[joint].position;
                valid[joint] = (position.x != 0 || position.y != 0 || position.z != 0) &&
                    WorldToScreen(position, projected[joint], view_matrix, screen_width, screen_height);
            }
            if (valid[connection[0]] && valid[connection[1]]) {
                const auto& a = projected[connection[0]]; const auto& b = projected[connection[1]];
                draw->AddLine({a.x,a.y},{b.x,b.y},packed,2.0f);
            }
        }
    }
    void DrawSkeleton(sdk::C_CSPlayerPawn* player, const sdk::ViewMatrix& view_matrix, int screen_width, int screen_height, const float color[4]) {
        if (!player) return;
        const auto scene = sdk::read_value<uintptr_t>(reinterpret_cast<uintptr_t>(player) + sdk::off::C_BaseEntity::m_pGameSceneNode);
        EspBones bones{};
        if (scene && ReadEspBones(scene,bones)) DrawEspSkeleton(bones,view_matrix,screen_width,screen_height,color);
    }

    // ======================== ENTITY LIST MANAGEMENT ========================
    std::vector<sdk::C_CSPlayerPawn*> GetPlayerList() {
        playerPawns.clear();
        std::vector<sdk::C_CSPlayerPawn*> players;
        player_names.clear();
        if (!game_state::IsInGame()) return players;

		uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");

		uintptr_t entity_list = game_state::GetEntityList();
        if (!entity_list || !sdk::is_valid_ptr(entity_list)) return players;

        try {
            for (int i = 1; i <= 64; i++) {
                uintptr_t entityList1 = entity_list;
                if (!entityList1 || !sdk::is_valid_ptr(entityList1)) continue;
                uintptr_t playerController = sdk::entity_at(entity_list, i);
                if (!playerController) continue;
                uint32_t playerPawn = sdk::read_value<uint32_t>(playerController + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
                if (!playerPawn) continue;
                uintptr_t entityList2 = entity_list;
                if (!entityList2) continue;
                uintptr_t pCSPlayerPawn = sdk::entity_from_handle(entity_list, playerPawn);
                if (!pCSPlayerPawn) continue;
                int health = sdk::read_value<int>(pCSPlayerPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);
                if (health <= 0 || reinterpret_cast<sdk::C_CSPlayerPawn*>(pCSPlayerPawn)->IsDormant()) continue;
                playerPawns.push_back(pCSPlayerPawn);
                char name[128]{};
                if (sdk::read_memory(playerController + sdk::off::CBasePlayerController::m_iszPlayerName, name)) {
                    name[127] = '\0'; player_names[pCSPlayerPawn] = name;
                }
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


    struct EspNameCache {
        uintptr_t controller = 0;
        uint32_t handle = UINT32_MAX;
        ULONGLONG expires = 0;
        char name[128]{};
    };
    static EspNameCache esp_names[65]{};
    static uintptr_t esp_name_system = 0, esp_name_local = 0;

    void RenderESP() {
        if (!config::esp::enabled) return;
        const auto state = game_state::GetSnapshot();
        if (!state.in_game || !state.pawn || !state.entity_list) {
            std::fill(std::begin(esp_names),std::end(esp_names),EspNameCache{});
            esp_name_system = esp_name_local = 0;
            return;
        }
        if (esp_name_system != state.entity_list || esp_name_local != state.pawn) {
            std::fill(std::begin(esp_names),std::end(esp_names),EspNameCache{});
            esp_name_system = state.entity_list; esp_name_local = state.pawn;
        }
        if (!config::esp::box && !config::esp::health_bar && !config::esp::name &&
            !config::esp::skeleton && !config::esp::distance && !config::esp::snaplines) return;
        const auto client = reinterpret_cast<uintptr_t>(GetModuleHandleA("client.dll"));
        if (!client || !sdk::read_memory(client + cs2_dumper::offsets::client_dll::dwViewMatrix, g_view_matrix)) return;
        UpdateScreenDimensions();
        uint8_t local_team = 0;
        sdk::Vector3 local_origin{};
        if (!sdk::read_memory(state.pawn + sdk::off::C_BaseEntity::m_iTeamNum,local_team) ||
            !sdk::read_memory(state.pawn + sdk::off::C_BasePlayerPawn::m_vOldOrigin,local_origin) ||
            !sdk::finite(local_origin)) return;
        const float max_distance = config::esp::max_distance / 0.0254f;
        if (!std::isfinite(max_distance) || max_distance < 0) return;
        const float max_distance_squared = max_distance * max_distance;
        const auto now = GetTickCount64();

        // Resolve current handles every frame: only names are cached. Position,
        // health, dormancy and serial checks never use stale player snapshots.
        for (int index = 1; index <= 64; ++index) {
            const auto controller = sdk::entity_at(state.entity_list,index);
            auto& cached_name = esp_names[index];
            if (!controller) { cached_name = {}; continue; }
            uint32_t handle = UINT32_MAX;
            if (!sdk::read_memory(controller + sdk::off::CCSPlayerController::m_hPlayerPawn,handle)) continue;
            const auto player = sdk::entity_from_handle(state.entity_list,handle);
            if (!player || player == state.pawn) continue;
            int health = 0; uint8_t team = 0;
            if (!sdk::read_memory(player + sdk::off::C_BaseEntity::m_iHealth,health) || health <= 0 ||
                !sdk::read_memory(player + sdk::off::C_BaseEntity::m_iTeamNum,team)) continue;
            if (config::esp::team_check && team == local_team) continue;
            sdk::Vector3 origin{};
            if (!sdk::read_memory(player + sdk::off::C_BasePlayerPawn::m_vOldOrigin,origin) || !sdk::finite(origin)) continue;
            const auto delta = local_origin - origin;
            const float distance_squared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
            if (!std::isfinite(distance_squared) || distance_squared > max_distance_squared) continue;
            const auto scene = sdk::read_value<uintptr_t>(player + sdk::off::C_BaseEntity::m_pGameSceneNode);
            bool dormant = true;
            if (!scene || !sdk::read_memory(scene + sdk::off::CGameSceneNode::m_bDormant,dormant) || dormant) continue;

            EspBones bones{};
            bool has_bones = false;
            sdk::Vector3 bone_head{};
            if (config::esp::skeleton) {
                has_bones = ReadEspBones(scene,bones);
                if (has_bones) bone_head = bones.bones[sdk::BONE_HEAD].position;
            } else {
                const auto array = sdk::read_value<uintptr_t>(scene + sdk::off::CSkeletonInstance::m_modelState + 0x80);
                if (array) sdk::read_memory(array + sdk::BONE_HEAD * 32,bone_head);
            }
            sdk::Vector3 head = origin + sdk::Vector3{0,0,65};
            if (sdk::finite(bone_head) && (bone_head.x != 0 || bone_head.y != 0 || bone_head.z != 0))
                head = bone_head + sdk::Vector3{0,0,5};
            sdk::Vector2 screen_pos{}, screen_head{};
            if (!WorldToScreen(origin,screen_pos,g_view_matrix,g_screen_width,g_screen_height) ||
                !WorldToScreen(head,screen_head,g_view_matrix,g_screen_width,g_screen_height)) continue;
            const float height = screen_pos.y - screen_head.y;
            const float width = height * 0.5f;
            if (!std::isfinite(height) || height <= 0) continue;
            // Skip fully off-screen boxes, including margin for labels/limbs.
            if (screen_head.x + width + 128 < 0 || screen_head.x - width - 128 > g_screen_width ||
                screen_pos.y + 32 < 0 || screen_head.y - 32 > g_screen_height) continue;
            const float* color = team == local_team ? config::esp::team_color : config::esp::box_color;

            if (config::esp::name) {
                if (cached_name.controller != controller || cached_name.handle != handle || now >= cached_name.expires) {
                    cached_name = {}; cached_name.controller = controller; cached_name.handle = handle;
                    cached_name.expires = now + 250;
                    sdk::read_memory(controller + sdk::off::CBasePlayerController::m_iszPlayerName,cached_name.name);
                    cached_name.name[127] = '\0';
                }
                if (cached_name.name[0]) DrawText({screen_head.x,screen_head.y-16},cached_name.name,color);
            }
            if (config::esp::box) DrawBox(screen_head,screen_pos,width,color);
            if (config::esp::health_bar) DrawHealthBar({screen_head.x-width/2,screen_head.y},{screen_pos.x-width/2,screen_pos.y},health,100);
            if (config::esp::skeleton && has_bones) DrawEspSkeleton(bones,g_view_matrix,g_screen_width,g_screen_height,config::esp::skeleton_color);
            if (config::esp::distance) {
                char distance_text[32];
                sprintf_s(distance_text,"%.0fm",std::sqrt(distance_squared)*0.0254f);
                DrawText({screen_pos.x-15,screen_pos.y+5},distance_text,color);
            }
            if (config::esp::snaplines) DrawLine({g_screen_width/2.0f,static_cast<float>(g_screen_height)},screen_pos,color,1.0f);
        }
    }

    // ======================== MISC FEATURES ========================
    void ReleaseInputs() {
        if (s_mouse_down) {
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            s_mouse_down = false;
        }
        if (s_jump_address) {
            sdk::write_memory(s_jump_address, uint32_t{256});
            s_jump_address = 0;
        }
    }

    void BunnyHop() {
        const uintptr_t pawn = game_state::GetLocalPawnRaw();
        const bool active = config::misc::bunny_hop && !s_input_blocked && game_state::IsInGame() && pawn &&
            (GetAsyncKeyState(VK_SPACE) & 0x8000) && sdk::read_value<int>(pawn + sdk::off::C_BaseEntity::m_iHealth) > 0;
        if (!active) {
            if (s_jump_address) sdk::write_memory(s_jump_address, uint32_t{256});
            s_jump_address = 0;
            return;
        }
        const uintptr_t client = reinterpret_cast<uintptr_t>(GetModuleHandleA("client.dll"));
        if (!client) return;
        s_jump_address = client + cs2_dumper::buttons::jump;
        const bool grounded = (sdk::read_value<uint32_t>(pawn + sdk::off::C_BaseEntity::m_fFlags) & 1u) != 0;
        sdk::write_memory(s_jump_address, grounded ? uint32_t{65537} : uint32_t{256});
    }

    void NoFlash() {
        if (!config::misc::no_flash || !game_state::IsInGame()) return;
        const uintptr_t pawn = game_state::GetLocalPawnRaw();
        if (pawn) sdk::write_memory(pawn + sdk::off::C_CSPlayerPawnBase::m_flFlashDuration, 0.0f);
    }

    void TriggerBot() {
        static uintptr_t tracked_target = 0;
        static ULONGLONG acquired_at = 0;
        const ULONGLONG now = GetTickCount64();
        if (s_mouse_down) {
            if (now - s_mouse_down_time >= 10 || s_input_blocked || !game_state::IsInGame()) {
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                s_mouse_down = false;
                tracked_target = 0;
            }
            return;
        }
        const bool auto_shoot = config::aimbot::enabled && config::aimbot::auto_shoot && !(GetAsyncKeyState(config::aimbot::pause_key) & 0x8000);
        const bool manual_trigger = config::misc::trigger_bot &&
            (config::misc::trigger_key == 0 || (GetAsyncKeyState(config::misc::trigger_key) & 0x8000));
        if ((!manual_trigger && !auto_shoot) || s_input_blocked || !game_state::IsInGame() ||
            (GetAsyncKeyState(VK_LBUTTON) & 0x8000)) { tracked_target = 0; return; }
        const uintptr_t pawn = game_state::GetLocalPawnRaw();
        if (!pawn || sdk::read_value<int>(pawn + sdk::off::C_BaseEntity::m_iHealth) <= 0) { tracked_target = 0; return; }
        const int index = sdk::read_value<int>(pawn + sdk::off::C_CSPlayerPawn::m_iIDEntIndex);
        const uintptr_t target = index > 0 ? sdk::entity_at(game_state::GetEntityList(), index) : 0;
        if (!target || target == pawn || sdk::read_value<int>(target + sdk::off::C_BaseEntity::m_iHealth) <= 0 ||
            sdk::read_value<uint8_t>(target + sdk::off::C_BaseEntity::m_iTeamNum) ==
            sdk::read_value<uint8_t>(pawn + sdk::off::C_BaseEntity::m_iTeamNum)) { tracked_target = 0; return; }
        if (target != tracked_target) { tracked_target = target; acquired_at = now; }
        const auto delay = static_cast<ULONGLONG>((std::isfinite(config::misc::trigger_delay) ? (std::clamp)(config::misc::trigger_delay, 0.0f, 10.0f) : 0.0f) * 1000.0f);
        if (now - acquired_at < delay) return;
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        s_mouse_down = true;
        s_mouse_down_time = now;
    }

    void RadarHack() {
        if (!config::misc::radar_hack || !game_state::IsInGame()) return;
        const uintptr_t local = game_state::GetLocalPawnRaw();
        if (!local) return;
        const auto team = sdk::read_value<uint8_t>(local + sdk::off::C_BaseEntity::m_iTeamNum);
        // Build a fresh list even when ESP is disabled.
        for (auto* player : GetPlayerList()) {
            if (reinterpret_cast<uintptr_t>(player) == local || player->GetTeam() == team) continue;
            sdk::write_memory(reinterpret_cast<uintptr_t>(player) + sdk::off::C_CSPlayerPawn::m_entitySpottedState +
                sdk::off::EntitySpottedState_t::m_bSpotted, true);
        }
    }
}
