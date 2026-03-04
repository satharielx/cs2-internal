#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/resourcesystem/InfoForResourceTypeIMaterial2.hpp"
#include "source2sdk/server/CBaseModelEntity.hpp"

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
        // Size: 0x860
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CFuncShatterglass : public source2sdk::server::CBaseModelEntity
        {
        public:
            matrix3x4_t m_matPanelTransform; // 0x730            
            matrix3x4_t m_matPanelTransformWsTemp; // 0x760            
            // m_vecShatterGlassShards has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<std::uint32_t> m_vecShatterGlassShards;
            char m_vecShatterGlassShards[0x18]; // 0x790            
            Vector2D m_PanelSize; // 0x7a8            
            source2sdk::entity2::GameTime_t m_flLastShatterSoundEmitTime; // 0x7b0            
            source2sdk::entity2::GameTime_t m_flLastCleanupTime; // 0x7b4            
            source2sdk::entity2::GameTime_t m_flInitAtTime; // 0x7b8            
            float m_flGlassThickness; // 0x7bc            
            float m_flSpawnInvulnerability; // 0x7c0            
            bool m_bBreakSilent; // 0x7c4            
            bool m_bBreakShardless; // 0x7c5            
            bool m_bBroken; // 0x7c6            
            bool m_bGlassNavIgnore; // 0x7c7            
            bool m_bGlassInFrame; // 0x7c8            
            bool m_bStartBroken; // 0x7c9            
            std::uint8_t m_iInitialDamageType; // 0x7ca            
            uint8_t _pad07cb[0x5]; // 0x7cb
            CUtlSymbolLarge m_szDamagePositioningEntityName01; // 0x7d0            
            CUtlSymbolLarge m_szDamagePositioningEntityName02; // 0x7d8            
            CUtlSymbolLarge m_szDamagePositioningEntityName03; // 0x7e0            
            CUtlSymbolLarge m_szDamagePositioningEntityName04; // 0x7e8            
            // m_vInitialDamagePositions has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<Vector> m_vInitialDamagePositions;
            char m_vInitialDamagePositions[0x18]; // 0x7f0            
            // m_vExtraDamagePositions has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<Vector> m_vExtraDamagePositions;
            char m_vExtraDamagePositions[0x18]; // 0x808            
            // m_vInitialPanelVertices has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<Vector4D> m_vInitialPanelVertices;
            char m_vInitialPanelVertices[0x18]; // 0x820            
            source2sdk::entity2::CEntityIOOutput m_OnBroken; // 0x838            
            std::uint8_t m_iSurfaceType; // 0x850            
            uint8_t _pad0851[0x7]; // 0x851
            // m_hMaterialDamageBase has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CStrongHandle<source2sdk::resourcesystem::InfoForResourceTypeIMaterial2> m_hMaterialDamageBase;
            char m_hMaterialDamageBase[0x8]; // 0x858            
            
            // Datamap fields:
            // void CFuncShatterglassGlassThink; // 0x0
            // void InputHit; // 0x0
            // void InputShatter; // 0x0
            // void InputRestore; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CFuncShatterglass because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CFuncShatterglass) == 0x860);
    };
};
