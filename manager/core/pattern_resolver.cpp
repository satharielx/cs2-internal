#include "constants/pattern_resolver_signatures.h"
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
    scan_global("client.dll", "EntityList", runtime_patterns::pattern_resolver::entity_system, 3, 7, entity_list);
    view_matrix = nullptr;
    scan_global("client.dll", "ViewMatrix", runtime_patterns::pattern_resolver::view_matrix, 3, 7, view_matrix);
    scan_global("client.dll", "LocalController", runtime_patterns::pattern_resolver::local_controller, 3, 7, local_controller);
    //scan_global("client.dll", "ViewAngles", runtime_patterns::pattern_resolver::view_angles_legacy, 3, 7, view_angles);
    //scan_global("client.dll", "GlobalVars", runtime_patterns::pattern_resolver::global_vars_legacy, 3, 7, global_vars);

    // CSGOInput pointer.
    scan_global("client.dll", "CSGOInput", runtime_patterns::pattern_resolver::csgo_input, 3, 7, csgo_input);

    // CreateMove function address (not a global pointer)
    uint8_t* cm_addr = sdk::find_pattern("client.dll", runtime_patterns::pattern_resolver::create_move);
    if (cm_addr) {
        create_move = reinterpret_cast<uintptr_t*>(cm_addr);
        con.Success("[Resolver] CreateMove = 0x%llX", (uintptr_t)cm_addr);
    }
    else {
        con.Warning("[Resolver] CreateMove pattern not found");
    }

    // engine2.dll
    scan_global("engine2.dll", "NetworkGameClient", runtime_patterns::pattern_resolver::network_game_client, 3, 7, network_game_client);

    return (entity_list != nullptr);
}
