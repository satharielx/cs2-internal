#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/IEconItemInterface.hpp"
#include "source2sdk/server/CAttributeList.hpp"

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
        // Standard-layout class: false
        // Size: 0x2a8
        // Has VTable
        // 
        // static metadata: MNetworkVarNames "item_definition_index_t m_iItemDefinitionIndex"
        // static metadata: MNetworkVarNames "int m_iEntityQuality"
        // static metadata: MNetworkVarNames "uint32 m_iEntityLevel"
        // static metadata: MNetworkVarNames "uint32 m_iItemIDHigh"
        // static metadata: MNetworkVarNames "uint32 m_iItemIDLow"
        // static metadata: MNetworkVarNames "uint32 m_iAccountID"
        // static metadata: MNetworkVarNames "uint32 m_iInventoryPosition"
        // static metadata: MNetworkVarNames "bool m_bInitialized"
        // static metadata: MNetworkVarNames "CAttributeList m_AttributeList"
        // static metadata: MNetworkVarNames "CAttributeList m_NetworkedDynamicAttributes"
        // static metadata: MNetworkVarNames "char m_szCustomName"
        #pragma pack(push, 1)
        class CEconItemView : public source2sdk::client::IEconItemInterface
        {
        public:
            uint8_t _pad0008[0x30]; // 0x8
            // metadata: MNetworkEnable
            std::uint16_t m_iItemDefinitionIndex; // 0x38            
            uint8_t _pad003a[0x2]; // 0x3a
            // metadata: MNetworkEnable
            std::int32_t m_iEntityQuality; // 0x3c            
            // metadata: MNetworkEnable
            std::uint32_t m_iEntityLevel; // 0x40            
            uint8_t _pad0044[0x4]; // 0x44
            std::uint64_t m_iItemID; // 0x48            
            // metadata: MNetworkEnable
            std::uint32_t m_iItemIDHigh; // 0x50            
            // metadata: MNetworkEnable
            std::uint32_t m_iItemIDLow; // 0x54            
            // metadata: MNetworkEnable
            std::uint32_t m_iAccountID; // 0x58            
            // metadata: MNetworkEnable
            std::uint32_t m_iInventoryPosition; // 0x5c            
            uint8_t _pad0060[0x8]; // 0x60
            // metadata: MNetworkEnable
            bool m_bInitialized; // 0x68            
            uint8_t _pad0069[0x7]; // 0x69
            // metadata: MNetworkEnable
            source2sdk::server::CAttributeList m_AttributeList; // 0x70            
            // metadata: MNetworkEnable
            source2sdk::server::CAttributeList m_NetworkedDynamicAttributes; // 0xe8            
            // metadata: MNetworkEnable
            char m_szCustomName[161]; // 0x160            
            char m_szCustomNameOverride[161]; // 0x201            
            uint8_t _pad02a2[0x6];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CEconItemView because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CEconItemView) == 0x2a8);
    };
};
