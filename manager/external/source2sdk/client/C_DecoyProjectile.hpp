#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseCSGrenadeProjectile.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"

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
        // Size: 0x1468
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "int m_nDecoyShotTick"
        #pragma pack(push, 1)
        class C_DecoyProjectile : public source2sdk::client::C_BaseCSGrenadeProjectile
        {
        public:
            // metadata: MNetworkEnable
            std::int32_t m_nDecoyShotTick; // 0x1438            
            std::int32_t m_nClientLastKnownDecoyShotTick; // 0x143c            
            uint8_t _pad1440[0x20]; // 0x1440
            source2sdk::entity2::GameTime_t m_flTimeParticleEffectSpawn; // 0x1460            
            uint8_t _pad1464[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_DecoyProjectile because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_DecoyProjectile) == 0x1468);
    };
};
