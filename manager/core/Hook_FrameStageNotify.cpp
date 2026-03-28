/*
Module Name: FrameStageNotify Hook Implementation
Authors: sathariel, martinmarinov
Product: Nephilimgate Multicheat
Tools used: imgui, a2x-cs2dumper
© 2026 sathariel & martinmarinov
*/

#include "Hook_FrameStageNotify.hpp"
#include <vector>
#include "debug_console.h"

namespace hooks {
    // Original FrameStageNotify function pointer - patched in by MinHook
    // Points to real implementation after hook trampoline created
    fnFrameStageNotify FrameStageNotify_o = nullptr;

    // Callback registry - vector of function pointers
    // Registered callbacks invoked sequentially during stage dispatch
    // No lock - assumes single-threaded game engine (safe assumption for Source2)
    std::vector<OnFrameStageNotifyCallback> g_frameStageCallbacks;

    // Frame stage tracking - only log on state changes to avoid spam
    // Prevents console from being flooded with per-frame logging
    static int g_lastLoggedStage = -2;
    static bool g_first_call = true;

    // Register a callback to be called on FrameStageNotify
    // Called during initialization phase - safe to directly push to vector
    // Callback receives stage number and executes synchronously
    void RegisterOnFrameStageNotify(OnFrameStageNotifyCallback callback)
    {
        if (callback == nullptr) {
            return;
        }

        g_frameStageCallbacks.push_back(callback);
    }

    // Dispatch FrameStageNotify event to all registered callbacks
    // Invokes each callback with current stage index
    // Execution order: callbacks registered first execute first (FIFO)
    void OnFrameStageNotify(int stage)
    {
        // Call all registered callbacks (no logging - runs every frame)
        for (const auto& callback : g_frameStageCallbacks)
        {
            if (callback != nullptr) {
                callback(stage);
            }
        }
    }

    // Hook function called by MinHook — receives original CSource2Client thisptr
    // Intercepts CSource2Client::FrameStageNotify virtual call
    // Flow: [game vtable call] -> [MinHook trampoline] -> [this function]
    //       -> [dispatch callbacks] -> [call original via FrameStageNotify_o]
    // Important: Must maintain __fastcall calling convention for proper thisptr passing
    void __fastcall Hook_FrameStageNotify(CSource2Client* pCSource2Client, int FrameStage)
    {
        // Log first hook call only - confirms interception successful
        // Subsequent calls run silently to avoid performance impact
        if (g_first_call) {
            debug_console::Console::Get().Success("[KNIFE] Hook_FrameStageNotify is FIRING! Stage %d", FrameStage);
            g_first_call = false;
        }

        // Dispatch to all registered callbacks - allows multiple systems to hook same event
        // Each callback can filter by stage and perform custom logic
        OnFrameStageNotify(FrameStage);

        // Call original FrameStageNotify function
        // Must call original to maintain game engine integrity
        if (FrameStageNotify_o != nullptr) {
            FrameStageNotify_o(pCSource2Client, FrameStage);
        }
    }
}
