#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseClientUIEntity.hpp"

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
        // Size: 0x1080
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "bool m_bIgnoreInput"
        // static metadata: MNetworkVarNames "float m_flWidth"
        // static metadata: MNetworkVarNames "float m_flHeight"
        // static metadata: MNetworkVarNames "float m_flDPI"
        // static metadata: MNetworkVarNames "float m_flInteractDistance"
        // static metadata: MNetworkVarNames "float m_flDepthOffset"
        // static metadata: MNetworkVarNames "uint32 m_unOwnerContext"
        // static metadata: MNetworkVarNames "uint32 m_unHorizontalAlign"
        // static metadata: MNetworkVarNames "uint32 m_unVerticalAlign"
        // static metadata: MNetworkVarNames "uint32 m_unOrientation"
        // static metadata: MNetworkVarNames "bool m_bAllowInteractionFromAllSceneWorlds"
        // static metadata: MNetworkVarNames "string_t m_vecCSSClasses"
        #pragma pack(push, 1)
        class C_PointClientUIHUD : public source2sdk::client::C_BaseClientUIEntity
        {
        public:
            uint8_t _pad0eb8[0x8]; // 0xeb8
            // metadata: MNotSaved
            bool m_bCheckCSSClasses; // 0xec0            
            uint8_t _pad0ec1[0x177]; // 0xec1
            // metadata: MNetworkEnable
            bool m_bIgnoreInput; // 0x1038            
            uint8_t _pad1039[0x3]; // 0x1039
            // metadata: MNetworkEnable
            float m_flWidth; // 0x103c            
            // metadata: MNetworkEnable
            float m_flHeight; // 0x1040            
            // metadata: MNetworkEnable
            float m_flDPI; // 0x1044            
            // metadata: MNetworkEnable
            float m_flInteractDistance; // 0x1048            
            // metadata: MNetworkEnable
            float m_flDepthOffset; // 0x104c            
            // metadata: MNetworkEnable
            std::uint32_t m_unOwnerContext; // 0x1050            
            // metadata: MNetworkEnable
            std::uint32_t m_unHorizontalAlign; // 0x1054            
            // metadata: MNetworkEnable
            std::uint32_t m_unVerticalAlign; // 0x1058            
            // metadata: MNetworkEnable
            std::uint32_t m_unOrientation; // 0x105c            
            // metadata: MNetworkEnable
            bool m_bAllowInteractionFromAllSceneWorlds; // 0x1060            
            uint8_t _pad1061[0x7]; // 0x1061
            // metadata: MNetworkEnable
            // m_vecCSSClasses has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // C_NetworkUtlVectorBase<CUtlSymbolLarge> m_vecCSSClasses;
            char m_vecCSSClasses[0x18]; // 0x1068            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_PointClientUIHUD because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_PointClientUIHUD) == 0x1080);
    };
};
