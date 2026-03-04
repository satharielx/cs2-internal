#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionForce.hpp"

// /////////////////////////////////////////////////////////////
// Module: particles
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace particles
    {
        // Registered alignment: 0x8
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x1f8
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_TwistAroundAxis : public source2sdk::particles::CParticleFunctionForce
        {
        public:
            // metadata: MPropertyFriendlyName "amount of force"
            float m_fForceAmount; // 0x1e0            
            // metadata: MPropertyFriendlyName "twist axis"
            // metadata: MVectorIsCoordinate
            Vector m_TwistAxis; // 0x1e4            
            // metadata: MPropertyFriendlyName "object local space axis 0/1"
            bool m_bLocalSpace; // 0x1f0            
            uint8_t _pad01f1[0x3]; // 0x1f1
            // metadata: MPropertyFriendlyName "control point"
            std::int32_t m_nControlPointNumber; // 0x1f4            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_TwistAroundAxis because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_TwistAroundAxis) == 0x1f8);
    };
};
