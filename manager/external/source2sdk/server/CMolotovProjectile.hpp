#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CBaseCSGrenadeProjectile.hpp"
#include "source2sdk/server/IntervalTimer.hpp"

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
        // Size: 0xcb0
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "bool m_bIsIncGrenade"
        #pragma pack(push, 1)
        class CMolotovProjectile : public source2sdk::server::CBaseCSGrenadeProjectile
        {
        public:
            // metadata: MNetworkEnable
            bool m_bIsIncGrenade; // 0xbb0            
            uint8_t _pad0bb1[0x17]; // 0xbb1
            bool m_bDetonated; // 0xbc8            
            uint8_t _pad0bc9[0x7]; // 0xbc9
            source2sdk::server::IntervalTimer m_stillTimer; // 0xbd0            
            uint8_t _pad0be0[0xd0];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CMolotovProjectile because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CMolotovProjectile) == 0xcb0);
    };
};
