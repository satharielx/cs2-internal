#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CSAdditionalPerRoundStats_t.hpp"

// /////////////////////////////////////////////////////////////
// Module: server
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace server
    {
        // Registered alignment: unknown
        // Alignment: 0x4
        // Standard-layout class: false
        // Size: 0x120
        #pragma pack(push, 1)
        struct CSAdditionalMatchStats_t : public source2sdk::server::CSAdditionalPerRoundStats_t
        {
        public:
            std::int32_t m_numRoundsSurvivedStreak; // 0xf0            
            std::int32_t m_maxNumRoundsSurvivedStreak; // 0xf4            
            std::int32_t m_numRoundsSurvivedTotal; // 0xf8            
            std::int32_t m_iRoundsWonWithoutPurchase; // 0xfc            
            std::int32_t m_iRoundsWonWithoutPurchaseTotal; // 0x100            
            std::int32_t m_numFirstKills; // 0x104            
            std::int32_t m_numClutchKills; // 0x108            
            std::int32_t m_numPistolKills; // 0x10c            
            std::int32_t m_numSniperKills; // 0x110            
            std::int32_t m_iNumSuicides; // 0x114            
            std::int32_t m_iNumTeamKills; // 0x118            
            float m_flTeamDamage; // 0x11c            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CSAdditionalMatchStats_t because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CSAdditionalMatchStats_t) == 0x120);
    };
};
