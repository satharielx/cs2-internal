#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionForce.hpp"

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
        // Size: 0x218
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_ForceBasedOnDistanceToPlane : public source2sdk::particles::CParticleFunctionForce
        {
        public:
            // metadata: MPropertyFriendlyName "min distance from plane"
            float m_flMinDist; // 0x1e0            
            // metadata: MPropertyFriendlyName "force at min distance"
            // metadata: MVectorIsCoordinate
            Vector m_vecForceAtMinDist; // 0x1e4            
            // metadata: MPropertyFriendlyName "max distance from plane"
            float m_flMaxDist; // 0x1f0            
            // metadata: MPropertyFriendlyName "force at max distance"
            // metadata: MVectorIsCoordinate
            Vector m_vecForceAtMaxDist; // 0x1f4            
            // metadata: MPropertyFriendlyName "plane normal"
            // metadata: MVectorIsCoordinate
            Vector m_vecPlaneNormal; // 0x200            
            // metadata: MPropertyFriendlyName "control point number"
            std::int32_t m_nControlPointNumber; // 0x20c            
            // metadata: MPropertyFriendlyName "exponent"
            float m_flExponent; // 0x210            
            uint8_t _pad0214[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_ForceBasedOnDistanceToPlane because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_ForceBasedOnDistanceToPlane) == 0x218);
    };
};
