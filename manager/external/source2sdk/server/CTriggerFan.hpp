#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CBaseTrigger.hpp"
#include "source2sdk/server/CountdownTimer.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CInfoFan;
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
        // Size: 0x950
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "Vector m_vFanOriginOffset"
        // static metadata: MNetworkVarNames "Vector m_vDirection"
        // static metadata: MNetworkVarNames "bool m_bPushTowardsInfoTarget"
        // static metadata: MNetworkVarNames "bool m_bPushAwayFromInfoTarget"
        // static metadata: MNetworkVarNames "Quaternion m_qNoiseDelta"
        // static metadata: MNetworkVarNames "CHandle< CInfoFan> m_hInfoFan"
        // static metadata: MNetworkVarNames "float m_flForce"
        // static metadata: MNetworkVarNames "bool m_bFalloff"
        // static metadata: MNetworkVarNames "CountdownTimer m_RampTimer"
        #pragma pack(push, 1)
        class CTriggerFan : public source2sdk::server::CBaseTrigger
        {
        public:
            // metadata: MNetworkEnable
            Vector m_vFanOriginOffset; // 0x890            
            // metadata: MNetworkEnable
            Vector m_vDirection; // 0x89c            
            // metadata: MNetworkEnable
            bool m_bPushTowardsInfoTarget; // 0x8a8            
            // metadata: MNetworkEnable
            bool m_bPushAwayFromInfoTarget; // 0x8a9            
            uint8_t _pad08aa[0x6]; // 0x8aa
            // metadata: MNetworkEnable
            Quaternion m_qNoiseDelta; // 0x8b0            
            // metadata: MNetworkEnable
            // m_hInfoFan has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CInfoFan> m_hInfoFan;
            char m_hInfoFan[0x4]; // 0x8c0            
            // metadata: MNetworkEnable
            float m_flForce; // 0x8c4            
            // metadata: MNetworkEnable
            bool m_bFalloff; // 0x8c8            
            uint8_t _pad08c9[0x7]; // 0x8c9
            // metadata: MNetworkEnable
            source2sdk::server::CountdownTimer m_RampTimer; // 0x8d0            
            VectorWS m_vFanOriginWS; // 0x8e8            
            Vector m_vFanOriginLS; // 0x8f4            
            Vector m_vFanEndLS; // 0x900            
            Vector m_vNoiseDirectionTarget; // 0x90c            
            CUtlSymbolLarge m_iszInfoFan; // 0x918            
            float m_flRopeForceScale; // 0x920            
            float m_flParticleForceScale; // 0x924            
            float m_flPlayerForce; // 0x928            
            bool m_bPlayerWindblock; // 0x92c            
            uint8_t _pad092d[0x3]; // 0x92d
            float m_flNPCForce; // 0x930            
            float m_flRampTime; // 0x934            
            float m_fNoiseDegrees; // 0x938            
            float m_fNoiseSpeed; // 0x93c            
            bool m_bPushPlayer; // 0x940            
            bool m_bRampDown; // 0x941            
            uint8_t _pad0942[0x2]; // 0x942
            std::int32_t m_nManagerFanIdx; // 0x944            
            uint8_t _pad0948[0x8];
            
            // Datamap fields:
            // void CTriggerFanPushThink; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CTriggerFan because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CTriggerFan) == 0x950);
    };
};
