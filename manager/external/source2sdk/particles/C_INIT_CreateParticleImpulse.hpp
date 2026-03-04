#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionInitializer.hpp"
#include "source2sdk/particles/ParticleFalloffFunction_t.hpp"
#include "source2sdk/particles/ParticleImpulseType_t.hpp"
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
        // Size: 0x638
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_INIT_CreateParticleImpulse : public source2sdk::particles::CParticleFunctionInitializer
        {
        public:
            // metadata: MPropertyFriendlyName "radius"
            source2sdk::particleslib::CPerParticleFloatInput m_InputRadius; // 0x1d8            
            // metadata: MPropertyFriendlyName "magnitude"
            source2sdk::particleslib::CPerParticleFloatInput m_InputMagnitude; // 0x348            
            // metadata: MPropertyFriendlyName "force falloff function"
            source2sdk::particles::ParticleFalloffFunction_t m_nFalloffFunction; // 0x4b8            
            uint8_t _pad04bc[0x4]; // 0x4bc
            // metadata: MPropertyFriendlyName "exponential falloff exponent"
            source2sdk::particleslib::CPerParticleFloatInput m_InputFalloffExp; // 0x4c0            
            // metadata: MPropertyFriendlyName "impulse type"
            source2sdk::particles::ParticleImpulseType_t m_nImpulseType; // 0x630            
            uint8_t _pad0634[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_INIT_CreateParticleImpulse because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_INIT_CreateParticleImpulse) == 0x638);
    };
};
