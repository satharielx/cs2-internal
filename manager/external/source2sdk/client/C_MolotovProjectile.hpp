#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseCSGrenadeProjectile.hpp"

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
        // Size: 0x1460
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "bool m_bIsIncGrenade"
        #pragma pack(push, 1)
        class C_MolotovProjectile : public source2sdk::client::C_BaseCSGrenadeProjectile
        {
        public:
            // metadata: MNetworkEnable
            bool m_bIsIncGrenade; // 0x1438            
            uint8_t _pad1439[0x27];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_MolotovProjectile because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_MolotovProjectile) == 0x1460);
    };
};
