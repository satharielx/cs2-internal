#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CTeam.hpp"

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
        // Size: 0x820
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "bool m_bSurrendered"
        // static metadata: MNetworkVarNames "char m_szTeamMatchStat"
        // static metadata: MNetworkVarNames "int m_numMapVictories"
        // static metadata: MNetworkVarNames "int32 m_scoreFirstHalf"
        // static metadata: MNetworkVarNames "int32 m_scoreSecondHalf"
        // static metadata: MNetworkVarNames "int32 m_scoreOvertime"
        // static metadata: MNetworkVarNames "char m_szClanTeamname"
        // static metadata: MNetworkVarNames "uint32 m_iClanID"
        // static metadata: MNetworkVarNames "char m_szTeamFlagImage"
        // static metadata: MNetworkVarNames "char m_szTeamLogoImage"
        #pragma pack(push, 1)
        class CCSTeam : public source2sdk::server::CTeam
        {
        public:
            std::int32_t m_nLastRecievedShorthandedRoundBonus; // 0x560            
            std::int32_t m_nShorthandedRoundBonusStartRound; // 0x564            
            // metadata: MNetworkEnable
            bool m_bSurrendered; // 0x568            
            // metadata: MNetworkEnable
            char m_szTeamMatchStat[512]; // 0x569            
            uint8_t _pad0769[0x3]; // 0x769
            // metadata: MNetworkEnable
            std::int32_t m_numMapVictories; // 0x76c            
            // metadata: MNetworkEnable
            std::int32_t m_scoreFirstHalf; // 0x770            
            // metadata: MNetworkEnable
            std::int32_t m_scoreSecondHalf; // 0x774            
            // metadata: MNetworkEnable
            std::int32_t m_scoreOvertime; // 0x778            
            // metadata: MNetworkEnable
            char m_szClanTeamname[129]; // 0x77c            
            uint8_t _pad07fd[0x3]; // 0x7fd
            // metadata: MNetworkEnable
            std::uint32_t m_iClanID; // 0x800            
            // metadata: MNetworkEnable
            char m_szTeamFlagImage[8]; // 0x804            
            // metadata: MNetworkEnable
            char m_szTeamLogoImage[8]; // 0x80c            
            float m_flNextResourceTime; // 0x814            
            std::int32_t m_iLastUpdateSentAt; // 0x818            
            uint8_t _pad081c[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSTeam because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CCSTeam) == 0x820);
    };
};
