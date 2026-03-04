#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_ModelPointEntity.hpp"
#include "source2sdk/client/PointWorldTextJustifyHorizontal_t.hpp"
#include "source2sdk/client/PointWorldTextJustifyVertical_t.hpp"
#include "source2sdk/client/PointWorldTextReorientMode_t.hpp"

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
        // Size: 0x1160
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
        class C_PointWorldText : public source2sdk::client::C_ModelPointEntity
        {
        public:
            uint8_t _pad0e88[0x8]; // 0xe88
            // metadata: MNotSaved
            bool m_bForceRecreateNextUpdate; // 0xe90            
            uint8_t _pad0e91[0x17]; // 0xe91
            std::int32_t m_nTextWidthPx; // 0xea8            
            std::int32_t m_nTextHeightPx; // 0xeac            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            char m_messageText[512]; // 0xeb0            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            char m_FontName[64]; // 0x10b0            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            char m_BackgroundMaterialName[64]; // 0x10f0            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bEnabled; // 0x1130            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bFullbright; // 0x1131            
            uint8_t _pad1132[0x2]; // 0x1132
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flWorldUnitsPerPx; // 0x1134            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flFontSize; // 0x1138            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flDepthOffset; // 0x113c            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bDrawBackground; // 0x1140            
            uint8_t _pad1141[0x3]; // 0x1141
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flBackgroundBorderWidth; // 0x1144            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flBackgroundBorderHeight; // 0x1148            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flBackgroundWorldToUV; // 0x114c            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            Color m_Color; // 0x1150            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            source2sdk::client::PointWorldTextJustifyHorizontal_t m_nJustifyHorizontal; // 0x1154            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            source2sdk::client::PointWorldTextJustifyVertical_t m_nJustifyVertical; // 0x1158            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            source2sdk::client::PointWorldTextReorientMode_t m_nReorientMode; // 0x115c            
            
            // Datamap fields:
            // void InputEnable; // 0x0
            // void InputDisable; // 0x0
            // void InputToggle; // 0x0
            // CUtlSymbolLarge InputSetMessage; // 0x0
            // int32_t InputSetIntMessage; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_PointWorldText because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_PointWorldText) == 0x1160);
    };
};
