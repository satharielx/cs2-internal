#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CSWeaponMode.hpp"
#include "source2sdk/client/C_BasePlayerWeapon.hpp"
#include "source2sdk/client/C_IronSightController.hpp"
#include "source2sdk/client/WeaponGameplayAnimState.hpp"
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/entity2/GameTick_t.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
namespace source2sdk
{
    namespace client
    {
        struct C_CSPlayerPawn;
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
        // Size: 0x1f40
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
        class C_CSWeaponBase : public source2sdk::client::C_BasePlayerWeapon
        {
        public:
            uint8_t _pad18f8[0x50]; // 0x18f8
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "WeaponGameplayAnimStateNetworkChangeCallback"
            source2sdk::client::WeaponGameplayAnimState m_iWeaponGameplayAnimState; // 0x1948            
            uint8_t _pad194a[0x2]; // 0x194a
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flWeaponGameplayAnimStateTimestamp; // 0x194c            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flInspectCancelCompleteTime; // 0x1950            
            // metadata: MNetworkEnable
            bool m_bInspectPending; // 0x1954            
            // metadata: MNetworkEnable
            bool m_bInspectShouldLoop; // 0x1955            
            uint8_t _pad1956[0x2a]; // 0x1956
            float m_flCrosshairDistance; // 0x1980            
            std::int32_t m_iAmmoLastCheck; // 0x1984            
            std::int32_t m_nLastEmptySoundCmdNum; // 0x1988            
            bool m_bFireOnEmpty; // 0x198c            
            uint8_t _pad198d[0x3]; // 0x198d
            source2sdk::entity2::CEntityIOOutput m_OnPlayerPickup; // 0x1990            
            // metadata: MNetworkEnable
            source2sdk::client::CSWeaponMode m_weaponMode; // 0x19a8            
            float m_flTurningInaccuracyDelta; // 0x19ac            
            Vector m_vecTurningInaccuracyEyeDirLast; // 0x19b0            
            float m_flTurningInaccuracy; // 0x19bc            
            // metadata: MNetworkEnable
            float m_fAccuracyPenalty; // 0x19c0            
            source2sdk::entity2::GameTime_t m_flLastAccuracyUpdateTime; // 0x19c4            
            float m_fAccuracySmoothedForZoom; // 0x19c8            
            // metadata: MNetworkEnable
            std::int32_t m_iRecoilIndex; // 0x19cc            
            // metadata: MNetworkEnable
            float m_flRecoilIndex; // 0x19d0            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnWeaponBurstModeNetworkChange"
            bool m_bBurstMode; // 0x19d4            
            uint8_t _pad19d5[0x3]; // 0x19d5
            source2sdk::entity2::GameTime_t m_flLastBurstModeChangeTime; // 0x19d8            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTick_t m_nPostponeFireReadyTicks; // 0x19dc            
            // metadata: MNetworkEnable
            float m_flPostponeFireReadyFrac; // 0x19e0            
            // metadata: MNetworkEnable
            bool m_bInReload; // 0x19e4            
            uint8_t _pad19e5[0x3]; // 0x19e5
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flDroppedAtTime; // 0x19e8            
            // metadata: MNetworkEnable
            bool m_bIsHauledBack; // 0x19ec            
            // metadata: MNetworkEnable
            bool m_bSilencerOn; // 0x19ed            
            uint8_t _pad19ee[0x2]; // 0x19ee
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flTimeSilencerSwitchComplete; // 0x19f0            
            // metadata: MNetworkEnable
            float m_flWeaponActionPlaybackRate; // 0x19f4            
            // metadata: MNetworkEnable
            std::int32_t m_iOriginalTeamNumber; // 0x19f8            
            // metadata: MNetworkEnable
            std::int32_t m_iMostRecentTeamNumber; // 0x19fc            
            // metadata: MNetworkEnable
            bool m_bDroppedNearBuyZone; // 0x1a00            
            uint8_t _pad1a01[0x3]; // 0x1a01
            float m_flNextAttackRenderTimeOffset; // 0x1a04            
            uint8_t _pad1a08[0x98]; // 0x1a08
            bool m_bClearWeaponIdentifyingUGC; // 0x1aa0            
            bool m_bVisualsDataSet; // 0x1aa1            
            bool m_bUIWeapon; // 0x1aa2            
            uint8_t _pad1aa3[0x1]; // 0x1aa3
            std::int32_t m_nCustomEconReloadEventId; // 0x1aa4            
            uint8_t _pad1aa8[0x8]; // 0x1aa8
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_nextPrevOwnerUseTime; // 0x1ab0            
            // metadata: MNetworkEnable
            // m_hPrevOwner has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_CSPlayerPawn> m_hPrevOwner;
            char m_hPrevOwner[0x4]; // 0x1ab4            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTick_t m_nDropTick; // 0x1ab8            
            // metadata: MNetworkEnable
            bool m_bWasActiveWeaponWhenDropped; // 0x1abc            
            uint8_t _pad1abd[0x1f]; // 0x1abd
            bool m_donated; // 0x1adc            
            uint8_t _pad1add[0x3]; // 0x1add
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fLastShotTime; // 0x1ae0            
            bool m_bWasOwnedByCT; // 0x1ae4            
            bool m_bWasOwnedByTerrorist; // 0x1ae5            
            uint8_t _pad1ae6[0x2]; // 0x1ae6
            float m_flNextClientFireBulletTime; // 0x1ae8            
            float m_flNextClientFireBulletTime_Repredict; // 0x1aec            
            uint8_t _pad1af0[0x160]; // 0x1af0
            source2sdk::client::C_IronSightController m_IronSightController; // 0x1c50            
            // metadata: MNetworkEnable
            std::int32_t m_iIronSightMode; // 0x1d00            
            uint8_t _pad1d04[0x14]; // 0x1d04
            source2sdk::entity2::GameTime_t m_flLastLOSTraceFailureTime; // 0x1d18            
            uint8_t _pad1d1c[0x5c]; // 0x1d1c
            // metadata: MNetworkEnable
            float m_flWatTickOffset; // 0x1d78            
            uint8_t _pad1d7c[0x10]; // 0x1d7c
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flLastShakeTime; // 0x1d8c            
            uint8_t _pad1d90[0x1b0];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_CSWeaponBase because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_CSWeaponBase) == 0x1f40);
    };
};
