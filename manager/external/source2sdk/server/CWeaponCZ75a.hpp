#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CCSWeaponBaseGun.hpp"

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
        // Size: 0x1160
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "bool m_bMagazineRemoved"
        #pragma pack(push, 1)
        class CWeaponCZ75a : public source2sdk::server::CCSWeaponBaseGun
        {
        public:
            // metadata: MNetworkEnable
            bool m_bMagazineRemoved; // 0x1150            
            uint8_t _pad1151[0xf];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CWeaponCZ75a because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CWeaponCZ75a) == 0x1160);
    };
};
