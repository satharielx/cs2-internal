#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CBaseEntity.hpp"
#include "source2sdk/server/CEconItemView.hpp"

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
        // Size: 0xcc0
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
        class CCSGO_TeamPreviewCharacterPosition : public source2sdk::server::CBaseEntity
        {
        public:
            // metadata: MNetworkEnable
            std::int32_t m_nVariant; // 0x4a8            
            // metadata: MNetworkEnable
            std::int32_t m_nRandom; // 0x4ac            
            // metadata: MNetworkEnable
            std::int32_t m_nOrdinal; // 0x4b0            
            uint8_t _pad04b4[0x4]; // 0x4b4
            // metadata: MNetworkEnable
            CUtlString m_sWeaponName; // 0x4b8            
            // metadata: MNetworkEnable
            std::uint64_t m_xuid; // 0x4c0            
            // metadata: MNetworkEnable
            source2sdk::server::CEconItemView m_agentItem; // 0x4c8            
            // metadata: MNetworkEnable
            source2sdk::server::CEconItemView m_glovesItem; // 0x770            
            // metadata: MNetworkEnable
            source2sdk::server::CEconItemView m_weaponItem; // 0xa18            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSGO_TeamPreviewCharacterPosition because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CCSGO_TeamPreviewCharacterPosition) == 0xcc0);
    };
};
