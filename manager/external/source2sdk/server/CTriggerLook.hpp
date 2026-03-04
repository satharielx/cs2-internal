#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CTriggerOnce.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CBaseEntity;
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
        // Registered alignment: 0x8
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x910
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "bool m_bTestOcclusion"
        // static metadata: MNetworkVarNames "bool m_bTestAllVisibleOcclusion"
        #pragma pack(push, 1)
        class CTriggerLook : public source2sdk::server::CTriggerOnce
        {
        public:
            // m_hLookTarget has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hLookTarget;
            char m_hLookTarget[0x4]; // 0x8a8            
            float m_flFieldOfView; // 0x8ac            
            float m_flLookTime; // 0x8b0            
            float m_flLookTimeTotal; // 0x8b4            
            source2sdk::entity2::GameTime_t m_flLookTimeLast; // 0x8b8            
            float m_flTimeoutDuration; // 0x8bc            
            bool m_bTimeoutFired; // 0x8c0            
            bool m_bIsLooking; // 0x8c1            
            bool m_b2DFOV; // 0x8c2            
            bool m_bUseVelocity; // 0x8c3            
            // metadata: MNetworkEnable
            bool m_bTestOcclusion; // 0x8c4            
            // metadata: MNetworkEnable
            bool m_bTestAllVisibleOcclusion; // 0x8c5            
            uint8_t _pad08c6[0x2]; // 0x8c6
            source2sdk::entity2::CEntityIOOutput m_OnTimeout; // 0x8c8            
            source2sdk::entity2::CEntityIOOutput m_OnStartLook; // 0x8e0            
            source2sdk::entity2::CEntityIOOutput m_OnEndLook; // 0x8f8            
            
            // Datamap fields:
            // void CTriggerLookTimeoutThink; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CTriggerLook because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CTriggerLook) == 0x910);
    };
};
