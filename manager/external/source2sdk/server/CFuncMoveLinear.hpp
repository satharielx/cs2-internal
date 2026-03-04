#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/server/CBaseToggle.hpp"
#include "source2sdk/server/MoveLinearAuthoredPos_t.hpp"

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
        // Size: 0x838
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkIncludeByName "m_fFlags"
        // static metadata: MNetworkIncludeByName "m_vecVelocity"
        // static metadata: MNetworkOverride "m_fFlags CBaseEntity"
        #pragma pack(push, 1)
        class CFuncMoveLinear : public source2sdk::server::CBaseToggle
        {
        public:
            source2sdk::server::MoveLinearAuthoredPos_t m_authoredPosition; // 0x7b0            
            QAngle m_angMoveEntitySpace; // 0x7b4            
            Vector m_vecMoveDirParentSpace; // 0x7c0            
            uint8_t _pad07cc[0x4]; // 0x7cc
            CUtlSymbolLarge m_soundStart; // 0x7d0            
            CUtlSymbolLarge m_soundStop; // 0x7d8            
            CUtlSymbolLarge m_currentSound; // 0x7e0            
            float m_flBlockDamage; // 0x7e8            
            float m_flStartPosition; // 0x7ec            
            uint8_t _pad07f0[0x8]; // 0x7f0
            source2sdk::entity2::CEntityIOOutput m_OnFullyOpen; // 0x7f8            
            source2sdk::entity2::CEntityIOOutput m_OnFullyClosed; // 0x810            
            bool m_bCreateMovableNavMesh; // 0x828            
            bool m_bAllowMovableNavMeshDockingOnEntireEntity; // 0x829            
            bool m_bCreateNavObstacle; // 0x82a            
            uint8_t _pad082b[0xd];
            
            // Datamap fields:
            // bool movewithoutpushingblockers; // 0x7fffffff
            // void CFuncMoveLinearNavObstacleThink; // 0x0
            // void CFuncMoveLinearNavMovableThink; // 0x0
            // void InputOpen; // 0x0
            // void InputClose; // 0x0
            // float InputSetPosition; // 0x0
            // float InputSetSpeed; // 0x0
            // CUtlSymbolLarge InputTeleportToTarget; // 0x0
            // float InputResetPosition; // 0x0
            // float InputSetMoveDistanceFromStart; // 0x0
            // float InputSetMoveDistanceFromEnd; // 0x0
            // void CFuncMoveLinearStopMoveSound; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CFuncMoveLinear because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CFuncMoveLinear) == 0x838);
    };
};
