#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_FuncBrush.hpp"
#include "source2sdk/client/ParticleIndex_t.hpp"

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
        // Size: 0xea0
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "string_t m_EffectName"
        // static metadata: MNetworkVarNames "bool m_bState"
        #pragma pack(push, 1)
        class C_FuncElectrifiedVolume : public source2sdk::client::C_FuncBrush
        {
        public:
            // metadata: MNotSaved
            source2sdk::client::ParticleIndex_t m_nAmbientEffect; // 0xe88            
            uint8_t _pad0e8c[0x4]; // 0xe8c
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            CUtlSymbolLarge m_EffectName; // 0xe90            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bState; // 0xe98            
            uint8_t _pad0e99[0x7];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_FuncElectrifiedVolume because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_FuncElectrifiedVolume) == 0xea0);
    };
};
