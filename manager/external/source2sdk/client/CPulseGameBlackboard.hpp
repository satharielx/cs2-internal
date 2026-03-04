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
        // Size: 0x620
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "CUtlString m_strGraphName"
        // static metadata: MNetworkVarNames "CUtlString m_strStateBlob"
        #pragma pack(push, 1)
        class CPulseGameBlackboard : public source2sdk::client::C_BaseEntity
        {
        public:
            uint8_t _pad0608[0x8]; // 0x608
            // metadata: MNetworkEnable
            CUtlString m_strGraphName; // 0x610            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnBlackboardStateChanged"
            CUtlString m_strStateBlob; // 0x618            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CPulseGameBlackboard because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::CPulseGameBlackboard) == 0x620);
    };
};
