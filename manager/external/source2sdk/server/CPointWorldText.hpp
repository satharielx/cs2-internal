#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/PointWorldTextJustifyHorizontal_t.hpp"
#include "source2sdk/client/PointWorldTextJustifyVertical_t.hpp"
#include "source2sdk/client/PointWorldTextReorientMode_t.hpp"
#include "source2sdk/server/CModelPointEntity.hpp"

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
        // Size: 0x9e0
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "char m_messageText"
        // static metadata: MNetworkVarNames "char m_FontName"
        // static metadata: MNetworkVarNames "char m_BackgroundMaterialName"
        // static metadata: MNetworkVarNames "bool m_bEnabled"
        // static metadata: MNetworkVarNames "bool m_bFullbright"
        // static metadata: MNetworkVarNames "float m_flWorldUnitsPerPx"
        // static metadata: MNetworkVarNames "float m_flFontSize"
        // static metadata: MNetworkVarNames "float m_flDepthOffset"
        // static metadata: MNetworkVarNames "bool m_bDrawBackground"
        // static metadata: MNetworkVarNames "float m_flBackgroundBorderWidth"
        // static metadata: MNetworkVarNames "float m_flBackgroundBorderHeight"
        // static metadata: MNetworkVarNames "float m_flBackgroundWorldToUV"
        // static metadata: MNetworkVarNames "Color m_Color"
        // static metadata: MNetworkVarNames "PointWorldTextJustifyHorizontal_t m_nJustifyHorizontal"
        // static metadata: MNetworkVarNames "PointWorldTextJustifyVertical_t m_nJustifyVertical"
        // static metadata: MNetworkVarNames "PointWorldTextReorientMode_t m_nReorientMode"
        #pragma pack(push, 1)
        class CPointWorldText : public source2sdk::server::CModelPointEntity
        {
        public:
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            char m_messageText[512]; // 0x730            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            char m_FontName[64]; // 0x930            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            char m_BackgroundMaterialName[64]; // 0x970            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bEnabled; // 0x9b0            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bFullbright; // 0x9b1            
            uint8_t _pad09b2[0x2]; // 0x9b2
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flWorldUnitsPerPx; // 0x9b4            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flFontSize; // 0x9b8            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flDepthOffset; // 0x9bc            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bDrawBackground; // 0x9c0            
            uint8_t _pad09c1[0x3]; // 0x9c1
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flBackgroundBorderWidth; // 0x9c4            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flBackgroundBorderHeight; // 0x9c8            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flBackgroundWorldToUV; // 0x9cc            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            Color m_Color; // 0x9d0            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            source2sdk::client::PointWorldTextJustifyHorizontal_t m_nJustifyHorizontal; // 0x9d4            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            source2sdk::client::PointWorldTextJustifyVertical_t m_nJustifyVertical; // 0x9d8            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            source2sdk::client::PointWorldTextReorientMode_t m_nReorientMode; // 0x9dc            
            
            // Datamap fields:
            // void InputEnable; // 0x0
            // void InputDisable; // 0x0
            // void InputToggle; // 0x0
            // CUtlSymbolLarge InputSetMessage; // 0x0
            // int32_t InputSetIntMessage; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CPointWorldText because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CPointWorldText) == 0x9e0);
    };
};
