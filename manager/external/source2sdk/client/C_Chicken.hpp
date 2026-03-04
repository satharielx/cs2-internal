#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_AttributeContainer.hpp"
#include "source2sdk/client/C_DynamicProp.hpp"
#include "source2sdk/client/ParticleIndex_t.hpp"
namespace source2sdk
{
    namespace client
    {
        struct CBaseAnimGraph;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct C_CSPlayerPawn;
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
        // Size: 0x18a0
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "bool m_jumpedThisFrame"
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerPawn> m_leader"
        // static metadata: MNetworkVarNames "CAttributeContainer m_AttributeManager"
        #pragma pack(push, 1)
        class C_Chicken : public source2sdk::client::C_DynamicProp
        {
        public:
            uint8_t _pad13a0[0x8]; // 0x13a0
            // m_hHolidayHatAddon has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::CBaseAnimGraph> m_hHolidayHatAddon;
            char m_hHolidayHatAddon[0x4]; // 0x13a8            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnJumpedChanged"
            bool m_jumpedThisFrame; // 0x13ac            
            uint8_t _pad13ad[0x3]; // 0x13ad
            // metadata: MNetworkEnable
            // m_leader has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_CSPlayerPawn> m_leader;
            char m_leader[0x4]; // 0x13b0            
            uint8_t _pad13b4[0x4]; // 0x13b4
            // metadata: MNetworkEnable
            source2sdk::client::C_AttributeContainer m_AttributeManager; // 0x13b8            
            bool m_bAttributesInitialized; // 0x1888            
            uint8_t _pad1889[0x3]; // 0x1889
            source2sdk::client::ParticleIndex_t m_hWaterWakeParticles; // 0x188c            
            bool m_bIsPreviewModel; // 0x1890            
            uint8_t _pad1891[0xf];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_Chicken because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_Chicken) == 0x18a0);
    };
};
