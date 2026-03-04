#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/server/CBaseToggle.hpp"
#include "source2sdk/server/FuncDoorSpawnPos_t.hpp"
#include "source2sdk/server/locksound_t.hpp"

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
        // Size: 0x930
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "bool m_bIsUsable"
        #pragma pack(push, 1)
        class CBaseDoor : public source2sdk::server::CBaseToggle
        {
        public:
            uint8_t _pad07b0[0x10]; // 0x7b0
            QAngle m_angMoveEntitySpace; // 0x7c0            
            Vector m_vecMoveDirParentSpace; // 0x7cc            
            // metadata: MNotSaved
            source2sdk::server::locksound_t m_ls; // 0x7d8            
            bool m_bForceClosed; // 0x7f8            
            bool m_bDoorGroup; // 0x7f9            
            bool m_bLocked; // 0x7fa            
            bool m_bIgnoreDebris; // 0x7fb            
            bool m_bNoNPCs; // 0x7fc            
            uint8_t _pad07fd[0x3]; // 0x7fd
            source2sdk::server::FuncDoorSpawnPos_t m_eSpawnPosition; // 0x800            
            float m_flBlockDamage; // 0x804            
            CUtlSymbolLarge m_NoiseMoving; // 0x808            
            CUtlSymbolLarge m_NoiseArrived; // 0x810            
            CUtlSymbolLarge m_NoiseMovingClosed; // 0x818            
            CUtlSymbolLarge m_NoiseArrivedClosed; // 0x820            
            CUtlSymbolLarge m_ChainTarget; // 0x828            
            source2sdk::entity2::CEntityIOOutput m_OnBlockedClosing; // 0x830            
            source2sdk::entity2::CEntityIOOutput m_OnBlockedOpening; // 0x848            
            source2sdk::entity2::CEntityIOOutput m_OnUnblockedClosing; // 0x860            
            source2sdk::entity2::CEntityIOOutput m_OnUnblockedOpening; // 0x878            
            source2sdk::entity2::CEntityIOOutput m_OnFullyClosed; // 0x890            
            source2sdk::entity2::CEntityIOOutput m_OnFullyOpen; // 0x8a8            
            source2sdk::entity2::CEntityIOOutput m_OnClose; // 0x8c0            
            source2sdk::entity2::CEntityIOOutput m_OnOpen; // 0x8d8            
            source2sdk::entity2::CEntityIOOutput m_OnLockedUse; // 0x8f0            
            bool m_bLoopMoveSound; // 0x908            
            uint8_t _pad0909[0x1f]; // 0x909
            bool m_bCreateNavObstacle; // 0x928            
            // metadata: MNotSaved
            bool m_isChaining; // 0x929            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bIsUsable; // 0x92a            
            uint8_t _pad092b[0x5];
            
            // Datamap fields:
            // void m_nMovingSoundGUID; // 0x90c
            //  m_ls.sLockedSound; // 0x7e0
            //  m_ls.sUnlockedSound; // 0x7e8
            // void InputOpen; // 0x0
            // void InputClose; // 0x0
            // void InputToggle; // 0x0
            // void InputLock; // 0x0
            // void InputUnlock; // 0x0
            // float InputSetSpeed; // 0x0
            // float InputSetToggleState; // 0x0
            // bool InputSetNoNPCs; // 0x0
            // void CBaseDoorDoorTouch; // 0x0
            // void CBaseDoorDoorGoUp; // 0x0
            // void CBaseDoorDoorGoDown; // 0x0
            // void CBaseDoorDoorHitTop; // 0x0
            // void CBaseDoorDoorHitBottom; // 0x0
            // void CBaseDoorMovingSoundThink; // 0x0
            // void CBaseDoorCloseAreaPortalsThink; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CBaseDoor because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CBaseDoor) == 0x930);
    };
};
