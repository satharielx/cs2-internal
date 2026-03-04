#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CCSPlayerLegacyJump.hpp"
#include "source2sdk/client/CCSPlayerModernJump.hpp"
#include "source2sdk/client/CPlayer_MovementServices_Humanoid.hpp"
#include "source2sdk/entity2/GameTick_t.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"

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
        // Size: 0xe58
        // Has VTable
        // 
        // static metadata: MNetworkVarNames "int m_nLadderSurfacePropIndex"
        // static metadata: MNetworkVarNames "bool m_bDucked"
        // static metadata: MNetworkVarNames "float m_flDuckAmount"
        // static metadata: MNetworkVarNames "float m_flDuckSpeed"
        // static metadata: MNetworkVarNames "bool m_bDuckOverride"
        // static metadata: MNetworkVarNames "bool m_bDesiresDuck"
        // static metadata: MNetworkVarNames "bool m_bDucking"
        // static metadata: MNetworkVarNames "float m_flDuckOffset"
        // static metadata: MNetworkVarNames "uint32 m_nDuckTimeMsecs"
        // static metadata: MNetworkVarNames "uint32 m_nDuckJumpTimeMsecs"
        // static metadata: MNetworkVarNames "uint32 m_nJumpTimeMsecs"
        // static metadata: MNetworkVarNames "float m_flLastDuckTime"
        // static metadata: MNetworkVarNames "int m_nGameCodeHasMovedPlayerAfterCommand"
        // static metadata: MNetworkVarNames "GameTime_t m_fStashGrenadeParameterWhen"
        // static metadata: MNetworkVarNames "ButtonBitMask_t m_nButtonDownMaskPrev"
        // static metadata: MNetworkVarNames "float m_flOffsetTickCompleteTime"
        // static metadata: MNetworkVarNames "float m_flOffsetTickStashedSpeed"
        // static metadata: MNetworkVarNames "float m_flStamina"
        // static metadata: MNetworkVarNames "CCSPlayerLegacyJump m_LegacyJump"
        // static metadata: MNetworkVarNames "CCSPlayerModernJump m_ModernJump"
        // static metadata: MNetworkVarNames "GameTick_t m_nLastJumpTick"
        // static metadata: MNetworkVarNames "float m_flLastJumpFrac"
        // static metadata: MNetworkVarNames "float m_flLastJumpVelocityZ"
        // static metadata: MNetworkVarNames "bool m_bJumpApexPending"
        // static metadata: MNetworkVarNames "bool m_bWasSurfing"
        #pragma pack(push, 1)
        class CCSPlayer_MovementServices : public source2sdk::client::CPlayer_MovementServices_Humanoid
        {
        public:
            Vector m_vecLadderNormal; // 0x270            
            // metadata: MNetworkEnable
            std::int32_t m_nLadderSurfacePropIndex; // 0x27c            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            bool m_bDucked; // 0x280            
            uint8_t _pad0281[0x3]; // 0x281
            // metadata: MNetworkEnable
            float m_flDuckAmount; // 0x284            
            // metadata: MNetworkEnable
            float m_flDuckSpeed; // 0x288            
            // metadata: MNetworkEnable
            bool m_bDuckOverride; // 0x28c            
            // metadata: MNetworkEnable
            bool m_bDesiresDuck; // 0x28d            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            bool m_bDucking; // 0x28e            
            uint8_t _pad028f[0x1]; // 0x28f
            // metadata: MNetworkEnable
            float m_flDuckOffset; // 0x290            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            // metadata: MNetworkPriority "32"
            std::uint32_t m_nDuckTimeMsecs; // 0x294            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            std::uint32_t m_nDuckJumpTimeMsecs; // 0x298            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            std::uint32_t m_nJumpTimeMsecs; // 0x29c            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            float m_flLastDuckTime; // 0x2a0            
            uint8_t _pad02a4[0xc]; // 0x2a4
            Vector2D m_vecLastPositionAtFullCrouchSpeed; // 0x2b0            
            bool m_duckUntilOnGround; // 0x2b8            
            bool m_bHasWalkMovedSinceLastJump; // 0x2b9            
            bool m_bInStuckTest; // 0x2ba            
            uint8_t _pad02bb[0x20d]; // 0x2bb
            std::int32_t m_nTraceCount; // 0x4c8            
            std::int32_t m_StuckLast; // 0x4cc            
            bool m_bSpeedCropped; // 0x4d0            
            uint8_t _pad04d1[0x3]; // 0x4d1
            std::int32_t m_nOldWaterLevel; // 0x4d4            
            float m_flWaterEntryTime; // 0x4d8            
            Vector m_vecForward; // 0x4dc            
            Vector m_vecLeft; // 0x4e8            
            Vector m_vecUp; // 0x4f4            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            std::int32_t m_nGameCodeHasMovedPlayerAfterCommand; // 0x500            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fStashGrenadeParameterWhen; // 0x504            
            // metadata: MNetworkEnable
            std::uint64_t m_nButtonDownMaskPrev; // 0x508            
            // metadata: MNetworkEnable
            float m_flOffsetTickCompleteTime; // 0x510            
            // metadata: MNetworkEnable
            float m_flOffsetTickStashedSpeed; // 0x514            
            // metadata: MNetworkEnable
            float m_flStamina; // 0x518            
            float m_flHeightAtJumpStart; // 0x51c            
            float m_flMaxJumpHeightThisJump; // 0x520            
            float m_flMaxJumpHeightLastJump; // 0x524            
            float m_flStaminaAtJumpStart; // 0x528            
            float m_flVelMulAtJumpStart; // 0x52c            
            float m_flAccumulatedJumpError; // 0x530            
            uint8_t _pad0534[0x4]; // 0x534
            // metadata: MNetworkEnable
            source2sdk::client::CCSPlayerLegacyJump m_LegacyJump; // 0x538            
            // metadata: MNetworkEnable
            source2sdk::client::CCSPlayerModernJump m_ModernJump; // 0x550            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTick_t m_nLastJumpTick; // 0x588            
            // metadata: MNetworkEnable
            // metadata: MNetworkBitCount "6"
            // metadata: MNetworkMinValue "0,000000"
            // metadata: MNetworkMaxValue "1,000000"
            // metadata: MNetworkEncodeFlags "4"
            float m_flLastJumpFrac; // 0x58c            
            // metadata: MNetworkEnable
            float m_flLastJumpVelocityZ; // 0x590            
            // metadata: MNetworkEnable
            bool m_bJumpApexPending; // 0x594            
            uint8_t _pad0595[0x3]; // 0x595
            float m_flTicksSinceLastSurfingDetected; // 0x598            
            // metadata: MNetworkEnable
            bool m_bWasSurfing; // 0x59c            
            uint8_t _pad059d[0x8f]; // 0x59d
            Vector m_vecInputRotated; // 0x62c            
            uint8_t _pad0638[0x820];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSPlayer_MovementServices because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::CCSPlayer_MovementServices) == 0xe58);
    };
};
