#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/physicslib/DynamicContinuousContactBehavior_t.hpp"
#include "source2sdk/server/CBreakableProp.hpp"
#include "source2sdk/server/CPhysicsProp_CrateType_t.hpp"

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
        // Size: 0xce0
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkExcludeByName "m_flexWeight"
        // static metadata: MNetworkExcludeByUserGroup "m_flPoseParameter"
        // static metadata: MNetworkExcludeByName "m_animationController.m_flPlaybackRate"
        // static metadata: MNetworkExcludeByUserGroup "overlay_vars"
        // static metadata: MNetworkIncludeByName "m_spawnflags"
        // static metadata: MNetworkVarNames "bool m_bAwake"
        #pragma pack(push, 1)
        class CPhysicsProp : public source2sdk::server::CBreakableProp
        {
        public:
            uint8_t _pad0ba0[0x10]; // 0xba0
            source2sdk::entity2::CEntityIOOutput m_MotionEnabled; // 0xbb0            
            source2sdk::entity2::CEntityIOOutput m_OnAwakened; // 0xbc8            
            source2sdk::entity2::CEntityIOOutput m_OnAwake; // 0xbe0            
            source2sdk::entity2::CEntityIOOutput m_OnAsleep; // 0xbf8            
            source2sdk::entity2::CEntityIOOutput m_OnPlayerUse; // 0xc10            
            source2sdk::entity2::CEntityIOOutput m_OnOutOfWorld; // 0xc28            
            source2sdk::entity2::CEntityIOOutput m_OnPlayerPickup; // 0xc40            
            bool m_bForceNavIgnore; // 0xc58            
            bool m_bNoNavmeshBlocker; // 0xc59            
            bool m_bForceNpcExclude; // 0xc5a            
            uint8_t _pad0c5b[0x1]; // 0xc5b
            float m_massScale; // 0xc5c            
            float m_buoyancyScale; // 0xc60            
            std::int32_t m_damageType; // 0xc64            
            std::int32_t m_damageToEnableMotion; // 0xc68            
            float m_flForceToEnableMotion; // 0xc6c            
            bool m_bThrownByPlayer; // 0xc70            
            bool m_bDroppedByPlayer; // 0xc71            
            bool m_bTouchedByPlayer; // 0xc72            
            bool m_bFirstCollisionAfterLaunch; // 0xc73            
            // metadata: MNotSaved
            bool m_bHasBeenAwakened; // 0xc74            
            // metadata: MNotSaved
            bool m_bIsOverrideProp; // 0xc75            
            uint8_t _pad0c76[0x2]; // 0xc76
            source2sdk::entity2::GameTime_t m_flLastBurn; // 0xc78            
            source2sdk::physicslib::DynamicContinuousContactBehavior_t m_nDynamicContinuousContactBehavior; // 0xc7c            
            uint8_t _pad0c7d[0x3]; // 0xc7d
            // metadata: MNotSaved
            source2sdk::entity2::GameTime_t m_fNextCheckDisableMotionContactsTime; // 0xc80            
            std::int32_t m_iInitialGlowState; // 0xc84            
            std::int32_t m_nGlowRange; // 0xc88            
            std::int32_t m_nGlowRangeMin; // 0xc8c            
            Color m_glowColor; // 0xc90            
            bool m_bShouldAutoConvertBackFromDebris; // 0xc94            
            bool m_bMuteImpactEffects; // 0xc95            
            uint8_t _pad0c96[0x6]; // 0xc96
            bool m_bUpdateNavWhenMoving; // 0xc9c            
            bool m_bForceNavObstacleCut; // 0xc9d            
            bool m_bAllowObstacleConvexHullMerging; // 0xc9e            
            bool m_bAcceptDamageFromHeldObjects; // 0xc9f            
            bool m_bEnableUseOutput; // 0xca0            
            uint8_t _pad0ca1[0x3]; // 0xca1
            source2sdk::server::CPhysicsProp_CrateType_t m_CrateType; // 0xca4            
            CUtlSymbolLarge m_strItemClass[4]; // 0xca8            
            std::int32_t m_nItemCount[4]; // 0xcc8            
            bool m_bRemovableForAmmoBalancing; // 0xcd8            
            // metadata: MNetworkEnable
            bool m_bAwake; // 0xcd9            
            bool m_bAttachedToReferenceFrame; // 0xcda            
            uint8_t _pad0cdb[0x5];
            
            // Datamap fields:
            // void InputEnableMotion; // 0x0
            // void InputDisableMotion; // 0x0
            // void InputWake; // 0x0
            // bool InputSetAutoConvertBackFromDebris; // 0x0
            // void InputSleep; // 0x0
            // void InputStartGlowing; // 0x0
            // void InputStopGlowing; // 0x0
            // Vector InputSetGlowOverride; // 0x0
            // int32_t InputSetGlowRange; // 0x0
            // float InputSetMass; // 0x0
            // void InputEnableGravity; // 0x0
            // void InputDisableGravity; // 0x0
            // void InputEnableDrag; // 0x0
            // void InputDisableDrag; // 0x0
            // void InputEnableCollisions; // 0x0
            // void InputDisableCollisions; // 0x0
            // bool forcemotiondisabled; // 0x7fffffff
            // bool phys_start_asleep; // 0x7fffffff
            // void CPhysicsPropClearFlagsThink; // 0x0
            // int32_t m_nNavObstacleType; // 0xc98
            // int32_t health; // 0x7fffffff
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CPhysicsProp because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CPhysicsProp) == 0xce0);
    };
};
