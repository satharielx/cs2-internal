#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/server/CRotButton.hpp"

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
        // Size: 0x968
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CMomentaryRotButton : public source2sdk::server::CRotButton
        {
        public:
            // m_Position has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<float,float> m_Position;
            char m_Position[0x20]; // 0x8b0            
            source2sdk::entity2::CEntityIOOutput m_OnUnpressed; // 0x8d0            
            source2sdk::entity2::CEntityIOOutput m_OnFullyOpen; // 0x8e8            
            source2sdk::entity2::CEntityIOOutput m_OnFullyClosed; // 0x900            
            source2sdk::entity2::CEntityIOOutput m_OnReachedPosition; // 0x918            
            std::int32_t m_lastUsed; // 0x930            
            QAngle m_start; // 0x934            
            QAngle m_end; // 0x940            
            float m_IdealYaw; // 0x94c            
            CUtlSymbolLarge m_sNoise; // 0x950            
            bool m_bUpdateTarget; // 0x958            
            uint8_t _pad0959[0x3]; // 0x959
            std::int32_t m_direction; // 0x95c            
            float m_returnSpeed; // 0x960            
            float m_flStartPosition; // 0x964            
            
            // Datamap fields:
            // bool m_bSolidBsp; // 0x808
            // void CMomentaryRotButtonUseMoveDone; // 0x0
            // void CMomentaryRotButtonReturnMoveDone; // 0x0
            // void CMomentaryRotButtonSetPositionMoveDone; // 0x0
            // void CMomentaryRotButtonUpdateThink; // 0x0
            // float InputSetPosition; // 0x0
            // float InputSetPositionImmediately; // 0x0
            // void InputDisableUpdateTarget; // 0x0
            // void InputEnableUpdateTarget; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CMomentaryRotButton because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CMomentaryRotButton) == 0x968);
    };
};
