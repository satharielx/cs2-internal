#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/QuestProgress_Reason.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CBasePlayerController.hpp"
#include "source2sdk/server/IntervalTimer.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CCSObserverPawn;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayerController_ActionTrackingServices;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayerController_DamageServices;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayerController_InGameMoneyServices;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayerController_InventoryServices;
    };
};
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
        // Registered alignment: 0x8
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0xaa0
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkUserGroupProxy "CCSPlayerController"
        // static metadata: MNetworkUserGroupProxy "CCSPlayerController"
        // static metadata: MNetworkUserGroupProxy "CCSPlayerController"
        // static metadata: MNetworkUserGroupProxy "CCSPlayerController"
        // static metadata: MNetworkVarNames "CCSPlayerController_InGameMoneyServices * m_pInGameMoneyServices"
        // static metadata: MNetworkVarNames "CCSPlayerController_InventoryServices * m_pInventoryServices"
        // static metadata: MNetworkVarNames "CCSPlayerController_ActionTrackingServices * m_pActionTrackingServices"
        // static metadata: MNetworkVarNames "CCSPlayerController_DamageServices * m_pDamageServices"
        // static metadata: MNetworkVarNames "uint32 m_iPing"
        // static metadata: MNetworkVarNames "bool m_bHasCommunicationAbuseMute"
        // static metadata: MNetworkVarNames "uint32 m_uiCommunicationMuteFlags"
        // static metadata: MNetworkVarNames "string_t m_szCrosshairCodes"
        // static metadata: MNetworkVarNames "uint8 m_iPendingTeamNum"
        // static metadata: MNetworkVarNames "GameTime_t m_flForceTeamTime"
        // static metadata: MNetworkVarNames "int m_iCompTeammateColor"
        // static metadata: MNetworkVarNames "bool m_bEverPlayedOnTeam"
        // static metadata: MNetworkVarNames "string_t m_szClan"
        // static metadata: MNetworkVarNames "int m_iCoachingTeam"
        // static metadata: MNetworkVarNames "uint64 m_nPlayerDominated"
        // static metadata: MNetworkVarNames "uint64 m_nPlayerDominatingMe"
        // static metadata: MNetworkVarNames "int m_iCompetitiveRanking"
        // static metadata: MNetworkVarNames "int m_iCompetitiveWins"
        // static metadata: MNetworkVarNames "int8 m_iCompetitiveRankType"
        // static metadata: MNetworkVarNames "int m_iCompetitiveRankingPredicted_Win"
        // static metadata: MNetworkVarNames "int m_iCompetitiveRankingPredicted_Loss"
        // static metadata: MNetworkVarNames "int m_iCompetitiveRankingPredicted_Tie"
        // static metadata: MNetworkVarNames "int m_nEndMatchNextMapVote"
        // static metadata: MNetworkVarNames "uint16 m_unActiveQuestId"
        // static metadata: MNetworkVarNames "RTime32 m_rtActiveMissionPeriod"
        // static metadata: MNetworkVarNames "QuestProgress::Reason m_nQuestProgressReason"
        // static metadata: MNetworkVarNames "uint32 m_unPlayerTvControlFlags"
        // static metadata: MNetworkVarNames "int m_nDisconnectionTick"
        // static metadata: MNetworkVarNames "bool m_bControllingBot"
        // static metadata: MNetworkVarNames "bool m_bHasControlledBotThisRound"
        // static metadata: MNetworkVarNames "bool m_bCanControlObservedBot"
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerPawn> m_hPlayerPawn"
        // static metadata: MNetworkVarNames "CHandle< CCSObserverPawn> m_hObserverPawn"
        // static metadata: MNetworkVarNames "bool m_bPawnIsAlive"
        // static metadata: MNetworkVarNames "uint32 m_iPawnHealth"
        // static metadata: MNetworkVarNames "int m_iPawnArmor"
        // static metadata: MNetworkVarNames "bool m_bPawnHasDefuser"
        // static metadata: MNetworkVarNames "bool m_bPawnHasHelmet"
        // static metadata: MNetworkVarNames "item_definition_index_t m_nPawnCharacterDefIndex"
        // static metadata: MNetworkVarNames "int m_iPawnLifetimeStart"
        // static metadata: MNetworkVarNames "int m_iPawnLifetimeEnd"
        // static metadata: MNetworkVarNames "int m_iPawnBotDifficulty"
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerController> m_hOriginalControllerOfCurrentPawn"
        // static metadata: MNetworkVarNames "int32 m_iScore"
        // static metadata: MNetworkVarNames "uint8 m_recentKillQueue"
        // static metadata: MNetworkVarNames "uint8 m_nFirstKill"
        // static metadata: MNetworkVarNames "uint8 m_nKillCount"
        // static metadata: MNetworkVarNames "bool m_bMvpNoMusic"
        // static metadata: MNetworkVarNames "int m_eMvpReason"
        // static metadata: MNetworkVarNames "int m_iMusicKitID"
        // static metadata: MNetworkVarNames "int m_iMusicKitMVPs"
        // static metadata: MNetworkVarNames "int m_iMVPs"
        // static metadata: MNetworkVarNames "bool m_bFireBulletsSeedSynchronized"
        #pragma pack(push, 1)
        class CCSPlayerController : public source2sdk::server::CBasePlayerController
        {
        public:
            uint8_t _pad07c8[0x10]; // 0x7c8
            // metadata: MNetworkEnable
            source2sdk::server::CCSPlayerController_InGameMoneyServices* m_pInGameMoneyServices; // 0x7d8            
            // metadata: MNetworkEnable
            source2sdk::server::CCSPlayerController_InventoryServices* m_pInventoryServices; // 0x7e0            
            // metadata: MNetworkEnable
            source2sdk::server::CCSPlayerController_ActionTrackingServices* m_pActionTrackingServices; // 0x7e8            
            // metadata: MNetworkEnable
            source2sdk::server::CCSPlayerController_DamageServices* m_pDamageServices; // 0x7f0            
            // metadata: MNetworkEnable
            std::uint32_t m_iPing; // 0x7f8            
            // metadata: MNetworkEnable
            bool m_bHasCommunicationAbuseMute; // 0x7fc            
            uint8_t _pad07fd[0x3]; // 0x7fd
            // metadata: MNetworkEnable
            std::uint32_t m_uiCommunicationMuteFlags; // 0x800            
            uint8_t _pad0804[0x4]; // 0x804
            // metadata: MNetworkEnable
            CUtlSymbolLarge m_szCrosshairCodes; // 0x808            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnPendingTeamChanged"
            std::uint8_t m_iPendingTeamNum; // 0x810            
            uint8_t _pad0811[0x3]; // 0x811
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flForceTeamTime; // 0x814            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnCompTeammateColorChanged"
            std::int32_t m_iCompTeammateColor; // 0x818            
            // metadata: MNetworkEnable
            bool m_bEverPlayedOnTeam; // 0x81c            
            bool m_bAttemptedToGetColor; // 0x81d            
            uint8_t _pad081e[0x2]; // 0x81e
            std::int32_t m_iTeammatePreferredColor; // 0x820            
            bool m_bTeamChanged; // 0x824            
            bool m_bInSwitchTeam; // 0x825            
            bool m_bHasSeenJoinGame; // 0x826            
            bool m_bJustBecameSpectator; // 0x827            
            bool m_bSwitchTeamsOnNextRoundReset; // 0x828            
            bool m_bRemoveAllItemsOnNextRoundReset; // 0x829            
            uint8_t _pad082a[0x2]; // 0x82a
            source2sdk::entity2::GameTime_t m_flLastJoinTeamTime; // 0x82c            
            // metadata: MNetworkEnable
            CUtlSymbolLarge m_szClan; // 0x830            
            // metadata: MNetworkEnable
            std::int32_t m_iCoachingTeam; // 0x838            
            uint8_t _pad083c[0x4]; // 0x83c
            // metadata: MNetworkEnable
            std::uint64_t m_nPlayerDominated; // 0x840            
            // metadata: MNetworkEnable
            std::uint64_t m_nPlayerDominatingMe; // 0x848            
            // metadata: MNetworkEnable
            std::int32_t m_iCompetitiveRanking; // 0x850            
            // metadata: MNetworkEnable
            std::int32_t m_iCompetitiveWins; // 0x854            
            // metadata: MNetworkEnable
            std::int8_t m_iCompetitiveRankType; // 0x858            
            uint8_t _pad0859[0x3]; // 0x859
            // metadata: MNetworkEnable
            std::int32_t m_iCompetitiveRankingPredicted_Win; // 0x85c            
            // metadata: MNetworkEnable
            std::int32_t m_iCompetitiveRankingPredicted_Loss; // 0x860            
            // metadata: MNetworkEnable
            std::int32_t m_iCompetitiveRankingPredicted_Tie; // 0x864            
            // metadata: MNetworkEnable
            std::int32_t m_nEndMatchNextMapVote; // 0x868            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            std::uint16_t m_unActiveQuestId; // 0x86c            
            uint8_t _pad086e[0x2]; // 0x86e
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            std::uint32_t m_rtActiveMissionPeriod; // 0x870            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            source2sdk::client::QuestProgress_Reason m_nQuestProgressReason; // 0x874            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            std::uint32_t m_unPlayerTvControlFlags; // 0x878            
            uint8_t _pad087c[0x2c]; // 0x87c
            std::int32_t m_iDraftIndex; // 0x8a8            
            std::uint32_t m_msQueuedModeDisconnectionTimestamp; // 0x8ac            
            std::uint32_t m_uiAbandonRecordedReason; // 0x8b0            
            std::uint32_t m_eNetworkDisconnectionReason; // 0x8b4            
            bool m_bCannotBeKicked; // 0x8b8            
            bool m_bEverFullyConnected; // 0x8b9            
            bool m_bAbandonAllowsSurrender; // 0x8ba            
            bool m_bAbandonOffersInstantSurrender; // 0x8bb            
            bool m_bDisconnection1MinWarningPrinted; // 0x8bc            
            bool m_bScoreReported; // 0x8bd            
            uint8_t _pad08be[0x2]; // 0x8be
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            std::int32_t m_nDisconnectionTick; // 0x8c0            
            uint8_t _pad08c4[0xc]; // 0x8c4
            // metadata: MNetworkEnable
            bool m_bControllingBot; // 0x8d0            
            // metadata: MNetworkEnable
            bool m_bHasControlledBotThisRound; // 0x8d1            
            bool m_bHasBeenControlledByPlayerThisRound; // 0x8d2            
            uint8_t _pad08d3[0x1]; // 0x8d3
            std::int32_t m_nBotsControlledThisRound; // 0x8d4            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            bool m_bCanControlObservedBot; // 0x8d8            
            uint8_t _pad08d9[0x3]; // 0x8d9
            // metadata: MNetworkEnable
            // m_hPlayerPawn has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_hPlayerPawn;
            char m_hPlayerPawn[0x4]; // 0x8dc            
            // metadata: MNetworkEnable
            // m_hObserverPawn has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSObserverPawn> m_hObserverPawn;
            char m_hObserverPawn[0x4]; // 0x8e0            
            std::int32_t m_DesiredObserverMode; // 0x8e4            
            CEntityHandle m_hDesiredObserverTarget; // 0x8e8            
            // metadata: MNetworkEnable
            bool m_bPawnIsAlive; // 0x8ec            
            uint8_t _pad08ed[0x3]; // 0x8ed
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "TeammateAndSpectatorExclusive"
            std::uint32_t m_iPawnHealth; // 0x8f0            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "TeammateAndSpectatorExclusive"
            std::int32_t m_iPawnArmor; // 0x8f4            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "TeammateAndSpectatorExclusive"
            bool m_bPawnHasDefuser; // 0x8f8            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "TeammateAndSpectatorExclusive"
            bool m_bPawnHasHelmet; // 0x8f9            
            // metadata: MNetworkEnable
            std::uint16_t m_nPawnCharacterDefIndex; // 0x8fa            
            // metadata: MNetworkEnable
            std::int32_t m_iPawnLifetimeStart; // 0x8fc            
            // metadata: MNetworkEnable
            std::int32_t m_iPawnLifetimeEnd; // 0x900            
            // metadata: MNetworkEnable
            std::int32_t m_iPawnBotDifficulty; // 0x904            
            // metadata: MNetworkEnable
            // m_hOriginalControllerOfCurrentPawn has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerController> m_hOriginalControllerOfCurrentPawn;
            char m_hOriginalControllerOfCurrentPawn[0x4]; // 0x908            
            // metadata: MNetworkEnable
            std::int32_t m_iScore; // 0x90c            
            std::int32_t m_iRoundScore; // 0x910            
            std::int32_t m_iRoundsWon; // 0x914            
            // metadata: MNetworkEnable
            std::uint8_t m_recentKillQueue[8]; // 0x918            
            // metadata: MNetworkEnable
            std::uint8_t m_nFirstKill; // 0x920            
            // metadata: MNetworkEnable
            std::uint8_t m_nKillCount; // 0x921            
            // metadata: MNetworkEnable
            bool m_bMvpNoMusic; // 0x922            
            uint8_t _pad0923[0x1]; // 0x923
            // metadata: MNetworkEnable
            std::int32_t m_eMvpReason; // 0x924            
            // metadata: MNetworkEnable
            std::int32_t m_iMusicKitID; // 0x928            
            // metadata: MNetworkEnable
            std::int32_t m_iMusicKitMVPs; // 0x92c            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnMVPCountChanged"
            std::int32_t m_iMVPs; // 0x930            
            std::int32_t m_nUpdateCounter; // 0x934            
            float m_flSmoothedPing; // 0x938            
            uint8_t _pad093c[0x4]; // 0x93c
            source2sdk::server::IntervalTimer m_lastHeldVoteTimer; // 0x940            
            uint8_t _pad0950[0x8]; // 0x950
            bool m_bShowHints; // 0x958            
            uint8_t _pad0959[0x3]; // 0x959
            std::int32_t m_iNextTimeCheck; // 0x95c            
            bool m_bJustDidTeamKill; // 0x960            
            bool m_bPunishForTeamKill; // 0x961            
            bool m_bGaveTeamDamageWarning; // 0x962            
            bool m_bGaveTeamDamageWarningThisRound; // 0x963            
            uint8_t _pad0964[0x4]; // 0x964
            double m_dblLastReceivedPacketPlatFloatTime; // 0x968            
            source2sdk::entity2::GameTime_t m_LastTeamDamageWarningTime; // 0x970            
            source2sdk::entity2::GameTime_t m_LastTimePlayerWasDisconnectedForPawnsRemove; // 0x974            
            std::uint32_t m_nSuspiciousHitCount; // 0x978            
            std::uint32_t m_nNonSuspiciousHitStreak; // 0x97c            
            uint8_t _pad0980[0xa1]; // 0x980
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            bool m_bFireBulletsSeedSynchronized; // 0xa21            
            uint8_t _pad0a22[0x7e];
            
            // Datamap fields:
            // void CCSPlayerControllerPlayerForceTeamThink; // 0x0
            // void CCSPlayerControllerResetForceTeamThink; // 0x0
            // void CCSPlayerControllerResourceDataThink; // 0x0
            // void CCSPlayerControllerInventoryUpdateThink; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSPlayerController because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CCSPlayerController) == 0xaa0);
    };
};
