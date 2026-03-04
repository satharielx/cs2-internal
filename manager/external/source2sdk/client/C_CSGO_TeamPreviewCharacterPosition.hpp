#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseEntity.hpp"
#include "source2sdk/client/C_EconItemView.hpp"

// /////////////////////////////////////////////////////////////
// Module: client
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace client
    {
        // Registered alignment: unknown
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x13a8
        // Has VTable
        // Is Abstract
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "int m_nVariant"
        // static metadata: MNetworkVarNames "int m_nRandom"
        // static metadata: MNetworkVarNames "int m_nOrdinal"
        // static metadata: MNetworkVarNames "CUtlString m_sWeaponName"
        // static metadata: MNetworkVarNames "XUID m_xuid"
        // static metadata: MNetworkVarNames "CEconItemView m_agentItem"
        // static metadata: MNetworkVarNames "CEconItemView m_glovesItem"
        // static metadata: MNetworkVarNames "CEconItemView m_weaponItem"
        #pragma pack(push, 1)
        class C_CSGO_TeamPreviewCharacterPosition : public source2sdk::client::C_BaseEntity
        {
        public:
            // metadata: MNetworkEnable
            std::int32_t m_nVariant; // 0x608            
            // metadata: MNetworkEnable
            std::int32_t m_nRandom; // 0x60c            
            // metadata: MNetworkEnable
            std::int32_t m_nOrdinal; // 0x610            
            uint8_t _pad0614[0x4]; // 0x614
            // metadata: MNetworkEnable
            CUtlString m_sWeaponName; // 0x618            
            // metadata: MNetworkEnable
            std::uint64_t m_xuid; // 0x620            
            // metadata: MNetworkEnable
            source2sdk::client::C_EconItemView m_agentItem; // 0x628            
            // metadata: MNetworkEnable
            source2sdk::client::C_EconItemView m_glovesItem; // 0xa98            
            // metadata: MNetworkEnable
            source2sdk::client::C_EconItemView m_weaponItem; // 0xf08            
            uint8_t _pad1378[0x30];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_CSGO_TeamPreviewCharacterPosition because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_CSGO_TeamPreviewCharacterPosition) == 0x13a8);
    };
};
