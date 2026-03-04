#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/ChatIgnoreType_t.hpp"
#include "source2sdk/client/PlayerConnectedState.hpp"
#include "source2sdk/server/CBaseEntity.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CBasePlayerPawn;
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
        // Size: 0x7c8
        // Has VTable
        // Construct allowed
        // MNetworkNoBase
        // 
        // static metadata: MNetworkIncludeByName "m_pEntity"
        // static metadata: MNetworkIncludeByName "m_flSimulationTime"
        // static metadata: MNetworkIncludeByName "m_flCreateTime"
        // static metadata: MNetworkIncludeByName "m_iTeamNum"
        // static metadata: MNetworkIncludeByName "m_nNextThinkTick"
        // static metadata: MNetworkIncludeByName "m_fFlags"
        // static metadata: MNetworkUserGroupProxy "CBasePlayerController"
        // static metadata: MNetworkUserGroupProxy "CBasePlayerController"
        // static metadata: MNetworkIncludeByUserGroup "LocalPlayerExclusive"
        // static metadata: MNetworkVarNames "uint32 m_nTickBase"
        // static metadata: MNetworkVarNames "CHandle< CBasePlayerPawn> m_hPawn"
        // static metadata: MNetworkVarNames "bool m_bKnownTeamMismatch"
        // static metadata: MNetworkVarNames "PlayerConnectedState m_iConnected"
        // static metadata: MNetworkVarNames "char m_iszPlayerName"
        // static metadata: MNetworkVarNames "uint64 m_steamID"
        // static metadata: MNetworkVarNames "bool m_bNoClipEnabled"
        // static metadata: MNetworkVarNames "uint32 m_iDesiredFOV"
        #pragma pack(push, 1)
        class CBasePlayerController : public source2sdk::server::CBaseEntity
        {
        public:
            uint8_t _pad04a8[0x8]; // 0x4a8
            // metadata: MNotSaved
            std::uint64_t m_nInButtonsWhichAreToggles; // 0x4b0            
            // metadata: MNetworkEnable
            // metadata: MNetworkPriority "1"
            // metadata: MNetworkUserGroup "LocalPlayerExclusive"
            // metadata: MNotSaved
            std::uint32_t m_nTickBase; // 0x4b8            
            uint8_t _pad04bc[0x24]; // 0x4bc
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnPawnChanged"
            // m_hPawn has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBasePlayerPawn> m_hPawn;
            char m_hPawn[0x4]; // 0x4e0            
            // metadata: MNetworkEnable
            bool m_bKnownTeamMismatch; // 0x4e4            
            uint8_t _pad04e5[0x3]; // 0x4e5
            // metadata: MNotSaved
            CSplitScreenSlot m_nSplitScreenSlot; // 0x4e8            
            // metadata: MNotSaved
            // m_hSplitOwner has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBasePlayerController> m_hSplitOwner;
            char m_hSplitOwner[0x4]; // 0x4ec            
            // metadata: MNotSaved
            // m_hSplitScreenPlayers has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<CHandle<source2sdk::server::CBasePlayerController>> m_hSplitScreenPlayers;
            char m_hSplitScreenPlayers[0x18]; // 0x4f0            
            bool m_bIsHLTV; // 0x508            
            uint8_t _pad0509[0x3]; // 0x509
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnConnectionStateChanged"
            // metadata: MNotSaved
            source2sdk::client::PlayerConnectedState m_iConnected; // 0x50c            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnPlayerControllerNameChanged"
            // metadata: MNotSaved
            char m_iszPlayerName[128]; // 0x510            
            // metadata: MNotSaved
            CUtlString m_szNetworkIDString; // 0x590            
            // metadata: MNotSaved
            float m_fLerpTime; // 0x598            
            // metadata: MNotSaved
            bool m_bLagCompensation; // 0x59c            
            // metadata: MNotSaved
            bool m_bPredict; // 0x59d            
            uint8_t _pad059e[0x6]; // 0x59e
            // metadata: MNotSaved
            bool m_bIsLowViolence; // 0x5a4            
            // metadata: MNotSaved
            bool m_bGamePaused; // 0x5a5            
            uint8_t _pad05a6[0x13a]; // 0x5a6
            // metadata: MNotSaved
            source2sdk::client::ChatIgnoreType_t m_iIgnoreGlobalChat; // 0x6e0            
            float m_flLastPlayerTalkTime; // 0x6e4            
            // metadata: MNotSaved
            float m_flLastEntitySteadyState; // 0x6e8            
            // metadata: MNotSaved
            std::int32_t m_nAvailableEntitySteadyState; // 0x6ec            
            // metadata: MNotSaved
            bool m_bHasAnySteadyStateEnts; // 0x6f0            
            uint8_t _pad06f1[0xf]; // 0x6f1
            // metadata: MNetworkEnable
            // metadata: MNetworkEncoder "fixed64"
            // metadata: MNetworkChangeCallback "OnSteamIDChanged"
            // metadata: MNotSaved
            std::uint64_t m_steamID; // 0x700            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnNoClipEnableChanged"
            bool m_bNoClipEnabled; // 0x708            
            uint8_t _pad0709[0x3]; // 0x709
            // metadata: MNetworkEnable
            std::uint32_t m_iDesiredFOV; // 0x70c            
            uint8_t _pad0710[0xb8];
            
            // Datamap fields:
            // bool fakeclient; // 0x7fffffff
            // bool is_hltv; // 0x7fffffff
            // const char * playername; // 0x7fffffff
            // bool reserving; // 0x7fffffff
            // void m_pCurrentCommand; // 0x6f8
            // int32_t m_nFramePerfSamplesTotalReceived; // 0x748
            // int32_t m_ePlayerControllerKickPolicyMask; // 0x5a0
            // void m_LastCmd; // 0x5a8
            // void m_nLastRealCommandNumberExecuted; // 0x6c4
            // void m_nLastLateCommandExecuted; // 0x6c8
            // void m_hConVarUserInfoSet; // 0x7b8
            // void m_AlwaysInPVSEntities; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CBasePlayerController because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CBasePlayerController) == 0x7c8);
    };
};
