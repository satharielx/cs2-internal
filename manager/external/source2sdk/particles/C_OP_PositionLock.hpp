#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionOperator.hpp"
#include "source2sdk/particles/ParticleAttributeIndex_t.hpp"
#include "source2sdk/particleslib/CParticleCollectionFloatInput.hpp"
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
        // Size: 0xa98
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_PositionLock : public source2sdk::particles::CParticleFunctionOperator
        {
        public:
            // metadata: MPropertyFriendlyName "transform input"
            source2sdk::particleslib::CParticleTransformInput m_TransformInput; // 0x1d0            
            // metadata: MPropertyFriendlyName "start fadeout min"
            float m_flStartTime_min; // 0x238            
            // metadata: MPropertyFriendlyName "start fadeout max"
            float m_flStartTime_max; // 0x23c            
            // metadata: MPropertyFriendlyName "start fadeout exponent"
            float m_flStartTime_exp; // 0x240            
            // metadata: MPropertyFriendlyName "end fadeout min"
            float m_flEndTime_min; // 0x244            
            // metadata: MPropertyFriendlyName "end fadeout max"
            float m_flEndTime_max; // 0x248            
            // metadata: MPropertyFriendlyName "end fadeout exponent"
            float m_flEndTime_exp; // 0x24c            
            // metadata: MPropertyFriendlyName "distance fade range"
            float m_flRange; // 0x250            
            uint8_t _pad0254[0x4]; // 0x254
            // metadata: MPropertyFriendlyName "distance fade bias"
            source2sdk::particleslib::CParticleCollectionFloatInput m_flRangeBias; // 0x258            
            // metadata: MPropertyFriendlyName "instant jump threshold"
            float m_flJumpThreshold; // 0x3c8            
            // metadata: MPropertyFriendlyName "previous position scale"
            float m_flPrevPosScale; // 0x3cc            
            // metadata: MPropertyFriendlyName "lock rotation"
            bool m_bLockRot; // 0x3d0            
            uint8_t _pad03d1[0x7]; // 0x3d1
            // metadata: MPropertyFriendlyName "component scale"
            source2sdk::particleslib::CParticleCollectionVecInput m_vecScale; // 0x3d8            
            // metadata: MPropertyFriendlyName "output field"
            // metadata: MPropertyAttributeChoiceName "particlefield_vector"
            source2sdk::particles::ParticleAttributeIndex_t m_nFieldOutput; // 0xa90            
            // metadata: MPropertyFriendlyName "output field prev"
            // metadata: MPropertyAttributeChoiceName "particlefield_vector"
            source2sdk::particles::ParticleAttributeIndex_t m_nFieldOutputPrev; // 0xa94            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_PositionLock because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_PositionLock) == 0xa98);
    };
};
