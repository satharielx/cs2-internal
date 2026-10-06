#include "entity_events.h"
#include "engine_trace.h"
#include "aim_profiler.h"
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
    static bool s_scope_down = false;
    static ULONGLONG s_scope_at = 0, s_scope_next = 0;
    static ULONGLONG s_mouse_down_time = 0;
    static uintptr_t s_jump_address = 0;
    static uintptr_t s_aim_fire_target = 0;
    static ULONGLONG s_aim_fire_until = 0;
    static void PublishFireTarget(uintptr_t pawn) { s_aim_fire_target=pawn; s_aim_fire_until=GetTickCount64()+100; }
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
        aim_profiler::Scope profile(aim_profiler::BoneRead);
        return pawn ? reinterpret_cast<sdk::C_CSPlayerPawn*>(pawn)->GetBonePosition(bone_index) : sdk::Vector3{};
    }
    static sdk::Vector3 GetEyePositionRaw(uintptr_t pawn) {
        aim_profiler::Scope profile(aim_profiler::EyeRead);
        return pawn ? reinterpret_cast<sdk::C_CSPlayerPawn*>(pawn)->GetEyePosition() : sdk::Vector3{};
    }
    static bool IsSpottedBy(uintptr_t pawn, int local_index) {
        uint32_t mask[2]{};
        return sdk::read_memory(pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState +
            cs2_dumper::schemas::client_dll::EntitySpottedState_t::m_bSpottedByMask, mask) && sdk::spotted_by(mask, local_index);
    }

    static bool AimDisabled(uintptr_t local) {
        return s_input_blocked ||
            (config::aimbot::disable_flashed && sdk::read_value<float>(local + sdk::off::C_CSPlayerPawnBase::m_flFlashDuration) > 0) ||
            (config::aimbot::disable_airborne && !(sdk::read_value<uint32_t>(local + sdk::off::C_BaseEntity::m_fFlags) & 1u)) ||
            (config::aimbot::disable_scoped && sdk::read_value<bool>(local + sdk::off::C_CSPlayerPawn::m_bIsScoped));
    }
    static sdk::Vector3 AimPoint(uintptr_t pawn, const sdk::Vector3& eye, const sdk::Vector2& view) {
        constexpr int bones[] = {sdk::BONE_HEAD, sdk::BONE_NECK, sdk::BONE_CHEST, sdk::BONE_PELVIS};
        sdk::Vector3 best{}; float bestFov = INFINITY;
        for (int i = 0; i < 4; ++i) {
            if (!config::aimbot::hitboxes[i]) continue;
            const auto point = GetBonePositionRaw(pawn, bones[i]);
            if (!sdk::finite(point) || point.Length() == 0) continue;
            const float fov = GetFov(view, CalcAngle(eye, point));
            if (fov < bestFov) { bestFov = fov; best = point; }
            if (!config::aimbot::hitscan) break;
        }
        return best;
    }
    struct AimSession {
        uintptr_t local = 0, system = 0, target = 0;
        uint32_t handle = UINT32_MAX;
        int kills = 0;
        ULONGLONG acquired = 0, kill_until = 0;
        void Begin(uintptr_t pawn, uintptr_t entities) {
            const auto controller = game_state::GetLocalController();
            const auto stats = controller ? sdk::read_value<uintptr_t>(controller + sdk::off::CCSPlayerController::m_pActionTrackingServices) : 0;
            const int current = stats ? sdk::read_value<int>(stats + sdk::off::CCSPlayerController_ActionTrackingServices::m_iNumRoundKills) : 0;
            if (local != pawn || system != entities) { *this = {}; local = pawn; system = entities; kills = current; }
            if (current > kills) kill_until = GetTickCount64() + Delay(config::aimbot::kill_delay);
            kills = current;
            if (target && (sdk::entity_from_handle(system, handle) != target ||
                sdk::read_value<int>(target + sdk::off::C_BaseEntity::m_iHealth) <= 0)) target = 0;
        }
        static ULONGLONG Delay(float seconds) {
            return static_cast<ULONGLONG>((std::isfinite(seconds) ? std::clamp(seconds, 0.0f, 10.0f) : 0.0f) * 1000);
        }
        bool Ready(uintptr_t selected, uint32_t identity) {
            if (selected != target) { target = selected; handle = identity; acquired = GetTickCount64(); }
            return selected && GetTickCount64() >= kill_until && GetTickCount64() - acquired >= Delay(config::aimbot::shot_delay);
        }
    };

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
            sdk::Vector3 headPos = GetBonePositionRaw(pawn, 6);
            float dist = localEye.Distance(headPos);
            if (dist > config::aimbot::max_distance) continue;
            sdk::Vector2 aimAngles2D = CalcAngle(localEye, headPos);
            sdk::QAngle aimAngles(aimAngles2D.x, aimAngles2D.y, 0.0f);
            sdk::QAngle currentAngles = cmd->csgoUserCmd.pBaseCmd->pViewAngles->angValue;
            float fov = GetFovA(currentAngles, aimAngles);
            if (fov < bestFov) {
                if (config::aimbot::visible_check &&
                    engine_trace::Visible(localEye, headPos, localPawn, pawn) != engine_trace::Result::Visible) continue;
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
        uint32_t handles[65]{};
        int local_index = -1;
    };
    static AimPlayers ReadAimPlayers(uintptr_t system, uintptr_t local, bool useEvents=false) {
        aim_profiler::Scope profile(aim_profiler::PlayerEnumeration);
        AimPlayers result{};
        unsigned controllers = 0, resolved = 0;
        std::array<entity_events::Entry,65> slots{};
        const bool cached=useEvents && entity_events::Snapshot(system,slots);
        if(useEvents && entity_events::enabled && !cached) return result;
        for (int i = 1; i <= 64; ++i) {
            const auto controller = cached ? slots[i].pointer : sdk::entity_at(system, i);
            if(cached && controller) {
                const auto identity=sdk::read_value<uintptr_t>(controller+0x10);
                uint32_t serial=UINT32_MAX;
                if(!identity || !sdk::read_memory(identity+0x10,serial) || serial!=slots[i].handle) continue;
            }
            if (!controller) continue;
            ++controllers;
            uint32_t handle = UINT32_MAX;
            if (!sdk::read_memory(controller + sdk::off::CCSPlayerController::m_hPlayerPawn, handle)) continue;
            const auto pawn = cached ? entity_events::Resolve(system,handle) : sdk::entity_from_handle(system, handle);
            // Enumeration is shared by normal and silent aim. Apply health and
            // optional team filtering in the callers, without duplicate reads.
            result.pawns[i] = pawn;
            result.handles[i] = handle;
            if (pawn) ++resolved;
            if (pawn == local) result.local_index = i;
        }
        aim_controllers = controllers;
        aim_resolved_pawns = resolved;
        return result;
    }
    struct SilentTargets {
        struct Target {
            uintptr_t pawn = 0; uint32_t handle = UINT32_MAX;
            int health = 0, team = 0; bool spotted = false;
            sdk::Vector3 head{}, points[4]{}; sdk::Vector2 angle{};
            float distance_squared = 0;
        };
        Target targets[65]{};
        int active_indices[64]{};
        int active_count = 0;
        uintptr_t system = 0, local = 0, weapon = 0;
        int local_index = -1;
    };
    static SilentTargets ReadSilentTargets(uintptr_t system, uintptr_t local, uintptr_t weapon) {
        aim_profiler::Scope profile(aim_profiler::Scan);
        SilentTargets cache{};
        cache.system=system; cache.local=local; cache.weapon=weapon;
        unsigned pointMask=0;
        for(int b=0;b<4;++b) if(config::aimbot::hitboxes[b]) pointMask |= 1u<<b;
        const auto players = ReadAimPlayers(system, local, true);
        cache.local_index = players.local_index;
        for (int i = 1; i <= 64; ++i) {
            auto& item = cache.targets[i];
            item.pawn = players.pawns[i];
            if (!item.pawn || item.pawn == local) continue;
            item.health = sdk::read_value<int>(item.pawn + sdk::off::C_BaseEntity::m_iHealth);
            if (item.health <= 0) continue;
            cache.active_indices[cache.active_count++] = i;
            item.handle = players.handles[i];
            item.team = sdk::read_value<uint8_t>(item.pawn + sdk::off::C_BaseEntity::m_iTeamNum);
            constexpr int bones[] = {sdk::BONE_HEAD, sdk::BONE_NECK, sdk::BONE_CHEST, sdk::BONE_PELVIS};
            for (int b = 0; b < 4; ++b) if(pointMask & (1u<<b)) item.points[b] = GetBonePositionRaw(item.pawn, bones[b]);
            item.head = item.points[0];
        }
        ++silent_target_scans;
        return cache;
    }
    static void PrepareSilentGeometry(SilentTargets& cache, const sdk::Vector3& eye) {
        aim_profiler::Scope profile(aim_profiler::Geometry);
        for (int n = 0; n < cache.active_count; ++n) {
            auto& target = cache.targets[cache.active_indices[n]];
            const auto delta = target.head - eye;
            target.distance_squared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
            target.angle = CalcAngle(eye, target.head);
        }
        ++silent_geometry_updates;
    }
    void RunSilentAimSubTick(DWORD* a1, sdk::C_CSPlayerPawn* localPawn)
    {
        aim_profiler::Scope profile(aim_profiler::Aim);

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
        if(entity_events::enabled) {
            std::array<entity_events::Entry,65> initial;
            if(!entity_events::Snapshot(entity_list,initial)) return;
        }
        const auto activeWeapon = entity_events::enabled ? entity_events::Resolve(entity_list,activeHandle) : sdk::entity_from_handle(entity_list, activeHandle);
        if (!activeWeapon) { silent_aim_status = "Active weapon unresolved"; return; }
        if (skins::IsKnife(skins::GetDefIndex(activeWeapon))) { silent_aim_status = "Knife equipped"; return; }
        int localHealth = sdk::read_value<int>(localPawnPtr + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);
        if (localHealth <= 0) { silent_aim_status = "Local pawn is dead"; return; }

        sdk::Vector3 localEye = GetEyePositionRaw(localPawnPtr);
        sample.data.eye = localEye;
        sample.data.eye_valid = sdk::finite(localEye);
        if (!sample.data.eye_valid) { silent_aim_status = "Eye position contains NaN/Inf"; return; }
        int localTeam = sdk::read_value<uint8_t>(localPawnPtr + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);

        if (AimDisabled(localPawnPtr)) { silent_aim_status = "Disabled by condition"; return; }
        auto players = ReadSilentTargets(entity_list, localPawnPtr, activeWeapon);
        PrepareSilentGeometry(players, localEye);
        bool collisionRejected = false;
    select_live_target:
        sample.data.alive = sample.data.enemies = sample.data.spotted = 0;
        sample.data.bones = sample.data.in_range = sample.data.in_fov = 0;
        sdk::C_CSPlayerPawn* bestTarget = nullptr;
        sdk::Vector3 bestHeadPos;
        const float maxFov = config::aimbot::fov;
        const float maxDistance = config::aimbot::max_distance;
        if (!std::isfinite(maxFov) || maxFov < 0 || !std::isfinite(maxDistance) || maxDistance < 0) return;
        const float maxFovSquared = maxFov * maxFov;
        const float maxDistanceSquared = maxDistance * maxDistance;
        float bestFovSquared = maxFovSquared;
        int bestControllerIndex = -1;

        for (int n = 0; n < players.active_count; ++n) {
            const int i = players.active_indices[n];
            auto& candidate = players.targets[i];
            uintptr_t pawn = candidate.pawn;
            if (!pawn || pawn == localPawnPtr) continue;

            int health = candidate.health;
            if (health <= 0) continue;
            ++sample.data.alive;

            int team = candidate.team;
            if (config::aimbot::team_check && team == localTeam) continue;
            ++sample.data.enemies;

            ++sample.data.spotted;

            sdk::Vector3 headPos = candidate.points[0];
            if (!config::aimbot::hitboxes[0] || config::aimbot::hitscan) {
                headPos = {}; float pointFov = INFINITY;
                for (int b = 0; b < 4; ++b) {
                    const auto point = candidate.points[b];
                    if (!config::aimbot::hitboxes[b] || !sdk::finite(point) || point.Length() == 0) continue;
                    const float fov = GetFov({sourceAngles.x, sourceAngles.y}, CalcAngle(localEye, point));
                    if (fov < pointFov) { pointFov = fov; headPos = point; }
                    if (!config::aimbot::hitscan) break;
                }
            }
            if(candidate.head.x != headPos.x || candidate.head.y != headPos.y || candidate.head.z != headPos.z) {
                candidate.head = headPos;
                candidate.angle = CalcAngle(localEye, headPos);
                const auto pointDelta = headPos - localEye;
                candidate.distance_squared = pointDelta.x * pointDelta.x + pointDelta.y * pointDelta.y + pointDelta.z * pointDelta.z;
            }



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
            PublishFireTarget(0);
            aim_profiler::Mark(aim_profiler::NoTarget);
            silent_aim_status = collisionRejected ? "No visible target within aim FOV" :
                !sample.data.alive ? "No living remote pawns resolved" :
                !sample.data.enemies ? "All living players rejected by team check" :
                !sample.data.bones ? "Enemy head positions unavailable" :
                !sample.data.in_range ? "Enemies outside maximum distance" : "Enemies outside aim FOV";
            return;
        }
        // Revalidate the selected
        // identity and health before writing, so a despawn/death is not reused.
        const auto& selected = players.targets[bestControllerIndex];
        bool validWinner;
        { aim_profiler::Scope validation(aim_profiler::WinnerValidation);
          validWinner=(entity_events::enabled ? entity_events::Resolve(entity_list,selected.handle) : sdk::entity_from_handle(entity_list, selected.handle))==selected.pawn &&
              sdk::read_value<int>(selected.pawn + sdk::off::C_BaseEntity::m_iHealth)>0; }
        if (!validWinner) {
            aim_profiler::Mark(aim_profiler::DeadWinner);
            // Remove this stale candidate for this pass and select the next one.
            // Each retry removes one slot, so the loop is bounded by 64 players.
            players.targets[bestControllerIndex].health = 0;
            goto select_live_target;
        }

        // Trace the closest candidate first, only after cheap filters and live
        // handle validation. A blocked candidate is discarded for this call.
        if (config::aimbot::visible_check) {
            aim_profiler::Scope traceProfile(aim_profiler::VisibilityTrace);
            const auto visibility = engine_trace::Visible(localEye, bestHeadPos, localPawnPtr, selected.pawn);
            if (visibility == engine_trace::Result::Unavailable || visibility == engine_trace::Result::Failed) {
                PublishFireTarget(0);
                silent_aim_status = engine_trace::status.load();
                return;
            }
            if (visibility == engine_trace::Result::Blocked) {
                collisionRejected = true;
                players.targets[bestControllerIndex].health = 0;
                goto select_live_target;
            }
        }

        PublishFireTarget(selected.pawn);
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
        static AimSession session;

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

                if (AimDisabled(localPawn)) { session = {}; normal_aim_status = "Disabled by condition"; continue; }
                int local_team = sdk::read_value<uint8_t>(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
                sdk::Vector3 local_eye = GetEyePositionRaw(localPawn);

				uintptr_t entity_list = game_state::GetEntityList();
                if (!entity_list || !sdk::is_valid_ptr(entity_list)) { normal_aim_status = "Entity list unavailable"; continue; }

                session.Begin(localPawn, entity_list);
                const auto players = ReadAimPlayers(entity_list, localPawn);
                localIndex = players.local_index;

                uintptr_t closest_pawn = 0;
                uint32_t closest_handle = UINT32_MAX;
                sdk::Vector3 closest_point{};
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
                    if (config::aimbot::lock_target && session.target && pawn != session.target) continue;
                    sdk::Vector3 head_pos = AimPoint(pawn, local_eye, current_angles);
                    if (head_pos.x == 0.0f && head_pos.y == 0.0f && head_pos.z == 0.0f) continue;
                    sdk::Vector2 target_angles = CalcAngle(local_eye, head_pos);
                    float fov = GetFov(current_angles, target_angles);
                    if (fov > config::aimbot::fov) continue;
                    float dist = local_eye.Distance(head_pos);
                    if (dist < closest_dist) {
                        closest_dist = dist;
                        closest_pawn = pawn;
                        closest_handle = players.handles[i];
                        closest_point = head_pos;
                    }
                }

                normal_aim_status = closest_pawn ? "Target selected; hold left mouse or enable Force shoot" :
                    (config::aimbot::visible_check && localIndex < 1 ? "Spotted check: local controller index unresolved" :
                        "No target passed team/spotted/bone/distance/FOV checks");
                const auto active_weapon = skins::GetActiveWeapon();
                if (closest_pawn && !active_weapon) normal_aim_status = "Active weapon unresolved";
                else if (closest_pawn && skins::IsKnife(skins::GetDefIndex(active_weapon))) normal_aim_status = "Knife equipped";
                const bool ready = session.Ready(closest_pawn, closest_handle);
                PublishFireTarget(ready ? closest_pawn : 0);
                bool aim_adjusted = false;
                if (config::aimbot::enabled && ready && closest_pawn && active_weapon && ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) || config::aimbot::auto_shoot) && !skins::IsKnife(skins::GetDefIndex(active_weapon))) {
                    sdk::Vector3 head_pos = closest_point;
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

    static float EspAlpha() { return std::isfinite(config::esp::alpha) ? std::clamp(config::esp::alpha, 0.0f, 1.0f) : 1.0f; }
    static float EspScale() { return std::isfinite(config::esp::scale) ? std::clamp(config::esp::scale, .5f, 2.0f) : 1.0f; }
    void DrawBox(const sdk::Vector2& top, const sdk::Vector2& bottom, float width, const float color[4]) {
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        float height = bottom.y - top.y;
        ImVec2 top_left(top.x - width / 2, top.y);
        ImVec2 bottom_right(top.x + width / 2, bottom.y);
        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(color[0], color[1], color[2], color[3] * EspAlpha()));
        draw_list->AddRect(top_left, bottom_right, col, 0.0f, 0, 2.0f);
    }

    void DrawLine(const sdk::Vector2& from, const sdk::Vector2& to, const float color[4], float thickness) {
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(color[0], color[1], color[2], color[3] * EspAlpha()));
        draw_list->AddLine(ImVec2(from.x, from.y), ImVec2(to.x, to.y), col, thickness);
    }

    void DrawText(const sdk::Vector2& pos, const char* text, const float color[4]) {
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(color[0], color[1], color[2], color[3] * EspAlpha()));
        ImU32 outline_col = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.8f * EspAlpha()));
        // One shadow pass instead of eight full copies of every glyph.
        draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize() * EspScale(), ImVec2(pos.x + 1, pos.y + 1), outline_col, text);
        draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize() * EspScale(), ImVec2(pos.x, pos.y), col, text);
    }

    void DrawFilledRect(const sdk::Vector2& pos, const sdk::Vector2& size, const float color[4]) {
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(color[0], color[1], color[2], color[3] * EspAlpha()));
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


    static bool CarriesBomb(uintptr_t services,uintptr_t system) {
        if(!services) return false;
        sdk::VectorView weapons;
        if(!sdk::read_vector(services+sdk::off::CPlayer_WeaponServices::m_hMyWeapons,weapons,64)) return false;
        for(int i=0;i<weapons.count;++i) {
            const auto handle=sdk::read_value<uint32_t>(weapons.data+i*sizeof(uint32_t));
            const auto weapon=sdk::entity_from_handle(system,handle);
            if(weapon && skins::GetDefIndex(weapon)==49) return true;
        }
        return false;
    }
    static uintptr_t ObserverServices(uintptr_t controller,uintptr_t fallback,uintptr_t system) {
        // Dead player pawns and dedicated observer pawns use different handles.
        const auto observer=sdk::entity_from_handle(system,sdk::read_value<uint32_t>(controller+sdk::off::CCSPlayerController::m_hObserverPawn));
        const auto active=sdk::entity_from_handle(system,sdk::read_value<uint32_t>(controller+sdk::off::CBasePlayerController::m_hPawn));
        const auto pawn=observer ? observer : active ? active : fallback;
        return pawn ? sdk::read_value<uintptr_t>(pawn+sdk::off::C_BasePlayerPawn::m_pObserverServices) : 0;
    }
    struct EspNameCache {
        uintptr_t controller = 0;
        uint32_t handle = UINT32_MAX;
        ULONGLONG expires = 0;
        char name[128]{};
    };
    static EspNameCache esp_names[65]{};
    static uintptr_t esp_name_system = 0, esp_name_local = 0;

    static bool ReadViewMatrix(uintptr_t client, sdk::ViewMatrix& matrix) {
        // Resolve the address once at initialization; read live camera values
        // for every render. The instruction uses LEA, so do not dereference a
        // pointer slot at the resolved address.
        const auto address = pattern_resolver::view_matrix
            ? reinterpret_cast<uintptr_t>(pattern_resolver::view_matrix)
            : client ? client + cs2_dumper::offsets::client_dll::dwViewMatrix : 0;
        return address && sdk::read_memory(address, matrix);
    }

    void RenderESP() {
        esp_diagnostics = {};
        esp_diagnostics.status = "Disabled";
        if (!config::esp::enabled) return;
        const auto state = game_state::GetSnapshot();
        if (!state.in_game || !state.pawn || !state.entity_list) {
            esp_diagnostics.status = "Game state, local pawn or entity system unavailable";
            std::fill(std::begin(esp_names),std::end(esp_names),EspNameCache{});
            esp_name_system = esp_name_local = 0;
            return;
        }
        if (esp_name_system != state.entity_list || esp_name_local != state.pawn) {
            std::fill(std::begin(esp_names),std::end(esp_names),EspNameCache{});
            esp_name_system = state.entity_list; esp_name_local = state.pawn;
        }

        const auto client = reinterpret_cast<uintptr_t>(GetModuleHandleA("client.dll"));
        esp_diagnostics.status = "View matrix unreadable";
        if (!ReadViewMatrix(client, g_view_matrix)) return;
        bool matrix_nonzero = false;
        for (const auto& row : g_view_matrix.matrix) for (float value : row) {
            if (!std::isfinite(value)) { esp_diagnostics.status = "View matrix contains invalid values"; return; }
            matrix_nonzero |= value != 0.f;
        }
        if (!matrix_nonzero) { esp_diagnostics.status = "View matrix is zero; check its offset"; return; }
        UpdateScreenDimensions();
        esp_diagnostics.status = "Local team or position unreadable";
        uint8_t local_team = 0;
        sdk::Vector3 local_origin{};
        if (!sdk::read_memory(state.pawn + sdk::off::C_BaseEntity::m_iTeamNum,local_team) ||
            !sdk::read_memory(state.pawn + sdk::off::C_BasePlayerPawn::m_vOldOrigin,local_origin) ||
            !sdk::finite(local_origin)) return;
        const float max_distance = config::esp::max_distance / 0.0254f;
        esp_diagnostics.status = "Invalid maximum distance";
        if (!std::isfinite(max_distance) || max_distance < 0) return;
        const float max_distance_squared = max_distance * max_distance;
        const auto now = GetTickCount64();
        esp_diagnostics.status = "No players passed filters; see counts below";

        // Resolve current handles every frame: only names are cached. Position,
        // health, dormancy and serial checks never use stale player snapshots.
        for (int index = 1; index <= 64; ++index) {
            const auto controller = sdk::entity_at(state.entity_list,index);
            auto& cached_name = esp_names[index];
            if (!controller) { cached_name = {}; continue; }
            ++esp_diagnostics.controllers;
            uint32_t handle = UINT32_MAX;
            if (!sdk::read_memory(controller + sdk::off::CCSPlayerController::m_hPlayerPawn,handle)) continue;
            const auto player = sdk::entity_from_handle(state.entity_list,handle);
            if (!player || player == state.pawn) continue;
            ++esp_diagnostics.pawns;
            int health = 0; uint8_t team = 0;
            if (!sdk::read_memory(player + sdk::off::C_BaseEntity::m_iHealth,health) || health <= 0 ||
                !sdk::read_memory(player + sdk::off::C_BaseEntity::m_iTeamNum,team)) continue;
            ++esp_diagnostics.alive;
            if (config::esp::team_check && team == local_team) continue;
            ++esp_diagnostics.team_pass;
            sdk::Vector3 origin{};
            if (!sdk::read_memory(player + sdk::off::C_BasePlayerPawn::m_vOldOrigin,origin) || !sdk::finite(origin)) continue;
            const auto delta = local_origin - origin;
            const float distance_squared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
            if (!std::isfinite(distance_squared) || distance_squared > max_distance_squared) continue;
            ++esp_diagnostics.range_pass;
            const auto scene = sdk::read_value<uintptr_t>(player + sdk::off::C_BaseEntity::m_pGameSceneNode);
            bool dormant = true;
            if (!scene || !sdk::read_memory(scene + sdk::off::CGameSceneNode::m_bDormant,dormant) || dormant) continue;
            ++esp_diagnostics.awake;

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
            const bool projected = WorldToScreen(origin,screen_pos,g_view_matrix,g_screen_width,g_screen_height) &&
                WorldToScreen(head,screen_head,g_view_matrix,g_screen_width,g_screen_height);
            const bool onscreen = projected && screen_pos.x >= 0 && screen_pos.x <= g_screen_width &&
                screen_pos.y >= 0 && screen_head.y <= g_screen_height;
            if (config::esp::out_of_fov_arrows && !onscreen) {
                sdk::Vector2 view{};
                if (sdk::read_memory(client + cs2_dumper::offsets::client_dll::dwViewAngles, view)) {
                    const float angle = (CalcAngle(local_origin, origin).y - view.y) * .01745329252f;
                    const ImVec2 center(g_screen_width * .5f, g_screen_height * .5f);
                    const float radius = (std::min)(g_screen_width, g_screen_height) * .36f;
                    const ImVec2 dir(std::sin(angle), -std::cos(angle)), side(-dir.y, dir.x);
                    const ImVec2 tip(center.x + dir.x * radius, center.y + dir.y * radius);
                    const float size = 12 * EspScale();
                    const auto c = team == local_team ? config::esp::team_color : config::esp::box_color;
                    ImGui::GetBackgroundDrawList()->AddTriangleFilled(tip,
                        {tip.x - dir.x * size + side.x * size * .5f, tip.y - dir.y * size + side.y * size * .5f},
                        {tip.x - dir.x * size - side.x * size * .5f, tip.y - dir.y * size - side.y * size * .5f},
                        ImGui::ColorConvertFloat4ToU32({c[0], c[1], c[2], c[3] * EspAlpha()}));
                }
            }
            if (!projected) continue;
            ++esp_diagnostics.projected;
            const float height = screen_pos.y - screen_head.y;
            const float width = height * 0.5f;
            if (!std::isfinite(height) || height <= 0) continue;
            // Skip fully off-screen boxes, including margin for labels/limbs.
            if (screen_head.x + width + 128 < 0 || screen_head.x - width - 128 > g_screen_width ||
                screen_pos.y + 32 < 0 || screen_head.y - 32 > g_screen_height) continue;
            ++esp_diagnostics.drawn;
            esp_diagnostics.status = "Players reached drawing; check enabled elements and alpha";
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
            if (config::esp::armor_bar) {
                const int armor = std::clamp(sdk::read_value<int>(player + sdk::off::C_CSPlayerPawn::m_ArmorValue), 0, 100);
                const float bg[] = {.05f,.05f,.05f,.8f}, ac[] = {.2f,.6f,1,1};
                const float bar = 3 * EspScale();
                DrawFilledRect({screen_pos.x-width*.5f,screen_pos.y+2}, {width,bar}, bg);
                DrawFilledRect({screen_pos.x-width*.5f,screen_pos.y+2}, {width*armor/100,bar}, ac);
            }
            float labelY = screen_head.y;
            const auto weaponServices = sdk::read_value<uintptr_t>(player + sdk::off::C_BasePlayerPawn::m_pWeaponServices);
            const auto weapon = weaponServices ? sdk::entity_from_handle(state.entity_list,
                sdk::read_value<uint32_t>(weaponServices + sdk::off::CPlayer_WeaponServices::m_hActiveWeapon)) : 0;
            auto label = [&](const char* text) { DrawText({screen_head.x+width*.5f+5,labelY},text,color); labelY += 15 * EspScale(); };
            if (config::esp::weapon_text && weapon) label(skins::GetWeaponName(skins::GetDefIndex(weapon)));
            if (config::esp::bomb && ((weapon && skins::GetDefIndex(weapon) == 49) || CarriesBomb(weaponServices,state.entity_list))) label("BOMB");
            const auto items = sdk::read_value<uintptr_t>(player + sdk::off::C_BasePlayerPawn::m_pItemServices);
            if (config::esp::defuse_kit && items && sdk::read_value<bool>(items + sdk::off::CCSPlayer_ItemServices::m_bHasDefuser)) label("KIT");
            if (config::esp::flashed && sdk::read_value<float>(player + sdk::off::C_CSPlayerPawnBase::m_flFlashDuration) > 0) label("FLASHED");
            if (config::esp::scoped && sdk::read_value<bool>(player + sdk::off::C_CSPlayerPawn::m_bIsScoped)) label("SCOPED");
            if (config::esp::defusing && sdk::read_value<bool>(player + sdk::off::C_CSPlayerPawn::m_bIsDefusing)) label("DEFUSING");
            if (config::esp::planting && weapon && skins::GetDefIndex(weapon) == 49 && sdk::read_value<bool>(weapon + sdk::off::C_C4::m_bStartedArming)) label("PLANTING");
            const auto hostages = sdk::read_value<uintptr_t>(player + sdk::off::C_CSPlayerPawn::m_pHostageServices);
            if (config::esp::hostage && (sdk::read_value<bool>(player + sdk::off::C_CSPlayerPawn::m_bIsGrabbingHostage) ||
                (hostages && sdk::entity_from_handle(state.entity_list, sdk::read_value<uint32_t>(hostages + sdk::off::CCSPlayer_HostageServices::m_hCarriedHostage))))) label("HOSTAGE");
            if (config::misc::show_money) {
                const auto money = sdk::read_value<uintptr_t>(controller + sdk::off::CCSPlayerController::m_pInGameMoneyServices);
                if (money) { char text[32]; sprintf_s(text,"$%d",sdk::read_value<int>(money + sdk::off::CCSPlayerController_InGameMoneyServices::m_iAccount)); label(text); }
            }
            if (config::esp::skeleton && has_bones) DrawEspSkeleton(bones,g_view_matrix,g_screen_width,g_screen_height,config::esp::skeleton_color);
            if (config::esp::distance) {
                char distance_text[32];
                sprintf_s(distance_text,"%.0fm",std::sqrt(distance_squared)*0.0254f);
                DrawText({screen_pos.x-15,screen_pos.y+5},distance_text,color);
            }
            if (config::esp::snaplines) DrawLine({g_screen_width/2.0f,static_cast<float>(g_screen_height)},screen_pos,color,1.0f);
        }
    }

    struct VisualField {
        uintptr_t system = 0, entity = 0; uint32_t handle = UINT32_MAX;
        uintptr_t offset = 0; size_t size = 0;
        unsigned char original[16]{}, applied[16]{};
        bool touched = false;
        bool Live() const { return sdk::entity_from_handle(system,handle) == entity; }
        void Restore() const {
            unsigned char current[16]{};
            // Restore only our own last value; leave subsequent engine updates intact.
            if (Live() && sdk::read_memory(entity+offset,current) && !std::memcmp(current,applied,size)) {
                SIZE_T written=0;
                ProfileWrite(GetCurrentProcess(),reinterpret_cast<void*>(entity+offset),original,size,&written);
            }
        }
    };
    static std::vector<VisualField> visual_fields;
    static void RestoreVisualFields() {
        for (const auto& field : visual_fields) field.Restore();
        visual_fields.clear();
    }
    template<class T> static void ApplyVisualField(uintptr_t system,uintptr_t entity,uint32_t handle,uintptr_t offset,const T& value) {
        static_assert(sizeof(T)<=16);
        auto field=std::find_if(visual_fields.begin(),visual_fields.end(),[&](const VisualField& f){return f.system==system && f.entity==entity && f.handle==handle && f.offset==offset;});
        if(field==visual_fields.end()) {
            VisualField entry; entry.system=system; entry.entity=entity; entry.handle=handle; entry.offset=offset; entry.size=sizeof(T);
            T original{};
            if(!sdk::read_memory(entity+offset,original)) return;
            std::memcpy(entry.original,&original,sizeof(T));
            visual_fields.push_back(entry); field=std::prev(visual_fields.end());
        }
        if(sdk::write_memory(entity+offset,value)) {std::memcpy(field->applied,&value,sizeof(T)); field->touched=true;}
    }
    static ImU32 VisualColor(const float color[4]) {
        ImVec4 value;
        float* channels=&value.x;
        for(int i=0;i<4;++i) channels[i]=std::isfinite(color[i]) ? std::clamp(color[i],0.0f,1.0f) : 1.0f;
        return ImGui::ColorConvertFloat4ToU32(value);
    }
    struct WorldEntity { uintptr_t entity=0; uint32_t handle=UINT32_MAX; char name[96]{}; };
    static std::vector<WorldEntity> world_entities;
    static uintptr_t world_system=0,world_local=0;
    static ULONGLONG next_world_scan=0;
    static void UpdateWorldVisuals(const game_state::Snapshot& state) {
        for(auto& field:visual_fields) field.touched=false;
        if(state.in_game && state.entity_list && state.pawn) {
            if(config::esp::glow) {
                const auto players=ReadAimPlayers(state.entity_list,state.pawn);
                const auto team=sdk::read_value<uint8_t>(state.pawn+sdk::off::C_BaseEntity::m_iTeamNum);
                for(int i=1;i<=64;++i) {
                    const auto pawn=players.pawns[i];
                    if(!pawn || pawn==state.pawn || sdk::read_value<int>(pawn+sdk::off::C_BaseEntity::m_iHealth)<=0) continue;
                    if(config::esp::team_check && sdk::read_value<uint8_t>(pawn+sdk::off::C_BaseEntity::m_iTeamNum)==team) continue;
                    const auto glow=sdk::off::C_BaseModelEntity::m_Glow;
                    ApplyVisualField(state.entity_list,pawn,players.handles[i],glow+sdk::off::CGlowProperty::m_iGlowType,int{3});
                    ApplyVisualField(state.entity_list,pawn,players.handles[i],glow+sdk::off::CGlowProperty::m_iGlowTeam,int{0});
                    ApplyVisualField(state.entity_list,pawn,players.handles[i],glow+sdk::off::CGlowProperty::m_glowColorOverride,static_cast<uint32_t>(VisualColor(config::esp::glow_color)));
                    ApplyVisualField(state.entity_list,pawn,players.handles[i],glow+sdk::off::CGlowProperty::m_bGlowing,true);
                    ApplyVisualField(state.entity_list,pawn,players.handles[i],glow+sdk::off::CGlowProperty::m_bEligibleForScreenHighlight,true);
                }
            }
            const bool scan=config::world::items || config::world::bomb || config::world::grenade_warning || config::world::smoke_color_enabled;
            if(world_system!=state.entity_list || world_local!=state.pawn) { world_entities.clear(); next_world_scan=0; world_system=state.entity_list; world_local=state.pawn; }
            if(scan && GetTickCount64()>=next_world_scan) {
                next_world_scan=GetTickCount64()+100;
                world_entities.clear();
                const int highest=std::clamp(sdk::read_value<int>(state.entity_list+cs2_dumper::offsets::client_dll::dwGameEntitySystem_highestEntityIndex),0,0x7FFE);
                for(int i=65;i<=highest;++i) {
                    const auto entity=sdk::entity_at(state.entity_list,i);
                    const auto identity=entity ? sdk::read_value<uintptr_t>(entity+0x10) : 0;
                    const auto name=identity ? sdk::read_value<uintptr_t>(identity+sdk::off::CEntityIdentity::m_designerName) : 0;
                    WorldEntity entry; entry.entity=entity;
                    if(!name || !sdk::read_memory(identity+0x10,entry.handle) ||
                        !sdk::read_memory(name,entry.name) || sdk::entity_from_handle(state.entity_list,entry.handle)!=entity) continue;
                    entry.name[95]=0;
                    if(!std::strncmp(entry.name,"weapon_",7) || std::strstr(entry.name,"projectile") ||
                        !std::strcmp(entry.name,"planted_c4") || !std::strcmp(entry.name,"item_defuser")) world_entities.push_back(entry);
                }
            }
            if(!scan) world_entities.clear();
            for(const auto& item:world_entities) {
                if(sdk::entity_from_handle(state.entity_list,item.handle)!=item.entity) continue;
                if(config::world::smoke_color_enabled && !std::strcmp(item.name,"smokegrenade_projectile")) {
                    sdk::Vector3 color{config::world::smoke_color[0],config::world::smoke_color[1],config::world::smoke_color[2]};
                    if(sdk::finite(color)) ApplyVisualField(state.entity_list,item.entity,item.handle,sdk::off::C_SmokeGrenadeProjectile::m_vSmokeColor,
                        sdk::Vector3(std::clamp(color.x,0.0f,1.0f)*255,std::clamp(color.y,0.0f,1.0f)*255,std::clamp(color.z,0.0f,1.0f)*255));
                }
            }
        } else { world_entities.clear(); world_system=world_local=0; }
        for(auto it=visual_fields.begin();it!=visual_fields.end();) {
            if(!it->touched) { it->Restore(); it=visual_fields.erase(it); } else ++it;
        }
    }
    static void RenderWorldEntities(const game_state::Snapshot& state,const sdk::Vector3& localOrigin,uintptr_t client) {
        if(world_entities.empty() || !sdk::finite(localOrigin)) return;
        sdk::ViewMatrix matrix{};
        if(!ReadViewMatrix(client,matrix)) return;
        const float color[]={1,.85f,.3f,1};
        for(const auto& item:world_entities) {
            if(sdk::entity_from_handle(state.entity_list,item.handle)!=item.entity) continue;
            const bool bomb=!std::strcmp(item.name,"planted_c4");
            const bool projectile=std::strstr(item.name,"projectile")!=nullptr;
            if((bomb && !config::world::bomb) || (projectile && !config::world::grenade_warning) || (!bomb && !projectile && !config::world::items)) continue;
            if(!bomb && !projectile && sdk::entity_from_handle(state.entity_list,sdk::read_value<uint32_t>(item.entity+sdk::off::C_BaseEntity::m_hOwnerEntity))) continue;
            if(bomb && (!sdk::read_value<bool>(item.entity+sdk::off::C_PlantedC4::m_bBombTicking) || sdk::read_value<bool>(item.entity+sdk::off::C_PlantedC4::m_bBombDefused))) continue;
            const auto origin=reinterpret_cast<sdk::C_BaseEntity*>(item.entity)->GetOrigin();
            const float distance=localOrigin.Distance(origin)*.0254f;
            if(!sdk::finite(origin) || !std::isfinite(distance) || distance>config::esp::max_distance) continue;
            sdk::Vector2 screen{};
            if(!WorldToScreen(origin,screen,matrix,g_screen_width,g_screen_height)) continue;
            char text[160];
            const char* name=bomb ? "PLANTED BOMB" : !std::strncmp(item.name,"weapon_",7) ? item.name+7 : item.name;
            sprintf_s(text,"%s %.0fm",name,distance);
            DrawText(screen,text,color);
            if(projectile) ImGui::GetBackgroundDrawList()->AddCircle({screen.x,screen.y-8},9,IM_COL32(255,90,60,220),24,2);
        }
    }

    void RenderOverlays() {
        if (!ImGui::GetCurrentContext()) return;
        const auto state = game_state::GetSnapshot();
        sdk::ScopedLocalReads reads;
        UpdateWorldVisuals(state);
        if (!state.in_game || !state.pawn || !state.entity_list) return;
        UpdateScreenDimensions();
        const auto draw = ImGui::GetBackgroundDrawList();
        const ImVec2 center(g_screen_width*.5f,g_screen_height*.5f);
        const auto client = reinterpret_cast<uintptr_t>(GetModuleHandleA("client.dll"));
        const auto camera = sdk::read_value<uintptr_t>(state.pawn + sdk::off::C_BasePlayerPawn::m_pCameraServices);
        float cameraFov = camera ? static_cast<float>(sdk::read_value<uint32_t>(camera + sdk::off::CCSPlayerBase_CameraServices::m_iFOV)) : 90.0f;
        if (config::viewmodel::camera_fov_enabled && !sdk::read_value<bool>(state.pawn + sdk::off::C_CSPlayerPawn::m_bIsScoped)) cameraFov = config::viewmodel::camera_fov;
        if (!std::isfinite(cameraFov) || cameraFov < 1 || cameraFov > 179) cameraFov = 90;
        if (config::aimbot::draw_fov && std::isfinite(config::aimbot::fov)) {
            const float radius = std::tan(std::clamp(config::aimbot::fov,0.0f,89.0f)*.01745329252f) /
                std::tan(cameraFov*.00872664626f) * g_screen_width*.5f;
            draw->AddCircle(center,radius,ImGui::ColorConvertFloat4ToU32({config::aimbot::fov_color[0],config::aimbot::fov_color[1],config::aimbot::fov_color[2],config::aimbot::fov_color[3]}),128);
        }
        static uintptr_t previousPawn = 0, previousController = 0;
        static int hits = 0, kills = 0;
        static ULONGLONG hitAt = 0, killAt = 0;
        const auto bullet = sdk::read_value<uintptr_t>(state.pawn + sdk::off::C_CSPlayerPawn::m_pBulletServices);
        const auto stats = state.controller ? sdk::read_value<uintptr_t>(state.controller + sdk::off::CCSPlayerController::m_pActionTrackingServices) : 0;
        const int currentHits = bullet ? sdk::read_value<int>(bullet + sdk::off::CCSPlayer_BulletServices::m_totalHitsOnServer) : 0;
        const int currentKills = stats ? sdk::read_value<int>(stats + sdk::off::CCSPlayerController_ActionTrackingServices::m_iNumRoundKills) : 0;
        const auto now = GetTickCount64();
        if (previousPawn != state.pawn || previousController != state.controller) { hits = currentHits; kills = currentKills; hitAt = killAt = 0; }
        if (currentHits > hits) hitAt = now;
        if (currentKills > kills) killAt = now;
        previousPawn = state.pawn; previousController = state.controller; hits = currentHits; kills = currentKills;
        const bool killed = killAt && now-killAt < 500;
        const bool hit = hitAt && now-hitAt < 350;
        if (config::misc::hit_marker && (hit || killed)) {
            const ImU32 color = killed ? IM_COL32(255,80,80,230) : IM_COL32(255,255,255,230);
            for (int x : {-1,1}) for (int y : {-1,1}) draw->AddLine({center.x+x*5.0f,center.y+y*5.0f},{center.x+x*12.0f,center.y+y*12.0f},color,2);
        }
        if ((config::misc::hit_effect && hit) || (config::misc::kill_effect && killed)) {
            const float life = killed ? (now-killAt)/500.0f : (now-hitAt)/350.0f;
            draw->AddCircle(center,18+life*45,IM_COL32(255,killed?70:255,killed?70:255,static_cast<int>((1-life)*180)),64,2);
        }
        const auto localOrigin = sdk::read_value<sdk::Vector3>(state.pawn + sdk::off::C_BasePlayerPawn::m_vOldOrigin);
        RenderWorldEntities(state,localOrigin,client);
        sdk::Vector2 view{};
        const bool hasView = client && sdk::read_memory(client + cs2_dumper::offsets::client_dll::dwViewAngles,view);
        if (config::misc::external_radar && hasView && sdk::finite(localOrigin)) {
            const float size = 200, radius = size*.5f;
            const ImVec2 radar(25+radius,80+radius);
            const float alpha = std::isfinite(config::misc::radar_alpha) ? std::clamp(config::misc::radar_alpha,0.0f,1.0f) : .8f;
            draw->AddRectFilled({radar.x-radius,radar.y-radius},{radar.x+radius,radar.y+radius},IM_COL32(15,18,25,static_cast<int>(alpha*220)),8);
            draw->AddLine({radar.x-radius,radar.y},{radar.x+radius,radar.y},IM_COL32(90,100,120,static_cast<int>(alpha*160)));
            draw->AddLine({radar.x,radar.y-radius},{radar.x,radar.y+radius},IM_COL32(90,100,120,static_cast<int>(alpha*160)));
            const float yaw = view.y*.01745329252f;
            const float scale = std::isfinite(config::misc::radar_scale) ? std::clamp(config::misc::radar_scale,.25f,4.0f)*.04f : .04f;
            const auto localTeam = sdk::read_value<uint8_t>(state.pawn + sdk::off::C_BaseEntity::m_iTeamNum);
            for (auto* entity : GetPlayerList()) {
                const auto pawn = reinterpret_cast<uintptr_t>(entity);
                if (pawn == state.pawn || !entity->IsAlive() || entity->IsDormant()) continue;
                const auto delta = entity->GetOrigin()-localOrigin;
                if (!sdk::finite(delta)) continue;
                const float x = (std::sin(yaw)*delta.x-std::cos(yaw)*delta.y)*scale;
                const float y = -(std::cos(yaw)*delta.x+std::sin(yaw)*delta.y)*scale;
                const float clamp = (std::max)(1.0f,(std::max)(std::abs(x),std::abs(y))/(radius-6));
                draw->AddCircleFilled({radar.x+x/clamp,radar.y+y/clamp},4,entity->GetTeam()==localTeam ? IM_COL32(80,170,255,static_cast<int>(alpha*255)) : IM_COL32(255,90,90,static_cast<int>(alpha*255)));
            }
            draw->AddTriangleFilled({radar.x,radar.y-5},{radar.x-4,radar.y+4},{radar.x+4,radar.y+4},IM_COL32(255,255,255,static_cast<int>(alpha*255)));
        }
        if (config::misc::spectator_list) {
            // When spectating, list viewers of the same observed pawn.
            uintptr_t observed = state.pawn;
            const auto localObserver = ObserverServices(state.controller,state.pawn,state.entity_list);
            if (localObserver && sdk::read_value<int>(state.pawn + sdk::off::C_BaseEntity::m_iHealth) <= 0) {
                const auto target = sdk::entity_from_handle(state.entity_list,sdk::read_value<uint32_t>(localObserver+sdk::off::CPlayer_ObserverServices::m_hObserverTarget));
                if (target) observed = target;
            }
            ImGui::SetNextWindowSize({220,0},ImGuiCond_FirstUseEver);
            if (ImGui::Begin("Spectators",nullptr,ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoFocusOnAppearing)) {
                for (int i=1;i<=64;++i) {
                    const auto controller = sdk::entity_at(state.entity_list,i);
                    if (!controller || controller == state.controller) continue;
                    const auto pawn = sdk::entity_from_handle(state.entity_list,sdk::read_value<uint32_t>(controller+sdk::off::CCSPlayerController::m_hPlayerPawn));
                    if (!pawn || sdk::read_value<int>(pawn+sdk::off::C_BaseEntity::m_iHealth)>0) continue;
                    const auto observer = ObserverServices(controller,pawn,state.entity_list);
                    if (!observer || sdk::entity_from_handle(state.entity_list,sdk::read_value<uint32_t>(observer+sdk::off::CPlayer_ObserverServices::m_hObserverTarget)) != observed) continue;
                    char name[128]{};
                    if (sdk::read_memory(controller+sdk::off::CBasePlayerController::m_iszPlayerName,name)) { name[127]=0; ImGui::TextUnformatted(name); }
                }
            }
            ImGui::End();
        }
        if (config::misc::knife_range || config::misc::taser_range) {
            const auto weapon = skins::GetActiveWeapon();
            const auto id = weapon ? skins::GetDefIndex(weapon) : 0;
            const float range = config::misc::knife_range && skins::IsKnife(id) ? 64.0f : config::misc::taser_range && id==31 ? 183.0f : 0;
            sdk::ViewMatrix matrix{};
            if (range && sdk::finite(localOrigin) && ReadViewMatrix(client,matrix)) {
                sdk::Vector2 previous{}; bool valid=false;
                for (int i=0;i<=64;++i) {
                    const float angle = i*6.28318530718f/64;
                    sdk::Vector2 screen{};
                    const bool projected=WorldToScreen(localOrigin+sdk::Vector3(std::cos(angle)*range,std::sin(angle)*range,3),screen,matrix,g_screen_width,g_screen_height);
                    if (valid && projected) draw->AddLine({previous.x,previous.y},{screen.x,screen.y},IM_COL32(255,200,60,180),1.5f);
                    previous=screen; valid=projected;
                }
            }
        }
    }

    // ======================== MISC FEATURES ========================
    void ReleaseInputs() {
        RestoreVisualFields();
        PublishFireTarget(0);
        if (s_scope_down) { mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0); s_scope_down=false; }
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

    static bool IsScopedWeapon(uint16_t id) { return id==9 || id==11 || id==38 || id==40; }
    static void AutoScope() {
        const auto now=GetTickCount64();
        if(s_scope_down) {
            if(now-s_scope_at>=10 || s_input_blocked || !game_state::IsInGame() || !config::aimbot::autoscope) {
                mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0); s_scope_down=false;
            }
            return;
        }
        if(!config::aimbot::enabled || !config::aimbot::autoscope || s_input_blocked || !game_state::IsInGame() ||
            (GetAsyncKeyState(config::aimbot::pause_key)&0x8000) || !s_aim_fire_target || now>s_aim_fire_until || now<s_scope_next || (GetAsyncKeyState(VK_RBUTTON)&0x8000)) return;
        const auto pawn=game_state::GetLocalPawnRaw();
        const auto weapon=skins::GetActiveWeapon();
        if(!pawn || !weapon || AimDisabled(pawn) || !IsScopedWeapon(skins::GetDefIndex(weapon)) || sdk::read_value<bool>(pawn+sdk::off::C_CSPlayerPawn::m_bIsScoped)) return;
        mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0); s_scope_down=true; s_scope_at=now; s_scope_next=now+300;
    }
    void TriggerBot() {
        AutoScope();
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
        if(!manual_trigger && config::aimbot::autoscope) {
            const auto weapon=skins::GetActiveWeapon();
            if(weapon && IsScopedWeapon(skins::GetDefIndex(weapon)) && !sdk::read_value<bool>(pawn+sdk::off::C_CSPlayerPawn::m_bIsScoped)) {tracked_target=0; return;}
        }
        const int index = sdk::read_value<int>(pawn + sdk::off::C_CSPlayerPawn::m_iIDEntIndex);
        const uintptr_t target = index > 0 ? sdk::entity_at(game_state::GetEntityList(), index) : 0;
        if (!target || target == pawn || sdk::read_value<int>(target + sdk::off::C_BaseEntity::m_iHealth) <= 0 ||
            sdk::read_value<uint8_t>(target + sdk::off::C_BaseEntity::m_iTeamNum) ==
            sdk::read_value<uint8_t>(pawn + sdk::off::C_BaseEntity::m_iTeamNum)) { tracked_target = 0; return; }
        if (!manual_trigger && (AimDisabled(pawn) || s_aim_fire_target != target || now > s_aim_fire_until)) { tracked_target=0; return; }
        if (target != tracked_target) { tracked_target = target; acquired_at = now; }
        const auto delay = AimSession::Delay(manual_trigger ? config::misc::trigger_delay : 0.0f);
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
