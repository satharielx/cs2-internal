#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CBaseEntity.hpp"

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
        // Size: 0x4e0
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
        class CEnvWindVolume : public source2sdk::server::CBaseEntity
        {
        public:
            // metadata: MNetworkEnable
            bool m_bActive; // 0x4a8            
            uint8_t _pad04a9[0x3]; // 0x4a9
            // metadata: MNetworkEnable
            Vector m_vBoxMins; // 0x4ac            
            // metadata: MNetworkEnable
            Vector m_vBoxMaxs; // 0x4b8            
            // metadata: MNetworkEnable
            bool m_bStartDisabled; // 0x4c4            
            uint8_t _pad04c5[0x3]; // 0x4c5
            // metadata: MNetworkEnable
            std::int32_t m_nShape; // 0x4c8            
            // metadata: MNetworkEnable
            float m_fWindSpeedMultiplier; // 0x4cc            
            // metadata: MNetworkEnable
            float m_fWindTurbulenceMultiplier; // 0x4d0            
            // metadata: MNetworkEnable
            float m_fWindSpeedVariationMultiplier; // 0x4d4            
            // metadata: MNetworkEnable
            float m_fWindDirectionVariationMultiplier; // 0x4d8            
            uint8_t _pad04dc[0x4];
            
            // Datamap fields:
            // bool InputEnable; // 0x0
            // bool InputDisable; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CEnvWindVolume because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CEnvWindVolume) == 0x4e0);
    };
};
