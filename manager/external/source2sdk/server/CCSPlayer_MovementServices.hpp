#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/GameTick_t.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CCSPlayerLegacyJump.hpp"
#include "source2sdk/server/CCSPlayerModernJump.hpp"
#include "source2sdk/server/CPlayer_MovementServices_Humanoid.hpp"

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
        // Size: 0xe68
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
        class CCSPlayer_MovementServices : public source2sdk::server::CPlayer_MovementServices_Humanoid
        {
        public:
            Vector m_vecLadderNormal; // 0x278            
            // metadata: MNetworkEnable
            std::int32_t m_nLadderSurfacePropIndex; // 0x284            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            bool m_bDucked; // 0x288            
            uint8_t _pad0289[0x3]; // 0x289
            // metadata: MNetworkEnable
            float m_flDuckAmount; // 0x28c            
            // metadata: MNetworkEnable
            float m_flDuckSpeed; // 0x290            
            // metadata: MNetworkEnable
            bool m_bDuckOverride; // 0x294            
            // metadata: MNetworkEnable
            bool m_bDesiresDuck; // 0x295            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            bool m_bDucking; // 0x296            
            uint8_t _pad0297[0x1]; // 0x297
            // metadata: MNetworkEnable
            float m_flDuckOffset; // 0x298            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            // metadata: MNetworkPriority "32"
            std::uint32_t m_nDuckTimeMsecs; // 0x29c            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            std::uint32_t m_nDuckJumpTimeMsecs; // 0x2a0            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            std::uint32_t m_nJumpTimeMsecs; // 0x2a4            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            float m_flLastDuckTime; // 0x2a8            
            uint8_t _pad02ac[0xc]; // 0x2ac
            Vector2D m_vecLastPositionAtFullCrouchSpeed; // 0x2b8            
            bool m_duckUntilOnGround; // 0x2c0            
            bool m_bHasWalkMovedSinceLastJump; // 0x2c1            
            bool m_bInStuckTest; // 0x2c2            
            uint8_t _pad02c3[0x20d]; // 0x2c3
            std::int32_t m_nTraceCount; // 0x4d0            
            std::int32_t m_StuckLast; // 0x4d4            
            bool m_bSpeedCropped; // 0x4d8            
            uint8_t _pad04d9[0x3]; // 0x4d9
            std::int32_t m_nOldWaterLevel; // 0x4dc            
            float m_flWaterEntryTime; // 0x4e0            
            Vector m_vecForward; // 0x4e4            
            Vector m_vecLeft; // 0x4f0            
            Vector m_vecUp; // 0x4fc            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            std::int32_t m_nGameCodeHasMovedPlayerAfterCommand; // 0x508            
            bool m_bMadeFootstepNoise; // 0x50c            
            uint8_t _pad050d[0x3]; // 0x50d
            std::int32_t m_iFootsteps; // 0x510            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fStashGrenadeParameterWhen; // 0x514            
            // metadata: MNetworkEnable
            std::uint64_t m_nButtonDownMaskPrev; // 0x518            
            // metadata: MNetworkEnable
            float m_flOffsetTickCompleteTime; // 0x520            
            // metadata: MNetworkEnable
            float m_flOffsetTickStashedSpeed; // 0x524            
            // metadata: MNetworkEnable
            float m_flStamina; // 0x528            
            float m_flHeightAtJumpStart; // 0x52c            
            float m_flMaxJumpHeightThisJump; // 0x530            
            float m_flMaxJumpHeightLastJump; // 0x534            
            float m_flStaminaAtJumpStart; // 0x538            
            float m_flVelMulAtJumpStart; // 0x53c            
            float m_flAccumulatedJumpError; // 0x540            
            uint8_t _pad0544[0x4]; // 0x544
            // metadata: MNetworkEnable
            source2sdk::server::CCSPlayerLegacyJump m_LegacyJump; // 0x548            
            // metadata: MNetworkEnable
            source2sdk::server::CCSPlayerModernJump m_ModernJump; // 0x560            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTick_t m_nLastJumpTick; // 0x598            
            // metadata: MNetworkEnable
            // metadata: MNetworkBitCount "6"
            // metadata: MNetworkMinValue "0,000000"
            // metadata: MNetworkMaxValue "1,000000"
            // metadata: MNetworkEncodeFlags "4"
            float m_flLastJumpFrac; // 0x59c            
            // metadata: MNetworkEnable
            float m_flLastJumpVelocityZ; // 0x5a0            
            // metadata: MNetworkEnable
            bool m_bJumpApexPending; // 0x5a4            
            uint8_t _pad05a5[0x3]; // 0x5a5
            float m_flTicksSinceLastSurfingDetected; // 0x5a8            
            // metadata: MNetworkEnable
            bool m_bWasSurfing; // 0x5ac            
            uint8_t _pad05ad[0x8f]; // 0x5ad
            Vector m_vecInputRotated; // 0x63c            
            uint8_t _pad0648[0x820];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSPlayer_MovementServices because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CCSPlayer_MovementServices) == 0xe68);
    };
};
