#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/AnimLoopMode_t.hpp"
#include "source2sdk/client/C_BreakableProp.hpp"
#include "source2sdk/entity2/CEntityIOOutput.hpp"

// /////////////////////////////////////////////////////////////
// Module: client
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace client
    {
        // Registered alignment: 0x10
        // Alignment: 0x10
        // Standard-layout class: false
        // Size: 0x13a0
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MEntityAllowsPortraitWorldSpawn
        // static metadata: MNetworkVarNames "bool m_bUseHitboxesForRenderBox"
        // static metadata: MNetworkVarNames "bool m_bUseAnimGraph"
        #pragma pack(push, 1)
        class C_DynamicProp : public source2sdk::client::C_BreakableProp
        {
        public:
            // metadata: MNetworkEnable
            bool m_bUseHitboxesForRenderBox; // 0x12d0            
            // metadata: MNetworkEnable
            bool m_bUseAnimGraph; // 0x12d1            
            uint8_t _pad12d2[0x6]; // 0x12d2
            source2sdk::entity2::CEntityIOOutput m_pOutputAnimBegun; // 0x12d8            
            source2sdk::entity2::CEntityIOOutput m_pOutputAnimOver; // 0x12f0            
            source2sdk::entity2::CEntityIOOutput m_pOutputAnimLoopCycleOver; // 0x1308            
            source2sdk::entity2::CEntityIOOutput m_OnAnimReachedStart; // 0x1320            
            source2sdk::entity2::CEntityIOOutput m_OnAnimReachedEnd; // 0x1338            
            CUtlSymbolLarge m_iszIdleAnim; // 0x1350            
            source2sdk::client::AnimLoopMode_t m_nIdleAnimLoopMode; // 0x1358            
            bool m_bRandomizeCycle; // 0x135c            
            bool m_bStartDisabled; // 0x135d            
            bool m_bFiredStartEndOutput; // 0x135e            
            // metadata: MNotSaved
            bool m_bForceNpcExclude; // 0x135f            
            // metadata: MNotSaved
            bool m_bCreateNonSolid; // 0x1360            
            // metadata: MNotSaved
            bool m_bIsOverrideProp; // 0x1361            
            uint8_t _pad1362[0x2]; // 0x1362
            std::int32_t m_iInitialGlowState; // 0x1364            
            std::int32_t m_nGlowRange; // 0x1368            
            std::int32_t m_nGlowRangeMin; // 0x136c            
            Color m_glowColor; // 0x1370            
            std::int32_t m_nGlowTeam; // 0x1374            
            // metadata: MNotSaved
            std::int32_t m_iCachedFrameCount; // 0x1378            
            // metadata: MNotSaved
            Vector m_vecCachedRenderMins; // 0x137c            
            // metadata: MNotSaved
            Vector m_vecCachedRenderMaxs; // 0x1388            
            uint8_t _pad1394[0xc];
            
            // Datamap fields:
            // CUtlSymbolLarge StartingAnim; // 0x7fffffff
            // CUtlSymbolLarge StartingAnimationLoopMode; // 0x7fffffff
            // CUtlSymbolLarge InputSetAnimationLooping; // 0x0
            // CUtlSymbolLarge InputSetAnimationNoResetLooping; // 0x0
            // CUtlSymbolLarge InputSetIdleAnimationLooping; // 0x0
            // CUtlSymbolLarge InputSetAnimationNotLooping; // 0x0
            // CUtlSymbolLarge InputSetAnimationNoResetNotLooping; // 0x0
            // CUtlSymbolLarge InputSetIdleAnimationNotLooping; // 0x0
            // CUtlSymbolLarge InputSetAnimation; // 0x0
            // CUtlSymbolLarge InputSetAnimationNoReset; // 0x0
            // CUtlSymbolLarge InputSetIdleAnimation; // 0x0
            // CUtlSymbolLarge InputSetDefaultAnimationLooping; // 0x0
            // CUtlSymbolLarge InputSetDefaultAnimationNotLooping; // 0x0
            // void InputTurnOn; // 0x0
            // void InputTurnOff; // 0x0
            // void InputEnable; // 0x0
            // void InputDisable; // 0x0
            // void InputEnableCollision; // 0x0
            // void InputDisableCollision; // 0x0
            // float InputSetPlaybackRate; // 0x0
            // void InputStartGlowing; // 0x0
            // void InputStopGlowing; // 0x0
            // Vector InputSetGlowOverride; // 0x0
            // int32_t InputSetGlowRange; // 0x0
            // void C_DynamicPropAnimThink; // 0x0
            // int32_t health; // 0x7fffffff
            // bool HoldAnimation; // 0x7fffffff
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_DynamicProp because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_DynamicProp) == 0x13a0);
    };
};
