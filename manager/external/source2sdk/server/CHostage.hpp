#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CHostageExpresserShim.hpp"
#include "source2sdk/server/CountdownTimer.hpp"
#include "source2sdk/server/EntitySpottedState_t.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CBaseEntity;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayerPawn;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CCSPlayerPawnBase;
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
        // Size: 0x2e00
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "EntitySpottedState_t m_entitySpottedState"
        // static metadata: MNetworkIncludeByName "m_iMaxHealth"
        // static metadata: MNetworkIncludeByName "m_iHealth"
        // static metadata: MNetworkIncludeByName "m_lifeState"
        // static metadata: MNetworkIncludeByName "m_fFlags"
        // static metadata: MNetworkIncludeByName "m_vecViewOffset"
        // static metadata: MNetworkVarNames "Vector m_vel"
        // static metadata: MNetworkVarNames "bool m_isRescued"
        // static metadata: MNetworkVarNames "bool m_jumpedThisFrame"
        // static metadata: MNetworkVarNames "int m_nHostageState"
        // static metadata: MNetworkVarNames "CHandle< CBaseEntity> m_leader"
        // static metadata: MNetworkVarNames "CountdownTimer m_reuseTimer"
        // static metadata: MNetworkVarNames "bool m_bHandsHaveBeenCut"
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerPawn> m_hHostageGrabber"
        // static metadata: MNetworkVarNames "GameTime_t m_flRescueStartTime"
        // static metadata: MNetworkVarNames "GameTime_t m_flGrabSuccessTime"
        // static metadata: MNetworkVarNames "GameTime_t m_flDropStartTime"
        #pragma pack(push, 1)
        class CHostage : public source2sdk::server::CHostageExpresserShim
        {
        public:
            uint8_t _pad0b80[0x18]; // 0xb80
            source2sdk::entity2::CEntityIOOutput m_OnHostageBeginGrab; // 0xb98            
            source2sdk::entity2::CEntityIOOutput m_OnFirstPickedUp; // 0xbb0            
            source2sdk::entity2::CEntityIOOutput m_OnDroppedNotRescued; // 0xbc8            
            source2sdk::entity2::CEntityIOOutput m_OnRescued; // 0xbe0            
            // metadata: MNetworkEnable
            source2sdk::server::EntitySpottedState_t m_entitySpottedState; // 0xbf8            
            std::int32_t m_nSpotRules; // 0xc10            
            std::uint32_t m_uiHostageSpawnExclusionGroupMask; // 0xc14            
            std::uint32_t m_nHostageSpawnRandomFactor; // 0xc18            
            bool m_bRemove; // 0xc1c            
            uint8_t _pad0c1d[0x3]; // 0xc1d
            // metadata: MNetworkEnable
            Vector m_vel; // 0xc20            
            // metadata: MNetworkEnable
            bool m_isRescued; // 0xc2c            
            // metadata: MNetworkEnable
            bool m_jumpedThisFrame; // 0xc2d            
            uint8_t _pad0c2e[0x2]; // 0xc2e
            // metadata: MNetworkEnable
            std::int32_t m_nHostageState; // 0xc30            
            // metadata: MNetworkEnable
            // m_leader has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_leader;
            char m_leader[0x4]; // 0xc34            
            // m_lastLeader has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawnBase> m_lastLeader;
            char m_lastLeader[0x4]; // 0xc38            
            uint8_t _pad0c3c[0x4]; // 0xc3c
            // metadata: MNetworkEnable
            source2sdk::server::CountdownTimer m_reuseTimer; // 0xc40            
            bool m_hasBeenUsed; // 0xc58            
            uint8_t _pad0c59[0x3]; // 0xc59
            Vector m_accel; // 0xc5c            
            bool m_isRunning; // 0xc68            
            bool m_isCrouching; // 0xc69            
            uint8_t _pad0c6a[0x6]; // 0xc6a
            source2sdk::server::CountdownTimer m_jumpTimer; // 0xc70            
            bool m_isWaitingForLeader; // 0xc88            
            uint8_t _pad0c89[0x200f]; // 0xc89
            source2sdk::server::CountdownTimer m_repathTimer; // 0x2c98            
            source2sdk::server::CountdownTimer m_inhibitDoorTimer; // 0x2cb0            
            uint8_t _pad2cc8[0x78]; // 0x2cc8
            source2sdk::server::CountdownTimer m_inhibitObstacleAvoidanceTimer; // 0x2d40            
            uint8_t _pad2d58[0x8]; // 0x2d58
            source2sdk::server::CountdownTimer m_wiggleTimer; // 0x2d60            
            uint8_t _pad2d78[0x4]; // 0x2d78
            bool m_isAdjusted; // 0x2d7c            
            // metadata: MNetworkEnable
            bool m_bHandsHaveBeenCut; // 0x2d7d            
            uint8_t _pad2d7e[0x2]; // 0x2d7e
            // metadata: MNetworkEnable
            // m_hHostageGrabber has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_hHostageGrabber;
            char m_hHostageGrabber[0x4]; // 0x2d80            
            source2sdk::entity2::GameTime_t m_fLastGrabTime; // 0x2d84            
            Vector m_vecPositionWhenStartedDroppingToGround; // 0x2d88            
            Vector m_vecGrabbedPos; // 0x2d94            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flRescueStartTime; // 0x2da0            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flGrabSuccessTime; // 0x2da4            
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flDropStartTime; // 0x2da8            
            std::int32_t m_nApproachRewardPayouts; // 0x2dac            
            std::int32_t m_nPickupEventCount; // 0x2db0            
            Vector m_vecSpawnGroundPos; // 0x2db4            
            uint8_t _pad2dc0[0x2c]; // 0x2dc0
            VectorWS m_vecHostageResetPosition; // 0x2dec            
            uint8_t _pad2df8[0x8];
            
            // Datamap fields:
            // void HostageRescueZoneTouch; // 0x0
            // void CHostageHostageUse; // 0x0
            // void CHostageHostageThink; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CHostage because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CHostage) == 0x2e00);
    };
};
