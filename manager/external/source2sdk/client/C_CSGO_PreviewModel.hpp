#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/AnimLoopMode_t.hpp"
#include "source2sdk/client/C_BaseFlex.hpp"

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
        // Size: 0x1860
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class C_CSGO_PreviewModel : public source2sdk::client::C_BaseFlex
        {
        public:
            CUtlString m_defaultAnim; // 0x1350            
            source2sdk::client::AnimLoopMode_t m_nDefaultAnimLoopMode; // 0x1358            
            float m_flInitialModelScale; // 0x135c            
            CUtlString m_sInitialWeaponState; // 0x1360            
            uint8_t _pad1368[0x4f8];
            
            // Datamap fields:
            // CUtlSymbolLarge InputSetItem; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_CSGO_PreviewModel because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_CSGO_PreviewModel) == 0x1860);
    };
};
