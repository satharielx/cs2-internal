#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/ESceneRequestState_t.hpp"
#include "source2sdk/server/SceneRequestHandle_t.hpp"

// /////////////////////////////////////////////////////////////
// Module: server
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace server
    {
        // Registered alignment: unknown
        // Alignment: 0x8
        // Standard-layout class: true
        // Size: 0x20
        // Has Trivial Destructor
        #pragma pack(push, 1)
        class CSceneRequest
        {
        public:
            CUtlSymbolLarge m_szPayloadTypeName; // 0x0            
            source2sdk::server::SceneRequestHandle_t m_uHandle; // 0x8            
            source2sdk::server::ESceneRequestState_t m_state; // 0xc            
            uint8_t _pad0010[0x10];
        };
        #pragma pack(pop)
        
        static_assert(offsetof(source2sdk::server::CSceneRequest, m_szPayloadTypeName) == 0x0);
        static_assert(offsetof(source2sdk::server::CSceneRequest, m_uHandle) == 0x8);
        static_assert(offsetof(source2sdk::server::CSceneRequest, m_state) == 0xc);
        
        static_assert(sizeof(source2sdk::server::CSceneRequest) == 0x20);
    };
};
