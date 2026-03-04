#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/GameTick_t.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CCSWeaponBase.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CCSWeaponBase;
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
        // Alignment: 0x10
        // Standard-layout class: false
        // Size: 0x1170
        // Has VTable
        // Construct disallowed
        // MConstructibleClassBase
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "bool m_bRedraw"
        // static metadata: MNetworkVarNames "bool m_bIsHeldByPlayer"
        // static metadata: MNetworkVarNames "bool m_bPinPulled"
        // static metadata: MNetworkVarNames "bool m_bJumpThrow"
        // static metadata: MNetworkVarNames "bool m_bThrowAnimating"
        // static metadata: MNetworkVarNames "GameTime_t m_fThrowTime"
        // static metadata: MNetworkVarNames "float m_flThrowStrength"
        // static metadata: MNetworkVarNames "GameTime_t m_fDropTime"
        // static metadata: MNetworkVarNames "GameTime_t m_fPinPullTime"
        // static metadata: MNetworkVarNames "bool m_bJustPulledPin"
        // static metadata: MNetworkVarNames "GameTick_t m_nNextHoldTick"
        // static metadata: MNetworkVarNames "float m_flNextHoldFrac"
        // static metadata: MNetworkVarNames "CHandle< CCSWeaponBase> m_hSwitchToWeaponAfterThrow"
        #pragma pack(push, 1)
        class CBaseCSGrenade : public source2sdk::server::CCSWeaponBase
        {
        public:
            // metadata: MNetworkEnable
            bool m_bRedraw; // 0x1130            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnGrenadeStateChanged"
            bool m_bIsHeldByPlayer; // 0x1131            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnGrenadeStateChanged"
            bool m_bPinPulled; // 0x1132            
            // metadata: MNetworkEnable
            bool m_bJumpThrow; // 0x1133            
            // metadata: MNetworkEnable
            bool m_bThrowAnimating; // 0x1134            
            uint8_t _pad1135[0x3]; // 0x1135
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fThrowTime; // 0x1138            
            // metadata: MNetworkEnable
            float m_flThrowStrength; // 0x113c            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fDropTime; // 0x1140            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fPinPullTime; // 0x1144            
            // metadata: MNetworkEnable
            bool m_bJustPulledPin; // 0x1148            
            uint8_t _pad1149[0x3]; // 0x1149
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTick_t m_nNextHoldTick; // 0x114c            
            // metadata: MNetworkEnable
            float m_flNextHoldFrac; // 0x1150            
            // metadata: MNetworkEnable
            // m_hSwitchToWeaponAfterThrow has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSWeaponBase> m_hSwitchToWeaponAfterThrow;
            char m_hSwitchToWeaponAfterThrow[0x4]; // 0x1154            
            uint8_t _pad1158[0x18];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CBaseCSGrenade because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CBaseCSGrenade) == 0x1170);
    };
};
