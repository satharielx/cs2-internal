#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CSPlayerState.hpp"
#include "source2sdk/client/C_BasePlayerPawn.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
namespace source2sdk
{
    namespace client
    {
        struct CCSPlayerController;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct CCSPlayer_PingServices;
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
        // Size: 0x1650
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkExcludeByName "m_flexWeight"
        // static metadata: MNetworkExcludeByUserGroup "m_flPoseParameter"
        // static metadata: MNetworkExcludeByName "m_baseLayer.m_hSequence"
        // static metadata: MNetworkExcludeByName "m_animationController.m_flPlaybackRate"
        // static metadata: MNetworkVarNames "CCSPlayer_PingServices * m_pPingServices"
        // static metadata: MNetworkVarNames "CSPlayerState m_iPlayerState"
        // static metadata: MNetworkVarNames "bool m_bHasMovedSinceSpawn"
        // static metadata: MNetworkVarNames "int m_iProgressBarDuration"
        // static metadata: MNetworkVarNames "float m_flProgressBarStartTime"
        // static metadata: MNetworkVarNames "float m_flFlashMaxAlpha"
        // static metadata: MNetworkVarNames "float m_flFlashDuration"
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerController> m_hOriginalController"
        #pragma pack(push, 1)
        class C_CSPlayerPawnBase : public source2sdk::client::C_BasePlayerPawn
        {
        public:
            uint8_t _pad15b0[0x10]; // 0x15b0
            // metadata: MNetworkEnable
            source2sdk::client::CCSPlayer_PingServices* m_pPingServices; // 0x15c0            
            source2sdk::client::CSPlayerState m_previousPlayerState; // 0x15c8            
            // metadata: MNetworkEnable
            source2sdk::client::CSPlayerState m_iPlayerState; // 0x15cc            
            // metadata: MNetworkEnable
            bool m_bHasMovedSinceSpawn; // 0x15d0            
            uint8_t _pad15d1[0x3]; // 0x15d1
            source2sdk::entity2::GameTime_t m_flLastSpawnTimeIndex; // 0x15d4            
            // metadata: MNetworkEnable
            std::int32_t m_iProgressBarDuration; // 0x15d8            
            // metadata: MNetworkEnable
            float m_flProgressBarStartTime; // 0x15dc            
            source2sdk::entity2::GameTime_t m_flClientDeathTime; // 0x15e0            
            float m_flFlashBangTime; // 0x15e4            
            float m_flFlashScreenshotAlpha; // 0x15e8            
            float m_flFlashOverlayAlpha; // 0x15ec            
            bool m_bFlashBuildUp; // 0x15f0            
            bool m_bFlashDspHasBeenCleared; // 0x15f1            
            bool m_bFlashScreenshotHasBeenGrabbed; // 0x15f2            
            uint8_t _pad15f3[0x1]; // 0x15f3
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnFlashMaxAlphaChanged"
            float m_flFlashMaxAlpha; // 0x15f4            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnFlashDurationChanged"
            float m_flFlashDuration; // 0x15f8            
            source2sdk::entity2::GameTime_t m_flClientHealthFadeChangeTimestamp; // 0x15fc            
            std::int32_t m_nClientHealthFadeParityValue; // 0x1600            
            float m_fNextThinkPushAway; // 0x1604            
            uint8_t _pad1608[0x4]; // 0x1608
            float m_flCurrentMusicStartTime; // 0x160c            
            float m_flMusicRoundStartTime; // 0x1610            
            bool m_bDeferStartMusicOnWarmup; // 0x1614            
            uint8_t _pad1615[0x3]; // 0x1615
            float m_flLastSmokeOverlayAlpha; // 0x1618            
            float m_flLastSmokeAge; // 0x161c            
            Vector m_vLastSmokeOverlayColor; // 0x1620            
            uint8_t _pad162c[0x1c]; // 0x162c
            // metadata: MNetworkEnable
            // m_hOriginalController has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::CCSPlayerController> m_hOriginalController;
            char m_hOriginalController[0x4]; // 0x1648            
            uint8_t _pad164c[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_CSPlayerPawnBase because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_CSPlayerPawnBase) == 0x1650);
    };
};
