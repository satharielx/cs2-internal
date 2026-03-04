#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseClientUIEntity.hpp"
namespace source2sdk
{
    namespace client
    {
        struct CPointOffScreenIndicatorUi;
    };
};

// /////////////////////////////////////////////////////////////
// Module: client
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace client
    {
        // Registered alignment: 0x10
        // Alignment: 0x10
        // Standard-layout class: false
        // Size: 0x10e0
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
        class C_PointClientUIWorldPanel : public source2sdk::client::C_BaseClientUIEntity
        {
        public:
            uint8_t _pad0eb8[0x8]; // 0xeb8
            // metadata: MNotSaved
            bool m_bForceRecreateNextUpdate; // 0xec0            
            // metadata: MNotSaved
            bool m_bMoveViewToPlayerNextThink; // 0xec1            
            // metadata: MNotSaved
            bool m_bCheckCSSClasses; // 0xec2            
            uint8_t _pad0ec3[0xd]; // 0xec3
            // metadata: MNotSaved
            CTransform m_anchorDeltaTransform; // 0xed0            
            uint8_t _pad0ef0[0x170]; // 0xef0
            // metadata: MNotSaved
            source2sdk::client::CPointOffScreenIndicatorUi* m_pOffScreenIndicator; // 0x1060            
            uint8_t _pad1068[0x20]; // 0x1068
            // metadata: MNetworkEnable
            bool m_bIgnoreInput; // 0x1088            
            // metadata: MNetworkEnable
            bool m_bLit; // 0x1089            
            // metadata: MNetworkEnable
            bool m_bFollowPlayerAcrossTeleport; // 0x108a            
            uint8_t _pad108b[0x1]; // 0x108b
            // metadata: MNetworkEnable
            float m_flWidth; // 0x108c            
            // metadata: MNetworkEnable
            float m_flHeight; // 0x1090            
            // metadata: MNetworkEnable
            float m_flDPI; // 0x1094            
            // metadata: MNetworkEnable
            float m_flInteractDistance; // 0x1098            
            // metadata: MNetworkEnable
            float m_flDepthOffset; // 0x109c            
            // metadata: MNetworkEnable
            std::uint32_t m_unOwnerContext; // 0x10a0            
            // metadata: MNetworkEnable
            std::uint32_t m_unHorizontalAlign; // 0x10a4            
            // metadata: MNetworkEnable
            std::uint32_t m_unVerticalAlign; // 0x10a8            
            // metadata: MNetworkEnable
            std::uint32_t m_unOrientation; // 0x10ac            
            // metadata: MNetworkEnable
            bool m_bAllowInteractionFromAllSceneWorlds; // 0x10b0            
            uint8_t _pad10b1[0x7]; // 0x10b1
            // metadata: MNetworkEnable
            // m_vecCSSClasses has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // C_NetworkUtlVectorBase<CUtlSymbolLarge> m_vecCSSClasses;
            char m_vecCSSClasses[0x18]; // 0x10b8            
            // metadata: MNetworkEnable
            bool m_bOpaque; // 0x10d0            
            // metadata: MNetworkEnable
            bool m_bNoDepth; // 0x10d1            
            // metadata: MNetworkEnable
            bool m_bVisibleWhenParentNoDraw; // 0x10d2            
            // metadata: MNetworkEnable
            bool m_bRenderBackface; // 0x10d3            
            // metadata: MNetworkEnable
            bool m_bUseOffScreenIndicator; // 0x10d4            
            // metadata: MNetworkEnable
            bool m_bExcludeFromSaveGames; // 0x10d5            
            // metadata: MNetworkEnable
            bool m_bGrabbable; // 0x10d6            
            // metadata: MNetworkEnable
            bool m_bOnlyRenderToTexture; // 0x10d7            
            // metadata: MNetworkEnable
            bool m_bDisableMipGen; // 0x10d8            
            uint8_t _pad10d9[0x3]; // 0x10d9
            // metadata: MNetworkEnable
            std::int32_t m_nExplicitImageLayout; // 0x10dc            
            
            // Datamap fields:
            // CUtlSymbolLarge css_class; // 0x7fffffff
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_PointClientUIWorldPanel because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_PointClientUIWorldPanel) == 0x10e0);
    };
};
