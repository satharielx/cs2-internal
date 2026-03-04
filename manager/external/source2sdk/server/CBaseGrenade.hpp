#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CBaseFlex.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayerPawn;
    };
};

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
        // Size: 0xb40
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkIncludeByName "m_fFlags"
        // static metadata: MNetworkIncludeByName "m_vecVelocity"
        // static metadata: MNetworkExcludeByName "m_flexWeight"
        // static metadata: MNetworkExcludeByUserGroup "m_flPoseParameter"
        // static metadata: MNetworkExcludeByName "m_nResetEventsParity"
        // static metadata: MNetworkExcludeByUserGroup "overlay_vars"
        // static metadata: MNetworkExcludeByUserGroup "m_flCycle"
        // static metadata: MNetworkExcludeByName "m_baseLayer.m_hSequence"
        // static metadata: MNetworkExcludeByName "m_animationController.m_flPlaybackRate"
        // static metadata: MNetworkExcludeByName "m_nNewSequenceParity"
        // static metadata: MNetworkVarNames "bool m_bIsLive"
        // static metadata: MNetworkVarNames "float32 m_DmgRadius"
        // static metadata: MNetworkVarNames "GameTime_t m_flDetonateTime"
        // static metadata: MNetworkVarNames "float32 m_flDamage"
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerPawn > m_hThrower"
        #pragma pack(push, 1)
        class CBaseGrenade : public source2sdk::server::CBaseFlex
        {
        public:
            uint8_t _pad0ab0[0x8]; // 0xab0
            source2sdk::entity2::CEntityIOOutput m_OnPlayerPickup; // 0xab8            
            source2sdk::entity2::CEntityIOOutput m_OnExplode; // 0xad0            
            bool m_bHasWarnedAI; // 0xae8            
            bool m_bIsSmokeGrenade; // 0xae9            
            // metadata: MNetworkEnable
            bool m_bIsLive; // 0xaea            
            uint8_t _pad0aeb[0x1]; // 0xaeb
            // metadata: MNetworkEnable
            // metadata: MNetworkBitCount "10"
            // metadata: MNetworkMinValue "0,000000"
            // metadata: MNetworkMaxValue "1024,000000"
            // metadata: MNetworkEncodeFlags "1"
            float m_DmgRadius; // 0xaec            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flDetonateTime; // 0xaf0            
            float m_flWarnAITime; // 0xaf4            
            // metadata: MNetworkEnable
            // metadata: MNetworkBitCount "10"
            // metadata: MNetworkMinValue "0,000000"
            // metadata: MNetworkMaxValue "256,000000"
            // metadata: MNetworkEncodeFlags "1"
            float m_flDamage; // 0xaf8            
            uint8_t _pad0afc[0x4]; // 0xafc
            CUtlSymbolLarge m_iszBounceSound; // 0xb00            
            CUtlString m_ExplosionSound; // 0xb08            
            uint8_t _pad0b10[0x4]; // 0xb10
            // metadata: MNetworkEnable
            // m_hThrower has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_hThrower;
            char m_hThrower[0x4]; // 0xb14            
            uint8_t _pad0b18[0x14]; // 0xb18
            source2sdk::entity2::GameTime_t m_flNextAttack; // 0xb2c            
            // m_hOriginalThrower has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_hOriginalThrower;
            char m_hOriginalThrower[0x4]; // 0xb30            
            uint8_t _pad0b34[0xc];
            
            // Datamap fields:
            // int32_t m_nExplosionType; // 0xb10
            // void CBaseGrenadeSmoke; // 0x0
            // void CBaseGrenadeBounceTouch; // 0x0
            // void CBaseGrenadeSlideTouch; // 0x0
            // void CBaseGrenadeExplodeTouch; // 0x0
            // void CBaseGrenadeDetonateUse; // 0x0
            // void CBaseGrenadeDangerSoundThink; // 0x0
            // void CBaseGrenadePreDetonate; // 0x0
            // void CBaseGrenadeDetonate; // 0x0
            // void CBaseGrenadeTumbleThink; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CBaseGrenade because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CBaseGrenade) == 0xb40);
    };
};
