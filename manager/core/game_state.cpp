#include "game_state.h"
#include "interfaces.h"
#include "../external/offsets/offsets.hpp"
#include "../sdk/feature_support.h"
#include "error_logger.h"
#include <thread>
#include <mutex>
#include <chrono>

namespace game_state {
    static std::atomic<bool> running{false};
    static std::thread worker;
    static std::mutex lifecycle_mutex;
    static std::mutex state_mutex;
    static Snapshot state{};

    Snapshot GetSnapshot() {
        std::lock_guard<std::mutex> lock(state_mutex);
        return state;
    }
    static void Publish(const Snapshot& next) {
        std::lock_guard<std::mutex> lock(state_mutex);
        state = next;
    }
    Snapshot ResolveSnapshot(uintptr_t client_base, uintptr_t entity_system,
        LocalPawnGetter getter, uintptr_t controller_slot) {
        Snapshot next{};
        if (!client_base) {
            next.status = "client.dll unavailable";
            return next;
        }
        next.pawn = sdk::read_value<uintptr_t>(client_base + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
        next.controller = sdk::read_value<uintptr_t>(controller_slot ? controller_slot :
            client_base + cs2_dumper::offsets::client_dll::dwLocalPlayerController);
        next.entity_list = sdk::is_valid_ptr(entity_system) ? entity_system : 0;
        if (!sdk::is_valid_ptr(next.controller)) next.controller = 0;
        bool used_getter = false;
        if (!sdk::is_valid_ptr(next.pawn) && getter) {
            next.pawn = getter(-1);
            used_getter = true;
        }
        if (!sdk::is_valid_ptr(next.pawn)) {
            next.pawn = 0;
            next.status = used_getter ? "dwLocalPlayerPawn and GetLocalPlayerPawn returned no readable pawn" :
                "dwLocalPlayerPawn is null or unreadable";
            return next;
        }
        // dwLocalPlayerPawn already contains the pawn address, not an entity handle.
        next.in_game = true;
        next.status = used_getter ?
            (next.entity_list ? "Local pawn ready (GetLocalPlayerPawn signature)" :
                "Local pawn ready (GetLocalPlayerPawn); entity system unavailable") :
            (next.entity_list ? "Local pawn ready (dwLocalPlayerPawn)" :
                "Local pawn ready (dwLocalPlayerPawn); entity system unavailable");
        return next;
    }
    static void MonitorLoop() {
        LocalPawnGetter getter = nullptr;
        uintptr_t controller_slot = 0;
        uintptr_t scanned_client = 0;
        ULONGLONG next_scan = 0;
        const char* last_status = nullptr;
        while (running) {
            const uintptr_t client = reinterpret_cast<uintptr_t>(GetModuleHandleA("client.dll"));
            if (client != scanned_client) {
                getter = nullptr;
                controller_slot = 0;
                scanned_client = client;
                next_scan = 0;
            }
            if (client && (!getter || !controller_slot) && GetTickCount64() >= next_scan) {
                next_scan = GetTickCount64() + 1000;
                // Function entry, not a RIP-relative pointer slot. Verified against the installed binary.
                if (!getter)
                    getter = reinterpret_cast<LocalPawnGetter>(sdk::find_pattern("client.dll",
                        "48 83 EC ? 83 F9 ? 75 ? 48 8B 0D ? ? ? ? 48 8D 54 24 ? ? ? ? FF 90 ? ? ? ? ? ? 48 63 C1 4C 8D 05"));
                if (!controller_slot) {
                    if (auto* instruction = sdk::find_pattern("client.dll", "48 8B 05 ? ? ? ? 41 89 BE"))
                        controller_slot = reinterpret_cast<uintptr_t>(sdk::resolve_absolute_rip_address(instruction, 3, 7));
                }
            }
            const Snapshot next = ResolveSnapshot(client,
                client ? reinterpret_cast<uintptr_t>(interfaces::GameEntitySystem()) : 0, getter, controller_slot);
            if (last_status != next.status) {
                char message[512];
                sprintf_s(message, "%s | entity system=%p controller=%p pawn=%p", next.status,
                    reinterpret_cast<void*>(next.entity_list), reinterpret_cast<void*>(next.controller), reinterpret_cast<void*>(next.pawn));
                error_logger::ErrorLogger::Get().Log("Game State", message, 0);
                last_status = next.status;
            }
            Publish(next); // Publish cleared values too, including on disconnect.
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        Publish({});
    }
    void Start() {
        std::lock_guard<std::mutex> lock(lifecycle_mutex);
        if (running) return;
        running = true;
        try { worker = std::thread(MonitorLoop); }
        catch (...) { running = false; throw; }
    }
    void Stop() {
        std::lock_guard<std::mutex> lock(lifecycle_mutex);
        running = false;
        if (worker.joinable()) worker.join();
        Publish({});
    }
    bool IsInGame() { return GetSnapshot().in_game; }
    uintptr_t GetEntityList() { return GetSnapshot().entity_list; }
    uintptr_t GetLocalController() { return GetSnapshot().controller; }
    sdk::C_CSPlayerPawn* GetLocalPawn() { return reinterpret_cast<sdk::C_CSPlayerPawn*>(GetSnapshot().pawn); }
    uintptr_t GetLocalPawnRaw() { return GetSnapshot().pawn; }
}
