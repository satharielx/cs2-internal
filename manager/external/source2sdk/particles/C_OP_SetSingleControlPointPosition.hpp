#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionPreEmission.hpp"
#include "source2sdk/particleslib/CParticleCollectionVecInput.hpp"
#include "source2sdk/particleslib/CParticleTransformInput.hpp"

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
        // Size: 0x900
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_SetSingleControlPointPosition : public source2sdk::particles::CParticleFunctionPreEmission
        {
        public:
            // metadata: MPropertyFriendlyName "only set position once"
            bool m_bSetOnce; // 0x1d8            
            uint8_t _pad01d9[0x3]; // 0x1d9
            // metadata: MPropertyFriendlyName "control point number"
            std::int32_t m_nCP1; // 0x1dc            
            // metadata: MPropertyFriendlyName "control point location"
            source2sdk::particleslib::CParticleCollectionVecInput m_vecCP1Pos; // 0x1e0            
            // metadata: MPropertyFriendlyName "transform to offset positions from"
            // metadata: MParticleInputOptional
            source2sdk::particleslib::CParticleTransformInput m_transformInput; // 0x898            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_SetSingleControlPointPosition because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_SetSingleControlPointPosition) == 0x900);
    };
};
