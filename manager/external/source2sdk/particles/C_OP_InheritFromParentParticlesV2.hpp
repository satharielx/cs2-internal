#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionOperator.hpp"
#include "source2sdk/particles/MissingParentInheritBehavior_t.hpp"
#include "source2sdk/particles/ParticleAttributeIndex_t.hpp"
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
        // Size: 0x630
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MParticleMinVersion "9"
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_InheritFromParentParticlesV2 : public source2sdk::particles::CParticleFunctionOperator
        {
        public:
            // metadata: MPropertyFriendlyName "scale"
            source2sdk::particleslib::CPerParticleFloatInput m_flScale; // 0x1d0            
            // metadata: MPropertyFriendlyName "inherited field"
            // metadata: MPropertyAttributeChoiceName "particlefield"
            source2sdk::particles::ParticleAttributeIndex_t m_nFieldOutput; // 0x340            
            uint8_t _pad0344[0x4]; // 0x344
            // metadata: MPropertyFriendlyName "particle increment amount"
            source2sdk::particleslib::CPerParticleFloatInput m_nIncrement; // 0x348            
            // metadata: MPropertyFriendlyName "random parent particle distribution"
            bool m_bRandomDistribution; // 0x4b8            
            // metadata: MPropertyFriendlyName "start at last parent particle in reverse"
            bool m_bReverse; // 0x4b9            
            uint8_t _pad04ba[0x2]; // 0x4ba
            // metadata: MPropertyFriendlyName "behavior if parent particle dies"
            source2sdk::particles::MissingParentInheritBehavior_t m_nMissingParentBehavior; // 0x4bc            
            // metadata: MPropertyFriendlyName "Interpolation"
            source2sdk::particleslib::CPerParticleFloatInput m_flInterpolation; // 0x4c0            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_InheritFromParentParticlesV2 because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_InheritFromParentParticlesV2) == 0x630);
    };
};
