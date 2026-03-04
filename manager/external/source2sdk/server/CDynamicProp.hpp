#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/AnimLoopMode_t.hpp"
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/server/CBreakableProp.hpp"

// /////////////////////////////////////////////////////////////
// Module: server
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace server
    {
        // Registered alignment: 0x10
        // Alignment: 0x10
        // Standard-layout class: false
        // Size: 0xc50
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MEntityAllowsPortraitWorldSpawn
        // static metadata: MNetworkVarNames "bool m_bUseHitboxesForRenderBox"
        // static metadata: MNetworkVarNames "bool m_bUseAnimGraph"
        #pragma pack(push, 1)
        class CDynamicProp : public source2sdk::server::CBreakableProp
        {
        public:
            uint8_t _pad0ba0[0x8]; // 0xba0
            bool m_bCreateNavObstacle; // 0xba8            
            bool m_bNavObstacleUpdatesOverridden; // 0xba9            
            // metadata: MNetworkEnable
            bool m_bUseHitboxesForRenderBox; // 0xbaa            
            // metadata: MNetworkEnable
            bool m_bUseAnimGraph; // 0xbab            
            uint8_t _pad0bac[0x4]; // 0xbac
            source2sdk::entity2::CEntityIOOutput m_pOutputAnimBegun; // 0xbb0            
            source2sdk::entity2::CEntityIOOutput m_pOutputAnimOver; // 0xbc8            
            source2sdk::entity2::CEntityIOOutput m_pOutputAnimLoopCycleOver; // 0xbe0            
            source2sdk::entity2::CEntityIOOutput m_OnAnimReachedStart; // 0xbf8            
            source2sdk::entity2::CEntityIOOutput m_OnAnimReachedEnd; // 0xc10            
            CUtlSymbolLarge m_iszIdleAnim; // 0xc28            
            source2sdk::client::AnimLoopMode_t m_nIdleAnimLoopMode; // 0xc30            
            bool m_bRandomizeCycle; // 0xc34            
            bool m_bStartDisabled; // 0xc35            
            bool m_bFiredStartEndOutput; // 0xc36            
            // metadata: MNotSaved
            bool m_bForceNpcExclude; // 0xc37            
            // metadata: MNotSaved
            bool m_bCreateNonSolid; // 0xc38            
            // metadata: MNotSaved
            bool m_bIsOverrideProp; // 0xc39            
            uint8_t _pad0c3a[0x2]; // 0xc3a
            std::int32_t m_iInitialGlowState; // 0xc3c            
            std::int32_t m_nGlowRange; // 0xc40            
            std::int32_t m_nGlowRangeMin; // 0xc44            
            Color m_glowColor; // 0xc48            
            std::int32_t m_nGlowTeam; // 0xc4c            
            
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
            // void CDynamicPropAnimThink; // 0x0
            // int32_t health; // 0x7fffffff
            // bool HoldAnimation; // 0x7fffffff
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CDynamicProp because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CDynamicProp) == 0xc50);
    };
};
