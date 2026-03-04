#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/particles/CParticleFunctionPreEmission.hpp"
#include "source2sdk/particles/ParticleParentSetMode_t.hpp"
#include "source2sdk/particles/ParticleTraceSet_t.hpp"

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
        // Size: 0x290
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_OP_DistanceBetweenCPsToCP : public source2sdk::particles::CParticleFunctionPreEmission
        {
        public:
            // metadata: MPropertyFriendlyName "starting control point"
            std::int32_t m_nStartCP; // 0x1d8            
            // metadata: MPropertyFriendlyName "ending control point"
            std::int32_t m_nEndCP; // 0x1dc            
            // metadata: MPropertyFriendlyName "output control point"
            std::int32_t m_nOutputCP; // 0x1e0            
            // metadata: MPropertyFriendlyName "output control point field"
            std::int32_t m_nOutputCPField; // 0x1e4            
            // metadata: MPropertyFriendlyName "only set distance once"
            bool m_bSetOnce; // 0x1e8            
            uint8_t _pad01e9[0x3]; // 0x1e9
            // metadata: MPropertyFriendlyName "distance minimum"
            float m_flInputMin; // 0x1ec            
            // metadata: MPropertyFriendlyName "distance maximum"
            float m_flInputMax; // 0x1f0            
            // metadata: MPropertyFriendlyName "output minimum"
            float m_flOutputMin; // 0x1f4            
            // metadata: MPropertyFriendlyName "output maximum"
            float m_flOutputMax; // 0x1f8            
            // metadata: MPropertyFriendlyName "maximum trace length"
            float m_flMaxTraceLength; // 0x1fc            
            // metadata: MPropertyFriendlyName "LOS Failure Scale"
            float m_flLOSScale; // 0x200            
            // metadata: MPropertyFriendlyName "ensure line of sight"
            bool m_bLOS; // 0x204            
            // metadata: MPropertyFriendlyName "LOS collision group"
            char m_CollisionGroupName[128]; // 0x205            
            uint8_t _pad0285[0x3]; // 0x285
            // metadata: MPropertyFriendlyName "Trace Set"
            source2sdk::particles::ParticleTraceSet_t m_nTraceSet; // 0x288            
            // metadata: MPropertyFriendlyName "set parent"
            source2sdk::particles::ParticleParentSetMode_t m_nSetParent; // 0x28c            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_OP_DistanceBetweenCPsToCP because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::particles::C_OP_DistanceBetweenCPsToCP) == 0x290);
    };
};
