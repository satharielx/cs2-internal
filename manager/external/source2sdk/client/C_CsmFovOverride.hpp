#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseEntity.hpp"

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
        // Size: 0x618
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class C_CsmFovOverride : public source2sdk::client::C_BaseEntity
        {
        public:
            CUtlString m_cameraName; // 0x608            
            float m_flCsmFovOverrideValue; // 0x610            
            uint8_t _pad0614[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_CsmFovOverride because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_CsmFovOverride) == 0x618);
    };
};
