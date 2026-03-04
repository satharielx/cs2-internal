#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CSWeaponMode.hpp"
#include "source2sdk/client/WeaponGameplayAnimState.hpp"
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/entity2/GameTick_t.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CBasePlayerWeapon.hpp"
#include "source2sdk/server/CIronSightController.hpp"
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
        // Alignment: 0x10
        // Standard-layout class: false
        // Size: 0x1130
        // Has VTable
        // Construct disallowed
        // MConstructibleClassBase
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkOutOfPVSUpdates
        // static metadata: MNetworkVarNames "WeaponGameplayAnimState m_iWeaponGameplayAnimState"
        // static metadata: MNetworkVarNames "GameTime_t m_flWeaponGameplayAnimStateTimestamp"
        // static metadata: MNetworkVarNames "GameTime_t m_flInspectCancelCompleteTime"
        // static metadata: MNetworkVarNames "bool m_bInspectPending"
        // static metadata: MNetworkVarNames "bool m_bInspectShouldLoop"
        // static metadata: MNetworkVarNames "CSWeaponMode m_weaponMode"
        // static metadata: MNetworkVarNames "float m_fAccuracyPenalty"
        // static metadata: MNetworkVarNames "int m_iRecoilIndex"
        // static metadata: MNetworkVarNames "float m_flRecoilIndex"
        // static metadata: MNetworkVarNames "bool m_bBurstMode"
        // static metadata: MNetworkVarNames "GameTick_t m_nPostponeFireReadyTicks"
        // static metadata: MNetworkVarNames "float m_flPostponeFireReadyFrac"
        // static metadata: MNetworkVarNames "bool m_bInReload"
        // static metadata: MNetworkVarNames "GameTime_t m_flDroppedAtTime"
        // static metadata: MNetworkVarNames "bool m_bIsHauledBack"
        // static metadata: MNetworkVarNames "bool m_bSilencerOn"
        // static metadata: MNetworkVarNames "GameTime_t m_flTimeSilencerSwitchComplete"
        // static metadata: MNetworkVarNames "float m_flWeaponActionPlaybackRate"
        // static metadata: MNetworkVarNames "int m_iOriginalTeamNumber"
        // static metadata: MNetworkVarNames "int m_iMostRecentTeamNumber"
        // static metadata: MNetworkVarNames "bool m_bDroppedNearBuyZone"
        // static metadata: MNetworkVarNames "GameTime_t m_nextPrevOwnerUseTime"
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerPawn> m_hPrevOwner"
        // static metadata: MNetworkVarNames "GameTick_t m_nDropTick"
        // static metadata: MNetworkVarNames "bool m_bWasActiveWeaponWhenDropped"
        // static metadata: MNetworkVarNames "GameTime_t m_fLastShotTime"
        // static metadata: MNetworkVarNames "int m_iIronSightMode"
        // static metadata: MNetworkVarNames "float m_flWatTickOffset"
        // static metadata: MNetworkVarNames "GameTime_t m_flLastShakeTime"
        #pragma pack(push, 1)
        class CCSWeaponBase : public source2sdk::server::CBasePlayerWeapon
        {
        public:
            bool m_bRemoveable; // 0xe20            
            bool m_bPlayerAmmoStockOnPickup; // 0xe21            
            bool m_bRequireUseToTouch; // 0xe22            
            uint8_t _pad0e23[0x1]; // 0xe23
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "WeaponGameplayAnimStateNetworkChangeCallback"
            source2sdk::client::WeaponGameplayAnimState m_iWeaponGameplayAnimState; // 0xe24            
            uint8_t _pad0e26[0x2]; // 0xe26
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flWeaponGameplayAnimStateTimestamp; // 0xe28            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flInspectCancelCompleteTime; // 0xe2c            
            // metadata: MNetworkEnable
            bool m_bInspectPending; // 0xe30            
            // metadata: MNetworkEnable
            bool m_bInspectShouldLoop; // 0xe31            
            uint8_t _pad0e32[0x2a]; // 0xe32
            std::int32_t m_nLastEmptySoundCmdNum; // 0xe5c            
            uint8_t _pad0e60[0x18]; // 0xe60
            bool m_bFireOnEmpty; // 0xe78            
            uint8_t _pad0e79[0x7]; // 0xe79
            source2sdk::entity2::CEntityIOOutput m_OnPlayerPickup; // 0xe80            
            // metadata: MNetworkEnable
            source2sdk::client::CSWeaponMode m_weaponMode; // 0xe98            
            float m_flTurningInaccuracyDelta; // 0xe9c            
            Vector m_vecTurningInaccuracyEyeDirLast; // 0xea0            
            float m_flTurningInaccuracy; // 0xeac            
            // metadata: MNetworkEnable
            float m_fAccuracyPenalty; // 0xeb0            
            source2sdk::entity2::GameTime_t m_flLastAccuracyUpdateTime; // 0xeb4            
            float m_fAccuracySmoothedForZoom; // 0xeb8            
            // metadata: MNetworkEnable
            std::int32_t m_iRecoilIndex; // 0xebc            
            // metadata: MNetworkEnable
            float m_flRecoilIndex; // 0xec0            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnWeaponBurstModeNetworkChange"
            bool m_bBurstMode; // 0xec4            
            uint8_t _pad0ec5[0x3]; // 0xec5
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTick_t m_nPostponeFireReadyTicks; // 0xec8            
            // metadata: MNetworkEnable
            float m_flPostponeFireReadyFrac; // 0xecc            
            // metadata: MNetworkEnable
            bool m_bInReload; // 0xed0            
            uint8_t _pad0ed1[0x3]; // 0xed1
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flDroppedAtTime; // 0xed4            
            // metadata: MNetworkEnable
            bool m_bIsHauledBack; // 0xed8            
            // metadata: MNetworkEnable
            bool m_bSilencerOn; // 0xed9            
            uint8_t _pad0eda[0x2]; // 0xeda
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flTimeSilencerSwitchComplete; // 0xedc            
            // metadata: MNetworkEnable
            float m_flWeaponActionPlaybackRate; // 0xee0            
            // metadata: MNetworkEnable
            std::int32_t m_iOriginalTeamNumber; // 0xee4            
            // metadata: MNetworkEnable
            std::int32_t m_iMostRecentTeamNumber; // 0xee8            
            // metadata: MNetworkEnable
            bool m_bDroppedNearBuyZone; // 0xeec            
            uint8_t _pad0eed[0x3]; // 0xeed
            float m_flNextAttackRenderTimeOffset; // 0xef0            
            uint8_t _pad0ef4[0x14]; // 0xef4
            bool m_bCanBePickedUp; // 0xf08            
            bool m_bUseCanOverrideNextOwnerTouchTime; // 0xf09            
            uint8_t _pad0f0a[0x2]; // 0xf0a
            source2sdk::entity2::GameTime_t m_nextOwnerTouchTime; // 0xf0c            
            source2sdk::entity2::GameTime_t m_nextPrevOwnerTouchTime; // 0xf10            
            uint8_t _pad0f14[0x4]; // 0xf14
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_nextPrevOwnerUseTime; // 0xf18            
            // metadata: MNetworkEnable
            // m_hPrevOwner has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_hPrevOwner;
            char m_hPrevOwner[0x4]; // 0xf1c            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTick_t m_nDropTick; // 0xf20            
            // metadata: MNetworkEnable
            bool m_bWasActiveWeaponWhenDropped; // 0xf24            
            uint8_t _pad0f25[0x1f]; // 0xf25
            bool m_donated; // 0xf44            
            uint8_t _pad0f45[0x3]; // 0xf45
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fLastShotTime; // 0xf48            
            bool m_bWasOwnedByCT; // 0xf4c            
            bool m_bWasOwnedByTerrorist; // 0xf4d            
            uint8_t _pad0f4e[0x2]; // 0xf4e
            std::int32_t m_numRemoveUnownedWeaponThink; // 0xf50            
            uint8_t _pad0f54[0x4]; // 0xf54
            source2sdk::server::CIronSightController m_IronSightController; // 0xf58            
            // metadata: MNetworkEnable
            std::int32_t m_iIronSightMode; // 0xf70            
            source2sdk::entity2::GameTime_t m_flLastLOSTraceFailureTime; // 0xf74            
            // metadata: MNetworkEnable
            float m_flWatTickOffset; // 0xf78            
            uint8_t _pad0f7c[0xc]; // 0xf7c
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flLastShakeTime; // 0xf88            
            uint8_t _pad0f8c[0x1a4];
            
            // Datamap fields:
            // void CCSWeaponBaseDefaultTouch; // 0x0
            // void CCSWeaponBaseRemoveUnownedWeaponThink; // 0x0
            // void InputToggleCanBePickedUp; // 0x0
            // float InputSetAmmoAmount; // 0x0
            // int32_t InputSetReserveAmmoAmount; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSWeaponBase because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CCSWeaponBase) == 0x1130);
    };
};
