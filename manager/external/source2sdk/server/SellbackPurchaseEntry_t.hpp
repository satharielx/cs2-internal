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
        // static metadata: MNetworkVarNames "item_definition_index_t m_unDefIdx"
        // static metadata: MNetworkVarNames "int m_nCost"
        // static metadata: MNetworkVarNames "int m_nPrevArmor"
        // static metadata: MNetworkVarNames "bool m_bPrevHelmet"
        // static metadata: MNetworkVarNames "CEntityHandle m_hItem"
        #pragma pack(push, 1)
        struct SellbackPurchaseEntry_t
        {
        public:
            uint8_t _pad0000[0x30]; // 0x0
            // metadata: MNetworkEnable
            std::uint16_t m_unDefIdx; // 0x30            
            uint8_t _pad0032[0x2]; // 0x32
            // metadata: MNetworkEnable
            std::int32_t m_nCost; // 0x34            
            // metadata: MNetworkEnable
            std::int32_t m_nPrevArmor; // 0x38            
            // metadata: MNetworkEnable
            bool m_bPrevHelmet; // 0x3c            
            uint8_t _pad003d[0x3]; // 0x3d
            // metadata: MNetworkEnable
            CEntityHandle m_hItem; // 0x40            
            uint8_t _pad0044[0x4];
        };
        #pragma pack(pop)
        
        static_assert(offsetof(source2sdk::server::SellbackPurchaseEntry_t, m_unDefIdx) == 0x30);
        static_assert(offsetof(source2sdk::server::SellbackPurchaseEntry_t, m_nCost) == 0x34);
        static_assert(offsetof(source2sdk::server::SellbackPurchaseEntry_t, m_nPrevArmor) == 0x38);
        static_assert(offsetof(source2sdk::server::SellbackPurchaseEntry_t, m_bPrevHelmet) == 0x3c);
        static_assert(offsetof(source2sdk::server::SellbackPurchaseEntry_t, m_hItem) == 0x40);
        
        static_assert(sizeof(source2sdk::server::SellbackPurchaseEntry_t) == 0x48);
    };
};
