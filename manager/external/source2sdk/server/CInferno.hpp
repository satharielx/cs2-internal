#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/navlib/Extent.hpp"
#include "source2sdk/server/CBaseModelEntity.hpp"
#include "source2sdk/server/CountdownTimer.hpp"
#include "source2sdk/server/IntervalTimer.hpp"

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
        // Size: 0x13b8
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "Vector m_firePositions"
        // static metadata: MNetworkVarNames "Vector m_fireParentPositions"
        // static metadata: MNetworkVarNames "bool m_bFireIsBurning"
        // static metadata: MNetworkVarNames "Vector m_BurnNormal"
        // static metadata: MNetworkVarNames "int m_fireCount"
        // static metadata: MNetworkVarNames "int m_nInfernoType"
        // static metadata: MNetworkVarNames "int m_nFireEffectTickBegin"
        // static metadata: MNetworkVarNames "float m_nFireLifetime"
        // static metadata: MNetworkVarNames "bool m_bInPostEffectTime"
        #pragma pack(push, 1)
        class CInferno : public source2sdk::server::CBaseModelEntity
        {
        public:
            // metadata: MNetworkEnable
            Vector m_firePositions[64]; // 0x730            
            // metadata: MNetworkEnable
            Vector m_fireParentPositions[64]; // 0xa30            
            // metadata: MNetworkEnable
            bool m_bFireIsBurning[64]; // 0xd30            
            // metadata: MNetworkEnable
            Vector m_BurnNormal[64]; // 0xd70            
            // metadata: MNetworkEnable
            std::int32_t m_fireCount; // 0x1070            
            // metadata: MNetworkEnable
            std::int32_t m_nInfernoType; // 0x1074            
            // metadata: MNetworkEnable
            std::int32_t m_nFireEffectTickBegin; // 0x1078            
            // metadata: MNetworkEnable
            float m_nFireLifetime; // 0x107c            
            // metadata: MNetworkEnable
            bool m_bInPostEffectTime; // 0x1080            
            bool m_bWasCreatedInSmoke; // 0x1081            
            uint8_t _pad1082[0x206]; // 0x1082
            source2sdk::navlib::Extent m_extent; // 0x1288            
            source2sdk::server::CountdownTimer m_damageTimer; // 0x12a0            
            source2sdk::server::CountdownTimer m_damageRampTimer; // 0x12b8            
            Vector m_splashVelocity; // 0x12d0            
            Vector m_InitialSplashVelocity; // 0x12dc            
            Vector m_startPos; // 0x12e8            
            Vector m_vecOriginalSpawnLocation; // 0x12f4            
            source2sdk::server::IntervalTimer m_activeTimer; // 0x1300            
            std::int32_t m_fireSpawnOffset; // 0x1310            
            std::int32_t m_nMaxFlames; // 0x1314            
            std::int32_t m_nSpreadCount; // 0x1318            
            uint8_t _pad131c[0x4]; // 0x131c
            source2sdk::server::CountdownTimer m_BookkeepingTimer; // 0x1320            
            source2sdk::server::CountdownTimer m_NextSpreadTimer; // 0x1338            
            std::uint16_t m_nSourceItemDefIndex; // 0x1350            
            uint8_t _pad1352[0x66];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CInferno because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CInferno) == 0x13b8);
    };
};
