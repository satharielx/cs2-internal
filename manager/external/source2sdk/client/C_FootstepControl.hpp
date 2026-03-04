#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseTrigger.hpp"

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
        // Size: 0xf68
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "string_t m_source"
        // static metadata: MNetworkVarNames "string_t m_destination"
        #pragma pack(push, 1)
        class C_FootstepControl : public source2sdk::client::C_BaseTrigger
        {
        public:
            // metadata: MNetworkEnable
            CUtlSymbolLarge m_source; // 0xf58            
            // metadata: MNetworkEnable
            CUtlSymbolLarge m_destination; // 0xf60            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_FootstepControl because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_FootstepControl) == 0xf68);
    };
};
