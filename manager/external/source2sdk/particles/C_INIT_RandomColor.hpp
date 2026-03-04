#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionInitializer.hpp"
#include "source2sdk/particles/ParticleAttributeIndex_t.hpp"
#include "source2sdk/particles/ParticleColorBlendMode_t.hpp"

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
        // Size: 0x220
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_INIT_RandomColor : public source2sdk::particles::CParticleFunctionInitializer
        {
        public:
            uint8_t _pad01d8[0x1c]; // 0x1d8
            // metadata: MPropertyFriendlyName "color1"
            Color m_ColorMin; // 0x1f4            
            // metadata: MPropertyFriendlyName "color2"
            Color m_ColorMax; // 0x1f8            
            // metadata: MPropertyFriendlyName "tint clamp min"
            Color m_TintMin; // 0x1fc            
            // metadata: MPropertyFriendlyName "tint clamp max"
            Color m_TintMax; // 0x200            
            // metadata: MPropertyFriendlyName "tint perc"
            float m_flTintPerc; // 0x204            
            // metadata: MPropertyFriendlyName "tint update movement threshold"
            float m_flUpdateThreshold; // 0x208            
            // metadata: MPropertyFriendlyName "tint control point"
            std::int32_t m_nTintCP; // 0x20c            
            // metadata: MPropertyFriendlyName "output field"
            // metadata: MPropertyAttributeChoiceName "particlefield_vector"
            source2sdk::particles::ParticleAttributeIndex_t m_nFieldOutput; // 0x210            
            // metadata: MPropertyFriendlyName "tint blend mode"
            source2sdk::particles::ParticleColorBlendMode_t m_nTintBlendMode; // 0x214            
            // metadata: MPropertyFriendlyName "light amplification amount"
            float m_flLightAmplification; // 0x218            
            uint8_t _pad021c[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_INIT_RandomColor because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_INIT_RandomColor) == 0x220);
    };
};
