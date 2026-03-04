#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/server/CBaseAnimGraph.hpp"

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
        // Size: 0xac0
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CItem : public source2sdk::server::CBaseAnimGraph
        {
        public:
            uint8_t _pad0a20[0x8]; // 0xa20
            source2sdk::entity2::CEntityIOOutput m_OnPlayerTouch; // 0xa28            
            source2sdk::entity2::CEntityIOOutput m_OnPlayerPickup; // 0xa40            
            bool m_bActivateWhenAtRest; // 0xa58            
            uint8_t _pad0a59[0x7]; // 0xa59
            source2sdk::entity2::CEntityIOOutput m_OnCacheInteraction; // 0xa60            
            source2sdk::entity2::CEntityIOOutput m_OnGlovePulled; // 0xa78            
            VectorWS m_vOriginalSpawnOrigin; // 0xa90            
            QAngle m_vOriginalSpawnAngles; // 0xa9c            
            // metadata: MNotSaved
            bool m_bPhysStartAsleep; // 0xaa8            
            uint8_t _pad0aa9[0x17];
            
            // Datamap fields:
            // void CItemItemTouch; // 0x0
            // void CItemMaterialize; // 0x0
            // void CItemComeToRest; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CItem because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CItem) == 0xac0);
    };
};
