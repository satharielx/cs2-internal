#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_CS2WeaponModuleBase.hpp"

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
        // Size: 0x1178
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        #pragma pack(push, 1)
        class C_StattrakModule : public source2sdk::client::C_CS2WeaponModuleBase
        {
        public:
            bool m_bKnife; // 0x1170            
            uint8_t _pad1171[0x7];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_StattrakModule because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_StattrakModule) == 0x1178);
    };
};
