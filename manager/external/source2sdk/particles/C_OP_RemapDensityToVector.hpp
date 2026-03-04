#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionOperator.hpp"
#include "source2sdk/particles/ParticleAttributeIndex_t.hpp"

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
        // Size: 0x200
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_RemapDensityToVector : public source2sdk::particles::CParticleFunctionOperator
        {
        public:
            // metadata: MPropertyFriendlyName "Radius scale for particle influence"
            float m_flRadiusScale; // 0x1d0            
            // metadata: MPropertyFriendlyName "Output field"
            // metadata: MPropertyAttributeChoiceName "particlefield_vector"
            source2sdk::particles::ParticleAttributeIndex_t m_nFieldOutput; // 0x1d4            
            // metadata: MPropertyFriendlyName "Density value to map to min value"
            float m_flDensityMin; // 0x1d8            
            // metadata: MPropertyFriendlyName "Density value to map to max value"
            float m_flDensityMax; // 0x1dc            
            // metadata: MPropertyFriendlyName "Output minimum"
            Vector m_vecOutputMin; // 0x1e0            
            // metadata: MPropertyFriendlyName "Output maximum"
            Vector m_vecOutputMax; // 0x1ec            
            // metadata: MPropertyFriendlyName "Use parent density instead of ours"
            bool m_bUseParentDensity; // 0x1f8            
            uint8_t _pad01f9[0x3]; // 0x1f9
            // metadata: MPropertyFriendlyName "Resolution to use for creating a voxel grid"
            std::int32_t m_nVoxelGridResolution; // 0x1fc            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_RemapDensityToVector because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_RemapDensityToVector) == 0x200);
    };
};
