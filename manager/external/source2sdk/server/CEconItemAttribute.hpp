#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>

// /////////////////////////////////////////////////////////////
// Module: server
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace server
    {
        // Registered alignment: unknown
        // Alignment: 0x4
        // Standard-layout class: true
        // Size: 0x48
        // Has VTable
        // 
        // static metadata: MNetworkVarNames "attrib_definition_index_t m_iAttributeDefinitionIndex"
        // static metadata: MNetworkVarNames "float m_flValue"
        // static metadata: MNetworkVarNames "float m_flInitialValue"
        // static metadata: MNetworkVarNames "int m_nRefundableCurrency"
        // static metadata: MNetworkVarNames "bool m_bSetBonus"
        #pragma pack(push, 1)
        class CEconItemAttribute
        {
        public:
            uint8_t _pad0000[0x30]; // 0x0
            // metadata: MNetworkEnable
            std::uint16_t m_iAttributeDefinitionIndex; // 0x30            
            uint8_t _pad0032[0x2]; // 0x32
            // metadata: MNetworkEnable
            // metadata: MNetworkAlias "m_iRawValue32"
            float m_flValue; // 0x34            
            // metadata: MNetworkEnable
            float m_flInitialValue; // 0x38            
            // metadata: MNetworkEnable
            std::int32_t m_nRefundableCurrency; // 0x3c            
            // metadata: MNetworkEnable
            bool m_bSetBonus; // 0x40            
            uint8_t _pad0041[0x7];
        };
        #pragma pack(pop)
        
        static_assert(offsetof(source2sdk::server::CEconItemAttribute, m_iAttributeDefinitionIndex) == 0x30);
        static_assert(offsetof(source2sdk::server::CEconItemAttribute, m_flValue) == 0x34);
        static_assert(offsetof(source2sdk::server::CEconItemAttribute, m_flInitialValue) == 0x38);
        static_assert(offsetof(source2sdk::server::CEconItemAttribute, m_nRefundableCurrency) == 0x3c);
        static_assert(offsetof(source2sdk::server::CEconItemAttribute, m_bSetBonus) == 0x40);
        
        static_assert(sizeof(source2sdk::server::CEconItemAttribute) == 0x48);
    };
};
