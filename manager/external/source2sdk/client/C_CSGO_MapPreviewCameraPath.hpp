#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseEntity.hpp"

// /////////////////////////////////////////////////////////////
// Module: client
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace client
    {
        // Registered alignment: 0x8
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x690
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class C_CSGO_MapPreviewCameraPath : public source2sdk::client::C_BaseEntity
        {
        public:
            float m_flZFar; // 0x608            
            float m_flZNear; // 0x60c            
            bool m_bLoop; // 0x610            
            bool m_bVerticalFOV; // 0x611            
            bool m_bConstantSpeed; // 0x612            
            uint8_t _pad0613[0x1]; // 0x613
            float m_flDuration; // 0x614            
            uint8_t _pad0618[0x40]; // 0x618
            float m_flPathLength; // 0x658            
            float m_flPathDuration; // 0x65c            
            uint8_t _pad0660[0x14]; // 0x660
            bool m_bDofEnabled; // 0x674            
            uint8_t _pad0675[0x3]; // 0x675
            float m_flDofNearBlurry; // 0x678            
            float m_flDofNearCrisp; // 0x67c            
            float m_flDofFarCrisp; // 0x680            
            float m_flDofFarBlurry; // 0x684            
            float m_flDofTiltToGround; // 0x688            
            uint8_t _pad068c[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_CSGO_MapPreviewCameraPath because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_CSGO_MapPreviewCameraPath) == 0x690);
    };
};
