#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CBaseTrigger.hpp"
#include "source2sdk/server/CBuoyancyHelper.hpp"

// /////////////////////////////////////////////////////////////
// Module: server
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace server
    {
        // Registered alignment: 0x8
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x9b0
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "float m_flFluidDensity"
        #pragma pack(push, 1)
        class CTriggerBuoyancy : public source2sdk::server::CBaseTrigger
        {
        public:
            source2sdk::server::CBuoyancyHelper m_BuoyancyHelper; // 0x890            
            // metadata: MNetworkEnable
            float m_flFluidDensity; // 0x9a8            
            uint8_t _pad09ac[0x4];
            
            // Datamap fields:
            // float InputSetFluidDensity; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CTriggerBuoyancy because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CTriggerBuoyancy) == 0x9b0);
    };
};
