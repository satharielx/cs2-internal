#pragma once
#include <atomic>
#include <cstdint>
#include "../sdk/entity.h"

namespace game_state {
    // Thread-safe game state — updated by a background monitor thread
    void Start();
    void Stop();

    bool IsInGame();
    uintptr_t GetEntityList();
    uintptr_t GetLocalController();
    sdk::C_CSPlayerPawn* GetLocalPawn();
}
