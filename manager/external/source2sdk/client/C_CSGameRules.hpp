#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_RetakeGameRules.hpp"
#include "source2sdk/client/C_TeamplayRules.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
namespace source2sdk
{
    namespace client
    {
        struct CCSGameModeRules;
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
        // Size: 0x4f60
        // Has VTable
        // 
        // static metadata: MNetworkVarNames "bool m_bFreezePeriod"
        // static metadata: MNetworkVarNames "bool m_bWarmupPeriod"
        // static metadata: MNetworkVarNames "GameTime_t m_fWarmupPeriodEnd"
        // static metadata: MNetworkVarNames "GameTime_t m_fWarmupPeriodStart"
        // static metadata: MNetworkVarNames "bool m_bTerroristTimeOutActive"
        // static metadata: MNetworkVarNames "bool m_bCTTimeOutActive"
        // static metadata: MNetworkVarNames "float m_flTerroristTimeOutRemaining"
        // static metadata: MNetworkVarNames "float m_flCTTimeOutRemaining"
        // static metadata: MNetworkVarNames "int m_nTerroristTimeOuts"
        // static metadata: MNetworkVarNames "int m_nCTTimeOuts"
        // static metadata: MNetworkVarNames "bool m_bTechnicalTimeOut"
        // static metadata: MNetworkVarNames "bool m_bMatchWaitingForResume"
        // static metadata: MNetworkVarNames "int m_iFreezeTime"
        // static metadata: MNetworkVarNames "int m_iRoundTime"
        // static metadata: MNetworkVarNames "float m_fMatchStartTime"
        // static metadata: MNetworkVarNames "GameTime_t m_fRoundStartTime"
        // static metadata: MNetworkVarNames "GameTime_t m_flRestartRoundTime"
        // static metadata: MNetworkVarNames "bool m_bGameRestart"
        // static metadata: MNetworkVarNames "float m_flGameStartTime"
        // static metadata: MNetworkVarNames "float m_timeUntilNextPhaseStarts"
        // static metadata: MNetworkVarNames "int m_gamePhase"
        // static metadata: MNetworkVarNames "int m_totalRoundsPlayed"
        // static metadata: MNetworkVarNames "int m_nRoundsPlayedThisPhase"
        // static metadata: MNetworkVarNames "int m_nOvertimePlaying"
        // static metadata: MNetworkVarNames "int m_iHostagesRemaining"
        // static metadata: MNetworkVarNames "bool m_bAnyHostageReached"
        // static metadata: MNetworkVarNames "bool m_bMapHasBombTarget"
        // static metadata: MNetworkVarNames "bool m_bMapHasRescueZone"
        // static metadata: MNetworkVarNames "bool m_bMapHasBuyZone"
        // static metadata: MNetworkVarNames "bool m_bIsQueuedMatchmaking"
        // static metadata: MNetworkVarNames "int m_nQueuedMatchmakingMode"
        // static metadata: MNetworkVarNames "bool m_bIsValveDS"
        // static metadata: MNetworkVarNames "bool m_bLogoMap"
        // static metadata: MNetworkVarNames "bool m_bPlayAllStepSoundsOnServer"
        // static metadata: MNetworkVarNames "int m_iSpectatorSlotCount"
        // static metadata: MNetworkVarNames "int m_MatchDevice"
        // static metadata: MNetworkVarNames "bool m_bHasMatchStarted"
        // static metadata: MNetworkVarNames "int m_nNextMapInMapgroup"
        // static metadata: MNetworkVarNames "char m_szTournamentEventName"
        // static metadata: MNetworkVarNames "char m_szTournamentEventStage"
        // static metadata: MNetworkVarNames "char m_szMatchStatTxt"
        // static metadata: MNetworkVarNames "char m_szTournamentPredictionsTxt"
        // static metadata: MNetworkVarNames "int m_nTournamentPredictionsPct"
        // static metadata: MNetworkVarNames "GameTime_t m_flCMMItemDropRevealStartTime"
        // static metadata: MNetworkVarNames "GameTime_t m_flCMMItemDropRevealEndTime"
        // static metadata: MNetworkVarNames "bool m_bIsDroppingItems"
        // static metadata: MNetworkVarNames "bool m_bIsQuestEligible"
        // static metadata: MNetworkVarNames "bool m_bIsHltvActive"
        // static metadata: MNetworkVarNames "bool m_bBombPlanted"
        // static metadata: MNetworkVarNames "uint16 m_arrProhibitedItemIndices"
        // static metadata: MNetworkVarNames "uint32 m_arrTournamentActiveCasterAccounts"
        // static metadata: MNetworkVarNames "int m_numBestOfMaps"
        // static metadata: MNetworkVarNames "int m_nHalloweenMaskListSeed"
        // static metadata: MNetworkVarNames "bool m_bBombDropped"
        // static metadata: MNetworkVarNames "int m_iRoundWinStatus"
        // static metadata: MNetworkVarNames "int m_eRoundWinReason"
        // static metadata: MNetworkVarNames "bool m_bTCantBuy"
        // static metadata: MNetworkVarNames "bool m_bCTCantBuy"
        // static metadata: MNetworkVarNames "int m_iMatchStats_RoundResults"
        // static metadata: MNetworkVarNames "int m_iMatchStats_PlayersAlive_CT"
        // static metadata: MNetworkVarNames "int m_iMatchStats_PlayersAlive_T"
        // static metadata: MNetworkVarNames "float m_TeamRespawnWaveTimes"
        // static metadata: MNetworkVarNames "GameTime_t m_flNextRespawnWave"
        // static metadata: MNetworkVarNames "Vector m_vMinimapMins"
        // static metadata: MNetworkVarNames "Vector m_vMinimapMaxs"
        // static metadata: MNetworkVarNames "float m_MinimapVerticalSectionHeights"
        // static metadata: MNetworkVarNames "int m_nEndMatchMapGroupVoteTypes"
        // static metadata: MNetworkVarNames "int m_nEndMatchMapGroupVoteOptions"
        // static metadata: MNetworkVarNames "int m_nEndMatchMapVoteWinner"
        // static metadata: MNetworkVarNames "int m_iNumConsecutiveCTLoses"
        // static metadata: MNetworkVarNames "int m_iNumConsecutiveTerroristLoses"
        // static metadata: MNetworkVarNames "int m_nMatchAbortedEarlyReason"
        // static metadata: MNetworkVarNames "CCSGameModeRules * m_pGameModeRules"
        // static metadata: MNetworkVarNames "CRetakeGameRules m_RetakeRules"
        // static metadata: MNetworkVarNames "uint8 m_nMatchEndCount"
        // static metadata: MNetworkVarNames "int m_nTTeamIntroVariant"
        // static metadata: MNetworkVarNames "int m_nCTTeamIntroVariant"
        // static metadata: MNetworkVarNames "bool m_bTeamIntroPeriod"
        // static metadata: MNetworkVarNames "int m_iRoundEndWinnerTeam"
        // static metadata: MNetworkVarNames "int m_eRoundEndReason"
        // static metadata: MNetworkVarNames "bool m_bRoundEndShowTimerDefend"
        // static metadata: MNetworkVarNames "int m_iRoundEndTimerTime"
        // static metadata: MNetworkVarNames "CUtlString m_sRoundEndFunFactToken"
        // static metadata: MNetworkVarNames "CPlayerSlot m_iRoundEndFunFactPlayerSlot"
        // static metadata: MNetworkVarNames "int m_iRoundEndFunFactData1"
        // static metadata: MNetworkVarNames "int m_iRoundEndFunFactData2"
        // static metadata: MNetworkVarNames "int m_iRoundEndFunFactData3"
        // static metadata: MNetworkVarNames "CUtlString m_sRoundEndMessage"
        // static metadata: MNetworkVarNames "int m_iRoundEndPlayerCount"
        // static metadata: MNetworkVarNames "bool m_bRoundEndNoMusic"
        // static metadata: MNetworkVarNames "int m_iRoundEndLegacy"
        // static metadata: MNetworkVarNames "uint8 m_nRoundEndCount"
        // static metadata: MNetworkVarNames "int m_iRoundStartRoundNumber"
        // static metadata: MNetworkVarNames "uint8 m_nRoundStartCount"
        #pragma pack(push, 1)
        class C_CSGameRules : public source2sdk::client::C_TeamplayRules
        {
        public:
            // metadata: MNetworkEnable
            bool m_bFreezePeriod; // 0x40            
            // metadata: MNetworkEnable
            bool m_bWarmupPeriod; // 0x41            
            uint8_t _pad0042[0x2]; // 0x42
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fWarmupPeriodEnd; // 0x44            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fWarmupPeriodStart; // 0x48            
            // metadata: MNetworkEnable
            bool m_bTerroristTimeOutActive; // 0x4c            
            // metadata: MNetworkEnable
            bool m_bCTTimeOutActive; // 0x4d            
            uint8_t _pad004e[0x2]; // 0x4e
            // metadata: MNetworkEnable
            float m_flTerroristTimeOutRemaining; // 0x50            
            // metadata: MNetworkEnable
            float m_flCTTimeOutRemaining; // 0x54            
            // metadata: MNetworkEnable
            std::int32_t m_nTerroristTimeOuts; // 0x58            
            // metadata: MNetworkEnable
            std::int32_t m_nCTTimeOuts; // 0x5c            
            // metadata: MNetworkEnable
            bool m_bTechnicalTimeOut; // 0x60            
            // metadata: MNetworkEnable
            bool m_bMatchWaitingForResume; // 0x61            
            uint8_t _pad0062[0x2]; // 0x62
            // metadata: MNetworkEnable
            std::int32_t m_iFreezeTime; // 0x64            
            // metadata: MNetworkEnable
            std::int32_t m_iRoundTime; // 0x68            
            // metadata: MNetworkEnable
            float m_fMatchStartTime; // 0x6c            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fRoundStartTime; // 0x70            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flRestartRoundTime; // 0x74            
            // metadata: MNetworkEnable
            bool m_bGameRestart; // 0x78            
            uint8_t _pad0079[0x3]; // 0x79
            // metadata: MNetworkEnable
            float m_flGameStartTime; // 0x7c            
            // metadata: MNetworkEnable
            float m_timeUntilNextPhaseStarts; // 0x80            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnGamePhaseChanged"
            std::int32_t m_gamePhase; // 0x84            
            // metadata: MNetworkEnable
            std::int32_t m_totalRoundsPlayed; // 0x88            
            // metadata: MNetworkEnable
            std::int32_t m_nRoundsPlayedThisPhase; // 0x8c            
            // metadata: MNetworkEnable
            std::int32_t m_nOvertimePlaying; // 0x90            
            // metadata: MNetworkEnable
            std::int32_t m_iHostagesRemaining; // 0x94            
            // metadata: MNetworkEnable
            bool m_bAnyHostageReached; // 0x98            
            // metadata: MNetworkEnable
            bool m_bMapHasBombTarget; // 0x99            
            // metadata: MNetworkEnable
            bool m_bMapHasRescueZone; // 0x9a            
            // metadata: MNetworkEnable
            bool m_bMapHasBuyZone; // 0x9b            
            // metadata: MNetworkEnable
            bool m_bIsQueuedMatchmaking; // 0x9c            
            uint8_t _pad009d[0x3]; // 0x9d
            // metadata: MNetworkEnable
            std::int32_t m_nQueuedMatchmakingMode; // 0xa0            
            // metadata: MNetworkEnable
            bool m_bIsValveDS; // 0xa4            
            // metadata: MNetworkEnable
            bool m_bLogoMap; // 0xa5            
            // metadata: MNetworkEnable
            bool m_bPlayAllStepSoundsOnServer; // 0xa6            
            uint8_t _pad00a7[0x1]; // 0xa7
            // metadata: MNetworkEnable
            std::int32_t m_iSpectatorSlotCount; // 0xa8            
            // metadata: MNetworkEnable
            std::int32_t m_MatchDevice; // 0xac            
            // metadata: MNetworkEnable
            bool m_bHasMatchStarted; // 0xb0            
            uint8_t _pad00b1[0x3]; // 0xb1
            // metadata: MNetworkEnable
            std::int32_t m_nNextMapInMapgroup; // 0xb4            
            // metadata: MNetworkEnable
            char m_szTournamentEventName[512]; // 0xb8            
            // metadata: MNetworkEnable
            char m_szTournamentEventStage[512]; // 0x2b8            
            // metadata: MNetworkEnable
            char m_szMatchStatTxt[512]; // 0x4b8            
            // metadata: MNetworkEnable
            char m_szTournamentPredictionsTxt[512]; // 0x6b8            
            // metadata: MNetworkEnable
            std::int32_t m_nTournamentPredictionsPct; // 0x8b8            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flCMMItemDropRevealStartTime; // 0x8bc            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flCMMItemDropRevealEndTime; // 0x8c0            
            // metadata: MNetworkEnable
            bool m_bIsDroppingItems; // 0x8c4            
            // metadata: MNetworkEnable
            bool m_bIsQuestEligible; // 0x8c5            
            // metadata: MNetworkEnable
            bool m_bIsHltvActive; // 0x8c6            
            // metadata: MNetworkEnable
            bool m_bBombPlanted; // 0x8c7            
            // metadata: MNetworkEnable
            std::uint16_t m_arrProhibitedItemIndices[100]; // 0x8c8            
            // metadata: MNetworkEnable
            std::uint32_t m_arrTournamentActiveCasterAccounts[4]; // 0x990            
            // metadata: MNetworkEnable
            std::int32_t m_numBestOfMaps; // 0x9a0            
            // metadata: MNetworkEnable
            std::int32_t m_nHalloweenMaskListSeed; // 0x9a4            
            // metadata: MNetworkEnable
            bool m_bBombDropped; // 0x9a8            
            uint8_t _pad09a9[0x3]; // 0x9a9
            // metadata: MNetworkEnable
            std::int32_t m_iRoundWinStatus; // 0x9ac            
            // metadata: MNetworkEnable
            std::int32_t m_eRoundWinReason; // 0x9b0            
            // metadata: MNetworkEnable
            bool m_bTCantBuy; // 0x9b4            
            // metadata: MNetworkEnable
            bool m_bCTCantBuy; // 0x9b5            
            uint8_t _pad09b6[0x2]; // 0x9b6
            // metadata: MNetworkEnable
            std::int32_t m_iMatchStats_RoundResults[30]; // 0x9b8            
            // metadata: MNetworkEnable
            std::int32_t m_iMatchStats_PlayersAlive_CT[30]; // 0xa30            
            // metadata: MNetworkEnable
            std::int32_t m_iMatchStats_PlayersAlive_T[30]; // 0xaa8            
            // metadata: MNetworkEnable
            float m_TeamRespawnWaveTimes[32]; // 0xb20            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flNextRespawnWave[32]; // 0xba0            
            // metadata: MNetworkEnable
            Vector m_vMinimapMins; // 0xc20            
            // metadata: MNetworkEnable
            Vector m_vMinimapMaxs; // 0xc2c            
            // metadata: MNetworkEnable
            float m_MinimapVerticalSectionHeights[8]; // 0xc38            
            std::uint64_t m_ullLocalMatchID; // 0xc58            
            // metadata: MNetworkEnable
            std::int32_t m_nEndMatchMapGroupVoteTypes[10]; // 0xc60            
            // metadata: MNetworkEnable
            std::int32_t m_nEndMatchMapGroupVoteOptions[10]; // 0xc88            
            // metadata: MNetworkEnable
            std::int32_t m_nEndMatchMapVoteWinner; // 0xcb0            
            // metadata: MNetworkEnable
            std::int32_t m_iNumConsecutiveCTLoses; // 0xcb4            
            // metadata: MNetworkEnable
            std::int32_t m_iNumConsecutiveTerroristLoses; // 0xcb8            
            uint8_t _pad0cbc[0xbc]; // 0xcbc
            // metadata: MNetworkEnable
            std::int32_t m_nMatchAbortedEarlyReason; // 0xd78            
            bool m_bHasTriggeredRoundStartMusic; // 0xd7c            
            bool m_bSwitchingTeamsAtRoundReset; // 0xd7d            
            uint8_t _pad0d7e[0x1a]; // 0xd7e
            // metadata: MNetworkEnable
            // metadata: MNetworkPolymorphic
            source2sdk::client::CCSGameModeRules* m_pGameModeRules; // 0xd98            
            // metadata: MNetworkEnable
            source2sdk::client::C_RetakeGameRules m_RetakeRules; // 0xda0            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnMatchEndCountChanged"
            std::uint8_t m_nMatchEndCount; // 0xef8            
            uint8_t _pad0ef9[0x3]; // 0xef9
            // metadata: MNetworkEnable
            std::int32_t m_nTTeamIntroVariant; // 0xefc            
            // metadata: MNetworkEnable
            std::int32_t m_nCTTeamIntroVariant; // 0xf00            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnTeamIntroPeriodChanged"
            bool m_bTeamIntroPeriod; // 0xf04            
            uint8_t _pad0f05[0x3]; // 0xf05
            // metadata: MNetworkEnable
            std::int32_t m_iRoundEndWinnerTeam; // 0xf08            
            // metadata: MNetworkEnable
            std::int32_t m_eRoundEndReason; // 0xf0c            
            // metadata: MNetworkEnable
            bool m_bRoundEndShowTimerDefend; // 0xf10            
            uint8_t _pad0f11[0x3]; // 0xf11
            // metadata: MNetworkEnable
            std::int32_t m_iRoundEndTimerTime; // 0xf14            
            // metadata: MNetworkEnable
            CUtlString m_sRoundEndFunFactToken; // 0xf18            
            // metadata: MNetworkEnable
            CPlayerSlot m_iRoundEndFunFactPlayerSlot; // 0xf20            
            // metadata: MNetworkEnable
            std::int32_t m_iRoundEndFunFactData1; // 0xf24            
            // metadata: MNetworkEnable
            std::int32_t m_iRoundEndFunFactData2; // 0xf28            
            // metadata: MNetworkEnable
            std::int32_t m_iRoundEndFunFactData3; // 0xf2c            
            // metadata: MNetworkEnable
            CUtlString m_sRoundEndMessage; // 0xf30            
            // metadata: MNetworkEnable
            std::int32_t m_iRoundEndPlayerCount; // 0xf38            
            // metadata: MNetworkEnable
            bool m_bRoundEndNoMusic; // 0xf3c            
            uint8_t _pad0f3d[0x3]; // 0xf3d
            // metadata: MNetworkEnable
            std::int32_t m_iRoundEndLegacy; // 0xf40            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnRoundEndCountChanged"
            std::uint8_t m_nRoundEndCount; // 0xf44            
            uint8_t _pad0f45[0x3]; // 0xf45
            // metadata: MNetworkEnable
            std::int32_t m_iRoundStartRoundNumber; // 0xf48            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnRoundStartCountChanged"
            std::uint8_t m_nRoundStartCount; // 0xf4c            
            uint8_t _pad0f4d[0x400b]; // 0xf4d
            double m_flLastPerfSampleTime; // 0x4f58            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_CSGameRules because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_CSGameRules) == 0x4f60);
    };
};
