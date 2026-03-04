#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/server/CBreakable.hpp"
#include "source2sdk/server/HoverPoseFlags_t.hpp"
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
        // Registered alignment: 0x8
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x8d0
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CPhysBox : public source2sdk::server::CBreakable
        {
        public:
            std::int32_t m_damageType; // 0x808            
            std::int32_t m_damageToEnableMotion; // 0x80c            
            float m_flForceToEnableMotion; // 0x810            
            Vector m_vHoverPosePosition; // 0x814            
            QAngle m_angHoverPoseAngles; // 0x820            
            bool m_bNotSolidToWorld; // 0x82c            
            bool m_bEnableUseOutput; // 0x82d            
            source2sdk::server::HoverPoseFlags_t m_nHoverPoseFlags; // 0x82e            
            uint8_t _pad082f[0x1]; // 0x82f
            float m_flTouchOutputPerEntityDelay; // 0x830            
            uint8_t _pad0834[0x4]; // 0x834
            source2sdk::entity2::CEntityIOOutput m_OnDamaged; // 0x838            
            source2sdk::entity2::CEntityIOOutput m_OnAwakened; // 0x850            
            source2sdk::entity2::CEntityIOOutput m_OnMotionEnabled; // 0x868            
            source2sdk::entity2::CEntityIOOutput m_OnPlayerUse; // 0x880            
            source2sdk::entity2::CEntityIOOutput m_OnStartTouch; // 0x898            
            // m_hCarryingPlayer has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBasePlayerPawn> m_hCarryingPlayer;
            char m_hCarryingPlayer[0x4]; // 0x8b0            
            uint8_t _pad08b4[0x1c];
            
            // Datamap fields:
            // QAngle preferredcarryangles; // 0x7fffffff
            // void InputWake; // 0x0
            // void InputSleep; // 0x0
            // void InputEnableMotion; // 0x0
            // void InputDisableMotion; // 0x0
            // void InputForceDrop; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CPhysBox because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CPhysBox) == 0x8d0);
    };
};
