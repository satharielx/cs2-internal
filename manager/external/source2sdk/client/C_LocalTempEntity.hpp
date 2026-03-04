#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CBaseAnimGraph.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"

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
        // Size: 0x1210
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class C_LocalTempEntity : public source2sdk::client::CBaseAnimGraph
        {
        public:
            // metadata: MNotSaved
            std::int32_t flags; // 0x1168            
            // metadata: MNotSaved
            source2sdk::entity2::GameTime_t die; // 0x116c            
            // metadata: MNotSaved
            float m_flFrameMax; // 0x1170            
            // metadata: MNotSaved
            float x; // 0x1174            
            // metadata: MNotSaved
            float y; // 0x1178            
            // metadata: MNotSaved
            float fadeSpeed; // 0x117c            
            // metadata: MNotSaved
            float bounceFactor; // 0x1180            
            // metadata: MNotSaved
            std::int32_t hitSound; // 0x1184            
            // metadata: MNotSaved
            std::int32_t priority; // 0x1188            
            // metadata: MNotSaved
            Vector tentOffset; // 0x118c            
            // metadata: MNotSaved
            QAngle m_vecTempEntAngVelocity; // 0x1198            
            // metadata: MNotSaved
            std::int32_t tempent_renderamt; // 0x11a4            
            // metadata: MNotSaved
            Vector m_vecNormal; // 0x11a8            
            // metadata: MNotSaved
            float m_flSpriteScale; // 0x11b4            
            // metadata: MNotSaved
            std::int32_t m_nFlickerFrame; // 0x11b8            
            // metadata: MNotSaved
            float m_flFrameRate; // 0x11bc            
            // metadata: MNotSaved
            float m_flFrame; // 0x11c0            
            uint8_t _pad11c4[0x4]; // 0x11c4
            // metadata: MNotSaved
            char* m_pszImpactEffect; // 0x11c8            
            // metadata: MNotSaved
            char* m_pszParticleEffect; // 0x11d0            
            // metadata: MNotSaved
            bool m_bParticleCollision; // 0x11d8            
            uint8_t _pad11d9[0x3]; // 0x11d9
            // metadata: MNotSaved
            std::int32_t m_iLastCollisionFrame; // 0x11dc            
            // metadata: MNotSaved
            Vector m_vLastCollisionOrigin; // 0x11e0            
            // metadata: MNotSaved
            Vector m_vecTempEntVelocity; // 0x11ec            
            // metadata: MNotSaved
            Vector m_vecPrevAbsOrigin; // 0x11f8            
            // metadata: MNotSaved
            Vector m_vecTempEntAcceleration; // 0x1204            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_LocalTempEntity because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_LocalTempEntity) == 0x1210);
    };
};
