#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/BaseExplosionTypes_t.hpp"
#include "source2sdk/client/BreakableContentsType_t.hpp"
#include "source2sdk/client/PerformanceMode_t.hpp"
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CBaseProp.hpp"
#include "source2sdk/server/CPropDataComponent.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CBaseEntity;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CBasePlayerPawn;
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
        // Registered alignment: 0x10
        // Alignment: 0x10
        // Standard-layout class: false
        // Size: 0xba0
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "CPropDataComponent::Storage_t m_CPropDataComponent"
        #pragma pack(push, 1)
        class CBreakableProp : public source2sdk::server::CBaseProp
        {
        public:
            uint8_t _pad0a50[0x8]; // 0xa50
            // metadata: MNetworkEnable
            // metadata: MNetworkUserGroup "CPropDataComponent"
            // metadata: MNetworkAlias "CPropDataComponent"
            // metadata: MNetworkTypeAlias "CPropDataComponent"
            source2sdk::server::CPropDataComponent m_CPropDataComponent; // 0xa58            
            source2sdk::entity2::CEntityIOOutput m_OnStartDeath; // 0xa98            
            source2sdk::entity2::CEntityIOOutput m_OnBreak; // 0xab0            
            // m_OnHealthChanged has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<float,float> m_OnHealthChanged;
            char m_OnHealthChanged[0x20]; // 0xac8            
            source2sdk::entity2::CEntityIOOutput m_OnTakeDamage; // 0xae8            
            float m_impactEnergyScale; // 0xb00            
            std::int32_t m_iMinHealthDmg; // 0xb04            
            QAngle m_preferredCarryAngles; // 0xb08            
            float m_flPressureDelay; // 0xb14            
            float m_flDefBurstScale; // 0xb18            
            Vector m_vDefBurstOffset; // 0xb1c            
            // m_hBreaker has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hBreaker;
            char m_hBreaker[0x4]; // 0xb28            
            source2sdk::client::PerformanceMode_t m_PerformanceMode; // 0xb2c            
            source2sdk::entity2::GameTime_t m_flPreventDamageBeforeTime; // 0xb30            
            source2sdk::client::BreakableContentsType_t m_BreakableContentsType; // 0xb34            
            CUtlString m_strBreakableContentsPropGroupOverride; // 0xb38            
            CUtlString m_strBreakableContentsParticleOverride; // 0xb40            
            bool m_bHasBreakPiecesOrCommands; // 0xb48            
            uint8_t _pad0b49[0x3]; // 0xb49
            float m_explodeDamage; // 0xb4c            
            float m_explodeRadius; // 0xb50            
            source2sdk::client::BaseExplosionTypes_t m_nExplosionType; // 0xb54            
            float m_explosionDelay; // 0xb58            
            uint8_t _pad0b5c[0x4]; // 0xb5c
            CUtlSymbolLarge m_explosionBuildupSound; // 0xb60            
            CUtlSymbolLarge m_explosionCustomEffect; // 0xb68            
            CUtlSymbolLarge m_explosionCustomSound; // 0xb70            
            CUtlSymbolLarge m_explosionModifier; // 0xb78            
            // m_hPhysicsAttacker has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBasePlayerPawn> m_hPhysicsAttacker;
            char m_hPhysicsAttacker[0x4]; // 0xb80            
            source2sdk::entity2::GameTime_t m_flLastPhysicsInfluenceTime; // 0xb84            
            float m_flDefaultFadeScale; // 0xb88            
            // m_hLastAttacker has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hLastAttacker;
            char m_hLastAttacker[0x4]; // 0xb8c            
            CUtlSymbolLarge m_iszPuntSound; // 0xb90            
            bool m_bUsePuntSound; // 0xb98            
            bool m_bOriginalBlockLOS; // 0xb99            
            uint8_t _pad0b9a[0x6];
            
            // Datamap fields:
            // void InputBreak; // 0x0
            // int32_t InputSetHealth; // 0x0
            // int32_t InputAddHealth; // 0x0
            // int32_t InputRemoveHealth; // 0x0
            // bool InputSetNavIgnore; // 0x0
            // void InputEnablePuntSound; // 0x0
            // void InputDisablePuntSound; // 0x0
            // void CBreakablePropBreakThink; // 0x0
            // void CBreakablePropRampToDefaultFadeScale; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CBreakableProp because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CBreakableProp) == 0xba0);
    };
};
