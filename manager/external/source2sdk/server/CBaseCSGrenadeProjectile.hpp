#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/resourcesystem/InfoForResourceTypeIParticleSystemDefinition.hpp"
#include "source2sdk/server/CBaseGrenade.hpp"

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
        // Size: 0xbb0
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "Vector m_vInitialPosition"
        // static metadata: MNetworkVarNames "Vector m_vInitialVelocity"
        // static metadata: MNetworkVarNames "int m_nBounces"
        // static metadata: MNetworkVarNames "HParticleSystemDefinitionStrong m_nExplodeEffectIndex"
        // static metadata: MNetworkVarNames "int m_nExplodeEffectTickBegin"
        // static metadata: MNetworkVarNames "Vector m_vecExplodeEffectOrigin"
        #pragma pack(push, 1)
        class CBaseCSGrenadeProjectile : public source2sdk::server::CBaseGrenade
        {
        public:
            // metadata: MNetworkEnable
            Vector m_vInitialPosition; // 0xb40            
            // metadata: MNetworkEnable
            Vector m_vInitialVelocity; // 0xb4c            
            // metadata: MNetworkEnable
            std::int32_t m_nBounces; // 0xb58            
            uint8_t _pad0b5c[0x4]; // 0xb5c
            // metadata: MNetworkEnable
            // m_nExplodeEffectIndex has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CStrongHandle<source2sdk::resourcesystem::InfoForResourceTypeIParticleSystemDefinition> m_nExplodeEffectIndex;
            char m_nExplodeEffectIndex[0x8]; // 0xb60            
            // metadata: MNetworkEnable
            std::int32_t m_nExplodeEffectTickBegin; // 0xb68            
            // metadata: MNetworkEnable
            Vector m_vecExplodeEffectOrigin; // 0xb6c            
            source2sdk::entity2::GameTime_t m_flSpawnTime; // 0xb78            
            std::uint8_t m_unOGSExtraFlags; // 0xb7c            
            bool m_bDetonationRecorded; // 0xb7d            
            std::uint16_t m_nItemIndex; // 0xb7e            
            Vector m_vecOriginalSpawnLocation; // 0xb80            
            source2sdk::entity2::GameTime_t m_flLastBounceSoundTime; // 0xb8c            
            RotationVector m_vecGrenadeSpin; // 0xb90            
            Vector m_vecLastHitSurfaceNormal; // 0xb9c            
            std::int32_t m_nTicksAtZeroVelocity; // 0xba8            
            bool m_bHasEverHitEnemy; // 0xbac            
            uint8_t _pad0bad[0x3];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CBaseCSGrenadeProjectile because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CBaseCSGrenadeProjectile) == 0xbb0);
    };
};
