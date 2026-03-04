#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionForce.hpp"
#include "source2sdk/particleslib/CPerParticleFloatInput.hpp"

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
        // Size: 0x358
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_CPVelocityForce : public source2sdk::particles::CParticleFunctionForce
        {
        public:
            // metadata: MPropertyFriendlyName "control point"
            std::int32_t m_nControlPointNumber; // 0x1e0            
            uint8_t _pad01e4[0x4]; // 0x1e4
            // metadata: MPropertyFriendlyName "velocity scale"
            source2sdk::particleslib::CPerParticleFloatInput m_flScale; // 0x1e8            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_CPVelocityForce because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_CPVelocityForce) == 0x358);
    };
};
