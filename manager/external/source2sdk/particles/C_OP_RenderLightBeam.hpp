#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionRenderer.hpp"
#include "source2sdk/particles/ParticleColorBlendType_t.hpp"
#include "source2sdk/particleslib/CParticleCollectionFloatInput.hpp"
#include "source2sdk/particleslib/CParticleCollectionVecInput.hpp"

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
        // Size: 0xea8
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_RenderLightBeam : public source2sdk::particles::CParticleFunctionRenderer
        {
        public:
            // metadata: MPropertyFriendlyName "Color Blend"
            source2sdk::particleslib::CParticleCollectionVecInput m_vColorBlend; // 0x220            
            // metadata: MPropertyFriendlyName "Color Blend Type"
            // metadata: MPropertySortPriority "700"
            source2sdk::particles::ParticleColorBlendType_t m_nColorBlendType; // 0x8d8            
            uint8_t _pad08dc[0x4]; // 0x8dc
            // metadata: MPropertyFriendlyName "Lumens Per Meter"
            source2sdk::particleslib::CParticleCollectionFloatInput m_flBrightnessLumensPerMeter; // 0x8e0            
            // metadata: MPropertyFriendlyName "Shadows"
            // metadata: MPropertySuppressExpr "mod == csgo"
            bool m_bCastShadows; // 0xa50            
            uint8_t _pad0a51[0x7]; // 0xa51
            // metadata: MPropertyFriendlyName "Skirt"
            source2sdk::particleslib::CParticleCollectionFloatInput m_flSkirt; // 0xa58            
            // metadata: MPropertyFriendlyName "Range"
            source2sdk::particleslib::CParticleCollectionFloatInput m_flRange; // 0xbc8            
            // metadata: MPropertyFriendlyName "Thickness"
            source2sdk::particleslib::CParticleCollectionFloatInput m_flThickness; // 0xd38            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_RenderLightBeam because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_RenderLightBeam) == 0xea8);
    };
};
