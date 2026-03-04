#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CBasePropDoor.hpp"
#include "source2sdk/server/PropDoorRotatingOpenDirection_e.hpp"
#include "source2sdk/server/PropDoorRotatingSpawnPos_t.hpp"
#include "source2sdk/server/doorCheck_e.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CEntityBlocker;
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
        // Registered alignment: 0x10
        // Alignment: 0x10
        // Standard-layout class: false
        // Size: 0xef0
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CPropDoorRotating : public source2sdk::server::CBasePropDoor
        {
        public:
            Vector m_vecAxis; // 0xe50            
            float m_flDistance; // 0xe5c            
            source2sdk::server::PropDoorRotatingSpawnPos_t m_eSpawnPosition; // 0xe60            
            source2sdk::server::PropDoorRotatingOpenDirection_e m_eOpenDirection; // 0xe64            
            // metadata: MNotSaved
            source2sdk::server::PropDoorRotatingOpenDirection_e m_eCurrentOpenDirection; // 0xe68            
            // metadata: MNotSaved
            source2sdk::server::doorCheck_e m_eDefaultCheckDirection; // 0xe6c            
            float m_flAjarAngle; // 0xe70            
            QAngle m_angRotationAjarDeprecated; // 0xe74            
            QAngle m_angRotationClosed; // 0xe80            
            QAngle m_angRotationOpenForward; // 0xe8c            
            QAngle m_angRotationOpenBack; // 0xe98            
            QAngle m_angGoal; // 0xea4            
            // metadata: MNotSaved
            Vector m_vecForwardBoundsMin; // 0xeb0            
            // metadata: MNotSaved
            Vector m_vecForwardBoundsMax; // 0xebc            
            // metadata: MNotSaved
            Vector m_vecBackBoundsMin; // 0xec8            
            // metadata: MNotSaved
            Vector m_vecBackBoundsMax; // 0xed4            
            bool m_bAjarDoorShouldntAlwaysOpen; // 0xee0            
            uint8_t _pad0ee1[0x3]; // 0xee1
            // m_hEntityBlocker has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CEntityBlocker> m_hEntityBlocker;
            char m_hEntityBlocker[0x4]; // 0xee4            
            uint8_t _pad0ee8[0x8];
            
            // Datamap fields:
            // float InputSetRotationDistance; // 0x0
            // float InputSetSpeed; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CPropDoorRotating because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CPropDoorRotating) == 0xef0);
    };
};
