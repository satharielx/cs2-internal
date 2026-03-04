#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CSPlayerBlockingUseAction_t.hpp"
#include "source2sdk/client/loadout_slot_t.hpp"
#include "source2sdk/entity2/GameTick_t.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CCSPlayerPawnBase.hpp"
#include "source2sdk/server/CEconItemView.hpp"
#include "source2sdk/server/EntitySpottedState_t.hpp"
#include "source2sdk/server/PredictedDamageTag_t.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CBaseEntity;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CCSBot;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayer_ActionTrackingServices;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayer_BulletServices;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayer_BuyServices;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayer_DamageReactServices;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayer_HostageServices;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayer_RadioServices;
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
        // Registered alignment: 0x10
        // Alignment: 0x10
        // Standard-layout class: false
        // Size: 0x1c30
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkOutOfPVSUpdates
        // static metadata: MNetworkVarTypeOverride "CCSPlayer_WeaponServices m_pWeaponServices"
        // static metadata: MNetworkIncludeByName "m_pWeaponServices"
        // static metadata: MNetworkVarTypeOverride "CCSPlayer_ItemServices m_pItemServices"
        // static metadata: MNetworkIncludeByName "m_pItemServices"
        // static metadata: MNetworkVarTypeOverride "CCSPlayer_UseServices m_pUseServices"
        // static metadata: MNetworkIncludeByName "m_pUseServices"
        // static metadata: MNetworkVarTypeOverride "CCSPlayer_WaterServices m_pWaterServices"
        // static metadata: MNetworkIncludeByName "m_pWaterServices"
        // static metadata: MNetworkVarTypeOverride "CCSPlayer_MovementServices m_pMovementServices"
        // static metadata: MNetworkIncludeByName "m_pMovementServices"
        // static metadata: MNetworkVarTypeOverride "CCSPlayer_CameraServices m_pCameraServices"
        // static metadata: MNetworkIncludeByName "m_pCameraServices"
        // static metadata: MNetworkVarNames "CCSPlayer_BulletServices * m_pBulletServices"
        // static metadata: MNetworkVarNames "CCSPlayer_HostageServices * m_pHostageServices"
        // static metadata: MNetworkVarNames "CCSPlayer_BuyServices * m_pBuyServices"
        // static metadata: MNetworkVarNames "CCSPlayer_ActionTrackingServices * m_pActionTrackingServices"
        // static metadata: MNetworkVarNames "bool m_bHasFemaleVoice"
        // static metadata: MNetworkVarNames "char m_szLastPlaceName"
        // static metadata: MNetworkVarNames "bool m_bInBuyZone"
        // static metadata: MNetworkVarNames "bool m_bInHostageRescueZone"
        // static metadata: MNetworkVarNames "bool m_bInBombZone"
        // static metadata: MNetworkVarNames "int m_iRetakesOffering"
        // static metadata: MNetworkVarNames "int m_iRetakesOfferingCard"
        // static metadata: MNetworkVarNames "bool m_bRetakesHasDefuseKit"
        // static metadata: MNetworkVarNames "bool m_bRetakesMVPLastRound"
        // static metadata: MNetworkVarNames "int m_iRetakesMVPBoostItem"
        // static metadata: MNetworkVarNames "loadout_slot_t m_RetakesMVPBoostExtraUtility"
        // static metadata: MNetworkVarNames "GameTime_t m_flHealthShotBoostExpirationTime"
        // static metadata: MNetworkVarNames "QAngle m_aimPunchAngle"
        // static metadata: MNetworkVarNames "QAngle m_aimPunchAngleVel"
        // static metadata: MNetworkVarNames "GameTick_t m_aimPunchTickBase"
        // static metadata: MNetworkVarNames "float m_aimPunchTickFraction"
        // static metadata: MNetworkVarNames "bool m_bIsBuyMenuOpen"
        // static metadata: MNetworkVarNames "GameTime_t m_flTimeOfLastInjury"
        // static metadata: MNetworkVarNames "GameTime_t m_flNextSprayDecalTime"
        // static metadata: MNetworkVarNames "int m_nRagdollDamageBone"
        // static metadata: MNetworkVarNames "Vector m_vRagdollDamageForce"
        // static metadata: MNetworkVarNames "Vector m_vRagdollDamagePosition"
        // static metadata: MNetworkVarNames "char m_szRagdollDamageWeaponName"
        // static metadata: MNetworkVarNames "bool m_bRagdollDamageHeadshot"
        // static metadata: MNetworkVarNames "Vector m_vRagdollServerOrigin"
        // static metadata: MNetworkVarNames "CEconItemView m_EconGloves"
        // static metadata: MNetworkVarNames "uint8 m_nEconGlovesChanged"
        // static metadata: MNetworkVarNames "QAngle m_qDeathEyeAngles"
        // static metadata: MNetworkVarNames "bool m_bLeftHanded"
        // static metadata: MNetworkVarNames "GameTime_t m_fSwitchedHandednessTime"
        // static metadata: MNetworkVarNames "float m_flViewmodelOffsetX"
        // static metadata: MNetworkVarNames "float m_flViewmodelOffsetY"
        // static metadata: MNetworkVarNames "float m_flViewmodelOffsetZ"
        // static metadata: MNetworkVarNames "float m_flViewmodelFOV"
        // static metadata: MNetworkVarNames "bool m_bIsWalking"
        // static metadata: MNetworkVarNames "CEntityIndex m_nLastKillerIndex"
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
        // static metadata: MNetworkVarNames "QAngle m_thirdPersonHeading"
        // static metadata: MNetworkVarNames "float m_flSlopeDropOffset"
        // static metadata: MNetworkVarNames "float m_flSlopeDropHeight"
        // static metadata: MNetworkVarNames "Vector m_vHeadConstraintOffset"
        // static metadata: MNetworkVarNames "int32 m_ArmorValue"
        // static metadata: MNetworkVarNames "uint16 m_unCurrentEquipmentValue"
        // static metadata: MNetworkVarNames "uint16 m_unRoundStartEquipmentValue"
        // static metadata: MNetworkVarNames "uint16 m_unFreezetimeEndEquipmentValue"
        // static metadata: MNetworkVarNames "uint32 m_vecPlayerPatchEconIndices"
        // static metadata: MNetworkVarNames "Color m_GunGameImmunityColor"
        // static metadata: MNetworkVarNames "PredictedDamageTag_t m_PredictedDamageTags"
        // static metadata: MNetworkVarNames "GameTime_t m_fImmuneToGunGameDamageTime"
        // static metadata: MNetworkVarNames "bool m_bGunGameImmunity"
        // static metadata: MNetworkVarNames "float m_fMolotovDamageTime"
        // static metadata: MNetworkVarNames "QAngle m_angEyeAngles"
        #pragma pack(push, 1)
        class CCSPlayerPawn : public source2sdk::server::CCSPlayerPawnBase
        {
        public:
            uint8_t _pad0e70[0x8]; // 0xe70
            // metadata: MNetworkEnable
            source2sdk::server::CCSPlayer_BulletServices* m_pBulletServices; // 0xe78            
            // metadata: MNetworkEnable
            source2sdk::server::CCSPlayer_HostageServices* m_pHostageServices; // 0xe80            
            // metadata: MNetworkEnable
            source2sdk::server::CCSPlayer_BuyServices* m_pBuyServices; // 0xe88            
            // metadata: MNetworkEnable
            source2sdk::server::CCSPlayer_ActionTrackingServices* m_pActionTrackingServices; // 0xe90            
            source2sdk::server::CCSPlayer_RadioServices* m_pRadioServices; // 0xe98            
            source2sdk::server::CCSPlayer_DamageReactServices* m_pDamageReactServices; // 0xea0            
            std::uint16_t m_nCharacterDefIndex; // 0xea8            
            // metadata: MNetworkEnable
            bool m_bHasFemaleVoice; // 0xeaa            
            uint8_t _pad0eab[0x5]; // 0xeab
            CUtlString m_strVOPrefix; // 0xeb0            
            // metadata: MNetworkEnable
            char m_szLastPlaceName[18]; // 0xeb8            
            uint8_t _pad0eca[0xde]; // 0xeca
            bool m_bInHostageResetZone; // 0xfa8            
            // metadata: MNetworkEnable
            bool m_bInBuyZone; // 0xfa9            
            uint8_t _pad0faa[0x6]; // 0xfaa
            // m_TouchingBuyZones has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<CHandle<source2sdk::server::CBaseEntity>> m_TouchingBuyZones;
            char m_TouchingBuyZones[0x18]; // 0xfb0            
            bool m_bWasInBuyZone; // 0xfc8            
            // metadata: MNetworkEnable
            bool m_bInHostageRescueZone; // 0xfc9            
            // metadata: MNetworkEnable
            bool m_bInBombZone; // 0xfca            
            bool m_bWasInHostageRescueZone; // 0xfcb            
            // metadata: MNetworkEnable
            std::int32_t m_iRetakesOffering; // 0xfcc            
            // metadata: MNetworkEnable
            std::int32_t m_iRetakesOfferingCard; // 0xfd0            
            // metadata: MNetworkEnable
            bool m_bRetakesHasDefuseKit; // 0xfd4            
            // metadata: MNetworkEnable
            bool m_bRetakesMVPLastRound; // 0xfd5            
            uint8_t _pad0fd6[0x2]; // 0xfd6
            // metadata: MNetworkEnable
            std::int32_t m_iRetakesMVPBoostItem; // 0xfd8            
            // metadata: MNetworkEnable
            source2sdk::client::loadout_slot_t m_RetakesMVPBoostExtraUtility; // 0xfdc            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flHealthShotBoostExpirationTime; // 0xfe0            
            float m_flLandingTimeSeconds; // 0xfe4            
            // metadata: MNetworkEnable
            // metadata: MNetworkBitCount "32"
            QAngle m_aimPunchAngle; // 0xfe8            
            // metadata: MNetworkEnable
            // metadata: MNetworkBitCount "32"
            QAngle m_aimPunchAngleVel; // 0xff4            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTick_t m_aimPunchTickBase; // 0x1000            
            // metadata: MNetworkEnable
            float m_aimPunchTickFraction; // 0x1004            
            // m_aimPunchCache has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<QAngle> m_aimPunchCache;
            char m_aimPunchCache[0x18]; // 0x1008            
            // metadata: MNetworkEnable
            bool m_bIsBuyMenuOpen; // 0x1020            
            uint8_t _pad1021[0x69f]; // 0x1021
            source2sdk::entity2::GameTime_t m_lastLandTime; // 0x16c0            
            bool m_bOnGroundLastTick; // 0x16c4            
            uint8_t _pad16c5[0x3]; // 0x16c5
            std::int32_t m_iPlayerLocked; // 0x16c8            
            uint8_t _pad16cc[0x4]; // 0x16cc
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flTimeOfLastInjury; // 0x16d0            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            source2sdk::entity2::GameTime_t m_flNextSprayDecalTime; // 0x16d4            
            bool m_bNextSprayDecalTimeExpedited; // 0x16d8            
            uint8_t _pad16d9[0x3]; // 0x16d9
            // metadata: MNetworkEnable
            std::int32_t m_nRagdollDamageBone; // 0x16dc            
            // metadata: MNetworkEnable
            Vector m_vRagdollDamageForce; // 0x16e0            
            // metadata: MNetworkEnable
            Vector m_vRagdollDamagePosition; // 0x16ec            
            // metadata: MNetworkEnable
            char m_szRagdollDamageWeaponName[64]; // 0x16f8            
            // metadata: MNetworkEnable
            bool m_bRagdollDamageHeadshot; // 0x1738            
            uint8_t _pad1739[0x3]; // 0x1739
            // metadata: MNetworkEnable
            Vector m_vRagdollServerOrigin; // 0x173c            
            // metadata: MNetworkEnable
            source2sdk::server::CEconItemView m_EconGloves; // 0x1748            
            // metadata: MNetworkEnable
            std::uint8_t m_nEconGlovesChanged; // 0x19f0            
            uint8_t _pad19f1[0x3]; // 0x19f1
            // metadata: MNetworkEnable
            QAngle m_qDeathEyeAngles; // 0x19f4            
            bool m_bSkipOneHeadConstraintUpdate; // 0x1a00            
            // metadata: MNetworkEnable
            bool m_bLeftHanded; // 0x1a01            
            uint8_t _pad1a02[0x2]; // 0x1a02
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fSwitchedHandednessTime; // 0x1a04            
            // metadata: MNetworkEnable
            // metadata: MNetworkMinValue "-2,000000"
            // metadata: MNetworkMaxValue "2,500000"
            float m_flViewmodelOffsetX; // 0x1a08            
            // metadata: MNetworkEnable
            // metadata: MNetworkMinValue "-2,000000"
            // metadata: MNetworkMaxValue "2,000000"
            float m_flViewmodelOffsetY; // 0x1a0c            
            // metadata: MNetworkEnable
            // metadata: MNetworkMinValue "-2,000000"
            // metadata: MNetworkMaxValue "2,000000"
            float m_flViewmodelOffsetZ; // 0x1a10            
            // metadata: MNetworkEnable
            // metadata: MNetworkMinValue "60,000000"
            // metadata: MNetworkMaxValue "68,000000"
            float m_flViewmodelFOV; // 0x1a14            
            // metadata: MNetworkEnable
            bool m_bIsWalking; // 0x1a18            
            uint8_t _pad1a19[0x3]; // 0x1a19
            float m_fLastGivenDefuserTime; // 0x1a1c            
            float m_fLastGivenBombTime; // 0x1a20            
            float m_flDealtDamageToEnemyMostRecentTimestamp; // 0x1a24            
            std::uint32_t m_iDisplayHistoryBits; // 0x1a28            
            float m_flLastAttackedTeammate; // 0x1a2c            
            source2sdk::entity2::GameTime_t m_allowAutoFollowTime; // 0x1a30            
            bool m_bResetArmorNextSpawn; // 0x1a34            
            uint8_t _pad1a35[0x3]; // 0x1a35
            // metadata: MNetworkEnable
            CEntityIndex m_nLastKillerIndex; // 0x1a38            
            uint8_t _pad1a3c[0x4]; // 0x1a3c
            // metadata: MNetworkEnable
            source2sdk::server::EntitySpottedState_t m_entitySpottedState; // 0x1a40            
            std::int32_t m_nSpotRules; // 0x1a58            
            // metadata: MNetworkEnable
            bool m_bIsScoped; // 0x1a5c            
            // metadata: MNetworkEnable
            bool m_bResumeZoom; // 0x1a5d            
            // metadata: MNetworkEnable
            bool m_bIsDefusing; // 0x1a5e            
            // metadata: MNetworkEnable
            bool m_bIsGrabbingHostage; // 0x1a5f            
            // metadata: MNetworkEnable
            source2sdk::client::CSPlayerBlockingUseAction_t m_iBlockingUseActionInProgress; // 0x1a60            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flEmitSoundTime; // 0x1a64            
            // metadata: MNetworkEnable
            bool m_bInNoDefuseArea; // 0x1a68            
            uint8_t _pad1a69[0x3]; // 0x1a69
            CEntityIndex m_iBombSiteIndex; // 0x1a6c            
            // metadata: MNetworkEnable
            std::int32_t m_nWhichBombZone; // 0x1a70            
            bool m_bInBombZoneTrigger; // 0x1a74            
            bool m_bWasInBombZoneTrigger; // 0x1a75            
            uint8_t _pad1a76[0x2]; // 0x1a76
            // metadata: MNetworkEnable
            std::int32_t m_iShotsFired; // 0x1a78            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            float m_flFlinchStack; // 0x1a7c            
            // metadata: MNetworkEnable
            float m_flVelocityModifier; // 0x1a80            
            // metadata: MNetworkEnable
            float m_flHitHeading; // 0x1a84            
            // metadata: MNetworkEnable
            std::int32_t m_nHitBodyPart; // 0x1a88            
            Vector m_vecTotalBulletForce; // 0x1a8c            
            // metadata: MNetworkEnable
            bool m_bWaitForNoAttack; // 0x1a98            
            uint8_t _pad1a99[0x3]; // 0x1a99
            float m_ignoreLadderJumpTime; // 0x1a9c            
            // metadata: MNetworkEnable
            bool m_bKilledByHeadshot; // 0x1aa0            
            uint8_t _pad1aa1[0x3]; // 0x1aa1
            std::int32_t m_LastHitBox; // 0x1aa4            
            source2sdk::server::CCSBot* m_pBot; // 0x1aa8            
            bool m_bBotAllowActive; // 0x1ab0            
            uint8_t _pad1ab1[0x3]; // 0x1ab1
            // metadata: MNetworkEnable
            // metadata: MNetworkEncoder "qangle_precise"
            // metadata: MNetworkPriority "32"
            QAngle m_thirdPersonHeading; // 0x1ab4            
            // metadata: MNetworkEnable
            // metadata: MNetworkPriority "32"
            float m_flSlopeDropOffset; // 0x1ac0            
            // metadata: MNetworkEnable
            // metadata: MNetworkPriority "32"
            float m_flSlopeDropHeight; // 0x1ac4            
            // metadata: MNetworkEnable
            // metadata: MNetworkPriority "32"
            Vector m_vHeadConstraintOffset; // 0x1ac8            
            std::int32_t m_nLastPickupPriority; // 0x1ad4            
            float m_flLastPickupPriorityTime; // 0x1ad8            
            // metadata: MNetworkEnable
            std::int32_t m_ArmorValue; // 0x1adc            
            // metadata: MNetworkEnable
            std::uint16_t m_unCurrentEquipmentValue; // 0x1ae0            
            // metadata: MNetworkEnable
            std::uint16_t m_unRoundStartEquipmentValue; // 0x1ae2            
            // metadata: MNetworkEnable
            std::uint16_t m_unFreezetimeEndEquipmentValue; // 0x1ae4            
            uint8_t _pad1ae6[0x2]; // 0x1ae6
            std::int32_t m_iLastWeaponFireUsercmd; // 0x1ae8            
            bool m_bIsSpawning; // 0x1aec            
            uint8_t _pad1aed[0xb]; // 0x1aed
            std::int32_t m_iDeathFlags; // 0x1af8            
            bool m_bHasDeathInfo; // 0x1afc            
            uint8_t _pad1afd[0x3]; // 0x1afd
            float m_flDeathInfoTime; // 0x1b00            
            Vector m_vecDeathInfoOrigin; // 0x1b04            
            // metadata: MNetworkEnable
            std::uint32_t m_vecPlayerPatchEconIndices[5]; // 0x1b10            
            // metadata: MNetworkEnable
            Color m_GunGameImmunityColor; // 0x1b24            
            source2sdk::entity2::GameTime_t m_grenadeParameterStashTime; // 0x1b28            
            bool m_bGrenadeParametersStashed; // 0x1b2c            
            uint8_t _pad1b2d[0x3]; // 0x1b2d
            QAngle m_angStashedShootAngles; // 0x1b30            
            Vector m_vecStashedGrenadeThrowPosition; // 0x1b3c            
            Vector m_vecStashedVelocity; // 0x1b48            
            QAngle m_angShootAngleHistory[2]; // 0x1b54            
            Vector m_vecThrowPositionHistory[2]; // 0x1b6c            
            Vector m_vecVelocityHistory[2]; // 0x1b84            
            uint8_t _pad1b9c[0x4]; // 0x1b9c
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            // m_PredictedDamageTags has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVectorEmbeddedNetworkVar<source2sdk::server::PredictedDamageTag_t> m_PredictedDamageTags;
            char m_PredictedDamageTags[0x68]; // 0x1ba0            
            std::int32_t m_nHighestAppliedDamageTagTick; // 0x1c08            
            bool m_bCommittingSuicideOnTeamChange; // 0x1c0c            
            bool m_wasNotKilledNaturally; // 0x1c0d            
            uint8_t _pad1c0e[0x2]; // 0x1c0e
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fImmuneToGunGameDamageTime; // 0x1c10            
            // metadata: MNetworkEnable
            bool m_bGunGameImmunity; // 0x1c14            
            uint8_t _pad1c15[0x3]; // 0x1c15
            // metadata: MNetworkEnable
            float m_fMolotovDamageTime; // 0x1c18            
            // metadata: MNetworkEnable
            // metadata: MNetworkEncoder "qangle_precise"
            // metadata: MNetworkPriority "32"
            QAngle m_angEyeAngles; // 0x1c1c            
            uint8_t _pad1c28[0x8];
            
            // Datamap fields:
            // CCSPlayer_WeaponServices m_pWeaponServices; // 0xb70
            // CCSPlayer_ItemServices m_pItemServices; // 0xb78
            // CCSPlayer_UseServices m_pUseServices; // 0xb98
            // CCSPlayer_WaterServices m_pWaterServices; // 0xb90
            // CCSPlayer_MovementServices m_pMovementServices; // 0xbb0
            // CCSPlayer_CameraServices m_pCameraServices; // 0xba8
            // void CCSPlayerPawnCheckStuffThink; // 0x0
            // void CCSPlayerPawnPushawayThink; // 0x0
            // void RescueZoneTouch; // 0x0
            // bool bot; // 0x7fffffff
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSPlayerPawn because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CCSPlayerPawn) == 0x1c30);
    };
};
