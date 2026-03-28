/*
Module Name: FrameStageNotify Hook
Authors: sathariel, martinmarinov
Product: Nephilimgate Multicheat
Tools used: imgui, a2x-cs2dumper
© 2026 sathariel & martinmarinov
*/

#pragma once
#include "../sdk/source2-sdk/interfaces/csource2client.hpp"

// Event-driven callback mechanism for FrameStageNotify hook
// Decouples hook implementation from callback logic - allows multiple systems
// to register for frame stage events without modifying hook code

typedef void (*OnFrameStageNotifyCallback)(int stage);

namespace hooks {
    // Virtual function signature for CSource2Client::FrameStageNotify
    // __fastcall: RCX=thisptr (CSource2Client*), RDX=FrameStage (int)
    // Memory layout: CSource2Client vtable at [pCSource2Client]; FrameStageNotify at vtable[index]
    using fnFrameStageNotify = void(__fastcall*)(CSource2Client* pCSource2Client, int FrameStage);

    // Registered callback type - fires during stage dispatch phase
    using OnFrameStageNotifyCallback = void(*)(int stage);

    // Original FrameStageNotify function pointer - set by MinHook during hook creation
    // MinHook preserves original fn ptr and calls it after hook handler completes
    extern fnFrameStageNotify FrameStageNotify_o;

    // Register callback to be invoked every time FrameStageNotify fires
    // Callback receives stage number; called before original fn
    void RegisterOnFrameStageNotify(OnFrameStageNotifyCallback callback);

    // Dispatch FrameStageNotify event to all registered callbacks
    // Iterates callback vector and invokes each registered handler
    void OnFrameStageNotify(int stage);

    // Hook implementation called by MinHook - intercepts original CSource2Client::FrameStageNotify
    // Flow: [game calls FrameStageNotify] -> [MinHook redirects to Hook_FrameStageNotify]
    //       -> [OnFrameStageNotify dispatches callbacks] -> [FrameStageNotify_o calls original]
    void __fastcall Hook_FrameStageNotify(CSource2Client* pCSource2Client, int FrameStage);
}

