#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionRenderer.hpp"
#include "source2sdk/particles/ParticleAttributeIndex_t.hpp"
#include "source2sdk/particles/ParticleColorBlendType_t.hpp"
#include "source2sdk/particles/ParticleLightBehaviorChoiceList_t.hpp"
#include "source2sdk/particles/ParticleLightFogLightingMode_t.hpp"
#include "source2sdk/particles/ParticleLightTypeChoiceList_t.hpp"
#include "source2sdk/particles/StandardLightingAttenuationStyle_t.hpp"
#include "source2sdk/particleslib/CParticleCollectionFloatInput.hpp"
#include "source2sdk/particleslib/CParticleCollectionRendererFloatInput.hpp"
#include "source2sdk/particleslib/CParticleCollectionVecInput.hpp"
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
        // Size: 0x14c0
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_RenderStandardLight : public source2sdk::particles::CParticleFunctionRenderer
        {
        public:
            // metadata: MPropertyFriendlyName "light type"
            source2sdk::particles::ParticleLightTypeChoiceList_t m_nLightType; // 0x220            
            uint8_t _pad0224[0x4]; // 0x224
            // metadata: MPropertyFriendlyName "color blend"
            source2sdk::particleslib::CParticleCollectionVecInput m_vecColorScale; // 0x228            
            // metadata: MPropertyFriendlyName "color blend type"
            source2sdk::particles::ParticleColorBlendType_t m_nColorBlendType; // 0x8e0            
            uint8_t _pad08e4[0x4]; // 0x8e4
            // metadata: MPropertyFriendlyName "intensity"
            source2sdk::particleslib::CPerParticleFloatInput m_flIntensity; // 0x8e8            
            // metadata: MPropertyFriendlyName "cast shadows"
            // metadata: MPropertySuppressExpr "m_nLightType == PARTICLE_LIGHT_TYPE_FX"
            bool m_bCastShadows; // 0xa58            
            uint8_t _pad0a59[0x7]; // 0xa59
            // metadata: MPropertyFriendlyName "inner cone angle"
            // metadata: MPropertySuppressExpr "m_nLightType != PARTICLE_LIGHT_TYPE_SPOT"
            source2sdk::particleslib::CParticleCollectionFloatInput m_flTheta; // 0xa60            
            // metadata: MPropertyFriendlyName "outer cone angle"
            // metadata: MPropertySuppressExpr "m_nLightType != PARTICLE_LIGHT_TYPE_SPOT"
            source2sdk::particleslib::CParticleCollectionFloatInput m_flPhi; // 0xbd0            
            // metadata: MPropertyFriendlyName "light radius multiplier"
            source2sdk::particleslib::CParticleCollectionFloatInput m_flRadiusMultiplier; // 0xd40            
            // metadata: MPropertyFriendlyName "attenuation type"
            source2sdk::particles::StandardLightingAttenuationStyle_t m_nAttenuationStyle; // 0xeb0            
            uint8_t _pad0eb4[0x4]; // 0xeb4
            // metadata: MPropertyFriendlyName "falloff linearity"
            // metadata: MPropertySuppressExpr "m_nAttenuationStyle == LIGHT_STYLE_NEW || ( m_nAttenuationStyle == LIGHT_STYLE_OLD && m_nLightType == PARTICLE_LIGHT_TYPE_FX )"
            source2sdk::particleslib::CParticleCollectionFloatInput m_flFalloffLinearity; // 0xeb8            
            // metadata: MPropertyFriendlyName "falloff fifty percent"
            // metadata: MPropertySuppressExpr "m_nAttenuationStyle == LIGHT_STYLE_OLD"
            source2sdk::particleslib::CParticleCollectionFloatInput m_flFiftyPercentFalloff; // 0x1028            
            // metadata: MPropertyFriendlyName "falloff zero percent"
            // metadata: MPropertySuppressExpr "m_nAttenuationStyle == LIGHT_STYLE_OLD"
            source2sdk::particleslib::CParticleCollectionFloatInput m_flZeroPercentFalloff; // 0x1198            
            // metadata: MPropertyFriendlyName "render diffuse"
            // metadata: MPropertySuppressExpr "m_nLightType == PARTICLE_LIGHT_TYPE_FX"
            bool m_bRenderDiffuse; // 0x1308            
            // metadata: MPropertyFriendlyName "render specular"
            // metadata: MPropertySuppressExpr "m_nLightType == PARTICLE_LIGHT_TYPE_FX"
            bool m_bRenderSpecular; // 0x1309            
            uint8_t _pad130a[0x6]; // 0x130a
            // metadata: MPropertyFriendlyName "light cookie string"
            CUtlString m_lightCookie; // 0x1310            
            // metadata: MPropertyFriendlyName "light priority"
            std::int32_t m_nPriority; // 0x1318            
            // metadata: MPropertyFriendlyName "fog lighting mode"
            // metadata: MPropertySuppressExpr "m_nLightType == PARTICLE_LIGHT_TYPE_FX"
            source2sdk::particles::ParticleLightFogLightingMode_t m_nFogLightingMode; // 0x131c            
            // metadata: MPropertyFriendlyName "fog contribution"
            // metadata: MPropertySuppressExpr "m_nLightType == PARTICLE_LIGHT_TYPE_FX"
            source2sdk::particleslib::CParticleCollectionRendererFloatInput m_flFogContribution; // 0x1320            
            // metadata: MPropertyFriendlyName "capsule behavior"
            source2sdk::particles::ParticleLightBehaviorChoiceList_t m_nCapsuleLightBehavior; // 0x1490            
            // metadata: MPropertyStartGroup "Capsule Light Controls"
            // metadata: MPropertyFriendlyName "capsule length"
            // metadata: MPropertySuppressExpr "m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_ROPE || m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_TRAILS"
            float m_flCapsuleLength; // 0x1494            
            // metadata: MPropertyFriendlyName "reverse point order"
            // metadata: MPropertySuppressExpr "m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_FOLLOW_DIRECTION || m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_TRAILS"
            bool m_bReverseOrder; // 0x1498            
            // metadata: MPropertyFriendlyName "Closed loop"
            // metadata: MPropertySuppressExpr "m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_FOLLOW_DIRECTION || m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_TRAILS"
            bool m_bClosedLoop; // 0x1499            
            uint8_t _pad149a[0x2]; // 0x149a
            // metadata: MPropertyFriendlyName "Anchor point source"
            // metadata: MPropertyAttributeChoiceName "particlefield_vector"
            // metadata: MPropertySuppressExpr "m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_FOLLOW_DIRECTION || m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_ROPE"
            source2sdk::particles::ParticleAttributeIndex_t m_nPrevPntSource; // 0x149c            
            // metadata: MPropertyFriendlyName "max length"
            // metadata: MPropertySuppressExpr "m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_FOLLOW_DIRECTION || m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_ROPE"
            float m_flMaxLength; // 0x14a0            
            // metadata: MPropertyFriendlyName "min length"
            // metadata: MPropertySuppressExpr "m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_FOLLOW_DIRECTION || m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_ROPE"
            float m_flMinLength; // 0x14a4            
            // metadata: MPropertyFriendlyName "ignore delta time"
            // metadata: MPropertySuppressExpr "m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_FOLLOW_DIRECTION || m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_ROPE"
            bool m_bIgnoreDT; // 0x14a8            
            uint8_t _pad14a9[0x3]; // 0x14a9
            // metadata: MPropertyFriendlyName "constrain radius to no more than this times the length"
            // metadata: MPropertySuppressExpr "m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_FOLLOW_DIRECTION || m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_ROPE"
            float m_flConstrainRadiusToLengthRatio; // 0x14ac            
            // metadata: MPropertyFriendlyName "amount to scale trail length by"
            // metadata: MPropertySuppressExpr "m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_FOLLOW_DIRECTION || m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_ROPE"
            float m_flLengthScale; // 0x14b0            
            // metadata: MPropertyFriendlyName "how long before a trail grows to its full length"
            // metadata: MPropertySuppressExpr "m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_FOLLOW_DIRECTION || m_nCapsuleLightBehavior == PARTICLE_LIGHT_BEHAVIOR_ROPE"
            float m_flLengthFadeInTime; // 0x14b4            
            uint8_t _pad14b8[0x8];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_RenderStandardLight because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_RenderStandardLight) == 0x14c0);
    };
};
