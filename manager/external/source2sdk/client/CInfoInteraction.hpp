#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_PointEntity.hpp"

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
        // Size: 0x658
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CInfoInteraction : public source2sdk::client::C_PointEntity
        {
        public:
            CUtlSymbolLarge m_strSlotEntityName[8]; // 0x608            
            CUtlSymbolLarge m_strInteractVData; // 0x648            
            float m_flInteractRadius; // 0x650            
            uint8_t _pad0654[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CInfoInteraction because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::CInfoInteraction) == 0x658);
    };
};
