#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CBaseTrigger.hpp"

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
        // Size: 0x8a0
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CTonemapTrigger : public source2sdk::server::CBaseTrigger
        {
        public:
            CUtlSymbolLarge m_tonemapControllerName; // 0x890            
            CEntityHandle m_hTonemapController; // 0x898            
            uint8_t _pad089c[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CTonemapTrigger because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CTonemapTrigger) == 0x8a0);
    };
};
