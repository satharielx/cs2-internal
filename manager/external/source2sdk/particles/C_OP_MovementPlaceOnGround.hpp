#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionOperator.hpp"
#include "source2sdk/particles/ParticleTraceMissBehavior_t.hpp"
#include "source2sdk/particles/ParticleTraceSet_t.hpp"
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
        // Size: 0x400
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_MovementPlaceOnGround : public source2sdk::particles::CParticleFunctionOperator
        {
        public:
            // metadata: MPropertyFriendlyName "offset"
            source2sdk::particleslib::CPerParticleFloatInput m_flOffset; // 0x1d0            
            // metadata: MPropertyFriendlyName "max trace length"
            float m_flMaxTraceLength; // 0x340            
            // metadata: MPropertyFriendlyName "CP movement tolerance"
            float m_flTolerance; // 0x344            
            // metadata: MPropertyFriendlyName "trace offset"
            float m_flTraceOffset; // 0x348            
            // metadata: MPropertyFriendlyName "interpolation rate"
            float m_flLerpRate; // 0x34c            
            // metadata: MPropertyFriendlyName "collision group"
            char m_CollisionGroupName[128]; // 0x350            
            // metadata: MPropertyFriendlyName "Trace Set"
            source2sdk::particles::ParticleTraceSet_t m_nTraceSet; // 0x3d0            
            // metadata: MPropertyFriendlyName "reference CP 1"
            std::int32_t m_nRefCP1; // 0x3d4            
            // metadata: MPropertyFriendlyName "reference CP 2"
            std::int32_t m_nRefCP2; // 0x3d8            
            // metadata: MPropertyFriendlyName "interploation distance tolerance cp"
            std::int32_t m_nLerpCP; // 0x3dc            
            uint8_t _pad03e0[0x8]; // 0x3e0
            // metadata: MPropertyFriendlyName "No Collision Behavior"
            source2sdk::particles::ParticleTraceMissBehavior_t m_nTraceMissBehavior; // 0x3e8            
            // metadata: MPropertyFriendlyName "include default contents trace hulls"
            bool m_bIncludeShotHull; // 0x3ec            
            // metadata: MPropertyFriendlyName "include water"
            bool m_bIncludeWater; // 0x3ed            
            uint8_t _pad03ee[0x2]; // 0x3ee
            // metadata: MPropertyFriendlyName "set normal"
            bool m_bSetNormal; // 0x3f0            
            // metadata: MPropertyFriendlyName "treat offset as scalar of particle radius"
            bool m_bScaleOffset; // 0x3f1            
            uint8_t _pad03f2[0x2]; // 0x3f2
            // metadata: MPropertyFriendlyName "preserve initial Z-offset relative to cp"
            std::int32_t m_nPreserveOffsetCP; // 0x3f4            
            // metadata: MPropertyFriendlyName "CP Entity to Ignore for Collisions"
            std::int32_t m_nIgnoreCP; // 0x3f8            
            uint8_t _pad03fc[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_MovementPlaceOnGround because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_MovementPlaceOnGround) == 0x400);
    };
};
