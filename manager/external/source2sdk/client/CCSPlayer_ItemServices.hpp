#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CPlayer_ItemServices.hpp"

// /////////////////////////////////////////////////////////////
// Module: client
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace client
    {
        // Registered alignment: unknown
        // Alignment: 0x4
        // Standard-layout class: false
        // Size: 0x50
        // Has VTable
        // 
        // static metadata: MNetworkVarNames "bool m_bHasDefuser"
        // static metadata: MNetworkVarNames "bool m_bHasHelmet"
        #pragma pack(push, 1)
        class CCSPlayer_ItemServices : public source2sdk::client::CPlayer_ItemServices
        {
        public:
            // metadata: MNetworkEnable
            bool m_bHasDefuser; // 0x48            
            // metadata: MNetworkEnable
            bool m_bHasHelmet; // 0x49            
            uint8_t _pad004a[0x6];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSPlayer_ItemServices because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::CCSPlayer_ItemServices) == 0x50);
    };
};
