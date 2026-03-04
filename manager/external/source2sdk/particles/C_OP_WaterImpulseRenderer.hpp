#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionRenderer.hpp"
#include "source2sdk/particles/EventTypeSelection_t.hpp"
#include "source2sdk/particleslib/CPerParticleFloatInput.hpp"
#include "source2sdk/particleslib/CPerParticleVecInput.hpp"

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
        // Size: 0x1010
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_WaterImpulseRenderer : public source2sdk::particles::CParticleFunctionRenderer
        {
        public:
            // metadata: MPropertyFriendlyName "impulse position"
            source2sdk::particleslib::CPerParticleVecInput m_vecPos; // 0x220            
            // metadata: MPropertyFriendlyName "impulse radius"
            source2sdk::particleslib::CPerParticleFloatInput m_flRadius; // 0x8d8            
            // metadata: MPropertyFriendlyName "impulse magnitude"
            source2sdk::particleslib::CPerParticleFloatInput m_flMagnitude; // 0xa48            
            // metadata: MPropertyFriendlyName "impulse wave roundness"
            source2sdk::particleslib::CPerParticleFloatInput m_flShape; // 0xbb8            
            // metadata: MPropertyFriendlyName "impulse wind speed (when wind is true)"
            source2sdk::particleslib::CPerParticleFloatInput m_flWindSpeed; // 0xd28            
            // metadata: MPropertyFriendlyName "impulse wobble radius"
            source2sdk::particleslib::CPerParticleFloatInput m_flWobble; // 0xe98            
            // metadata: MPropertyFriendlyName "impulse is wind"
            bool m_bIsRadialWind; // 0x1008            
            uint8_t _pad1009[0x3]; // 0x1009
            // metadata: MPropertyFriendlyName "event type"
            source2sdk::particles::EventTypeSelection_t m_nEventType; // 0x100c            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_WaterImpulseRenderer because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_WaterImpulseRenderer) == 0x1010);
    };
};
