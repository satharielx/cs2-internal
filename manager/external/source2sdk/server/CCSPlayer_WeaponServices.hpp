#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CPlayer_WeaponServices.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CBasePlayerWeapon;
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
        // Standard-layout class: false
        // Size: 0x18d8
        // Has VTable
        // 
        // static metadata: MNetworkVarNames "GameTime_t m_flNextAttack"
        // static metadata: MNetworkVarNames "bool m_bIsLookingAtWeapon"
        // static metadata: MNetworkVarNames "bool m_bIsHoldingLookAtWeapon"
        // static metadata: MNetworkVarNames "uint8 m_networkAnimTiming"
        // static metadata: MNetworkVarNames "bool m_bBlockInspectUntilNextGraphUpdate"
        #pragma pack(push, 1)
        class CCSPlayer_WeaponServices : public source2sdk::server::CPlayer_WeaponServices
        {
        public:
            uint8_t _pad00b0[0x10]; // 0xb0
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            source2sdk::entity2::GameTime_t m_flNextAttack; // 0xc0            
            // metadata: MNetworkEnable
            bool m_bIsLookingAtWeapon; // 0xc4            
            // metadata: MNetworkEnable
            bool m_bIsHoldingLookAtWeapon; // 0xc5            
            uint8_t _pad00c6[0x2]; // 0xc6
            // m_hSavedWeapon has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBasePlayerWeapon> m_hSavedWeapon;
            char m_hSavedWeapon[0x4]; // 0xc8            
            std::int32_t m_nTimeToMelee; // 0xcc            
            std::int32_t m_nTimeToSecondary; // 0xd0            
            std::int32_t m_nTimeToPrimary; // 0xd4            
            std::int32_t m_nTimeToSniperRifle; // 0xd8            
            bool m_bIsBeingGivenItem; // 0xdc            
            bool m_bIsPickingUpItemWithUse; // 0xdd            
            bool m_bPickedUpWeapon; // 0xde            
            bool m_bDisableAutoDeploy; // 0xdf            
            bool m_bIsPickingUpGroundWeapon; // 0xe0            
            uint8_t _pad00e1[0x17d7]; // 0xe1
            // metadata: MNetworkEnable
            // m_networkAnimTiming has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CNetworkUtlVectorBase<std::uint8_t> m_networkAnimTiming;
            char m_networkAnimTiming[0x18]; // 0x18b8            
            // metadata: MNetworkEnable
            bool m_bBlockInspectUntilNextGraphUpdate; // 0x18d0            
            uint8_t _pad18d1[0x7];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSPlayer_WeaponServices because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CCSPlayer_WeaponServices) == 0x18d8);
    };
};
