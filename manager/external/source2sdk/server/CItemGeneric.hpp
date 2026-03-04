#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/resourcesystem/InfoForResourceTypeIParticleSystemDefinition.hpp"
#include "source2sdk/server/CItem.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CBaseFilter;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CItemGenericTriggerHelper;
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
        // Size: 0xbf0
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CItemGeneric : public source2sdk::server::CItem
        {
        public:
            uint8_t _pad0ac0[0x14]; // 0xac0
            // metadata: MNotSaved
            bool m_bHasTriggerRadius; // 0xad4            
            // metadata: MNotSaved
            bool m_bHasPickupRadius; // 0xad5            
            uint8_t _pad0ad6[0x2]; // 0xad6
            // metadata: MNotSaved
            float m_flPickupRadiusSqr; // 0xad8            
            // metadata: MNotSaved
            float m_flTriggerRadiusSqr; // 0xadc            
            // metadata: MNotSaved
            source2sdk::entity2::GameTime_t m_flLastPickupCheck; // 0xae0            
            // metadata: MNotSaved
            bool m_bPlayerCounterListenerAdded; // 0xae4            
            // metadata: MNotSaved
            bool m_bPlayerInTriggerRadius; // 0xae5            
            uint8_t _pad0ae6[0x2]; // 0xae6
            // metadata: MNotSaved
            // m_hSpawnParticleEffect has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CStrongHandle<source2sdk::resourcesystem::InfoForResourceTypeIParticleSystemDefinition> m_hSpawnParticleEffect;
            char m_hSpawnParticleEffect[0x8]; // 0xae8            
            // metadata: MNotSaved
            CUtlSymbolLarge m_pAmbientSoundEffect; // 0xaf0            
            // metadata: MNotSaved
            bool m_bAutoStartAmbientSound; // 0xaf8            
            uint8_t _pad0af9[0x7]; // 0xaf9
            // metadata: MNotSaved
            CUtlSymbolLarge m_pSpawnScriptFunction; // 0xb00            
            // metadata: MNotSaved
            // m_hPickupParticleEffect has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CStrongHandle<source2sdk::resourcesystem::InfoForResourceTypeIParticleSystemDefinition> m_hPickupParticleEffect;
            char m_hPickupParticleEffect[0x8]; // 0xb08            
            // metadata: MNotSaved
            CUtlSymbolLarge m_pPickupSoundEffect; // 0xb10            
            // metadata: MNotSaved
            CUtlSymbolLarge m_pPickupScriptFunction; // 0xb18            
            // metadata: MNotSaved
            // m_hTimeoutParticleEffect has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CStrongHandle<source2sdk::resourcesystem::InfoForResourceTypeIParticleSystemDefinition> m_hTimeoutParticleEffect;
            char m_hTimeoutParticleEffect[0x8]; // 0xb20            
            // metadata: MNotSaved
            CUtlSymbolLarge m_pTimeoutSoundEffect; // 0xb28            
            // metadata: MNotSaved
            CUtlSymbolLarge m_pTimeoutScriptFunction; // 0xb30            
            // metadata: MNotSaved
            CUtlSymbolLarge m_pPickupFilterName; // 0xb38            
            // metadata: MNotSaved
            // m_hPickupFilter has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseFilter> m_hPickupFilter;
            char m_hPickupFilter[0x4]; // 0xb40            
            uint8_t _pad0b44[0x4]; // 0xb44
            source2sdk::entity2::CEntityIOOutput m_OnPickup; // 0xb48            
            source2sdk::entity2::CEntityIOOutput m_OnTimeout; // 0xb60            
            source2sdk::entity2::CEntityIOOutput m_OnTriggerStartTouch; // 0xb78            
            source2sdk::entity2::CEntityIOOutput m_OnTriggerTouch; // 0xb90            
            source2sdk::entity2::CEntityIOOutput m_OnTriggerEndTouch; // 0xba8            
            // metadata: MNotSaved
            CUtlSymbolLarge m_pAllowPickupScriptFunction; // 0xbc0            
            // metadata: MNotSaved
            float m_flPickupRadius; // 0xbc8            
            // metadata: MNotSaved
            float m_flTriggerRadius; // 0xbcc            
            // metadata: MNotSaved
            CUtlSymbolLarge m_pTriggerSoundEffect; // 0xbd0            
            // metadata: MNotSaved
            bool m_bGlowWhenInTrigger; // 0xbd8            
            // metadata: MNotSaved
            Color m_glowColor; // 0xbd9            
            // metadata: MNotSaved
            bool m_bUseable; // 0xbdd            
            uint8_t _pad0bde[0x2]; // 0xbde
            // metadata: MNotSaved
            // m_hTriggerHelper has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CItemGenericTriggerHelper> m_hTriggerHelper;
            char m_hTriggerHelper[0x4]; // 0xbe0            
            uint8_t _pad0be4[0xc];
            
            // Datamap fields:
            // void CItemGenericItemGenericTouch; // 0x0
            // void InputStartAmbientSound; // 0x0
            // void InputStopAmbientSound; // 0x0
            // void InputToggleAmbientSound; // 0x0
            // void m_hAmbientSound; // 0xac0
            // float auto_remove_timeout; // 0x7fffffff
            // float drag_override; // 0x7fffffff
            // float damping_override; // 0x7fffffff
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CItemGeneric because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CItemGeneric) == 0xbf0);
    };
};
