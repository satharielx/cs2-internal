#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CBaseCSGrenadeProjectile.hpp"

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
        // Size: 0x2eb0
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "int m_nSmokeEffectTickBegin"
        // static metadata: MNetworkVarNames "bool m_bDidSmokeEffect"
        // static metadata: MNetworkVarNames "int m_nRandomSeed"
        // static metadata: MNetworkVarNames "Vector m_vSmokeColor"
        // static metadata: MNetworkVarNames "Vector m_vSmokeDetonationPos"
        // static metadata: MNetworkVarNames "uint8 m_VoxelFrameData"
        // static metadata: MNetworkVarNames "int m_nVoxelFrameDataSize"
        // static metadata: MNetworkVarNames "int m_nVoxelUpdate"
        #pragma pack(push, 1)
        class CSmokeGrenadeProjectile : public source2sdk::server::CBaseCSGrenadeProjectile
        {
        public:
            uint8_t _pad0bb0[0x28]; // 0xbb0
            // metadata: MNetworkEnable
            std::int32_t m_nSmokeEffectTickBegin; // 0xbd8            
            // metadata: MNetworkEnable
            bool m_bDidSmokeEffect; // 0xbdc            
            uint8_t _pad0bdd[0x3]; // 0xbdd
            // metadata: MNetworkEnable
            std::int32_t m_nRandomSeed; // 0xbe0            
            // metadata: MNetworkEnable
            Vector m_vSmokeColor; // 0xbe4            
            // metadata: MNetworkEnable
            Vector m_vSmokeDetonationPos; // 0xbf0            
            uint8_t _pad0bfc[0x4]; // 0xbfc
            // metadata: MNetworkEnable
            // m_VoxelFrameData has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CNetworkUtlVectorBase<std::uint8_t> m_VoxelFrameData;
            char m_VoxelFrameData[0x18]; // 0xc00            
            // metadata: MNetworkEnable
            std::int32_t m_nVoxelFrameDataSize; // 0xc18            
            // metadata: MNetworkEnable
            std::int32_t m_nVoxelUpdate; // 0xc1c            
            source2sdk::entity2::GameTime_t m_flLastBounce; // 0xc20            
            source2sdk::entity2::GameTime_t m_fllastSimulationTime; // 0xc24            
            uint8_t _pad0c28[0x2280]; // 0xc28
            bool m_bExplodeFromInferno; // 0x2ea8            
            bool m_bDidGroundScorch; // 0x2ea9            
            uint8_t _pad2eaa[0x6];
            
            // Datamap fields:
            // void CSmokeGrenadeProjectileThink_Detonate; // 0x0
            // void CSmokeGrenadeProjectileThink_Update; // 0x0
            // void CSmokeGrenadeProjectileThink_Remove; // 0x0
            // void CSmokeGrenadeProjectileThink_BuildingSmokeVolume; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CSmokeGrenadeProjectile because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CSmokeGrenadeProjectile) == 0x2eb0);
    };
};
