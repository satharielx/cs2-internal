#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/BaseExplosionTypes_t.hpp"
#include "source2sdk/client/BreakableContentsType_t.hpp"
#include "source2sdk/client/CBaseProp.hpp"
#include "source2sdk/client/CPropDataComponent.hpp"
#include "source2sdk/client/PerformanceMode_t.hpp"
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
namespace source2sdk
{
    namespace client
    {
        struct C_BaseEntity;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct C_BasePlayerPawn;
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
        // Size: 0x12d0
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "CPropDataComponent::Storage_t m_CPropDataComponent"
        #pragma pack(push, 1)
        class C_BreakableProp : public source2sdk::client::CBaseProp
        {
        public:
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "CPropDataComponent"
            // metadata: MNetworkAlias "CPropDataComponent"
            // metadata: MNetworkTypeAlias "CPropDataComponent"
            source2sdk::client::CPropDataComponent m_CPropDataComponent; // 0x11a0            
            source2sdk::entity2::CEntityIOOutput m_OnStartDeath; // 0x11e0            
            source2sdk::entity2::CEntityIOOutput m_OnBreak; // 0x11f8            
            // m_OnHealthChanged has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<float,float> m_OnHealthChanged;
            char m_OnHealthChanged[0x20]; // 0x1210            
            source2sdk::entity2::CEntityIOOutput m_OnTakeDamage; // 0x1230            
            float m_impactEnergyScale; // 0x1248            
            std::int32_t m_iMinHealthDmg; // 0x124c            
            float m_flPressureDelay; // 0x1250            
            float m_flDefBurstScale; // 0x1254            
            Vector m_vDefBurstOffset; // 0x1258            
            // m_hBreaker has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_BaseEntity> m_hBreaker;
            char m_hBreaker[0x4]; // 0x1264            
            source2sdk::client::PerformanceMode_t m_PerformanceMode; // 0x1268            
            source2sdk::entity2::GameTime_t m_flPreventDamageBeforeTime; // 0x126c            
            source2sdk::client::BreakableContentsType_t m_BreakableContentsType; // 0x1270            
            uint8_t _pad1274[0x4]; // 0x1274
            CUtlString m_strBreakableContentsPropGroupOverride; // 0x1278            
            CUtlString m_strBreakableContentsParticleOverride; // 0x1280            
            bool m_bHasBreakPiecesOrCommands; // 0x1288            
            uint8_t _pad1289[0x3]; // 0x1289
            float m_explodeDamage; // 0x128c            
            float m_explodeRadius; // 0x1290            
            source2sdk::client::BaseExplosionTypes_t m_nExplosionType; // 0x1294            
            float m_explosionDelay; // 0x1298            
            uint8_t _pad129c[0x4]; // 0x129c
            CUtlSymbolLarge m_explosionBuildupSound; // 0x12a0            
            CUtlSymbolLarge m_explosionCustomEffect; // 0x12a8            
            CUtlSymbolLarge m_explosionCustomSound; // 0x12b0            
            CUtlSymbolLarge m_explosionModifier; // 0x12b8            
            // m_hPhysicsAttacker has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_BasePlayerPawn> m_hPhysicsAttacker;
            char m_hPhysicsAttacker[0x4]; // 0x12c0            
            source2sdk::entity2::GameTime_t m_flLastPhysicsInfluenceTime; // 0x12c4            
            float m_flDefaultFadeScale; // 0x12c8            
            // m_hLastAttacker has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_BaseEntity> m_hLastAttacker;
            char m_hLastAttacker[0x4]; // 0x12cc            
            
            // Datamap fields:
            // void InputBreak; // 0x0
            // int32_t InputSetHealth; // 0x0
            // int32_t InputAddHealth; // 0x0
            // int32_t InputRemoveHealth; // 0x0
            // void C_BreakablePropBreakThink; // 0x0
            // void C_BreakablePropRampToDefaultFadeScale; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_BreakableProp because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_BreakableProp) == 0x12d0);
    };
};
