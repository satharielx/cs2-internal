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
        // static metadata: MNetworkVarNames "int m_nMode"
        // static metadata: MNetworkVarNames "Vector m_vBoxSize"
        // static metadata: MNetworkVarNames "bool m_bEnabled"
        #pragma pack(push, 1)
        class C_InfoVisibilityBox : public source2sdk::client::C_BaseEntity
        {
        public:
            uint8_t _pad0608[0x4]; // 0x608
            // metadata: MNetworkEnable
            std::int32_t m_nMode; // 0x60c            
            // metadata: MNetworkEnable
            Vector m_vBoxSize; // 0x610            
            // metadata: MNetworkEnable
            bool m_bEnabled; // 0x61c            
            uint8_t _pad061d[0x3];
            
            // Datamap fields:
            // void InputEnable; // 0x0
            // void InputDisable; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_InfoVisibilityBox because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_InfoVisibilityBox) == 0x620);
    };
};
