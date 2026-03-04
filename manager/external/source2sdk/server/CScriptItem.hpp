#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/MoveType_t.hpp"
#include "source2sdk/server/CItem.hpp"

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
        // Size: 0xad0
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CScriptItem : public source2sdk::server::CItem
        {
        public:
            // metadata: MNotSaved
            source2sdk::client::MoveType_t m_MoveTypeOverride; // 0xac0            
            uint8_t _pad0ac1[0xf];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CScriptItem because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CScriptItem) == 0xad0);
    };
};
