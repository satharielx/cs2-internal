#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CSPlayerBlockingUseAction_t.hpp"
#include "source2sdk/client/C_CSPlayerPawnBase.hpp"
#include "source2sdk/client/C_EconItemView.hpp"
#include "source2sdk/client/CountdownTimer.hpp"
#include "source2sdk/client/EntitySpottedState_t.hpp"
#include "source2sdk/client/ParticleIndex_t.hpp"
#include "source2sdk/client/PredictedDamageTag_t.hpp"
#include "source2sdk/client/loadout_slot_t.hpp"
#include "source2sdk/entity2/GameTick_t.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
namespace source2sdk
{
    namespace client
    {
        struct CCSPlayer_ActionTrackingServices;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct CCSPlayer_BulletServices;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct CCSPlayer_BuyServices;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct CCSPlayer_DamageReactServices;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct CCSPlayer_GlowServices;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct CCSPlayer_HostageServices;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct C_BulletHitModel;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct C_CS2HudModelArms;
    };
};
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
        // Registered alignment: 0x10
        // Alignment: 0x10
        // Standard-layout class: false
        // Size: 0x3ef0
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "CCSPlayer_BulletServices * m_pBulletServices"
        // static metadata: MNetworkVarNames "CCSPlayer_HostageServices * m_pHostageServices"
        // static metadata: MNetworkVarNames "CCSPlayer_BuyServices * m_pBuyServices"
        // static metadata: MNetworkVarNames "CCSPlayer_GlowServices * m_pGlowServices"
        // static metadata: MNetworkVarNames "CCSPlayer_ActionTrackingServices * m_pActionTrackingServices"
        // static metadata: MNetworkVarTypeOverride "CCSPlayer_UseServices m_pUseServices"
        // static metadata: MNetworkIncludeByName "m_pUseServices"
        // static metadata: MNetworkVarTypeOverride "CCSPlayer_ItemServices m_pItemServices"
        // static metadata: MNetworkIncludeByName "m_pItemServices"
        // static metadata: MNetworkVarTypeOverride "CCSPlayer_MovementServices m_pMovementServices"
        // static metadata: MNetworkIncludeByName "m_pMovementServices"
        // static metadata: MNetworkVarTypeOverride "CCSPlayer_WaterServices m_pWaterServices"
        // static metadata: MNetworkIncludeByName "m_pWaterServices"
        // static metadata: MNetworkVarTypeOverride "CCSPlayer_CameraServices m_pCameraServices"
        // static metadata: MNetworkIncludeByName "m_pCameraServices"
        // static metadata: MNetworkVarTypeOverride "CCSPlayer_WeaponServices m_pWeaponServices"
        // static metadata: MNetworkIncludeByName "m_pWeaponServices"
        // static metadata: MNetworkIncludeByName "m_ArmorValue"
        // static metadata: MNetworkVarNames "GameTime_t m_flHealthShotBoostExpirationTime"
        // static metadata: MNetworkVarNames "bool m_bHasFemaleVoice"
        // static metadata: MNetworkVarNames "char m_szLastPlaceName"
        // static metadata: MNetworkVarNames "bool m_bInBuyZone"
        // static metadata: MNetworkVarNames "QAngle m_aimPunchAngle"
        // static metadata: MNetworkVarNames "QAngle m_aimPunchAngleVel"
        // static metadata: MNetworkVarNames "GameTick_t m_aimPunchTickBase"
        // static metadata: MNetworkVarNames "float m_aimPunchTickFraction"
        // static metadata: MNetworkVarNames "bool m_bInHostageRescueZone"
        // static metadata: MNetworkVarNames "bool m_bInBombZone"
        // static metadata: MNetworkVarNames "bool m_bIsBuyMenuOpen"
        // static metadata: MNetworkVarNames "GameTime_t m_flTimeOfLastInjury"
        // static metadata: MNetworkVarNames "GameTime_t m_flNextSprayDecalTime"
        // static metadata: MNetworkVarNames "int m_iRetakesOffering"
        // static metadata: MNetworkVarNames "int m_iRetakesOfferingCard"
        // static metadata: MNetworkVarNames "bool m_bRetakesHasDefuseKit"
        // static metadata: MNetworkVarNames "bool m_bRetakesMVPLastRound"
        // static metadata: MNetworkVarNames "int m_iRetakesMVPBoostItem"
        // static metadata: MNetworkVarNames "loadout_slot_t m_RetakesMVPBoostExtraUtility"
        // static metadata: MNetworkVarNames "CEconItemView m_EconGloves"
        // static metadata: MNetworkVarNames "uint8 m_nEconGlovesChanged"
        // static metadata: MNetworkVarNames "int m_nRagdollDamageBone"
        // static metadata: MNetworkVarNames "Vector m_vRagdollDamageForce"
        // static metadata: MNetworkVarNames "Vector m_vRagdollDamagePosition"
        // static metadata: MNetworkVarNames "char m_szRagdollDamageWeaponName"
        // static metadata: MNetworkVarNames "bool m_bRagdollDamageHeadshot"
        // static metadata: MNetworkVarNames "Vector m_vRagdollServerOrigin"
        // static metadata: MNetworkReplayCompatField "m_bClientRagdoll"
        // static metadata: MNetworkVarNames "QAngle m_qDeathEyeAngles"
        // static metadata: MNetworkVarNames "bool m_bLeftHanded"
        // static metadata: MNetworkVarNames "GameTime_t m_fSwitchedHandednessTime"
        // static metadata: MNetworkVarNames "float m_flViewmodelOffsetX"
        // static metadata: MNetworkVarNames "float m_flViewmodelOffsetY"
        // static metadata: MNetworkVarNames "float m_flViewmodelOffsetZ"
        // static metadata: MNetworkVarNames "float m_flViewmodelFOV"
        // static metadata: MNetworkVarNames "uint32 m_vecPlayerPatchEconIndices"
        // static metadata: MNetworkVarNames "Color m_GunGameImmunityColor"
        // static metadata: MNetworkVarNames "bool m_bIsWalking"
        // static metadata: MNetworkVarNames "EntitySpottedState_t m_entitySpottedState"
        // static metadata: MNetworkVarNames "bool m_bIsScoped"
        // static metadata: MNetworkVarNames "bool m_bResumeZoom"
        // static metadata: MNetworkVarNames "bool m_bIsDefusing"
        // static metadata: MNetworkVarNames "bool m_bIsGrabbingHostage"
        // static metadata: MNetworkVarNames "CSPlayerBlockingUseAction_t m_iBlockingUseActionInProgress"
        // static metadata: MNetworkVarNames "GameTime_t m_flEmitSoundTime"
        // static metadata: MNetworkVarNames "bool m_bInNoDefuseArea"
        // static metadata: MNetworkVarNames "int m_nWhichBombZone"
        // static metadata: MNetworkVarNames "int m_iShotsFired"
        // static metadata: MNetworkVarNames "float m_flFlinchStack"
        // static metadata: MNetworkVarNames "float m_flVelocityModifier"
        // static metadata: MNetworkVarNames "float m_flHitHeading"
        // static metadata: MNetworkVarNames "int m_nHitBodyPart"
        // static metadata: MNetworkVarNames "bool m_bWaitForNoAttack"
        // static metadata: MNetworkVarNames "bool m_bKilledByHeadshot"
        // static metadata: MNetworkVarNames "int32 m_ArmorValue"
        // static metadata: MNetworkVarNames "uint16 m_unCurrentEquipmentValue"
        // static metadata: MNetworkVarNames "uint16 m_unRoundStartEquipmentValue"
        // static metadata: MNetworkVarNames "uint16 m_unFreezetimeEndEquipmentValue"
        // static metadata: MNetworkVarNames "CEntityIndex m_nLastKillerIndex"
        // static metadata: MNetworkVarNames "PredictedDamageTag_t m_PredictedDamageTags"
        // static metadata: MNetworkVarNames "GameTime_t m_fImmuneToGunGameDamageTime"
        // static metadata: MNetworkVarNames "bool m_bGunGameImmunity"
        // static metadata: MNetworkVarNames "float m_fMolotovDamageTime"
        #pragma pack(push, 1)
        class C_CSPlayerPawn : public source2sdk::client::C_CSPlayerPawnBase
        {
        public:
            uint8_t _pad1650[0x10]; // 0x1650
            // metadata: MNetworkEnable
            source2sdk::client::CCSPlayer_BulletServices* m_pBulletServices; // 0x1660            
            // metadata: MNetworkEnable
            source2sdk::client::CCSPlayer_HostageServices* m_pHostageServices; // 0x1668            
            // metadata: MNetworkEnable
            source2sdk::client::CCSPlayer_BuyServices* m_pBuyServices; // 0x1670            
            // metadata: MNetworkEnable
            source2sdk::client::CCSPlayer_GlowServices* m_pGlowServices; // 0x1678            
            // metadata: MNetworkEnable
            source2sdk::client::CCSPlayer_ActionTrackingServices* m_pActionTrackingServices; // 0x1680            
            source2sdk::client::CCSPlayer_DamageReactServices* m_pDamageReactServices; // 0x1688            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flHealthShotBoostExpirationTime; // 0x1690            
            source2sdk::entity2::GameTime_t m_flLastFiredWeaponTime; // 0x1694            
            // metadata: MNetworkEnable
            bool m_bHasFemaleVoice; // 0x1698            
            uint8_t _pad1699[0x3]; // 0x1699
            float m_flLandingTimeSeconds; // 0x169c            
            float m_flOldFallVelocity; // 0x16a0            
            // metadata: MNetworkEnable
            char m_szLastPlaceName[18]; // 0x16a4            
            bool m_bPrevDefuser; // 0x16b6            
            bool m_bPrevHelmet; // 0x16b7            
            std::int32_t m_nPrevArmorVal; // 0x16b8            
            std::int32_t m_nPrevGrenadeAmmoCount; // 0x16bc            
            std::uint32_t m_unPreviousWeaponHash; // 0x16c0            
            std::uint32_t m_unWeaponHash; // 0x16c4            
            // metadata: MNetworkEnable
            bool m_bInBuyZone; // 0x16c8            
            bool m_bPreviouslyInBuyZone; // 0x16c9            
            uint8_t _pad16ca[0x2]; // 0x16ca
            // metadata: MNetworkEnable
            // metadata: MNetworkBitCount "32"
            QAngle m_aimPunchAngle; // 0x16cc            
            // metadata: MNetworkEnable
            // metadata: MNetworkBitCount "32"
            QAngle m_aimPunchAngleVel; // 0x16d8            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTick_t m_aimPunchTickBase; // 0x16e4            
            // metadata: MNetworkEnable
            float m_aimPunchTickFraction; // 0x16e8            
            uint8_t _pad16ec[0x24]; // 0x16ec
            bool m_bInLanding; // 0x1710            
            uint8_t _pad1711[0x3]; // 0x1711
            float m_flLandingStartTime; // 0x1714            
            // metadata: MNetworkEnable
            bool m_bInHostageRescueZone; // 0x1718            
            // metadata: MNetworkEnable
            bool m_bInBombZone; // 0x1719            
            // metadata: MNetworkEnable
            bool m_bIsBuyMenuOpen; // 0x171a            
            uint8_t _pad171b[0x1]; // 0x171b
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flTimeOfLastInjury; // 0x171c            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            source2sdk::entity2::GameTime_t m_flNextSprayDecalTime; // 0x1720            
            uint8_t _pad1724[0x154]; // 0x1724
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnRetakesOfferingChanged"
            std::int32_t m_iRetakesOffering; // 0x1878            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnRetakesOfferingCardChanged"
            std::int32_t m_iRetakesOfferingCard; // 0x187c            
            // metadata: MNetworkEnable
            bool m_bRetakesHasDefuseKit; // 0x1880            
            // metadata: MNetworkEnable
            bool m_bRetakesMVPLastRound; // 0x1881            
            uint8_t _pad1882[0x2]; // 0x1882
            // metadata: MNetworkEnable
            std::int32_t m_iRetakesMVPBoostItem; // 0x1884            
            // metadata: MNetworkEnable
            source2sdk::client::loadout_slot_t m_RetakesMVPBoostExtraUtility; // 0x1888            
            uint8_t _pad188c[0x1]; // 0x188c
            bool m_bNeedToReApplyGloves; // 0x188d            
            uint8_t _pad188e[0x2]; // 0x188e
            // metadata: MNetworkEnable
            source2sdk::client::C_EconItemView m_EconGloves; // 0x1890            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnEconGlovesChanged"
            std::uint8_t m_nEconGlovesChanged; // 0x1d00            
            bool m_bMustSyncRagdollState; // 0x1d01            
            uint8_t _pad1d02[0x2]; // 0x1d02
            // metadata: MNetworkEnable
            std::int32_t m_nRagdollDamageBone; // 0x1d04            
            // metadata: MNetworkEnable
            Vector m_vRagdollDamageForce; // 0x1d08            
            // metadata: MNetworkEnable
            Vector m_vRagdollDamagePosition; // 0x1d14            
            // metadata: MNetworkEnable
            char m_szRagdollDamageWeaponName[64]; // 0x1d20            
            // metadata: MNetworkEnable
            bool m_bRagdollDamageHeadshot; // 0x1d60            
            uint8_t _pad1d61[0x3]; // 0x1d61
            // metadata: MNetworkEnable
            Vector m_vRagdollServerOrigin; // 0x1d64            
            uint8_t _pad1d70[0x670]; // 0x1d70
            source2sdk::entity2::GameTime_t m_lastLandTime; // 0x23e0            
            bool m_bOnGroundLastTick; // 0x23e4            
            uint8_t _pad23e5[0x1b]; // 0x23e5
            // m_hHudModelArms has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_CS2HudModelArms> m_hHudModelArms;
            char m_hHudModelArms[0x4]; // 0x2400            
            // metadata: MNetworkEnable
            QAngle m_qDeathEyeAngles; // 0x2404            
            bool m_bSkipOneHeadConstraintUpdate; // 0x2410            
            // metadata: MNetworkEnable
            bool m_bLeftHanded; // 0x2411            
            uint8_t _pad2412[0x2]; // 0x2412
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fSwitchedHandednessTime; // 0x2414            
            // metadata: MNetworkEnable
            // metadata: MNetworkMinValue "-2,000000"
            // metadata: MNetworkMaxValue "2,500000"
            float m_flViewmodelOffsetX; // 0x2418            
            // metadata: MNetworkEnable
            // metadata: MNetworkMinValue "-2,000000"
            // metadata: MNetworkMaxValue "2,000000"
            float m_flViewmodelOffsetY; // 0x241c            
            // metadata: MNetworkEnable
            // metadata: MNetworkMinValue "-2,000000"
            // metadata: MNetworkMaxValue "2,000000"
            float m_flViewmodelOffsetZ; // 0x2420            
            // metadata: MNetworkEnable
            // metadata: MNetworkMinValue "60,000000"
            // metadata: MNetworkMaxValue "68,000000"
            float m_flViewmodelFOV; // 0x2424            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "RecvProxy_PatchEconIndices"
            std::uint32_t m_vecPlayerPatchEconIndices[5]; // 0x2428            
            uint8_t _pad243c[0x24]; // 0x243c
            // metadata: MNetworkEnable
            Color m_GunGameImmunityColor; // 0x2460            
            uint8_t _pad2464[0x4c]; // 0x2464
            // m_vecBulletHitModels has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<source2sdk::client::C_BulletHitModel*> m_vecBulletHitModels;
            char m_vecBulletHitModels[0x18]; // 0x24b0            
            // metadata: MNetworkEnable
            bool m_bIsWalking; // 0x24c8            
            uint8_t _pad24c9[0x7]; // 0x24c9
            // metadata: MNetworkEnable
            // metadata: MNetworkEncoder "qangle_precise"
            // metadata: MNetworkChangeCallback "OnThirdPersonHeadingChanged"
            // metadata: MNetworkPriority "32"
            QAngle m_thirdPersonHeading; // 0x24d0            
            uint8_t _pad24dc[0x84]; // 0x24dc
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnSlopeDropOffsetChanged"
            // metadata: MNetworkPriority "32"
            float m_flSlopeDropOffset; // 0x2560            
            uint8_t _pad2564[0x74]; // 0x2564
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnSlopeDropHeightChanged"
            // metadata: MNetworkPriority "32"
            float m_flSlopeDropHeight; // 0x25d8            
            uint8_t _pad25dc[0x74]; // 0x25dc
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnHeadConstraintChanged"
            // metadata: MNetworkPriority "32"
            Vector m_vHeadConstraintOffset; // 0x2650            
            uint8_t _pad265c[0x84]; // 0x265c
            // metadata: MNetworkEnable
            source2sdk::client::EntitySpottedState_t m_entitySpottedState; // 0x26e0            
            // metadata: MNetworkEnable
            bool m_bIsScoped; // 0x26f8            
            // metadata: MNetworkEnable
            bool m_bResumeZoom; // 0x26f9            
            // metadata: MNetworkEnable
            bool m_bIsDefusing; // 0x26fa            
            // metadata: MNetworkEnable
            bool m_bIsGrabbingHostage; // 0x26fb            
            // metadata: MNetworkEnable
            source2sdk::client::CSPlayerBlockingUseAction_t m_iBlockingUseActionInProgress; // 0x26fc            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flEmitSoundTime; // 0x2700            
            // metadata: MNetworkEnable
            bool m_bInNoDefuseArea; // 0x2704            
            uint8_t _pad2705[0x3]; // 0x2705
            // metadata: MNetworkEnable
            std::int32_t m_nWhichBombZone; // 0x2708            
            // metadata: MNetworkEnable
            std::int32_t m_iShotsFired; // 0x270c            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            float m_flFlinchStack; // 0x2710            
            // metadata: MNetworkEnable
            float m_flVelocityModifier; // 0x2714            
            // metadata: MNetworkEnable
            float m_flHitHeading; // 0x2718            
            // metadata: MNetworkEnable
            std::int32_t m_nHitBodyPart; // 0x271c            
            // metadata: MNetworkEnable
            bool m_bWaitForNoAttack; // 0x2720            
            uint8_t _pad2721[0x3]; // 0x2721
            float m_ignoreLadderJumpTime; // 0x2724            
            uint8_t _pad2728[0x1]; // 0x2728
            // metadata: MNetworkEnable
            bool m_bKilledByHeadshot; // 0x2729            
            uint8_t _pad272a[0x2]; // 0x272a
            // metadata: MNetworkEnable
            std::int32_t m_ArmorValue; // 0x272c            
            // metadata: MNetworkEnable
            std::uint16_t m_unCurrentEquipmentValue; // 0x2730            
            // metadata: MNetworkEnable
            std::uint16_t m_unRoundStartEquipmentValue; // 0x2732            
            // metadata: MNetworkEnable
            std::uint16_t m_unFreezetimeEndEquipmentValue; // 0x2734            
            uint8_t _pad2736[0x2]; // 0x2736
            // metadata: MNetworkEnable
            CEntityIndex m_nLastKillerIndex; // 0x2738            
            bool m_bOldIsScoped; // 0x273c            
            bool m_bHasDeathInfo; // 0x273d            
            uint8_t _pad273e[0x2]; // 0x273e
            float m_flDeathInfoTime; // 0x2740            
            Vector m_vecDeathInfoOrigin; // 0x2744            
            uint8_t _pad2750[0x4]; // 0x2750
            source2sdk::entity2::GameTime_t m_grenadeParameterStashTime; // 0x2754            
            bool m_bGrenadeParametersStashed; // 0x2758            
            uint8_t _pad2759[0x3]; // 0x2759
            QAngle m_angStashedShootAngles; // 0x275c            
            Vector m_vecStashedGrenadeThrowPosition; // 0x2768            
            Vector m_vecStashedVelocity; // 0x2774            
            QAngle m_angShootAngleHistory[2]; // 0x2780            
            Vector m_vecThrowPositionHistory[2]; // 0x2798            
            Vector m_vecVelocityHistory[2]; // 0x27b0            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            // metadata: MNetworkChangeCallback "OnPredictedDamageTagsChanged"
            // m_PredictedDamageTags has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // C_UtlVectorEmbeddedNetworkVar<source2sdk::client::PredictedDamageTag_t> m_PredictedDamageTags;
            char m_PredictedDamageTags[0x68]; // 0x27c8            
            source2sdk::entity2::GameTick_t m_nPrevHighestReceivedDamageTagTick; // 0x2830            
            std::int32_t m_nHighestAppliedDamageTagTick; // 0x2834            
            uint8_t _pad2838[0x1534]; // 0x2838
            bool m_bShouldAutobuyDMWeapons; // 0x3d6c            
            uint8_t _pad3d6d[0x3]; // 0x3d6d
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fImmuneToGunGameDamageTime; // 0x3d70            
            // metadata: MNetworkEnable
            bool m_bGunGameImmunity; // 0x3d74            
            uint8_t _pad3d75[0x3]; // 0x3d75
            source2sdk::entity2::GameTime_t m_fImmuneToGunGameDamageTimeLast; // 0x3d78            
            // metadata: MNetworkEnable
            float m_fMolotovDamageTime; // 0x3d7c            
            uint8_t _pad3d80[0x4]; // 0x3d80
            Vector m_vecLastAliveLocalVelocity; // 0x3d84            
            float m_fRenderingClipPlane[4]; // 0x3d90            
            std::int32_t m_nLastClipPlaneSetupFrame; // 0x3da0            
            Vector m_vecLastClipCameraPos; // 0x3da4            
            Vector m_vecLastClipCameraForward; // 0x3db0            
            bool m_bClipHitStaticWorld; // 0x3dbc            
            bool m_bCachedPlaneIsValid; // 0x3dbd            
            uint8_t _pad3dbe[0x2]; // 0x3dbe
            source2sdk::client::C_CSWeaponBase* m_pClippingWeapon; // 0x3dc0            
            source2sdk::client::ParticleIndex_t m_nPlayerInfernoBodyFx; // 0x3dc8            
            uint8_t _pad3dcc[0x4]; // 0x3dcc
            // metadata: MNetworkEnable
            // metadata: MNetworkEncoder "qangle_precise"
            // metadata: MNetworkChangeCallback "playerEyeAnglesChanged"
            // metadata: MNetworkPriority "32"
            QAngle m_angEyeAngles; // 0x3dd0            
            uint8_t _pad3ddc[0x84]; // 0x3ddc
            source2sdk::entity2::GameTime_t m_arrOldEyeAnglesTimes[4]; // 0x3e60            
            QAngle m_arrOldEyeAngles[4]; // 0x3e70            
            QAngle m_angEyeAnglesVelocity; // 0x3ea0            
            CEntityIndex m_iIDEntIndex; // 0x3eac            
            source2sdk::client::CountdownTimer m_delayTargetIDTimer; // 0x3eb0            
            CEntityIndex m_iTargetItemEntIdx; // 0x3ec8            
            CEntityIndex m_iOldIDEntIndex; // 0x3ecc            
            source2sdk::client::CountdownTimer m_holdTargetIDTimer; // 0x3ed0            
            uint8_t _pad3ee8[0x8];
            
            // Datamap fields:
            // CCSPlayer_ItemServices m_pItemServices; // 0x13e0
            // CCSPlayer_UseServices m_pUseServices; // 0x1400
            // CCSPlayer_WaterServices m_pWaterServices; // 0x13f8
            // CCSPlayer_MovementServices m_pMovementServices; // 0x1418
            // CCSPlayer_WeaponServices m_pWeaponServices; // 0x13d8
            // CCSPlayer_CameraServices m_pCameraServices; // 0x1410
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_CSPlayerPawn because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_CSPlayerPawn) == 0x3ef0);
    };
};
