#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseEntity.hpp"

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
        // Size: 0xf50
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
        class C_MapVetoPickController : public source2sdk::client::C_BaseEntity
        {
        public:
            uint8_t _pad0608[0x10]; // 0x608
            // metadata: MNetworkEnable
            std::int32_t m_nDraftType; // 0x618            
            // metadata: MNetworkEnable
            std::int32_t m_nTeamWinningCoinToss; // 0x61c            
            // metadata: MNetworkEnable
            std::int32_t m_nTeamWithFirstChoice[64]; // 0x620            
            // metadata: MNetworkEnable
            std::int32_t m_nVoteMapIdsList[7]; // 0x720            
            // metadata: MNetworkEnable
            std::int32_t m_nAccountIDs[64]; // 0x73c            
            // metadata: MNetworkEnable
            std::int32_t m_nMapId0[64]; // 0x83c            
            // metadata: MNetworkEnable
            std::int32_t m_nMapId1[64]; // 0x93c            
            // metadata: MNetworkEnable
            std::int32_t m_nMapId2[64]; // 0xa3c            
            // metadata: MNetworkEnable
            std::int32_t m_nMapId3[64]; // 0xb3c            
            // metadata: MNetworkEnable
            std::int32_t m_nMapId4[64]; // 0xc3c            
            // metadata: MNetworkEnable
            std::int32_t m_nMapId5[64]; // 0xd3c            
            // metadata: MNetworkEnable
            std::int32_t m_nStartingSide0[64]; // 0xe3c            
            // metadata: MNetworkEnable
            std::int32_t m_nCurrentPhase; // 0xf3c            
            // metadata: MNetworkEnable
            std::int32_t m_nPhaseStartTick; // 0xf40            
            // metadata: MNetworkEnable
            std::int32_t m_nPhaseDurationTicks; // 0xf44            
            std::int32_t m_nPostDataUpdateTick; // 0xf48            
            bool m_bDisabledHud; // 0xf4c            
            uint8_t _pad0f4d[0x3];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_MapVetoPickController because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_MapVetoPickController) == 0xf50);
    };
};
