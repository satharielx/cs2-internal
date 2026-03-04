#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CBaseAnimGraph.hpp"
#include "source2sdk/client/C_AttributeContainer.hpp"
#include "source2sdk/client/EntitySpottedState_t.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
namespace source2sdk
{
    namespace client
    {
        struct CBasePlayerController;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct C_CSPlayerPawn;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct C_Multimeter;
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
        // Size: 0x16e0
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "bool m_bBombTicking"
        // static metadata: MNetworkVarNames "int m_nBombSite"
        // static metadata: MNetworkVarNames "int m_nSourceSoundscapeHash"
        // static metadata: MNetworkVarNames "EntitySpottedState_t m_entitySpottedState"
        // static metadata: MNetworkVarNames "GameTime_t m_flC4Blow"
        // static metadata: MNetworkVarNames "bool m_bCannotBeDefused"
        // static metadata: MNetworkVarNames "bool m_bHasExploded"
        // static metadata: MNetworkVarNames "float m_flTimerLength"
        // static metadata: MNetworkVarNames "bool m_bBeingDefused"
        // static metadata: MNetworkVarNames "float m_flDefuseLength"
        // static metadata: MNetworkVarNames "GameTime_t m_flDefuseCountDown"
        // static metadata: MNetworkVarNames "bool m_bBombDefused"
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerPawn> m_hBombDefuser"
        // static metadata: MNetworkVarNames "CAttributeContainer m_AttributeManager"
        #pragma pack(push, 1)
        class C_PlantedC4 : public source2sdk::client::CBaseAnimGraph
        {
        public:
            uint8_t _pad1168[0x8]; // 0x1168
            // metadata: MNetworkEnable
            bool m_bBombTicking; // 0x1170            
            uint8_t _pad1171[0x3]; // 0x1171
            // metadata: MNetworkEnable
            std::int32_t m_nBombSite; // 0x1174            
            // metadata: MNetworkEnable
            std::int32_t m_nSourceSoundscapeHash; // 0x1178            
            uint8_t _pad117c[0x4]; // 0x117c
            // metadata: MNetworkEnable
            source2sdk::client::EntitySpottedState_t m_entitySpottedState; // 0x1180            
            source2sdk::entity2::GameTime_t m_flNextGlow; // 0x1198            
            source2sdk::entity2::GameTime_t m_flNextBeep; // 0x119c            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flC4Blow; // 0x11a0            
            // metadata: MNetworkEnable
            bool m_bCannotBeDefused; // 0x11a4            
            // metadata: MNetworkEnable
            bool m_bHasExploded; // 0x11a5            
            uint8_t _pad11a6[0x2]; // 0x11a6
            // metadata: MNetworkEnable
            float m_flTimerLength; // 0x11a8            
            // metadata: MNetworkEnable
            bool m_bBeingDefused; // 0x11ac            
            uint8_t _pad11ad[0x3]; // 0x11ad
            float m_bTriggerWarning; // 0x11b0            
            float m_bExplodeWarning; // 0x11b4            
            bool m_bC4Activated; // 0x11b8            
            bool m_bTenSecWarning; // 0x11b9            
            uint8_t _pad11ba[0x2]; // 0x11ba
            // metadata: MNetworkEnable
            float m_flDefuseLength; // 0x11bc            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flDefuseCountDown; // 0x11c0            
            // metadata: MNetworkEnable
            bool m_bBombDefused; // 0x11c4            
            uint8_t _pad11c5[0x3]; // 0x11c5
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnDefuserChanged"
            // m_hBombDefuser has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_CSPlayerPawn> m_hBombDefuser;
            char m_hBombDefuser[0x4]; // 0x11c8            
            uint8_t _pad11cc[0x4]; // 0x11cc
            // metadata: MNetworkEnable
            source2sdk::client::C_AttributeContainer m_AttributeManager; // 0x11d0            
            // m_hDefuserMultimeter has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_Multimeter> m_hDefuserMultimeter;
            char m_hDefuserMultimeter[0x4]; // 0x16a0            
            source2sdk::entity2::GameTime_t m_flNextRadarFlashTime; // 0x16a4            
            bool m_bRadarFlash; // 0x16a8            
            uint8_t _pad16a9[0x3]; // 0x16a9
            // m_pBombDefuser has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_CSPlayerPawn> m_pBombDefuser;
            char m_pBombDefuser[0x4]; // 0x16ac            
            source2sdk::entity2::GameTime_t m_fLastDefuseTime; // 0x16b0            
            uint8_t _pad16b4[0x4]; // 0x16b4
            source2sdk::client::CBasePlayerController* m_pPredictionOwner; // 0x16b8            
            Vector m_vecC4ExplodeSpectatePos; // 0x16c0            
            QAngle m_vecC4ExplodeSpectateAng; // 0x16cc            
            float m_flC4ExplodeSpectateDuration; // 0x16d8            
            uint8_t _pad16dc[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_PlantedC4 because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_PlantedC4) == 0x16e0);
    };
};
