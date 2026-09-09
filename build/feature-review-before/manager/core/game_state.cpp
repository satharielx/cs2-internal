#include "game_state.h"
#include "debug_console.h"
#include "interfaces.h"
#include "../external/offsets/offsets.hpp"
#include "../sdk/mem.h"
#include <Windows.h>
#include <thread>
#include <atomic>
#include <mutex>

namespace game_state {

    // Atomic state — written by monitor thread, read by render thread
    static std::atomic<bool> s_in_game{ false };
    static std::atomic<uintptr_t> s_entity_list{ 0 };
    static std::atomic<uintptr_t> s_local_controller{ 0 };
    static std::atomic<sdk::C_CSPlayerPawn*> s_local_pawn{ nullptr };
    static std::atomic<uintptr_t> s_local_pawn_raw { 0 };

    // Thread control
    static std::atomic<bool> s_running{ false };
    static std::thread s_thread;

    // Base addresses (resolved once)
    static uintptr_t s_client_base = 0;
    static uintptr_t s_local_controller_addr = 0;
    static uintptr_t s_local_pawn_addr = 0;

    static bool ResolveBaseAddresses() {
        HMODULE client = GetModuleHandle(L"client.dll");
        if (!client) return false;

        s_client_base = (uintptr_t)GetModuleHandle(L"client.dll");
        s_local_controller_addr = s_client_base + cs2_dumper::offsets::client_dll::dwLocalPlayerController;
        s_local_pawn_addr = s_client_base + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn;

        debug_console::Console::Get().Info("[GameState] client.dll base: 0x%llX", s_client_base);
        debug_console::Console::Get().Pointer("dwLocalPlayerController", s_local_controller_addr, 0, true);
        debug_console::Console::Get().Pointer("dwLocalPlayerPawn", s_local_pawn_addr, 0, true);
        return true;
    }

    static void MonitorLoop() {
        debug_console::Console::Get().Info("[GameState] Monitor thread started");

        // Wait for client.dll and resolve addresses once
        while (s_running.load()) {
            if (ResolveBaseAddresses()) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        if (!s_running.load()) return;
        debug_console::Console::Get().Success("[GameState] Base addresses resolved");

        while (s_running.load()) {
            bool in_game = false;

            // CGameEntitySystem is the entity list. Resolve it through the
            // interface accessor, using the GetBaseEntity call-site pattern.
            uintptr_t entity_list = reinterpret_cast<uintptr_t>(
                interfaces::GameEntitySystem());
            if (entity_list && sdk::is_valid_ptr(entity_list)) {
                s_entity_list.store(entity_list);
            }
            else {
                entity_list = 0;
                s_entity_list.store(0);
            }

            // Read local controller
            uintptr_t controller = 0;
            if (sdk::is_valid_ptr(s_local_controller_addr)) {
                controller = *(uintptr_t*)(s_local_controller_addr);
                if (controller != 0 && sdk::is_valid_ptr(controller)) {
                    s_local_controller.store(controller);
                } else {
                    controller = 0;
                    s_local_controller.store(0);
                }
            }

            // Read local pawn directly via dwLocalPlayerPawn
            sdk::C_CSPlayerPawn* pawn = nullptr;
            uintptr_t pawn_addr = 0;
            if (sdk::is_valid_ptr(s_local_pawn_addr)) {
                pawn_addr = *(uintptr_t*)(s_local_pawn_addr);
                if (pawn_addr != 0 && sdk::is_valid_ptr(pawn_addr)) {
                    pawn = reinterpret_cast<sdk::C_CSPlayerPawn*>(pawn_addr);
                }
            }
            s_local_pawn.store(pawn);
            s_local_pawn_raw.store(pawn_addr);

            // We are in-game if entity list, controller, and pawn are all valid
            in_game = (entity_list != 0) && (controller != 0) && (pawn != nullptr);
            s_in_game.store(in_game);

            // Poll every 100ms — fast enough to detect map changes, slow enough to not waste CPU
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        debug_console::Console::Get().Pointer("EntityList", s_entity_list.load(), s_entity_list.load(), sdk::is_valid_ptr(s_entity_list.load()));
        debug_console::Console::Get().Pointer("LocalController", s_local_controller.load(), s_local_controller.load(), sdk::is_valid_ptr(s_local_controller.load()));
        debug_console::Console::Get().Pointer("LocalPawn", s_local_pawn_raw.load(), s_local_pawn_raw.load(), sdk::is_valid_ptr(s_local_pawn_raw.load()));
        debug_console::Console::Get().Info("[GameState] Monitor thread stopped");
    }

    void Start() {
        if (s_running.load()) return;
        s_running.store(true);
        s_thread = std::thread(MonitorLoop);
    }

    void Stop() {
        s_running.store(false);
        if (s_thread.joinable()) {
            s_thread.join();
        }
        s_in_game.store(false);
        s_entity_list.store(0);
        s_local_controller.store(0);
        s_local_pawn.store(nullptr);
        s_local_pawn_raw.store(0);
    }

    bool IsInGame()               { return s_in_game.load(); }
    uintptr_t GetEntityList()     { return s_entity_list.load(); }
    uintptr_t GetLocalController(){ return s_local_controller.load(); }
    sdk::C_CSPlayerPawn* GetLocalPawn() { return s_local_pawn.load(); }
    uintptr_t GetLocalPawnRaw()   { return s_local_pawn_raw.load(); }
}
