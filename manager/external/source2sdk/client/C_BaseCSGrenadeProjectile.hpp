#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseGrenade.hpp"
#include "source2sdk/client/ParticleIndex_t.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/resourcesystem/InfoForResourceTypeIParticleSnapshot.hpp"
#include "source2sdk/resourcesystem/InfoForResourceTypeIParticleSystemDefinition.hpp"

// /////////////////////////////////////////////////////////////
// Module: client
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace client
    {
        // Registered alignment: 0x8
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x1438
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
        class C_BaseCSGrenadeProjectile : public source2sdk::client::C_BaseGrenade
        {
        public:
            // metadata: MNetworkEnable
            Vector m_vInitialPosition; // 0x13a0            
            // metadata: MNetworkEnable
            Vector m_vInitialVelocity; // 0x13ac            
            // metadata: MNetworkEnable
            std::int32_t m_nBounces; // 0x13b8            
            uint8_t _pad13bc[0x4]; // 0x13bc
            // metadata: MNetworkEnable
            // m_nExplodeEffectIndex has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CStrongHandle<source2sdk::resourcesystem::InfoForResourceTypeIParticleSystemDefinition> m_nExplodeEffectIndex;
            char m_nExplodeEffectIndex[0x8]; // 0x13c0            
            // metadata: MNetworkEnable
            std::int32_t m_nExplodeEffectTickBegin; // 0x13c8            
            // metadata: MNetworkEnable
            Vector m_vecExplodeEffectOrigin; // 0x13cc            
            source2sdk::entity2::GameTime_t m_flSpawnTime; // 0x13d8            
            Vector vecLastTrailLinePos; // 0x13dc            
            source2sdk::entity2::GameTime_t flNextTrailLineTime; // 0x13e8            
            bool m_bExplodeEffectBegan; // 0x13ec            
            bool m_bCanCreateGrenadeTrail; // 0x13ed            
            uint8_t _pad13ee[0x2]; // 0x13ee
            source2sdk::client::ParticleIndex_t m_nSnapshotTrajectoryEffectIndex; // 0x13f0            
            uint8_t _pad13f4[0x4]; // 0x13f4
            // m_hSnapshotTrajectoryParticleSnapshot has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CStrongHandle<source2sdk::resourcesystem::InfoForResourceTypeIParticleSnapshot> m_hSnapshotTrajectoryParticleSnapshot;
            char m_hSnapshotTrajectoryParticleSnapshot[0x8]; // 0x13f8            
            // m_arrTrajectoryTrailPoints has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<Vector> m_arrTrajectoryTrailPoints;
            char m_arrTrajectoryTrailPoints[0x18]; // 0x1400            
            // m_arrTrajectoryTrailPointCreationTimes has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<float> m_arrTrajectoryTrailPointCreationTimes;
            char m_arrTrajectoryTrailPointCreationTimes[0x18]; // 0x1418            
            float m_flTrajectoryTrailEffectCreationTime; // 0x1430            
            uint8_t _pad1434[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_BaseCSGrenadeProjectile because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_BaseCSGrenadeProjectile) == 0x1438);
    };
};
