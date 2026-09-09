#pragma once
#include <atomic>
#include <cstdint>
#include "../sdk/entity.h"

namespace game_state {
   
    void Start();
    void Stop();

    bool IsInGame();
    uintptr_t GetEntityList();
    uintptr_t GetLocalController();
    sdk::C_CSPlayerPawn* GetLocalPawn();
    uintptr_t GetLocalPawnRaw();   
}