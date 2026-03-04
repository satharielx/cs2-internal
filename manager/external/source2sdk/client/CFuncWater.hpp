#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CBuoyancyHelper.hpp"
#include "source2sdk/client/C_BaseModelEntity.hpp"

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
        // Size: 0xfa0
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CFuncWater : public source2sdk::client::C_BaseModelEntity
        {
        public:
            source2sdk::client::CBuoyancyHelper m_BuoyancyHelper; // 0xe88            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CFuncWater because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::CFuncWater) == 0xfa0);
    };
};
