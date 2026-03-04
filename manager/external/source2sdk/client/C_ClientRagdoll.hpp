#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CBaseAnimGraph.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/modellib/AttachmentHandle_t.hpp"

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
        // Size: 0x1200
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class C_ClientRagdoll : public source2sdk::client::CBaseAnimGraph
        {
        public:
            bool m_bFadeOut; // 0x1168            
            bool m_bImportant; // 0x1169            
            uint8_t _pad116a[0x2]; // 0x116a
            source2sdk::entity2::GameTime_t m_flEffectTime; // 0x116c            
            source2sdk::entity2::GameTime_t m_gibDespawnTime; // 0x1170            
            std::int32_t m_iCurrentFriction; // 0x1174            
            std::int32_t m_iMinFriction; // 0x1178            
            std::int32_t m_iMaxFriction; // 0x117c            
            std::int32_t m_iFrictionAnimState; // 0x1180            
            bool m_bReleaseRagdoll; // 0x1184            
            source2sdk::modellib::AttachmentHandle_t m_iEyeAttachment; // 0x1185            
            bool m_bFadingOut; // 0x1186            
            uint8_t _pad1187[0x1]; // 0x1187
            float m_flScaleEnd[10]; // 0x1188            
            source2sdk::entity2::GameTime_t m_flScaleTimeStart[10]; // 0x11b0            
            source2sdk::entity2::GameTime_t m_flScaleTimeEnd[10]; // 0x11d8            
            
            // Datamap fields:
            // Color m_clrRender; // 0xb80
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_ClientRagdoll because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_ClientRagdoll) == 0x1200);
    };
};
