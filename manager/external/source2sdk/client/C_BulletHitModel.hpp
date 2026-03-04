#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CBaseAnimGraph.hpp"
namespace source2sdk
{
    namespace client
    {
        struct C_BaseEntity;
    };
};

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
        // Size: 0x11b8
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class C_BulletHitModel : public source2sdk::client::CBaseAnimGraph
        {
        public:
            matrix3x4_t m_matLocal; // 0x1168            
            std::int32_t m_iBoneIndex; // 0x1198            
            // m_hPlayerParent has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_BaseEntity> m_hPlayerParent;
            char m_hPlayerParent[0x4]; // 0x119c            
            bool m_bIsHit; // 0x11a0            
            uint8_t _pad11a1[0x3]; // 0x11a1
            float m_flTimeCreated; // 0x11a4            
            Vector m_vecStartPos; // 0x11a8            
            uint8_t _pad11b4[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_BulletHitModel because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_BulletHitModel) == 0x11b8);
    };
};
