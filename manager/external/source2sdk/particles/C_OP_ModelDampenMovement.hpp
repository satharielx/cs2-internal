#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionOperator.hpp"
#include "source2sdk/particleslib/CPerParticleVecInput.hpp"

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
        // Size: 0x918
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_ModelDampenMovement : public source2sdk::particles::CParticleFunctionOperator
        {
        public:
            // metadata: MPropertyFriendlyName "control point number"
            std::int32_t m_nControlPointNumber; // 0x1d0            
            // metadata: MPropertyFriendlyName "use only bounding box"
            bool m_bBoundBox; // 0x1d4            
            // metadata: MPropertyFriendlyName "dampen outside instead of inside"
            bool m_bOutside; // 0x1d5            
            // metadata: MPropertyFriendlyName "use bones instead of hitboxes"
            bool m_bUseBones; // 0x1d6            
            // metadata: MPropertyFriendlyName "hitbox set"
            char m_HitboxSetName[128]; // 0x1d7            
            uint8_t _pad0257[0x1]; // 0x257
            // metadata: MPropertyFriendlyName "test position offset"
            // metadata: MVectorIsCoordinate
            source2sdk::particleslib::CPerParticleVecInput m_vecPosOffset; // 0x258            
            // metadata: MPropertyFriendlyName "drag"
            // metadata: MPropertyAttributeRange "-1 1"
            float m_fDrag; // 0x910            
            uint8_t _pad0914[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_ModelDampenMovement because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_ModelDampenMovement) == 0x918);
    };
};
