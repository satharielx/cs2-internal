#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/soundsystem_voicecontainers/CSoundContainerReference.hpp"
#include "source2sdk/soundsystem_voicecontainers/CVoiceContainerBase.hpp"

// /////////////////////////////////////////////////////////////
// Module: soundsystem_voicecontainers
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace soundsystem_voicecontainers
    {
        // Registered alignment: 0x8
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x1e8
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        // static metadata: MPropertyFriendlyName "Parameter Blender"
        // static metadata: MPropertyDescription "Blends two containers according to parameter curves."
        #pragma pack(push, 1)
        class CVoiceContainerParameterBlender : public source2sdk::soundsystem_voicecontainers::CVoiceContainerBase
        {
        public:
            // metadata: MPropertyFriendlyName "First Sound"
            source2sdk::soundsystem_voicecontainers::CSoundContainerReference m_firstSound; // 0xa8            
            // metadata: MPropertyFriendlyName "Second Sound"
            source2sdk::soundsystem_voicecontainers::CSoundContainerReference m_secondSound; // 0xc0            
            // metadata: MPropertyStartGroup "Occlusion"
            // metadata: MPropertyFriendlyName "Enable Occlusion Blend"
            bool m_bEnableOcclusionBlend; // 0xd8            
            uint8_t _pad00d9[0x7]; // 0xd9
            // metadata: MPropertySuppressExpr "m_bEnableOcclusionBlend == false"
            // metadata: MPropertyFriendlyName "First Curve"
            CPiecewiseCurve m_curve1; // 0xe0            
            // metadata: MPropertySuppressExpr "m_bEnableOcclusionBlend == false"
            // metadata: MPropertyFriendlyName "Second Curve"
            CPiecewiseCurve m_curve2; // 0x120            
            // metadata: MPropertyStartGroup "Distance"
            // metadata: MPropertyFriendlyName "Enable Distance Blend"
            bool m_bEnableDistanceBlend; // 0x160            
            uint8_t _pad0161[0x7]; // 0x161
            // metadata: MPropertySuppressExpr "m_bEnableDistanceBlend == false"
            // metadata: MPropertyFriendlyName "First Curve"
            CPiecewiseCurve m_curve3; // 0x168            
            // metadata: MPropertySuppressExpr "m_bEnableDistanceBlend == false"
            // metadata: MPropertyFriendlyName "Second Curve"
            CPiecewiseCurve m_curve4; // 0x1a8            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CVoiceContainerParameterBlender because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::soundsystem_voicecontainers::CVoiceContainerParameterBlender) == 0x1e8);
    };
};
