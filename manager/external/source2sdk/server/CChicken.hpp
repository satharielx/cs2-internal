#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CAttributeContainer.hpp"
#include "source2sdk/server/CDynamicProp.hpp"
#include "source2sdk/server/ChickenActivity.hpp"
#include "source2sdk/server/CountdownTimer.hpp"
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
        // Size: 0x3190
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "CAttributeContainer m_AttributeManager"
        // static metadata: MNetworkVarNames "bool m_jumpedThisFrame"
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerPawn> m_leader"
        #pragma pack(push, 1)
        class CChicken : public source2sdk::server::CDynamicProp
        {
        public:
            uint8_t _pad0c50[0x20]; // 0xc50
            // metadata: MNetworkEnable
            source2sdk::server::CAttributeContainer m_AttributeManager; // 0xc70            
            source2sdk::server::CountdownTimer m_updateTimer; // 0xf68            
            Vector m_stuckAnchor; // 0xf80            
            uint8_t _pad0f8c[0x4]; // 0xf8c
            source2sdk::server::CountdownTimer m_stuckTimer; // 0xf90            
            source2sdk::server::CountdownTimer m_collisionStuckTimer; // 0xfa8            
            bool m_isOnGround; // 0xfc0            
            uint8_t _pad0fc1[0x3]; // 0xfc1
            Vector m_vFallVelocity; // 0xfc4            
            source2sdk::server::ChickenActivity m_desiredActivity; // 0xfd0            
            source2sdk::server::ChickenActivity m_currentActivity; // 0xfd4            
            source2sdk::server::CountdownTimer m_activityTimer; // 0xfd8            
            float m_turnRate; // 0xff0            
            // m_fleeFrom has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_fleeFrom;
            char m_fleeFrom[0x4]; // 0xff4            
            source2sdk::server::CountdownTimer m_moveRateThrottleTimer; // 0xff8            
            source2sdk::server::CountdownTimer m_startleTimer; // 0x1010            
            source2sdk::server::CountdownTimer m_vocalizeTimer; // 0x1028            
            source2sdk::entity2::GameTime_t m_flWhenZombified; // 0x1040            
            // metadata: MNetworkEnable
            bool m_jumpedThisFrame; // 0x1044            
            uint8_t _pad1045[0x3]; // 0x1045
            // metadata: MNetworkEnable
            // m_leader has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_leader;
            char m_leader[0x4]; // 0x1048            
            uint8_t _pad104c[0x14]; // 0x104c
            source2sdk::server::CountdownTimer m_reuseTimer; // 0x1060            
            bool m_hasBeenUsed; // 0x1078            
            uint8_t _pad1079[0x7]; // 0x1079
            source2sdk::server::CountdownTimer m_jumpTimer; // 0x1080            
            float m_flLastJumpTime; // 0x1098            
            bool m_bInJump; // 0x109c            
            uint8_t _pad109d[0x200b]; // 0x109d
            source2sdk::server::CountdownTimer m_repathTimer; // 0x30a8            
            uint8_t _pad30c0[0x80]; // 0x30c0
            Vector m_vecPathGoal; // 0x3140            
            source2sdk::entity2::GameTime_t m_flActiveFollowStartTime; // 0x314c            
            source2sdk::server::CountdownTimer m_followMinuteTimer; // 0x3150            
            uint8_t _pad3168[0x8]; // 0x3168
            source2sdk::server::CountdownTimer m_BlockDirectionTimer; // 0x3170            
            uint8_t _pad3188[0x8];
            
            // Datamap fields:
            // void CChickenChickenTouch; // 0x0
            // void CChickenChickenThink; // 0x0
            // void CChickenChickenUse; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CChicken because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CChicken) == 0x3190);
    };
};
