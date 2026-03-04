#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
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
        // Alignment: 0x4
        // Standard-layout class: false
        // Size: 0x80
        // Has VTable
        // 
        // static metadata: MNetworkVarNames "int m_iEnemy5Ks"
        // static metadata: MNetworkVarNames "int m_iEnemy4Ks"
        // static metadata: MNetworkVarNames "int m_iEnemy3Ks"
        // static metadata: MNetworkVarNames "int m_iEnemyKnifeKills"
        // static metadata: MNetworkVarNames "int m_iEnemyTaserKills"
        #pragma pack(push, 1)
        struct CSMatchStats_t : public source2sdk::client::CSPerRoundStats_t
        {
        public:
            // metadata: MNetworkEnable
            std::int32_t m_iEnemy5Ks; // 0x68            
            // metadata: MNetworkEnable
            std::int32_t m_iEnemy4Ks; // 0x6c            
            // metadata: MNetworkEnable
            std::int32_t m_iEnemy3Ks; // 0x70            
            // metadata: MNetworkEnable
            std::int32_t m_iEnemyKnifeKills; // 0x74            
            // metadata: MNetworkEnable
            std::int32_t m_iEnemyTaserKills; // 0x78            
            uint8_t _pad007c[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CSMatchStats_t because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::CSMatchStats_t) == 0x80);
    };
};
