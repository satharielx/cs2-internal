#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_CSWeaponBase.hpp"
#include "source2sdk/entity2/GameTick_t.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
namespace source2sdk
{
    namespace client
    {
        struct C_CSWeaponBase;
    };
};

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
        // Size: 0x2000
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
        // static metadata: MNetworkVarNames "GameTime_t m_fDropTime"
        // static metadata: MNetworkVarNames "GameTime_t m_fPinPullTime"
        // static metadata: MNetworkVarNames "bool m_bJustPulledPin"
        // static metadata: MNetworkVarNames "GameTick_t m_nNextHoldTick"
        // static metadata: MNetworkVarNames "float m_flNextHoldFrac"
        // static metadata: MNetworkVarNames "CHandle< CCSWeaponBase> m_hSwitchToWeaponAfterThrow"
        #pragma pack(push, 1)
        class C_BaseCSGrenade : public source2sdk::client::C_CSWeaponBase
        {
        public:
            bool m_bClientPredictDelete; // 0x1f40            
            // metadata: MNetworkEnable
            bool m_bRedraw; // 0x1f41            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnGrenadeStateChanged"
            bool m_bIsHeldByPlayer; // 0x1f42            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnGrenadeStateChanged"
            bool m_bPinPulled; // 0x1f43            
            // metadata: MNetworkEnable
            bool m_bJumpThrow; // 0x1f44            
            // metadata: MNetworkEnable
            bool m_bThrowAnimating; // 0x1f45            
            uint8_t _pad1f46[0x2]; // 0x1f46
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fThrowTime; // 0x1f48            
            uint8_t _pad1f4c[0x4]; // 0x1f4c
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnThrowStrengthChanged"
            float m_flThrowStrength; // 0x1f50            
            uint8_t _pad1f54[0x74]; // 0x1f54
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fDropTime; // 0x1fc8            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fPinPullTime; // 0x1fcc            
            // metadata: MNetworkEnable
            bool m_bJustPulledPin; // 0x1fd0            
            uint8_t _pad1fd1[0x3]; // 0x1fd1
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTick_t m_nNextHoldTick; // 0x1fd4            
            // metadata: MNetworkEnable
            float m_flNextHoldFrac; // 0x1fd8            
            // metadata: MNetworkEnable
            // m_hSwitchToWeaponAfterThrow has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_CSWeaponBase> m_hSwitchToWeaponAfterThrow;
            char m_hSwitchToWeaponAfterThrow[0x4]; // 0x1fdc            
            uint8_t _pad1fe0[0x20];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_BaseCSGrenade because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_BaseCSGrenade) == 0x2000);
    };
};
