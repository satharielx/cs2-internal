#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_Team.hpp"

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
        // Size: 0x970
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "char m_szTeamMatchStat"
        // static metadata: MNetworkVarNames "int m_numMapVictories"
        // static metadata: MNetworkVarNames "bool m_bSurrendered"
        // static metadata: MNetworkVarNames "int32 m_scoreFirstHalf"
        // static metadata: MNetworkVarNames "int32 m_scoreSecondHalf"
        // static metadata: MNetworkVarNames "int32 m_scoreOvertime"
        // static metadata: MNetworkVarNames "char m_szClanTeamname"
        // static metadata: MNetworkVarNames "uint32 m_iClanID"
        // static metadata: MNetworkVarNames "char m_szTeamFlagImage"
        // static metadata: MNetworkVarNames "char m_szTeamLogoImage"
        #pragma pack(push, 1)
        class C_CSTeam : public source2sdk::client::C_Team
        {
        public:
            // metadata: MNetworkEnable
            char m_szTeamMatchStat[512]; // 0x6c0            
            // metadata: MNetworkEnable
            std::int32_t m_numMapVictories; // 0x8c0            
            // metadata: MNetworkEnable
            bool m_bSurrendered; // 0x8c4            
            uint8_t _pad08c5[0x3]; // 0x8c5
            // metadata: MNetworkEnable
            std::int32_t m_scoreFirstHalf; // 0x8c8            
            // metadata: MNetworkEnable
            std::int32_t m_scoreSecondHalf; // 0x8cc            
            // metadata: MNetworkEnable
            std::int32_t m_scoreOvertime; // 0x8d0            
            // metadata: MNetworkEnable
            char m_szClanTeamname[129]; // 0x8d4            
            uint8_t _pad0955[0x3]; // 0x955
            // metadata: MNetworkEnable
            std::uint32_t m_iClanID; // 0x958            
            // metadata: MNetworkEnable
            char m_szTeamFlagImage[8]; // 0x95c            
            // metadata: MNetworkEnable
            char m_szTeamLogoImage[8]; // 0x964            
            uint8_t _pad096c[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_CSTeam because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_CSTeam) == 0x970);
    };
};
