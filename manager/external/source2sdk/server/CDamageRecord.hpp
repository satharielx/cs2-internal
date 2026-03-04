#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/EKillTypes_t.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayerController;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayerPawn;
    };
};

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
        // Size: 0x78
        // Has VTable
        // 
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerPawn > m_PlayerDamager"
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerPawn > m_PlayerRecipient"
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerController > m_hPlayerControllerDamager"
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerController > m_hPlayerControllerRecipient"
        // static metadata: MNetworkVarNames "CUtlString m_szPlayerDamagerName"
        // static metadata: MNetworkVarNames "CUtlString m_szPlayerRecipientName"
        // static metadata: MNetworkVarNames "uint64 m_DamagerXuid"
        // static metadata: MNetworkVarNames "uint64 m_RecipientXuid"
        // static metadata: MNetworkReplayCompatField "m_iDamage"
        // static metadata: MNetworkVarNames "float m_flDamage"
        // static metadata: MNetworkReplayCompatField "m_iActualHealthRemoved"
        // static metadata: MNetworkVarNames "float m_flActualHealthRemoved"
        // static metadata: MNetworkVarNames "int m_iNumHits"
        // static metadata: MNetworkVarNames "int m_iLastBulletUpdate"
        // static metadata: MNetworkVarNames "bool m_bIsOtherEnemy"
        // static metadata: MNetworkVarNames "EKillTypes_t m_killType"
        #pragma pack(push, 1)
        class CDamageRecord
        {
        public:
            uint8_t _pad0000[0x30]; // 0x0
            // metadata: MNetworkEnable
            // m_PlayerDamager has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_PlayerDamager;
            char m_PlayerDamager[0x4]; // 0x30            
            // metadata: MNetworkEnable
            // m_PlayerRecipient has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_PlayerRecipient;
            char m_PlayerRecipient[0x4]; // 0x34            
            // metadata: MNetworkEnable
            // m_hPlayerControllerDamager has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerController> m_hPlayerControllerDamager;
            char m_hPlayerControllerDamager[0x4]; // 0x38            
            // metadata: MNetworkEnable
            // m_hPlayerControllerRecipient has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerController> m_hPlayerControllerRecipient;
            char m_hPlayerControllerRecipient[0x4]; // 0x3c            
            // metadata: MNetworkEnable
            CUtlString m_szPlayerDamagerName; // 0x40            
            // metadata: MNetworkEnable
            CUtlString m_szPlayerRecipientName; // 0x48            
            // metadata: MNetworkEnable
            std::uint64_t m_DamagerXuid; // 0x50            
            // metadata: MNetworkEnable
            std::uint64_t m_RecipientXuid; // 0x58            
            float m_flBulletsDamage; // 0x60            
            // metadata: MNetworkEnable
            float m_flDamage; // 0x64            
            // metadata: MNetworkEnable
            float m_flActualHealthRemoved; // 0x68            
            // metadata: MNetworkEnable
            std::int32_t m_iNumHits; // 0x6c            
            // metadata: MNetworkEnable
            std::int32_t m_iLastBulletUpdate; // 0x70            
            // metadata: MNetworkEnable
            bool m_bIsOtherEnemy; // 0x74            
            // metadata: MNetworkEnable
            source2sdk::client::EKillTypes_t m_killType; // 0x75            
            uint8_t _pad0076[0x2];
        };
        #pragma pack(pop)
        
        static_assert(offsetof(source2sdk::server::CDamageRecord, m_PlayerDamager) == 0x30);
        static_assert(offsetof(source2sdk::server::CDamageRecord, m_PlayerRecipient) == 0x34);
        static_assert(offsetof(source2sdk::server::CDamageRecord, m_hPlayerControllerDamager) == 0x38);
        static_assert(offsetof(source2sdk::server::CDamageRecord, m_hPlayerControllerRecipient) == 0x3c);
        static_assert(offsetof(source2sdk::server::CDamageRecord, m_szPlayerDamagerName) == 0x40);
        static_assert(offsetof(source2sdk::server::CDamageRecord, m_szPlayerRecipientName) == 0x48);
        static_assert(offsetof(source2sdk::server::CDamageRecord, m_DamagerXuid) == 0x50);
        static_assert(offsetof(source2sdk::server::CDamageRecord, m_RecipientXuid) == 0x58);
        static_assert(offsetof(source2sdk::server::CDamageRecord, m_flBulletsDamage) == 0x60);
        static_assert(offsetof(source2sdk::server::CDamageRecord, m_flDamage) == 0x64);
        static_assert(offsetof(source2sdk::server::CDamageRecord, m_flActualHealthRemoved) == 0x68);
        static_assert(offsetof(source2sdk::server::CDamageRecord, m_iNumHits) == 0x6c);
        static_assert(offsetof(source2sdk::server::CDamageRecord, m_iLastBulletUpdate) == 0x70);
        static_assert(offsetof(source2sdk::server::CDamageRecord, m_bIsOtherEnemy) == 0x74);
        static_assert(offsetof(source2sdk::server::CDamageRecord, m_killType) == 0x75);
        
        static_assert(sizeof(source2sdk::server::CDamageRecord) == 0x78);
    };
};
