#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CBaseTrigger.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CPathSimple;
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
        // Size: 0x8c8
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CTriggerPush : public source2sdk::server::CBaseTrigger
        {
        public:
            QAngle m_angPushEntitySpace; // 0x890            
            Vector m_vecPushDirEntitySpace; // 0x89c            
            bool m_bTriggerOnStartTouch; // 0x8a8            
            bool m_bUsePathSimple; // 0x8a9            
            uint8_t _pad08aa[0x6]; // 0x8aa
            CUtlSymbolLarge m_iszPathSimpleName; // 0x8b0            
            // metadata: MClassPtr
            source2sdk::server::CPathSimple* m_PathSimple; // 0x8b8            
            std::uint32_t m_splinePushType; // 0x8c0            
            uint8_t _pad08c4[0x4];
            
            // Datamap fields:
            // Vector InputSetPushDirection; // 0x0
            // float InputSetPushSpeed; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CTriggerPush because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CTriggerPush) == 0x8c8);
    };
};
