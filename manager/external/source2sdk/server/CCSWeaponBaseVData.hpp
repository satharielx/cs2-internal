#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CFiringModeFloat.hpp"
#include "source2sdk/client/CFiringModeInt.hpp"
#include "source2sdk/client/CSWeaponCategory.hpp"
#include "source2sdk/client/CSWeaponSilencerType.hpp"
#include "source2sdk/client/CSWeaponType.hpp"
#include "source2sdk/client/gear_slot_t.hpp"
#include "source2sdk/client/loadout_slot_t.hpp"
#include "source2sdk/resourcesystem/InfoForResourceTypeCNmSkeleton.hpp"
#include "source2sdk/resourcesystem/InfoForResourceTypeIParticleSystemDefinition.hpp"
#include "source2sdk/server/CBasePlayerWeaponVData.hpp"

// /////////////////////////////////////////////////////////////
// Module: server
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace server
    {
        // Registered alignment: 0x8
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x7c0
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        // static metadata: MPropertySuppressBaseClassField
        // static metadata: MPropertySuppressBaseClassField
        #pragma pack(push, 1)
        class CCSWeaponBaseVData : public source2sdk::server::CBasePlayerWeaponVData
        {
        public:
            source2sdk::client::CSWeaponType m_WeaponType; // 0x440            
            source2sdk::client::CSWeaponCategory m_WeaponCategory; // 0x444            
            // metadata: MPropertyStartGroup "Visuals"
            // m_szAnimSkeleton has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CResourceNameTyped<CWeakHandle<source2sdk::resourcesystem::InfoForResourceTypeCNmSkeleton>> m_szAnimSkeleton;
            char m_szAnimSkeleton[0xe0]; // 0x448            
            Vector m_vecMuzzlePos0; // 0x528            
            Vector m_vecMuzzlePos1; // 0x534            
            // metadata: MPropertyDescription "Effect to actually fire into the world from this weapon"
            // m_szTracerParticle has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CResourceNameTyped<CWeakHandle<source2sdk::resourcesystem::InfoForResourceTypeIParticleSystemDefinition>> m_szTracerParticle;
            char m_szTracerParticle[0xe0]; // 0x540            
            // metadata: MPropertyStartGroup "HUD Positions"
            // metadata: MPropertyFriendlyName "HUD Bucket"
            // metadata: MPropertyDescription "Which 'column' to display this weapon in the HUD"
            source2sdk::client::gear_slot_t m_GearSlot; // 0x620            
            std::int32_t m_GearSlotPosition; // 0x624            
            // metadata: MPropertyFriendlyName "HUD Bucket Position"
            // metadata: MPropertyDescription "Default team (non Terrorist or Counter-Terrorist) 'row' to display this weapon in the HUD."
            source2sdk::client::loadout_slot_t m_DefaultLoadoutSlot; // 0x628            
            // metadata: MPropertyStartGroup "In-Game Data"
            std::int32_t m_nPrice; // 0x62c            
            std::int32_t m_nKillAward; // 0x630            
            std::int32_t m_nPrimaryReserveAmmoMax; // 0x634            
            std::int32_t m_nSecondaryReserveAmmoMax; // 0x638            
            bool m_bMeleeWeapon; // 0x63c            
            bool m_bHasBurstMode; // 0x63d            
            bool m_bIsRevolver; // 0x63e            
            bool m_bCannotShootUnderwater; // 0x63f            
            // metadata: MPropertyFriendlyName "In-Code weapon name"
            CGlobalSymbol m_szName; // 0x640            
            source2sdk::client::CSWeaponSilencerType m_eSilencerType; // 0x648            
            std::int32_t m_nCrosshairMinDistance; // 0x64c            
            std::int32_t m_nCrosshairDeltaDistance; // 0x650            
            bool m_bIsFullAuto; // 0x654            
            uint8_t _pad0655[0x3]; // 0x655
            std::int32_t m_nNumBullets; // 0x658            
            bool m_bReloadsSingleShells; // 0x65c            
            uint8_t _pad065d[0x3]; // 0x65d
            // metadata: MPropertyStartGroup "Firing Mode Data"
            source2sdk::client::CFiringModeFloat m_flCycleTime; // 0x660            
            source2sdk::client::CFiringModeFloat m_flMaxSpeed; // 0x668            
            source2sdk::client::CFiringModeFloat m_flSpread; // 0x670            
            source2sdk::client::CFiringModeFloat m_flInaccuracyCrouch; // 0x678            
            source2sdk::client::CFiringModeFloat m_flInaccuracyStand; // 0x680            
            source2sdk::client::CFiringModeFloat m_flInaccuracyJump; // 0x688            
            source2sdk::client::CFiringModeFloat m_flInaccuracyLand; // 0x690            
            source2sdk::client::CFiringModeFloat m_flInaccuracyLadder; // 0x698            
            source2sdk::client::CFiringModeFloat m_flInaccuracyFire; // 0x6a0            
            source2sdk::client::CFiringModeFloat m_flInaccuracyMove; // 0x6a8            
            source2sdk::client::CFiringModeFloat m_flRecoilAngle; // 0x6b0            
            source2sdk::client::CFiringModeFloat m_flRecoilAngleVariance; // 0x6b8            
            source2sdk::client::CFiringModeFloat m_flRecoilMagnitude; // 0x6c0            
            source2sdk::client::CFiringModeFloat m_flRecoilMagnitudeVariance; // 0x6c8            
            source2sdk::client::CFiringModeInt m_nTracerFrequency; // 0x6d0            
            float m_flInaccuracyJumpInitial; // 0x6d8            
            float m_flInaccuracyJumpApex; // 0x6dc            
            float m_flInaccuracyReload; // 0x6e0            
            float m_flDeployDuration; // 0x6e4            
            float m_flDisallowAttackAfterReloadStartDuration; // 0x6e8            
            std::int32_t m_nBurstShotCount; // 0x6ec            
            bool m_bAllowBurstHolster; // 0x6f0            
            uint8_t _pad06f1[0x3]; // 0x6f1
            // metadata: MPropertyStartGroup "Firing"
            std::int32_t m_nRecoilSeed; // 0x6f4            
            std::int32_t m_nSpreadSeed; // 0x6f8            
            float m_flAttackMovespeedFactor; // 0x6fc            
            float m_flInaccuracyPitchShift; // 0x700            
            float m_flInaccuracyAltSoundThreshold; // 0x704            
            CUtlString m_szUseRadioSubtitle; // 0x708            
            // metadata: MPropertyStartGroup "Zooming"
            bool m_bUnzoomsAfterShot; // 0x710            
            bool m_bHideViewModelWhenZoomed; // 0x711            
            uint8_t _pad0712[0x2]; // 0x712
            std::int32_t m_nZoomLevels; // 0x714            
            std::int32_t m_nZoomFOV1; // 0x718            
            std::int32_t m_nZoomFOV2; // 0x71c            
            float m_flZoomTime0; // 0x720            
            float m_flZoomTime1; // 0x724            
            float m_flZoomTime2; // 0x728            
            // metadata: MPropertyStartGroup "Iron Sights"
            float m_flIronSightPullUpSpeed; // 0x72c            
            float m_flIronSightPutDownSpeed; // 0x730            
            float m_flIronSightFOV; // 0x734            
            float m_flIronSightPivotForward; // 0x738            
            float m_flIronSightLooseness; // 0x73c            
            // metadata: MPropertyStartGroup "Damage"
            std::int32_t m_nDamage; // 0x740            
            float m_flHeadshotMultiplier; // 0x744            
            float m_flArmorRatio; // 0x748            
            float m_flPenetration; // 0x74c            
            float m_flRange; // 0x750            
            float m_flRangeModifier; // 0x754            
            float m_flFlinchVelocityModifierLarge; // 0x758            
            float m_flFlinchVelocityModifierSmall; // 0x75c            
            // metadata: MPropertyStartGroup "Recovery"
            float m_flRecoveryTimeCrouch; // 0x760            
            float m_flRecoveryTimeStand; // 0x764            
            float m_flRecoveryTimeCrouchFinal; // 0x768            
            float m_flRecoveryTimeStandFinal; // 0x76c            
            std::int32_t m_nRecoveryTransitionStartBullet; // 0x770            
            std::int32_t m_nRecoveryTransitionEndBullet; // 0x774            
            // metadata: MPropertyStartGroup "Grenade Data"
            float m_flThrowVelocity; // 0x778            
            Vector m_vSmokeColor; // 0x77c            
            CGlobalSymbol m_szAnimClass; // 0x788            
            uint8_t _pad0790[0x30];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSWeaponBaseVData because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CCSWeaponBaseVData) == 0x7c0);
    };
};
