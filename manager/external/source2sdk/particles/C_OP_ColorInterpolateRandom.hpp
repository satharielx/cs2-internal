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
        // Size: 0x210
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_ColorInterpolateRandom : public source2sdk::particles::CParticleFunctionOperator
        {
        public:
            // metadata: MPropertyFriendlyName "color fade min"
            Color m_ColorFadeMin; // 0x1d0            
            uint8_t _pad01d4[0x18]; // 0x1d4
            // metadata: MPropertyFriendlyName "color fade max"
            Color m_ColorFadeMax; // 0x1ec            
            uint8_t _pad01f0[0xc]; // 0x1f0
            // metadata: MPropertyFriendlyName "fade start time"
            float m_flFadeStartTime; // 0x1fc            
            // metadata: MPropertyFriendlyName "fade end time"
            float m_flFadeEndTime; // 0x200            
            // metadata: MPropertyFriendlyName "output field"
            // metadata: MPropertyAttributeChoiceName "particlefield_vector"
            source2sdk::particles::ParticleAttributeIndex_t m_nFieldOutput; // 0x204            
            // metadata: MPropertyFriendlyName "ease in and out"
            bool m_bEaseInOut; // 0x208            
            uint8_t _pad0209[0x7];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_ColorInterpolateRandom because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_ColorInterpolateRandom) == 0x210);
    };
};
