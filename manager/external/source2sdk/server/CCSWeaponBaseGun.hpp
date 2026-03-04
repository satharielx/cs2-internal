#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CCSWeaponBase.hpp"

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
        // Size: 0x1150
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "int m_zoomLevel"
        // static metadata: MNetworkVarNames "int m_iBurstShotsRemaining"
        // static metadata: MNetworkVarNames "bool m_bNeedsBoltAction"
        // static metadata: MNetworkVarNames "int32 m_nRevolverCylinderIdx"
        #pragma pack(push, 1)
        class CCSWeaponBaseGun : public source2sdk::server::CCSWeaponBase
        {
        public:
            // metadata: MNetworkEnable
            std::int32_t m_zoomLevel; // 0x1130            
            // metadata: MNetworkEnable
            std::int32_t m_iBurstShotsRemaining; // 0x1134            
            uint8_t _pad1138[0x8]; // 0x1138
            std::int32_t m_silencedModelIndex; // 0x1140            
            bool m_inPrecache; // 0x1144            
            // metadata: MNetworkEnable
            bool m_bNeedsBoltAction; // 0x1145            
            uint8_t _pad1146[0x2]; // 0x1146
            // metadata: MNetworkEnable
            std::int32_t m_nRevolverCylinderIdx; // 0x1148            
            bool m_bSkillReloadAvailable; // 0x114c            
            bool m_bSkillReloadLiftedReloadKey; // 0x114d            
            bool m_bSkillBoltInterruptAvailable; // 0x114e            
            bool m_bSkillBoltLiftedFireKey; // 0x114f            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSWeaponBaseGun because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CCSWeaponBaseGun) == 0x1150);
    };
};
