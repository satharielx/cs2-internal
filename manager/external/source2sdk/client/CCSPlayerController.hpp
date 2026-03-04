#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CBasePlayerController.hpp"
#include "source2sdk/client/QuestProgress_Reason.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
namespace source2sdk
{
    namespace client
    {
        struct CCSPlayerController_ActionTrackingServices;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct CCSPlayerController_DamageServices;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct CCSPlayerController_InGameMoneyServices;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct CCSPlayerController_InventoryServices;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct C_CSObserverPawn;
    };
};
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
        // Registered alignment: 0x8
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x958
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
        class CCSPlayerController : public source2sdk::client::CBasePlayerController
        {
        public:
            uint8_t _pad07f8[0x10]; // 0x7f8
            // metadata: MNetworkEnable
            source2sdk::client::CCSPlayerController_InGameMoneyServices* m_pInGameMoneyServices; // 0x808            
            // metadata: MNetworkEnable
            source2sdk::client::CCSPlayerController_InventoryServices* m_pInventoryServices; // 0x810            
            // metadata: MNetworkEnable
            source2sdk::client::CCSPlayerController_ActionTrackingServices* m_pActionTrackingServices; // 0x818            
            // metadata: MNetworkEnable
            source2sdk::client::CCSPlayerController_DamageServices* m_pDamageServices; // 0x820            
            // metadata: MNetworkEnable
            std::uint32_t m_iPing; // 0x828            
            // metadata: MNetworkEnable
            bool m_bHasCommunicationAbuseMute; // 0x82c            
            uint8_t _pad082d[0x3]; // 0x82d
            // metadata: MNetworkEnable
            std::uint32_t m_uiCommunicationMuteFlags; // 0x830            
            uint8_t _pad0834[0x4]; // 0x834
            // metadata: MNetworkEnable
            CUtlSymbolLarge m_szCrosshairCodes; // 0x838            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnPendingTeamChanged"
            std::uint8_t m_iPendingTeamNum; // 0x840            
            uint8_t _pad0841[0x3]; // 0x841
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flForceTeamTime; // 0x844            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnCompTeammateColorChanged"
            std::int32_t m_iCompTeammateColor; // 0x848            
            // metadata: MNetworkEnable
            bool m_bEverPlayedOnTeam; // 0x84c            
            uint8_t _pad084d[0x3]; // 0x84d
            source2sdk::entity2::GameTime_t m_flPreviousForceJoinTeamTime; // 0x850            
            uint8_t _pad0854[0x4]; // 0x854
            // metadata: MNetworkEnable
            CUtlSymbolLarge m_szClan; // 0x858            
            CUtlString m_sSanitizedPlayerName; // 0x860            
            // metadata: MNetworkEnable
            std::int32_t m_iCoachingTeam; // 0x868            
            uint8_t _pad086c[0x4]; // 0x86c
            // metadata: MNetworkEnable
            std::uint64_t m_nPlayerDominated; // 0x870            
            // metadata: MNetworkEnable
            std::uint64_t m_nPlayerDominatingMe; // 0x878            
            // metadata: MNetworkEnable
            std::int32_t m_iCompetitiveRanking; // 0x880            
            // metadata: MNetworkEnable
            std::int32_t m_iCompetitiveWins; // 0x884            
            // metadata: MNetworkEnable
            std::int8_t m_iCompetitiveRankType; // 0x888            
            uint8_t _pad0889[0x3]; // 0x889
            // metadata: MNetworkEnable
            std::int32_t m_iCompetitiveRankingPredicted_Win; // 0x88c            
            // metadata: MNetworkEnable
            std::int32_t m_iCompetitiveRankingPredicted_Loss; // 0x890            
            // metadata: MNetworkEnable
            std::int32_t m_iCompetitiveRankingPredicted_Tie; // 0x894            
            // metadata: MNetworkEnable
            std::int32_t m_nEndMatchNextMapVote; // 0x898            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            std::uint16_t m_unActiveQuestId; // 0x89c            
            uint8_t _pad089e[0x2]; // 0x89e
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            std::uint32_t m_rtActiveMissionPeriod; // 0x8a0            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            source2sdk::client::QuestProgress_Reason m_nQuestProgressReason; // 0x8a4            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            std::uint32_t m_unPlayerTvControlFlags; // 0x8a8            
            uint8_t _pad08ac[0x2c]; // 0x8ac
            std::int32_t m_iDraftIndex; // 0x8d8            
            std::uint32_t m_msQueuedModeDisconnectionTimestamp; // 0x8dc            
            std::uint32_t m_uiAbandonRecordedReason; // 0x8e0            
            std::uint32_t m_eNetworkDisconnectionReason; // 0x8e4            
            bool m_bCannotBeKicked; // 0x8e8            
            bool m_bEverFullyConnected; // 0x8e9            
            bool m_bAbandonAllowsSurrender; // 0x8ea            
            bool m_bAbandonOffersInstantSurrender; // 0x8eb            
            bool m_bDisconnection1MinWarningPrinted; // 0x8ec            
            bool m_bScoreReported; // 0x8ed            
            uint8_t _pad08ee[0x2]; // 0x8ee
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            std::int32_t m_nDisconnectionTick; // 0x8f0            
            uint8_t _pad08f4[0xc]; // 0x8f4
            // metadata: MNetworkEnable
            bool m_bControllingBot; // 0x900            
            // metadata: MNetworkEnable
            bool m_bHasControlledBotThisRound; // 0x901            
            bool m_bHasBeenControlledByPlayerThisRound; // 0x902            
            uint8_t _pad0903[0x1]; // 0x903
            std::int32_t m_nBotsControlledThisRound; // 0x904            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            bool m_bCanControlObservedBot; // 0x908            
            uint8_t _pad0909[0x3]; // 0x909
            // metadata: MNetworkEnable
            // m_hPlayerPawn has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_CSPlayerPawn> m_hPlayerPawn;
            char m_hPlayerPawn[0x4]; // 0x90c            
            // metadata: MNetworkEnable
            // m_hObserverPawn has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_CSObserverPawn> m_hObserverPawn;
            char m_hObserverPawn[0x4]; // 0x910            
            // metadata: MNetworkEnable
            bool m_bPawnIsAlive; // 0x914            
            uint8_t _pad0915[0x3]; // 0x915
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "TeammateAndSpectatorExclusive"
            std::uint32_t m_iPawnHealth; // 0x918            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "TeammateAndSpectatorExclusive"
            std::int32_t m_iPawnArmor; // 0x91c            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "TeammateAndSpectatorExclusive"
            bool m_bPawnHasDefuser; // 0x920            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "TeammateAndSpectatorExclusive"
            bool m_bPawnHasHelmet; // 0x921            
            // metadata: MNetworkEnable
            std::uint16_t m_nPawnCharacterDefIndex; // 0x922            
            // metadata: MNetworkEnable
            std::int32_t m_iPawnLifetimeStart; // 0x924            
            // metadata: MNetworkEnable
            std::int32_t m_iPawnLifetimeEnd; // 0x928            
            // metadata: MNetworkEnable
            std::int32_t m_iPawnBotDifficulty; // 0x92c            
            // metadata: MNetworkEnable
            // m_hOriginalControllerOfCurrentPawn has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::CCSPlayerController> m_hOriginalControllerOfCurrentPawn;
            char m_hOriginalControllerOfCurrentPawn[0x4]; // 0x930            
            // metadata: MNetworkEnable
            std::int32_t m_iScore; // 0x934            
            // metadata: MNetworkEnable
            std::uint8_t m_recentKillQueue[8]; // 0x938            
            // metadata: MNetworkEnable
            std::uint8_t m_nFirstKill; // 0x940            
            // metadata: MNetworkEnable
            std::uint8_t m_nKillCount; // 0x941            
            // metadata: MNetworkEnable
            bool m_bMvpNoMusic; // 0x942            
            uint8_t _pad0943[0x1]; // 0x943
            // metadata: MNetworkEnable
            std::int32_t m_eMvpReason; // 0x944            
            // metadata: MNetworkEnable
            std::int32_t m_iMusicKitID; // 0x948            
            // metadata: MNetworkEnable
            std::int32_t m_iMusicKitMVPs; // 0x94c            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnMVPCountChanged"
            std::int32_t m_iMVPs; // 0x950            
            bool m_bIsPlayerNameDirty; // 0x954            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            bool m_bFireBulletsSeedSynchronized; // 0x955            
            uint8_t _pad0956[0x2];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSPlayerController because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::CCSPlayerController) == 0x958);
    };
};
