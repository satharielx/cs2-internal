#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionRenderer.hpp"
#include "source2sdk/particles/ParticleAttrBoxFlags_t.hpp"
#include "source2sdk/particles/ParticleColorBlendType_t.hpp"
#include "source2sdk/particleslib/CParticleCollectionFloatInput.hpp"

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
        // Size: 0x530
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_ClientPhysics : public source2sdk::particles::CParticleFunctionRenderer
        {
        public:
            // metadata: MPropertyFriendlyName "client physics type"
            // metadata: MPropertyAttributeEditor "VDataChoice( scripts/misc.vdata!generic_physics_particle_spawner )"
            CUtlString m_strPhysicsType; // 0x220            
            // metadata: MPropertyFriendlyName "start all physics asleep"
            bool m_bStartAsleep; // 0x228            
            uint8_t _pad0229[0x7]; // 0x229
            // metadata: MPropertyFriendlyName "Player Wake Radius"
            source2sdk::particleslib::CParticleCollectionFloatInput m_flPlayerWakeRadius; // 0x230            
            // metadata: MPropertyFriendlyName "Vehicle Wake Radius"
            source2sdk::particleslib::CParticleCollectionFloatInput m_flVehicleWakeRadius; // 0x3a0            
            // metadata: MPropertyFriendlyName "use high quality simulation"
            bool m_bUseHighQualitySimulation; // 0x510            
            uint8_t _pad0511[0x3]; // 0x511
            // metadata: MPropertyFriendlyName "max particle count"
            std::int32_t m_nMaxParticleCount; // 0x514            
            // metadata: MPropertyFriendlyName "prevent spawning in exclusion volumes"
            // metadata: MPropertySuppressExpr "m_bKillParticles == true"
            bool m_bRespectExclusionVolumes; // 0x518            
            // metadata: MPropertyFriendlyName "kill physics particles"
            bool m_bKillParticles; // 0x519            
            // metadata: MPropertyFriendlyName "delete physics sim when stopped"
            // metadata: MPropertySuppressExpr "m_bKillParticles == false"
            bool m_bDeleteSim; // 0x51a            
            uint8_t _pad051b[0x1]; // 0x51b
            // metadata: MPropertyFriendlyName "control point (for finding nearest sim)"
            // metadata: MPropertySuppressExpr "m_bKillParticles == true"
            std::int32_t m_nControlPoint; // 0x51c            
            // metadata: MPropertyFriendlyName "specific sim id"
            // metadata: MPropertySuppressExpr "m_bKillParticles == true"
            std::int32_t m_nForcedSimId; // 0x520            
            // metadata: MPropertyFriendlyName "tint blend (color vs prop group gradient)"
            source2sdk::particles::ParticleColorBlendType_t m_nColorBlendType; // 0x524            
            // metadata: MPropertyFriendlyName "forced status effect flags"
            source2sdk::particles::ParticleAttrBoxFlags_t m_nForcedStatusEffects; // 0x528            
            uint8_t _pad052c[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_ClientPhysics because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_ClientPhysics) == 0x530);
    };
};
