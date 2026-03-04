#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CBaseAnimGraph.hpp"

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
        // Size: 0x11a0
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CBaseProp : public source2sdk::client::CBaseAnimGraph
        {
        public:
            bool m_bModelOverrodeBlockLOS; // 0x1168            
            uint8_t _pad1169[0x3]; // 0x1169
            std::int32_t m_iShapeType; // 0x116c            
            bool m_bConformToCollisionBounds; // 0x1170            
            uint8_t _pad1171[0xf]; // 0x1171
            CTransform m_mPreferredCatchTransform; // 0x1180            
            
            // Datamap fields:
            // void health; // 0x7fffffff
            // CUtlSymbolLarge propdata_override; // 0x7fffffff
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CBaseProp because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::CBaseProp) == 0x11a0);
    };
};
