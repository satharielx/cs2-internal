#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionConstraint.hpp"
#include "source2sdk/particles/ParticleAttributeIndex_t.hpp"
#include "source2sdk/particles/ParticleCollisionMode_t.hpp"
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
        // Size: 0x9d0
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_WorldTraceConstraint : public source2sdk::particles::CParticleFunctionConstraint
        {
        public:
            // metadata: MPropertyFriendlyName "control point for fast collision tests"
            std::int32_t m_nCP; // 0x1d0            
            // metadata: MPropertyFriendlyName "control point offset for fast collisions"
            // metadata: MVectorIsCoordinate
            Vector m_vecCpOffset; // 0x1d4            
            // metadata: MPropertyFriendlyName "collision mode"
            source2sdk::particles::ParticleCollisionMode_t m_nCollisionMode; // 0x1e0            
            // metadata: MPropertyFriendlyName "minimum detail collision mode"
            source2sdk::particles::ParticleCollisionMode_t m_nCollisionModeMin; // 0x1e4            
            // metadata: MPropertyStartGroup "Collision Options"
            // metadata: MPropertyFriendlyName "Trace Set"
            source2sdk::particles::ParticleTraceSet_t m_nTraceSet; // 0x1e8            
            // metadata: MPropertyFriendlyName "collision group"
            char m_CollisionGroupName[128]; // 0x1ec            
            // metadata: MPropertyFriendlyName "World Only"
            bool m_bWorldOnly; // 0x26c            
            // metadata: MPropertyFriendlyName "brush only"
            bool m_bBrushOnly; // 0x26d            
            // metadata: MPropertyFriendlyName "include water"
            // metadata: MPropertySuppressExpr "m_nTraceSet == PARTICLE_TRACE_SET_STATIC"
            bool m_bIncludeWater; // 0x26e            
            uint8_t _pad026f[0x1]; // 0x26f
            // metadata: MPropertyFriendlyName "CP Entity to Ignore for Collisions"
            // metadata: MPropertySuppressExpr "m_nTraceSet == PARTICLE_TRACE_SET_STATIC"
            std::int32_t m_nIgnoreCP; // 0x270            
            // metadata: MPropertyFriendlyName "control point movement distance tolerance"
            // metadata: MPropertySuppressExpr "m_nCollisionMode == COLLISION_MODE_PER_PARTICLE_TRACE"
            float m_flCpMovementTolerance; // 0x274            
            // metadata: MPropertyFriendlyName "plane cache retest rate"
            // metadata: MPropertySuppressExpr "m_nCollisionMode != COLLISION_MODE_PER_FRAME_PLANESET"
            float m_flRetestRate; // 0x278            
            // metadata: MPropertyFriendlyName "trace accuracy tolerance"
            // metadata: MPropertySuppressExpr "m_nCollisionMode != COLLISION_MODE_USE_NEAREST_TRACE"
            float m_flTraceTolerance; // 0x27c            
            // metadata: MPropertyFriendlyName "Confirm Collision Speed Threshold"
            // metadata: MPropertySuppressExpr "m_nCollisionMode == COLLISION_MODE_PER_PARTICLE_TRACE"
            float m_flCollisionConfirmationSpeed; // 0x280            
            // metadata: MPropertyFriendlyName "Max Confirmation Traces Per Fame"
            // metadata: MPropertySuppressExpr "m_nCollisionMode == COLLISION_MODE_PER_PARTICLE_TRACE"
            float m_nMaxTracesPerFrame; // 0x284            
            // metadata: MPropertyStartGroup "Impact Options"
            // metadata: MPropertyFriendlyName "radius scale"
            source2sdk::particleslib::CPerParticleFloatInput m_flRadiusScale; // 0x288            
            // metadata: MPropertyFriendlyName "amount of bounce"
            source2sdk::particleslib::CPerParticleFloatInput m_flBounceAmount; // 0x3f8            
            // metadata: MPropertyFriendlyName "amount of slide"
            source2sdk::particleslib::CPerParticleFloatInput m_flSlideAmount; // 0x568            
            // metadata: MPropertyFriendlyName "Random Direction scale"
            source2sdk::particleslib::CPerParticleFloatInput m_flRandomDirScale; // 0x6d8            
            // metadata: MPropertyFriendlyName "Add Decay to Bounce"
            bool m_bDecayBounce; // 0x848            
            // metadata: MPropertyFriendlyName "kill particle on collision"
            bool m_bKillonContact; // 0x849            
            uint8_t _pad084a[0x2]; // 0x84a
            // metadata: MPropertyFriendlyName "minimum speed to kill on collision"
            float m_flMinSpeed; // 0x84c            
            // metadata: MPropertyFriendlyName "Set Normal"
            bool m_bSetNormal; // 0x850            
            uint8_t _pad0851[0x3]; // 0x851
            // metadata: MPropertyFriendlyName "Stick On Collision Cache Field"
            // metadata: MPropertyAttributeChoiceName "particlefield_vector"
            source2sdk::particles::ParticleAttributeIndex_t m_nStickOnCollisionField; // 0x854            
            // metadata: MPropertyFriendlyName "Speed to stop when sticking"
            source2sdk::particleslib::CPerParticleFloatInput m_flStopSpeed; // 0x858            
            // metadata: MPropertyFriendlyName "Entity Hitbox Cache Field (Requires Stick on Collision)"
            // metadata: MPropertyAttributeChoiceName "particlefield_vector"
            source2sdk::particles::ParticleAttributeIndex_t m_nEntityStickDataField; // 0x9c8            
            // metadata: MPropertyFriendlyName "Entity Normal Cache Field (Requires Stick on Collision)"
            // metadata: MPropertyAttributeChoiceName "particlefield_vector"
            source2sdk::particles::ParticleAttributeIndex_t m_nEntityStickNormalField; // 0x9cc            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_WorldTraceConstraint because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_WorldTraceConstraint) == 0x9d0);
    };
};
