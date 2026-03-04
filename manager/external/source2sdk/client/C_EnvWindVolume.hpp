#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseEntity.hpp"

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
        // Size: 0x640
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MEntityAllowsPortraitWorldSpawn
        // static metadata: MNetworkVarNames "bool m_bActive"
        // static metadata: MNetworkVarNames "Vector m_vBoxMins"
        // static metadata: MNetworkVarNames "Vector m_vBoxMaxs"
        // static metadata: MNetworkVarNames "bool m_bStartDisabled"
        // static metadata: MNetworkVarNames "int m_nShape"
        // static metadata: MNetworkVarNames "float m_fWindSpeedMultiplier"
        // static metadata: MNetworkVarNames "float m_fWindTurbulenceMultiplier"
        // static metadata: MNetworkVarNames "float m_fWindSpeedVariationMultiplier"
        // static metadata: MNetworkVarNames "float m_fWindDirectionVariationMultiplier"
        #pragma pack(push, 1)
        class C_EnvWindVolume : public source2sdk::client::C_BaseEntity
        {
        public:
            // metadata: MNetworkEnable
            bool m_bActive; // 0x608            
            uint8_t _pad0609[0x3]; // 0x609
            // metadata: MNetworkEnable
            Vector m_vBoxMins; // 0x60c            
            // metadata: MNetworkEnable
            Vector m_vBoxMaxs; // 0x618            
            // metadata: MNetworkEnable
            bool m_bStartDisabled; // 0x624            
            uint8_t _pad0625[0x3]; // 0x625
            // metadata: MNetworkEnable
            std::int32_t m_nShape; // 0x628            
            // metadata: MNetworkEnable
            float m_fWindSpeedMultiplier; // 0x62c            
            // metadata: MNetworkEnable
            float m_fWindTurbulenceMultiplier; // 0x630            
            // metadata: MNetworkEnable
            float m_fWindSpeedVariationMultiplier; // 0x634            
            // metadata: MNetworkEnable
            float m_fWindDirectionVariationMultiplier; // 0x638            
            uint8_t _pad063c[0x4];
            
            // Datamap fields:
            // bool InputEnable; // 0x0
            // bool InputDisable; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_EnvWindVolume because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_EnvWindVolume) == 0x640);
    };
};
