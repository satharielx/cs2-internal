#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/server/CBaseModelEntity.hpp"

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
        // Size: 0x7f8
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkOverride "m_angRotation CGameSceneNode"
        // static metadata: MNetworkOverride "m_flSimulationTime CBaseEntity"
        #pragma pack(push, 1)
        class CFuncRotating : public source2sdk::server::CBaseModelEntity
        {
        public:
            source2sdk::entity2::CEntityIOOutput m_OnStopped; // 0x730            
            source2sdk::entity2::CEntityIOOutput m_OnStarted; // 0x748            
            source2sdk::entity2::CEntityIOOutput m_OnReachedStart; // 0x760            
            RotationVector m_localRotationVector; // 0x778            
            float m_flFanFriction; // 0x784            
            float m_flAttenuation; // 0x788            
            float m_flVolume; // 0x78c            
            float m_flTargetSpeed; // 0x790            
            float m_flMaxSpeed; // 0x794            
            float m_flBlockDamage; // 0x798            
            uint8_t _pad079c[0x4]; // 0x79c
            CUtlSymbolLarge m_NoiseRunning; // 0x7a0            
            bool m_bReversed; // 0x7a8            
            bool m_bAccelDecel; // 0x7a9            
            uint8_t _pad07aa[0x16]; // 0x7aa
            QAngle m_prevLocalAngles; // 0x7c0            
            QAngle m_angStart; // 0x7cc            
            bool m_bStopAtStartPos; // 0x7d8            
            uint8_t _pad07d9[0x3]; // 0x7d9
            // metadata: MNotSaved
            Vector m_vecClientOrigin; // 0x7dc            
            // metadata: MNotSaved
            QAngle m_vecClientAngles; // 0x7e8            
            uint8_t _pad07f4[0x4];
            
            // Datamap fields:
            // bool movewithoutpushingblockers; // 0x7fffffff
            // void CFuncRotatingSpinUpMove; // 0x0
            // void CFuncRotatingSpinDownMove; // 0x0
            // void CFuncRotatingHurtTouch; // 0x0
            // void CFuncRotatingRotatingUse; // 0x0
            // void CFuncRotatingRotateMove; // 0x0
            // void CFuncRotatingReverseMove; // 0x0
            // float InputSetSpeed; // 0x0
            // void InputStart; // 0x0
            // void InputStop; // 0x0
            // void InputToggle; // 0x0
            // void InputReverse; // 0x0
            // void InputStartForward; // 0x0
            // void InputStartBackward; // 0x0
            // void InputStopAtStartPos; // 0x0
            // void InputSnapToStartPos; // 0x0
            // Vector InputSetStartPos; // 0x0
            // void InputEnableAccelDecel; // 0x0
            // void InputDisableAccelDecel; // 0x0
            // void m_nNoiseRunningGuid; // 0x7ac
            // int32_t fanfriction; // 0x7fffffff
            // int32_t Volume; // 0x7fffffff
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CFuncRotating because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CFuncRotating) == 0x7f8);
    };
};
