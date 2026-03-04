#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CCollisionProperty.hpp"
#include "source2sdk/client/CGlowProperty.hpp"
#include "source2sdk/client/CHitboxComponent.hpp"
#include "source2sdk/client/CNetworkViewOffsetVector.hpp"
#include "source2sdk/client/C_BaseEntity.hpp"
#include "source2sdk/client/DecalRtEncoding_t.hpp"
#include "source2sdk/client/EntityRenderAttribute_t.hpp"
#include "source2sdk/client/HitGroup_t.hpp"
#include "source2sdk/client/RenderFx_t.hpp"
#include "source2sdk/client/RenderMode_t.hpp"
namespace source2sdk
{
    namespace client
    {
        struct CClientAlphaProperty;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct CDestructiblePartsComponent;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct CRenderComponent;
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
        // Registered alignment: 0x8
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0xe88
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "CRenderComponent::Storage_t m_CRenderComponent"
        // static metadata: MNetworkVarNames "CHitboxComponent::Storage_t m_CHitboxComponent"
        // static metadata: MNetworkVarNames "CDestructiblePartsComponent * m_pDestructiblePartsSystemComponent"
        // static metadata: MNetworkVarNames "RenderMode_t m_nRenderMode"
        // static metadata: MNetworkVarNames "RenderFx_t m_nRenderFX"
        // static metadata: MNetworkVarNames "Color m_clrRender"
        // static metadata: MNetworkVarNames "EntityRenderAttribute_t m_vecRenderAttributes"
        // static metadata: MNetworkVarNames "bool m_bRenderToCubemaps"
        // static metadata: MNetworkVarNames "bool m_bNoInterpolate"
        // static metadata: MNetworkVarNames "CCollisionProperty m_Collision"
        // static metadata: MNetworkVarNames "CGlowProperty m_Glow"
        // static metadata: MNetworkVarNames "float m_flGlowBackfaceMult"
        // static metadata: MNetworkVarNames "float32 m_fadeMinDist"
        // static metadata: MNetworkVarNames "float32 m_fadeMaxDist"
        // static metadata: MNetworkVarNames "float32 m_flFadeScale"
        // static metadata: MNetworkVarNames "float32 m_flShadowStrength"
        // static metadata: MNetworkVarNames "uint8 m_nObjectCulling"
        // static metadata: MNetworkVarNames "uint32 m_bvDisabledHitGroups"
        #pragma pack(push, 1)
        class C_BaseModelEntity : public source2sdk::client::C_BaseEntity
        {
        public:
            uint8_t _pad0608[0x4e8]; // 0x608
            // metadata: MNotSaved
            source2sdk::client::CRenderComponent* m_CRenderComponent; // 0xaf0            
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "CHitboxComponent"
            // metadata: MNetworkAlias "CHitboxComponent"
            // metadata: MNetworkTypeAlias "CHitboxComponent"
            source2sdk::client::CHitboxComponent m_CHitboxComponent; // 0xaf8            
            source2sdk::client::HitGroup_t m_nDestructiblePartInitialStateDestructed0; // 0xb10            
            source2sdk::client::HitGroup_t m_nDestructiblePartInitialStateDestructed1; // 0xb14            
            source2sdk::client::HitGroup_t m_nDestructiblePartInitialStateDestructed2; // 0xb18            
            source2sdk::client::HitGroup_t m_nDestructiblePartInitialStateDestructed3; // 0xb1c            
            source2sdk::client::HitGroup_t m_nDestructiblePartInitialStateDestructed4; // 0xb20            
            std::int32_t m_nDestructiblePartInitialStateDestructed0_PartIndex; // 0xb24            
            std::int32_t m_nDestructiblePartInitialStateDestructed1_PartIndex; // 0xb28            
            std::int32_t m_nDestructiblePartInitialStateDestructed2_PartIndex; // 0xb2c            
            std::int32_t m_nDestructiblePartInitialStateDestructed3_PartIndex; // 0xb30            
            std::int32_t m_nDestructiblePartInitialStateDestructed4_PartIndex; // 0xb34            
            // metadata: MNetworkEnable
            // metadata: MPtrAutoallocate
            // metadata: MNetworkTypeAlias "CDestructiblePartsSystemComponent*"
            source2sdk::client::CDestructiblePartsComponent* m_pDestructiblePartsSystemComponent; // 0xb38            
            uint8_t _pad0b40[0x18]; // 0xb40
            // metadata: MNotSaved
            bool m_bInitModelEffects; // 0xb58            
            // metadata: MNotSaved
            bool m_bDoingModelEffects; // 0xb59            
            // metadata: MNotSaved
            bool m_bIsStaticProp; // 0xb5a            
            uint8_t _pad0b5b[0x1]; // 0xb5b
            // metadata: MNotSaved
            std::int32_t m_iOldHealth; // 0xb5c            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnRenderModeChanged"
            source2sdk::client::RenderMode_t m_nRenderMode; // 0xb60            
            // metadata: MNetworkEnable
            source2sdk::client::RenderFx_t m_nRenderFX; // 0xb61            
            bool m_bAllowFadeInView; // 0xb62            
            uint8_t _pad0b63[0x1d]; // 0xb63
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnColorChanged"
            Color m_clrRender; // 0xb80            
            uint8_t _pad0b84[0x4]; // 0xb84
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnRenderAttributesChanged"
            // m_vecRenderAttributes has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // C_UtlVectorEmbeddedNetworkVar<source2sdk::client::EntityRenderAttribute_t> m_vecRenderAttributes;
            char m_vecRenderAttributes[0x68]; // 0xb88            
            uint8_t _pad0bf0[0x18]; // 0xbf0
            // metadata: MNetworkEnable
            bool m_bRenderToCubemaps; // 0xc08            
            // metadata: MNetworkEnable
            bool m_bNoInterpolate; // 0xc09            
            uint8_t _pad0c0a[0x6]; // 0xc0a
            // metadata: MNetworkEnable
            source2sdk::client::CCollisionProperty m_Collision; // 0xc10            
            // metadata: MNetworkEnable
            source2sdk::client::CGlowProperty m_Glow; // 0xcc0            
            // metadata: MNetworkEnable
            float m_flGlowBackfaceMult; // 0xd18            
            // metadata: MNetworkEnable
            float m_fadeMinDist; // 0xd1c            
            // metadata: MNetworkEnable
            float m_fadeMaxDist; // 0xd20            
            // metadata: MNetworkEnable
            float m_flFadeScale; // 0xd24            
            // metadata: MNetworkEnable
            float m_flShadowStrength; // 0xd28            
            // metadata: MNetworkEnable
            std::uint8_t m_nObjectCulling; // 0xd2c            
            source2sdk::client::DecalRtEncoding_t m_nRequiredDecalRtEncoding; // 0xd2d            
            uint8_t _pad0d2e[0x2a]; // 0xd2e
            // metadata: MNetworkEnable
            // metadata: MNetworkPriority "32"
            // metadata: MNetworkUserGroup "Player"
            // metadata: MNetworkChangeCallback "OnViewOffsetChanged"
            source2sdk::client::CNetworkViewOffsetVector m_vecViewOffset; // 0xd58            
            uint8_t _pad0d80[0xb8]; // 0xd80
            // metadata: MNotSaved
            source2sdk::client::CClientAlphaProperty* m_pClientAlphaProperty; // 0xe38            
            // metadata: MNotSaved
            Color m_ClientOverrideTint; // 0xe40            
            // metadata: MNotSaved
            bool m_bUseClientOverrideTint; // 0xe44            
            uint8_t _pad0e45[0x3b]; // 0xe45
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnDisabledHitgroupsChanged"
            // metadata: MSaveOpsForField
            std::uint32_t m_bvDisabledHitGroups[1]; // 0xe80            
            uint8_t _pad0e84[0x4];
            
            // Datamap fields:
            // void m_bvDisabledHitGroups; // 0xe80
            // void m_bodyGroupChoices; // 0xd30
            // int32_t InputAlpha; // 0x0
            // Color InputColor; // 0x0
            // int32_t InputSkin; // 0x0
            // CUtlString add_attribute; // 0x7fffffff
            // void m_Ropes; // 0xb40
            // Color rendercolor32; // 0x7fffffff
            // Color rendercolor; // 0x7fffffff
            // int32_t renderamt; // 0x7fffffff
            // Vector mins; // 0x7fffffff
            // Vector maxs; // 0x7fffffff
            // const char * skin; // 0x7fffffff
            // CUtlString bodygroups; // 0x7fffffff
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_BaseModelEntity because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_BaseModelEntity) == 0xe88);
    };
};
