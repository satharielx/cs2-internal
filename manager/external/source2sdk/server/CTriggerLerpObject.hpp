#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/modellib/AttachmentHandle_t.hpp"
#include "source2sdk/server/CBaseTrigger.hpp"
#include "source2sdk/server/lerpdata_t.hpp"
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
        // Size: 0x930
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CTriggerLerpObject : public source2sdk::server::CBaseTrigger
        {
        public:
            CUtlSymbolLarge m_iszLerpTarget; // 0x890            
            // m_hLerpTarget has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hLerpTarget;
            char m_hLerpTarget[0x4]; // 0x898            
            uint8_t _pad089c[0x4]; // 0x89c
            CUtlSymbolLarge m_iszLerpTargetAttachment; // 0x8a0            
            source2sdk::modellib::AttachmentHandle_t m_hLerpTargetAttachment; // 0x8a8            
            uint8_t _pad08a9[0x3]; // 0x8a9
            float m_flLerpDuration; // 0x8ac            
            bool m_bAttachedEntityWasParented; // 0x8b0            
            bool m_bLerpRestoreMoveType; // 0x8b1            
            bool m_bSingleLerpObject; // 0x8b2            
            uint8_t _pad08b3[0x5]; // 0x8b3
            // m_vecLerpingObjects has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<source2sdk::server::lerpdata_t> m_vecLerpingObjects;
            char m_vecLerpingObjects[0x18]; // 0x8b8            
            CUtlSymbolLarge m_iszLerpEffect; // 0x8d0            
            CUtlSymbolLarge m_iszLerpSound; // 0x8d8            
            bool m_bAttachTouchingObject; // 0x8e0            
            uint8_t _pad08e1[0x3]; // 0x8e1
            // m_hEntityToWaitForDisconnect has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hEntityToWaitForDisconnect;
            char m_hEntityToWaitForDisconnect[0x4]; // 0x8e4            
            source2sdk::entity2::CEntityIOOutput m_OnLerpStarted; // 0x8e8            
            source2sdk::entity2::CEntityIOOutput m_OnLerpFinished; // 0x900            
            source2sdk::entity2::CEntityIOOutput m_OnDetached; // 0x918            
            
            // Datamap fields:
            // void CTriggerLerpObjectLerpThink; // 0x0
            // void CTriggerLerpObjectUnsetWaitForEntity; // 0x0
            // void CTriggerLerpObjectAttachedEntityThink; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CTriggerLerpObject because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CTriggerLerpObject) == 0x930);
    };
};
