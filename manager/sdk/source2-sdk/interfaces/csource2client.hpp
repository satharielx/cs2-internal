#pragma once
#include <cstdint>

// Forward declarations
class CEconItemSystem;

// CSource2Client virtual interface - engine's main client interface
// Vtable provides access to critical game state and event callbacks
// FrameStageNotify callback fires every game frame with stage index (0-11)
// Stages represent game engine lifecycle phases; stage 7 = render post-processing
class CSource2Client
{
public:
    // Get the economic item system interface
    // Used for inventory, loadout, and item data queries
    virtual CEconItemSystem* GetEconItemSystem() = 0;

    // Frame stage notification — called every frame with current stage (0-11)
    // nStage lifecycle:
    //   0=PreRender, 1=CreateNetworkStringTables, 2=OnPreDataChanged, 3=OnDataChanged,
    //   4=OnPostDataChanged, 5=OnRenderStart, 6=OnRenderEnd, 7=OnPostRender, 8-11=Various
    // Stage 7 is optimal for SetModel calls - skeleton instance guaranteed valid at this point
    virtual void FrameStageNotify( int nStage ) = 0;
};

