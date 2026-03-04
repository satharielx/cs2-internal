#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionRenderer.hpp"
#include "source2sdk/particles/ParticleVRHandChoiceList_t.hpp"
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
        // Size: 0x3a0
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_RenderVRHapticEvent : public source2sdk::particles::CParticleFunctionRenderer
        {
        public:
            // metadata: MPropertyFriendlyName "haptic hand"
            source2sdk::particles::ParticleVRHandChoiceList_t m_nHand; // 0x220            
            // metadata: MPropertyFriendlyName "hand control point number"
            std::int32_t m_nOutputHandCP; // 0x224            
            // metadata: MPropertyFriendlyName "cp field"
            // metadata: MPropertyAttributeChoiceName "vector_component"
            std::int32_t m_nOutputField; // 0x228            
            uint8_t _pad022c[0x4]; // 0x22c
            // metadata: MPropertyFriendlyName "amplitude"
            source2sdk::particleslib::CPerParticleFloatInput m_flAmplitude; // 0x230            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_RenderVRHapticEvent because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_RenderVRHapticEvent) == 0x3a0);
    };
};
