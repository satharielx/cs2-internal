#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CAttributeContainer.hpp"
#include "source2sdk/server/CBaseAnimGraph.hpp"
#include "source2sdk/server/EntitySpottedState_t.hpp"
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
        // Registered alignment: 0x10
        // Alignment: 0x10
        // Standard-layout class: false
        // Size: 0xdf0
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "bool m_bBombTicking"
        // static metadata: MNetworkVarNames "GameTime_t m_flC4Blow"
        // static metadata: MNetworkVarNames "int m_nBombSite"
        // static metadata: MNetworkVarNames "int m_nSourceSoundscapeHash"
        // static metadata: MNetworkVarNames "CAttributeContainer m_AttributeManager"
        // static metadata: MNetworkVarNames "bool m_bCannotBeDefused"
        // static metadata: MNetworkVarNames "EntitySpottedState_t m_entitySpottedState"
        // static metadata: MNetworkVarNames "bool m_bHasExploded"
        // static metadata: MNetworkVarNames "bool m_bBombDefused"
        // static metadata: MNetworkVarNames "float m_flTimerLength"
        // static metadata: MNetworkVarNames "bool m_bBeingDefused"
        // static metadata: MNetworkVarNames "float m_flDefuseLength"
        // static metadata: MNetworkVarNames "GameTime_t m_flDefuseCountDown"
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerPawn> m_hBombDefuser"
        #pragma pack(push, 1)
        class CPlantedC4 : public source2sdk::server::CBaseAnimGraph
        {
        public:
            uint8_t _pad0a20[0x8]; // 0xa20
            // metadata: MNetworkEnable
            bool m_bBombTicking; // 0xa28            
            uint8_t _pad0a29[0x3]; // 0xa29
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flC4Blow; // 0xa2c            
            // metadata: MNetworkEnable
            std::int32_t m_nBombSite; // 0xa30            
            // metadata: MNetworkEnable
            std::int32_t m_nSourceSoundscapeHash; // 0xa34            
            bool m_bAbortDetonationBecauseWorldIsFrozen; // 0xa38            
            uint8_t _pad0a39[0x7]; // 0xa39
            // metadata: MNetworkEnable
            source2sdk::server::CAttributeContainer m_AttributeManager; // 0xa40            
            source2sdk::entity2::CEntityIOOutput m_OnBombDefused; // 0xd38            
            source2sdk::entity2::CEntityIOOutput m_OnBombBeginDefuse; // 0xd50            
            source2sdk::entity2::CEntityIOOutput m_OnBombDefuseAborted; // 0xd68            
            // metadata: MNetworkEnable
            bool m_bCannotBeDefused; // 0xd80            
            uint8_t _pad0d81[0x7]; // 0xd81
            // metadata: MNetworkEnable
            source2sdk::server::EntitySpottedState_t m_entitySpottedState; // 0xd88            
            std::int32_t m_nSpotRules; // 0xda0            
            // metadata: MNetworkEnable
            bool m_bHasExploded; // 0xda4            
            // metadata: MNetworkEnable
            bool m_bBombDefused; // 0xda5            
            bool m_bTrainingPlacedByPlayer; // 0xda6            
            uint8_t _pad0da7[0x1]; // 0xda7
            // metadata: MNetworkEnable
            float m_flTimerLength; // 0xda8            
            // metadata: MNetworkEnable
            bool m_bBeingDefused; // 0xdac            
            uint8_t _pad0dad[0x7]; // 0xdad
            source2sdk::entity2::GameTime_t m_fLastDefuseTime; // 0xdb4            
            uint8_t _pad0db8[0x4]; // 0xdb8
            // metadata: MNetworkEnable
            float m_flDefuseLength; // 0xdbc            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flDefuseCountDown; // 0xdc0            
            // metadata: MNetworkEnable
            // m_hBombDefuser has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_hBombDefuser;
            char m_hBombDefuser[0x4]; // 0xdc4            
            std::int32_t m_iProgressBarTime; // 0xdc8            
            bool m_bVoiceAlertFired; // 0xdcc            
            bool m_bVoiceAlertPlayed[4]; // 0xdcd            
            uint8_t _pad0dd1[0x3]; // 0xdd1
            source2sdk::entity2::GameTime_t m_flNextBotBeepTime; // 0xdd4            
            uint8_t _pad0dd8[0x4]; // 0xdd8
            QAngle m_angCatchUpToPlayerEye; // 0xddc            
            source2sdk::entity2::GameTime_t m_flLastSpinDetectionTime; // 0xde8            
            uint8_t _pad0dec[0x4];
            
            // Datamap fields:
            // void CPlantedC4C4Think; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CPlantedC4 because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CPlantedC4) == 0xdf0);
    };
};
