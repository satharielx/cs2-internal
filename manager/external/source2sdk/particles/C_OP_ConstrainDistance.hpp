#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionConstraint.hpp"
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
        // Size: 0x4c8
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_ConstrainDistance : public source2sdk::particles::CParticleFunctionConstraint
        {
        public:
            // metadata: MPropertyFriendlyName "minimum distance"
            source2sdk::particleslib::CParticleCollectionFloatInput m_fMinDistance; // 0x1d0            
            // metadata: MPropertyFriendlyName "maximum distance"
            source2sdk::particleslib::CParticleCollectionFloatInput m_fMaxDistance; // 0x340            
            // metadata: MPropertyFriendlyName "control point number"
            std::int32_t m_nControlPointNumber; // 0x4b0            
            // metadata: MPropertyFriendlyName "offset of center"
            // metadata: MVectorIsCoordinate
            Vector m_CenterOffset; // 0x4b4            
            // metadata: MPropertyFriendlyName "global center point"
            bool m_bGlobalCenter; // 0x4c0            
            uint8_t _pad04c1[0x7];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_ConstrainDistance because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_ConstrainDistance) == 0x4c8);
    };
};
