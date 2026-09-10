#pragma once
#include <atomic>
#include <cstdint>
#include "../sdk/entity.h"

namespace game_state {
    struct Snapshot {
        bool in_game = false;
        uintptr_t entity_list = 0;
        uintptr_t controller = 0;
        uintptr_t pawn = 0;
        const char* status = "Waiting for game-state reader";
    };
    using LocalPawnGetter = uintptr_t(__fastcall*)(int slot);
    Snapshot ResolveSnapshot(uintptr_t client_base, uintptr_t entity_system,
        LocalPawnGetter getter = nullptr, uintptr_t controller_slot = 0);
    Snapshot GetSnapshot();

    void Start();
    void Stop();

    bool IsInGame();
    uintptr_t GetEntityList();
    uintptr_t GetLocalController();
    sdk::C_CSPlayerPawn* GetLocalPawn();
    uintptr_t GetLocalPawnRaw();
}
