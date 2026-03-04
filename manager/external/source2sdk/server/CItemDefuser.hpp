#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CItem.hpp"
#include "source2sdk/server/EntitySpottedState_t.hpp"

// /////////////////////////////////////////////////////////////
// Module: server
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace server
    {
        // Registered alignment: 0x10
        // Alignment: 0x10
        // Standard-layout class: false
        // Size: 0xae0
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        #pragma pack(push, 1)
        class CItemDefuser : public source2sdk::server::CItem
        {
        public:
            source2sdk::server::EntitySpottedState_t m_entitySpottedState; // 0xac0            
            std::int32_t m_nSpotRules; // 0xad8            
            uint8_t _pad0adc[0x4];
            
            // Datamap fields:
            // void CItemDefuserActivateThink; // 0x0
            // void CItemDefuserDefuserTouch; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CItemDefuser because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CItemDefuser) == 0xae0);
    };
};
