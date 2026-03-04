#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CBaseClientUIEntity.hpp"

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
        // Size: 0x8e8
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MEntityAllowsPortraitWorldSpawn
        // static metadata: MNetworkVarNames "bool m_bIgnoreInput"
        // static metadata: MNetworkVarNames "bool m_bLit"
        // static metadata: MNetworkVarNames "bool m_bFollowPlayerAcrossTeleport"
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
        // static metadata: MNetworkVarNames "bool m_bOpaque"
        // static metadata: MNetworkVarNames "bool m_bNoDepth"
        // static metadata: MNetworkVarNames "bool m_bVisibleWhenParentNoDraw"
        // static metadata: MNetworkVarNames "bool m_bRenderBackface"
        // static metadata: MNetworkVarNames "bool m_bUseOffScreenIndicator"
        // static metadata: MNetworkVarNames "bool m_bExcludeFromSaveGames"
        // static metadata: MNetworkVarNames "bool m_bGrabbable"
        // static metadata: MNetworkVarNames "bool m_bOnlyRenderToTexture"
        // static metadata: MNetworkVarNames "bool m_bDisableMipGen"
        // static metadata: MNetworkVarNames "int32 m_nExplicitImageLayout"
        #pragma pack(push, 1)
        class CPointClientUIWorldPanel : public source2sdk::server::CBaseClientUIEntity
        {
        public:
            // metadata: MNetworkEnable
            bool m_bIgnoreInput; // 0x890            
            // metadata: MNetworkEnable
            bool m_bLit; // 0x891            
            // metadata: MNetworkEnable
            bool m_bFollowPlayerAcrossTeleport; // 0x892            
            uint8_t _pad0893[0x1]; // 0x893
            // metadata: MNetworkEnable
            float m_flWidth; // 0x894            
            // metadata: MNetworkEnable
            float m_flHeight; // 0x898            
            // metadata: MNetworkEnable
            float m_flDPI; // 0x89c            
            // metadata: MNetworkEnable
            float m_flInteractDistance; // 0x8a0            
            // metadata: MNetworkEnable
            float m_flDepthOffset; // 0x8a4            
            // metadata: MNetworkEnable
            std::uint32_t m_unOwnerContext; // 0x8a8            
            // metadata: MNetworkEnable
            std::uint32_t m_unHorizontalAlign; // 0x8ac            
            // metadata: MNetworkEnable
            std::uint32_t m_unVerticalAlign; // 0x8b0            
            // metadata: MNetworkEnable
            std::uint32_t m_unOrientation; // 0x8b4            
            // metadata: MNetworkEnable
            bool m_bAllowInteractionFromAllSceneWorlds; // 0x8b8            
            uint8_t _pad08b9[0x7]; // 0x8b9
            // metadata: MNetworkEnable
            // m_vecCSSClasses has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CNetworkUtlVectorBase<CUtlSymbolLarge> m_vecCSSClasses;
            char m_vecCSSClasses[0x18]; // 0x8c0            
            // metadata: MNetworkEnable
            bool m_bOpaque; // 0x8d8            
            // metadata: MNetworkEnable
            bool m_bNoDepth; // 0x8d9            
            // metadata: MNetworkEnable
            bool m_bVisibleWhenParentNoDraw; // 0x8da            
            // metadata: MNetworkEnable
            bool m_bRenderBackface; // 0x8db            
            // metadata: MNetworkEnable
            bool m_bUseOffScreenIndicator; // 0x8dc            
            // metadata: MNetworkEnable
            bool m_bExcludeFromSaveGames; // 0x8dd            
            // metadata: MNetworkEnable
            bool m_bGrabbable; // 0x8de            
            // metadata: MNetworkEnable
            bool m_bOnlyRenderToTexture; // 0x8df            
            // metadata: MNetworkEnable
            bool m_bDisableMipGen; // 0x8e0            
            uint8_t _pad08e1[0x3]; // 0x8e1
            // metadata: MNetworkEnable
            std::int32_t m_nExplicitImageLayout; // 0x8e4            
            
            // Datamap fields:
            // CUtlSymbolLarge css_class; // 0x7fffffff
            // void InputIgnoreUserInput; // 0x0
            // void InputAcceptUserInput; // 0x0
            // CUtlSymbolLarge InputAddCSSClass; // 0x0
            // CUtlSymbolLarge InputRemoveCSSClass; // 0x0
            // CUtlSymbolLarge InputLocalPlayerAddCSSClass; // 0x0
            // CUtlSymbolLarge InputLocalPlayerRemoveCSSClass; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CPointClientUIWorldPanel because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CPointClientUIWorldPanel) == 0x8e8);
    };
};
