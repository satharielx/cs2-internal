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
        // Size: 0x540
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // MNetworkNoBase
        // 
        // static metadata: MNetworkVarNames "bool m_bHostageAlive"
        // static metadata: MNetworkVarNames "bool m_isHostageFollowingSomeone"
        // static metadata: MNetworkVarNames "CEntityIndex m_iHostageEntityIDs"
        // static metadata: MNetworkVarNames "Vector m_bombsiteCenterA"
        // static metadata: MNetworkVarNames "Vector m_bombsiteCenterB"
        // static metadata: MNetworkVarNames "int m_hostageRescueX"
        // static metadata: MNetworkVarNames "int m_hostageRescueY"
        // static metadata: MNetworkVarNames "int m_hostageRescueZ"
        // static metadata: MNetworkVarNames "bool m_bEndMatchNextMapAllVoted"
        #pragma pack(push, 1)
        class CCSPlayerResource : public source2sdk::server::CBaseEntity
        {
        public:
            // metadata: MNetworkEnable
            bool m_bHostageAlive[12]; // 0x4a8            
            // metadata: MNetworkEnable
            bool m_isHostageFollowingSomeone[12]; // 0x4b4            
            // metadata: MNetworkEnable
            CEntityIndex m_iHostageEntityIDs[12]; // 0x4c0            
            // metadata: MNetworkEnable
            Vector m_bombsiteCenterA; // 0x4f0            
            // metadata: MNetworkEnable
            Vector m_bombsiteCenterB; // 0x4fc            
            // metadata: MNetworkEnable
            std::int32_t m_hostageRescueX[4]; // 0x508            
            // metadata: MNetworkEnable
            std::int32_t m_hostageRescueY[4]; // 0x518            
            // metadata: MNetworkEnable
            std::int32_t m_hostageRescueZ[4]; // 0x528            
            // metadata: MNetworkEnable
            bool m_bEndMatchNextMapAllVoted; // 0x538            
            bool m_foundGoalPositions; // 0x539            
            uint8_t _pad053a[0x6];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSPlayerResource because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CCSPlayerResource) == 0x540);
    };
};
