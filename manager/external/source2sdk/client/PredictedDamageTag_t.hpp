#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/GameTick_t.hpp"

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
        // Standard-layout class: true
        // Size: 0x40
        // Has VTable
        // 
        // static metadata: MNetworkVarNames "GameTick_t nTagTick"
        // static metadata: MNetworkVarNames "float flFlinchModSmall"
        // static metadata: MNetworkVarNames "float flFlinchModLarge"
        // static metadata: MNetworkVarNames "float flFriendlyFireDamageReductionRatio"
        #pragma pack(push, 1)
        struct PredictedDamageTag_t
        {
        public:
            uint8_t _pad0000[0x30]; // 0x0
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTick_t nTagTick; // 0x30            
            // metadata: MNetworkEnable
            float flFlinchModSmall; // 0x34            
            // metadata: MNetworkEnable
            float flFlinchModLarge; // 0x38            
            // metadata: MNetworkEnable
            float flFriendlyFireDamageReductionRatio; // 0x3c            
        };
        #pragma pack(pop)
        
        static_assert(offsetof(source2sdk::client::PredictedDamageTag_t, nTagTick) == 0x30);
        static_assert(offsetof(source2sdk::client::PredictedDamageTag_t, flFlinchModSmall) == 0x34);
        static_assert(offsetof(source2sdk::client::PredictedDamageTag_t, flFlinchModLarge) == 0x38);
        static_assert(offsetof(source2sdk::client::PredictedDamageTag_t, flFriendlyFireDamageReductionRatio) == 0x3c);
        
        static_assert(sizeof(source2sdk::client::PredictedDamageTag_t) == 0x40);
    };
};
