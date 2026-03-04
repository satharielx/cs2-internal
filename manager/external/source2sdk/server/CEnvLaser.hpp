#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CBeam.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CSprite;
    };
};

// /////////////////////////////////////////////////////////////
// Module: server
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace server
    {
        // Registered alignment: 0x8
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x7f8
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CEnvLaser : public source2sdk::server::CBeam
        {
        public:
            CUtlSymbolLarge m_iszLaserTarget; // 0x7d0            
            // metadata: MClassPtr
            source2sdk::server::CSprite* m_pSprite; // 0x7d8            
            CUtlSymbolLarge m_iszSpriteName; // 0x7e0            
            Vector m_firePosition; // 0x7e8            
            float m_flStartFrame; // 0x7f4            
            
            // Datamap fields:
            // void CEnvLaserStrikeThink; // 0x0
            // void InputTurnOn; // 0x0
            // void InputTurnOff; // 0x0
            // void InputToggle; // 0x0
            // float width; // 0x7fffffff
            // int32_t NoiseAmplitude; // 0x7fffffff
            // int32_t TextureScroll; // 0x7fffffff
            // CUtlString texture; // 0x7fffffff
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CEnvLaser because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CEnvLaser) == 0x7f8);
    };
};
