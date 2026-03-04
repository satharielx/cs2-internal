#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CPhysicsProp.hpp"
#include "source2sdk/server/shard_model_desc_t.hpp"

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
        // Size: 0xd70
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkExcludeByName "m_hModel"
        // static metadata: MNetworkVarNames "shard_model_desc_t m_ShardDesc"
        #pragma pack(push, 1)
        class CShatterGlassShardPhysics : public source2sdk::server::CPhysicsProp
        {
        public:
            bool m_bDebris; // 0xce0            
            uint8_t _pad0ce1[0x3]; // 0xce1
            std::uint32_t m_hParentShard; // 0xce4            
            // metadata: MNetworkEnable
            source2sdk::server::shard_model_desc_t m_ShardDesc; // 0xce8            
            uint8_t _pad0d68[0x8];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CShatterGlassShardPhysics because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CShatterGlassShardPhysics) == 0xd70);
    };
};
