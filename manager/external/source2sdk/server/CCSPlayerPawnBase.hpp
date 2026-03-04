#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CSPlayerState.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CBasePlayerPawn.hpp"
#include "source2sdk/server/CTouchExpansionComponent.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayerController;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayer_PingServices;
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
        // Size: 0xe70
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "CTouchExpansionComponent::Storage_t m_CTouchExpansionComponent"
        // static metadata: MNetworkExcludeByName "m_flexWeight"
        // static metadata: MNetworkExcludeByUserGroup "m_flPoseParameter"
        // static metadata: MNetworkExcludeByName "m_baseLayer.m_hSequence"
        // static metadata: MNetworkExcludeByName "m_animationController.m_flPlaybackRate"
        // static metadata: MNetworkVarNames "CCSPlayer_PingServices * m_pPingServices"
        // static metadata: MNetworkVarNames "CSPlayerState m_iPlayerState"
        // static metadata: MNetworkVarNames "bool m_bHasMovedSinceSpawn"
        // static metadata: MNetworkVarNames "float m_flFlashDuration"
        // static metadata: MNetworkVarNames "float m_flFlashMaxAlpha"
        // static metadata: MNetworkVarNames "float m_flProgressBarStartTime"
        // static metadata: MNetworkVarNames "int m_iProgressBarDuration"
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerController> m_hOriginalController"
        #pragma pack(push, 1)
        class CCSPlayerPawnBase : public source2sdk::server::CBasePlayerPawn
        {
        public:
            uint8_t _pad0d20[0x10]; // 0xd20
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "CTouchExpansionComponent"
            // metadata: MNetworkAlias "CTouchExpansionComponent"
            // metadata: MNetworkTypeAlias "CTouchExpansionComponent"
            source2sdk::server::CTouchExpansionComponent m_CTouchExpansionComponent; // 0xd30            
            // metadata: MNetworkEnable
            source2sdk::server::CCSPlayer_PingServices* m_pPingServices; // 0xd80            
            source2sdk::entity2::GameTime_t m_blindUntilTime; // 0xd88            
            source2sdk::entity2::GameTime_t m_blindStartTime; // 0xd8c            
            // metadata: MNetworkEnable
            source2sdk::client::CSPlayerState m_iPlayerState; // 0xd90            
            uint8_t _pad0d94[0xac]; // 0xd94
            bool m_bRespawning; // 0xe40            
            // metadata: MNetworkEnable
            bool m_bHasMovedSinceSpawn; // 0xe41            
            uint8_t _pad0e42[0x2]; // 0xe42
            std::int32_t m_iNumSpawns; // 0xe44            
            uint8_t _pad0e48[0x4]; // 0xe48
            float m_flIdleTimeSinceLastAction; // 0xe4c            
            float m_fNextRadarUpdateTime; // 0xe50            
            // metadata: MNetworkEnable
            float m_flFlashDuration; // 0xe54            
            // metadata: MNetworkEnable
            float m_flFlashMaxAlpha; // 0xe58            
            // metadata: MNetworkEnable
            float m_flProgressBarStartTime; // 0xe5c            
            // metadata: MNetworkEnable
            std::int32_t m_iProgressBarDuration; // 0xe60            
            // metadata: MNetworkEnable
            // m_hOriginalController has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerController> m_hOriginalController;
            char m_hOriginalController[0x4]; // 0xe64            
            uint8_t _pad0e68[0x8];
            
            // Datamap fields:
            // CHandle< CBaseEntity > original_controller; // 0x7fffffff
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSPlayerPawnBase because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CCSPlayerPawnBase) == 0xe70);
    };
};
