#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseModelEntity.hpp"
#include "source2sdk/client/GrenadeType_t.hpp"

// /////////////////////////////////////////////////////////////
// Module: client
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace client
    {
        // Registered alignment: 0x10
        // Alignment: 0x10
        // Standard-layout class: false
        // Size: 0x1320
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CGrenadeTracer : public source2sdk::client::C_BaseModelEntity
        {
        public:
            uint8_t _pad0e88[0x18]; // 0xe88
            float m_flTracerDuration; // 0xea0            
            source2sdk::client::GrenadeType_t m_nType; // 0xea4            
            uint8_t _pad0ea8[0x478];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CGrenadeTracer because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::CGrenadeTracer) == 0x1320);
    };
};
