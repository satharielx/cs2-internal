#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CPlayerControllerComponent.hpp"
#include "source2sdk/client/CSMatchStats_t.hpp"
#include "source2sdk/client/CSPerRoundStats_t.hpp"

// /////////////////////////////////////////////////////////////
// Module: client
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace client
    {
        // Registered alignment: unknown
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x138
        // Has VTable
        // 
        // static metadata: MNetworkVarNames "CSPerRoundStats_t m_perRoundStats"
        // static metadata: MNetworkVarNames "CSMatchStats_t m_matchStats"
        // static metadata: MNetworkVarNames "int m_iNumRoundKills"
        // static metadata: MNetworkVarNames "int m_iNumRoundKillsHeadshots"
        // static metadata: MNetworkVarNames "float m_flTotalRoundDamageDealt"
        #pragma pack(push, 1)
        class CCSPlayerController_ActionTrackingServices : public source2sdk::client::CPlayerControllerComponent
        {
        public:
            // metadata: MNetworkEnable
            // m_perRoundStats has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // C_UtlVectorEmbeddedNetworkVar<source2sdk::client::CSPerRoundStats_t> m_perRoundStats;
            char m_perRoundStats[0x68]; // 0x40            
            // metadata: MNetworkEnable
            source2sdk::client::CSMatchStats_t m_matchStats; // 0xa8            
            // metadata: MNetworkEnable
            std::int32_t m_iNumRoundKills; // 0x128            
            // metadata: MNetworkEnable
            std::int32_t m_iNumRoundKillsHeadshots; // 0x12c            
            // metadata: MNetworkEnable
            float m_flTotalRoundDamageDealt; // 0x130            
            uint8_t _pad0134[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSPlayerController_ActionTrackingServices because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::CCSPlayerController_ActionTrackingServices) == 0x138);
    };
};
