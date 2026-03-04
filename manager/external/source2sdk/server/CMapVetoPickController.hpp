#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CBaseEntity.hpp"

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
        // Size: 0xea8
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "int m_nDraftType"
        // static metadata: MNetworkVarNames "int m_nTeamWinningCoinToss"
        // static metadata: MNetworkVarNames "int m_nTeamWithFirstChoice"
        // static metadata: MNetworkVarNames "int m_nVoteMapIdsList"
        // static metadata: MNetworkVarNames "int m_nAccountIDs"
        // static metadata: MNetworkVarNames "int m_nMapId0"
        // static metadata: MNetworkVarNames "int m_nMapId1"
        // static metadata: MNetworkVarNames "int m_nMapId2"
        // static metadata: MNetworkVarNames "int m_nMapId3"
        // static metadata: MNetworkVarNames "int m_nMapId4"
        // static metadata: MNetworkVarNames "int m_nMapId5"
        // static metadata: MNetworkVarNames "int m_nStartingSide0"
        // static metadata: MNetworkVarNames "int m_nCurrentPhase"
        // static metadata: MNetworkVarNames "int m_nPhaseStartTick"
        // static metadata: MNetworkVarNames "int m_nPhaseDurationTicks"
        #pragma pack(push, 1)
        class CMapVetoPickController : public source2sdk::server::CBaseEntity
        {
        public:
            bool m_bPlayedIntroVcd; // 0x4a8            
            bool m_bNeedToPlayFiveSecondsRemaining; // 0x4a9            
            uint8_t _pad04aa[0x1e]; // 0x4aa
            double m_dblPreMatchDraftSequenceTime; // 0x4c8            
            bool m_bPreMatchDraftStateChanged; // 0x4d0            
            uint8_t _pad04d1[0x3]; // 0x4d1
            // metadata: MNetworkEnable
            std::int32_t m_nDraftType; // 0x4d4            
            // metadata: MNetworkEnable
            std::int32_t m_nTeamWinningCoinToss; // 0x4d8            
            // metadata: MNetworkEnable
            std::int32_t m_nTeamWithFirstChoice[64]; // 0x4dc            
            // metadata: MNetworkEnable
            std::int32_t m_nVoteMapIdsList[7]; // 0x5dc            
            // metadata: MNetworkEnable
            std::int32_t m_nAccountIDs[64]; // 0x5f8            
            // metadata: MNetworkEnable
            std::int32_t m_nMapId0[64]; // 0x6f8            
            // metadata: MNetworkEnable
            std::int32_t m_nMapId1[64]; // 0x7f8            
            // metadata: MNetworkEnable
            std::int32_t m_nMapId2[64]; // 0x8f8            
            // metadata: MNetworkEnable
            std::int32_t m_nMapId3[64]; // 0x9f8            
            // metadata: MNetworkEnable
            std::int32_t m_nMapId4[64]; // 0xaf8            
            // metadata: MNetworkEnable
            std::int32_t m_nMapId5[64]; // 0xbf8            
            // metadata: MNetworkEnable
            std::int32_t m_nStartingSide0[64]; // 0xcf8            
            // metadata: MNetworkEnable
            std::int32_t m_nCurrentPhase; // 0xdf8            
            // metadata: MNetworkEnable
            std::int32_t m_nPhaseStartTick; // 0xdfc            
            // metadata: MNetworkEnable
            std::int32_t m_nPhaseDurationTicks; // 0xe00            
            uint8_t _pad0e04[0x4]; // 0xe04
            // m_OnMapVetoed has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<CUtlSymbolLarge,CUtlSymbolLarge> m_OnMapVetoed;
            char m_OnMapVetoed[0x20]; // 0xe08            
            // m_OnMapPicked has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<CUtlSymbolLarge,CUtlSymbolLarge> m_OnMapPicked;
            char m_OnMapPicked[0x20]; // 0xe28            
            // m_OnSidesPicked has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<std::int32_t,std::int32_t> m_OnSidesPicked;
            char m_OnSidesPicked[0x20]; // 0xe48            
            // m_OnNewPhaseStarted has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<std::int32_t,std::int32_t> m_OnNewPhaseStarted;
            char m_OnNewPhaseStarted[0x20]; // 0xe68            
            // m_OnLevelTransition has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<std::int32_t,std::int32_t> m_OnLevelTransition;
            char m_OnLevelTransition[0x20]; // 0xe88            
            
            // Datamap fields:
            // void CMapVetoPickControllerVoteControllerThink; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CMapVetoPickController because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CMapVetoPickController) == 0xea8);
    };
};
