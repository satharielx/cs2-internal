#pragma once
#include <cstdint>
#include "../sdk/entity.h"

namespace pattern_resolver {

    // ---------- GLOBAL POINTERS (pointers to engine singletons) ----------
    inline uintptr_t* entity_list = nullptr;           // CEntitySystem*
    inline uintptr_t* local_controller = nullptr;      // CCSPlayerController*
    inline void** global_vars = nullptr;           // CGlobalVarsBase*
    inline uintptr_t* view_matrix = nullptr;           // VMatrix*
    inline uintptr_t* planted_c4 = nullptr;            // CPlantedC4*
    inline uintptr_t* csgo_input = nullptr;            // CCSGOInput*
    inline uintptr_t* network_game_client = nullptr;   // INetworkGameClient*
    inline uintptr_t* entity_system = nullptr;         // CEntitySystem* (alternative)
    inline float* sensitivity = nullptr;          // mouse sensitivity
    inline uintptr_t* auto_accept_array = nullptr;     // for auto accept
    inline uintptr_t* view_model = nullptr;
    inline uintptr_t* view_angles = nullptr;
	inline uintptr_t* create_move = nullptr;
    inline uintptr_t* get_fov = nullptr;
	inline uintptr_t* schema_system = nullptr;

	using fnGetBonePositionByName = uintptr_t(__fastcall*)(sdk::C_CSPlayerPawn* pawn, char* bone_name);
	inline fnGetBonePositionByName* oGetBonePositionByName = nullptr;

    // ---------- INITIALIZATION ----------
    bool Initialize();

    // Helper: resolve RIP-relative address
    uintptr_t GetAbsoluteAddress(uintptr_t instruction, int offset, int size);
}