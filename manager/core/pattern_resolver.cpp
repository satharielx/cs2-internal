#include "pattern_resolver.h"
#include "../sdk/mem.h"
#include "debug_console.h"
#include <Windows.h>
#include <type_traits>

uintptr_t pattern_resolver::GetAbsoluteAddress(uintptr_t instruction, int offset, int size) {
    if (!instruction) return 0;
    int32_t relative = *reinterpret_cast<int32_t*>(instruction + offset);
    return instruction + size + relative;
}

bool pattern_resolver::Initialize() {
    auto& con = debug_console::Console::Get();
    auto scan_global = [&](const char* module, const char* name, const char* pattern, int offset, int size, auto& out_ptr) {
        uint8_t* addr = sdk::find_pattern(module, pattern);
        if (addr) {
            uintptr_t resolved = GetAbsoluteAddress(reinterpret_cast<uintptr_t>(addr), offset, size);
            using output_pointer = std::remove_reference_t<decltype(out_ptr)>;
            out_ptr = reinterpret_cast<output_pointer>(resolved);
            con.Success("[Resolver] %s = 0x%llX", name, resolved);
        }
        else {
            con.Warning("[Resolver] %s pattern not found", name);
        }
        };

    // ───────── client.dll globals ─────────
    // Resolve the global used by CGameEntitySystem::GetBaseEntity. Keep the
    // signature short: the destination register after the call varies by build.
    scan_global("client.dll", "EntityList", "48 89 0D ? ? ? ? E9 ? ? ? ? CC", 3, 7, entity_list);
    //scan_global("client.dll", "ViewMatrix", "48 8D 0D ? ? ? ? 48 C1 E0 06", 3, 7, view_matrix);
    scan_global("client.dll", "LocalController", "48 8B 05 ? ? ? ? 41 89 BE", 3, 7, local_controller);
    //scan_global("client.dll", "ViewAngles", "F3 0F 11 86 ? ? ? ? F3 0F 10 44 24", 3, 7, view_angles);
    //scan_global("client.dll", "GlobalVars", "48 8B 05 ? ? ? ? 8B 38 E8 00", 3, 7, global_vars);

    // CSGOInput (unchanged)
    scan_global("client.dll", "CSGOInput", "48 8B 0D ? ? ? ? 8B 10 E8 ? ? ? ? 45 32 FF", 3, 7, csgo_input);

    // CreateMove function address (not a global pointer)
    uint8_t* cm_addr = sdk::find_pattern("client.dll", "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F9 48 8B");
    if (cm_addr) {
        create_move = reinterpret_cast<uintptr_t*>(cm_addr);
        con.Success("[Resolver] CreateMove = 0x%llX", (uintptr_t)cm_addr);
    }
    else {
        con.Warning("[Resolver] CreateMove pattern not found");
    }

    // engine2.dll
    scan_global("engine2.dll", "NetworkGameClient", "48 89 3D ? ? ? ? FF 87", 3, 7, network_game_client);

    return (entity_list != nullptr);
}
