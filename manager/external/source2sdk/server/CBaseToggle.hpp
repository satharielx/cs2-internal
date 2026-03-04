#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CBaseModelEntity.hpp"
#include "source2sdk/server/TOGGLE_STATE.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CBaseEntity;
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
        // Size: 0x7b0
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CBaseToggle : public source2sdk::server::CBaseModelEntity
        {
        public:
            source2sdk::server::TOGGLE_STATE m_toggle_state; // 0x730            
            float m_flMoveDistance; // 0x734            
            float m_flWait; // 0x738            
            float m_flLip; // 0x73c            
            bool m_bAlwaysFireBlockedOutputs; // 0x740            
            uint8_t _pad0741[0x3]; // 0x741
            Vector m_vecPosition1; // 0x744            
            Vector m_vecPosition2; // 0x750            
            QAngle m_vecMoveAng; // 0x75c            
            QAngle m_vecAngle1; // 0x768            
            QAngle m_vecAngle2; // 0x774            
            float m_flHeight; // 0x780            
            // m_hActivator has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hActivator;
            char m_hActivator[0x4]; // 0x784            
            Vector m_vecFinalDest; // 0x788            
            QAngle m_vecFinalAngle; // 0x794            
            std::int32_t m_movementType; // 0x7a0            
            uint8_t _pad07a4[0x4]; // 0x7a4
            CUtlSymbolLarge m_sMaster; // 0x7a8            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CBaseToggle because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CBaseToggle) == 0x7b0);
    };
};
