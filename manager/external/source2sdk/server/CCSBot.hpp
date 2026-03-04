#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CBot.hpp"
#include "source2sdk/server/CountdownTimer.hpp"
#include "source2sdk/server/IntervalTimer.hpp"
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
        // Registered alignment: unknown
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x6e18
        // Has VTable
        #pragma pack(push, 1)
        class CCSBot : public source2sdk::server::CBot
        {
        public:
            uint8_t _pad0100[0x8]; // 0x100
            VectorWS m_eyePosition; // 0x108            
            char m_name[64]; // 0x114            
            float m_combatRange; // 0x154            
            bool m_isRogue; // 0x158            
            uint8_t _pad0159[0x7]; // 0x159
            source2sdk::server::CountdownTimer m_rogueTimer; // 0x160            
            uint8_t _pad0178[0x4]; // 0x178
            bool m_diedLastRound; // 0x17c            
            uint8_t _pad017d[0x3]; // 0x17d
            float m_safeTime; // 0x180            
            bool m_wasSafe; // 0x184            
            uint8_t _pad0185[0x7]; // 0x185
            bool m_blindFire; // 0x18c            
            uint8_t _pad018d[0x3]; // 0x18d
            source2sdk::server::CountdownTimer m_surpriseTimer; // 0x190            
            bool m_bAllowActive; // 0x1a8            
            bool m_isFollowing; // 0x1a9            
            uint8_t _pad01aa[0x2]; // 0x1aa
            // m_leader has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_leader;
            char m_leader[0x4]; // 0x1ac            
            float m_followTimestamp; // 0x1b0            
            float m_allowAutoFollowTime; // 0x1b4            
            source2sdk::server::CountdownTimer m_hurryTimer; // 0x1b8            
            source2sdk::server::CountdownTimer m_alertTimer; // 0x1d0            
            source2sdk::server::CountdownTimer m_sneakTimer; // 0x1e8            
            source2sdk::server::CountdownTimer m_panicTimer; // 0x200            
            uint8_t _pad0218[0x380]; // 0x218
            float m_stateTimestamp; // 0x598            
            bool m_isAttacking; // 0x59c            
            bool m_isOpeningDoor; // 0x59d            
            uint8_t _pad059e[0x6]; // 0x59e
            // m_taskEntity has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_taskEntity;
            char m_taskEntity[0x4]; // 0x5a4            
            uint8_t _pad05a8[0xc]; // 0x5a8
            VectorWS m_goalPosition; // 0x5b4            
            // m_goalEntity has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_goalEntity;
            char m_goalEntity[0x4]; // 0x5c0            
            // m_avoid has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_avoid;
            char m_avoid[0x4]; // 0x5c4            
            float m_avoidTimestamp; // 0x5c8            
            bool m_isStopping; // 0x5cc            
            bool m_hasVisitedEnemySpawn; // 0x5cd            
            uint8_t _pad05ce[0x2]; // 0x5ce
            source2sdk::server::IntervalTimer m_stillTimer; // 0x5d0            
            bool m_bEyeAnglesUnderPathFinderControl; // 0x5e0            
            uint8_t _pad05e1[0x58ff]; // 0x5e1
            std::int32_t m_pathIndex; // 0x5ee0            
            source2sdk::entity2::GameTime_t m_areaEnteredTimestamp; // 0x5ee4            
            source2sdk::server::CountdownTimer m_repathTimer; // 0x5ee8            
            source2sdk::server::CountdownTimer m_avoidFriendTimer; // 0x5f00            
            bool m_isFriendInTheWay; // 0x5f18            
            uint8_t _pad5f19[0x7]; // 0x5f19
            source2sdk::server::CountdownTimer m_politeTimer; // 0x5f20            
            bool m_isWaitingBehindFriend; // 0x5f38            
            uint8_t _pad5f39[0x2b]; // 0x5f39
            float m_pathLadderEnd; // 0x5f64            
            uint8_t _pad5f68[0x48]; // 0x5f68
            source2sdk::server::CountdownTimer m_mustRunTimer; // 0x5fb0            
            source2sdk::server::CountdownTimer m_waitTimer; // 0x5fc8            
            source2sdk::server::CountdownTimer m_updateTravelDistanceTimer; // 0x5fe0            
            float m_playerTravelDistance[64]; // 0x5ff8            
            std::uint8_t m_travelDistancePhase; // 0x60f8            
            uint8_t _pad60f9[0x197]; // 0x60f9
            std::uint8_t m_hostageEscortCount; // 0x6290            
            uint8_t _pad6291[0x3]; // 0x6291
            float m_hostageEscortCountTimestamp; // 0x6294            
            std::int32_t m_desiredTeam; // 0x6298            
            bool m_hasJoined; // 0x629c            
            bool m_isWaitingForHostage; // 0x629d            
            uint8_t _pad629e[0x2]; // 0x629e
            source2sdk::server::CountdownTimer m_inhibitWaitingForHostageTimer; // 0x62a0            
            source2sdk::server::CountdownTimer m_waitForHostageTimer; // 0x62b8            
            Vector m_noisePosition; // 0x62d0            
            float m_noiseTravelDistance; // 0x62dc            
            float m_noiseTimestamp; // 0x62e0            
            uint8_t _pad62e4[0x4]; // 0x62e4
            source2sdk::server::CCSPlayerPawn* m_noiseSource; // 0x62e8            
            uint8_t _pad62f0[0x10]; // 0x62f0
            source2sdk::server::CountdownTimer m_noiseBendTimer; // 0x6300            
            Vector m_bentNoisePosition; // 0x6318            
            bool m_bendNoisePositionValid; // 0x6324            
            uint8_t _pad6325[0x3]; // 0x6325
            float m_lookAroundStateTimestamp; // 0x6328            
            float m_lookAheadAngle; // 0x632c            
            float m_forwardAngle; // 0x6330            
            float m_inhibitLookAroundTimestamp; // 0x6334            
            uint8_t _pad6338[0x4]; // 0x6338
            Vector m_lookAtSpot; // 0x633c            
            uint8_t _pad6348[0x4]; // 0x6348
            float m_lookAtSpotDuration; // 0x634c            
            float m_lookAtSpotTimestamp; // 0x6350            
            float m_lookAtSpotAngleTolerance; // 0x6354            
            bool m_lookAtSpotClearIfClose; // 0x6358            
            bool m_lookAtSpotAttack; // 0x6359            
            uint8_t _pad635a[0x6]; // 0x635a
            char* m_lookAtDesc; // 0x6360            
            float m_peripheralTimestamp; // 0x6368            
            uint8_t _pad636c[0x184]; // 0x636c
            std::uint8_t m_approachPointCount; // 0x64f0            
            uint8_t _pad64f1[0x3]; // 0x64f1
            Vector m_approachPointViewPosition; // 0x64f4            
            source2sdk::server::IntervalTimer m_viewSteadyTimer; // 0x6500            
            uint8_t _pad6510[0x8]; // 0x6510
            source2sdk::server::CountdownTimer m_tossGrenadeTimer; // 0x6518            
            uint8_t _pad6530[0x8]; // 0x6530
            source2sdk::server::CountdownTimer m_isAvoidingGrenade; // 0x6538            
            uint8_t _pad6550[0x8]; // 0x6550
            float m_spotCheckTimestamp; // 0x6558            
            uint8_t _pad655c[0x404]; // 0x655c
            std::int32_t m_checkedHidingSpotCount; // 0x6960            
            float m_lookPitch; // 0x6964            
            float m_lookPitchVel; // 0x6968            
            float m_lookYaw; // 0x696c            
            float m_lookYawVel; // 0x6970            
            Vector m_targetSpot; // 0x6974            
            Vector m_targetSpotVelocity; // 0x6980            
            Vector m_targetSpotPredicted; // 0x698c            
            QAngle m_aimError; // 0x6998            
            QAngle m_aimGoal; // 0x69a4            
            source2sdk::entity2::GameTime_t m_targetSpotTime; // 0x69b0            
            float m_aimFocus; // 0x69b4            
            float m_aimFocusInterval; // 0x69b8            
            source2sdk::entity2::GameTime_t m_aimFocusNextUpdate; // 0x69bc            
            uint8_t _pad69c0[0x8]; // 0x69c0
            source2sdk::server::CountdownTimer m_ignoreEnemiesTimer; // 0x69c8            
            // m_enemy has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_enemy;
            char m_enemy[0x4]; // 0x69e0            
            bool m_isEnemyVisible; // 0x69e4            
            std::uint8_t m_visibleEnemyParts; // 0x69e5            
            uint8_t _pad69e6[0x2]; // 0x69e6
            Vector m_lastEnemyPosition; // 0x69e8            
            float m_lastSawEnemyTimestamp; // 0x69f4            
            float m_firstSawEnemyTimestamp; // 0x69f8            
            float m_currentEnemyAcquireTimestamp; // 0x69fc            
            float m_enemyDeathTimestamp; // 0x6a00            
            float m_friendDeathTimestamp; // 0x6a04            
            bool m_isLastEnemyDead; // 0x6a08            
            uint8_t _pad6a09[0x3]; // 0x6a09
            std::int32_t m_nearbyEnemyCount; // 0x6a0c            
            uint8_t _pad6a10[0x208]; // 0x6a10
            // m_bomber has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_bomber;
            char m_bomber[0x4]; // 0x6c18            
            std::int32_t m_nearbyFriendCount; // 0x6c1c            
            // m_closestVisibleFriend has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_closestVisibleFriend;
            char m_closestVisibleFriend[0x4]; // 0x6c20            
            // m_closestVisibleHumanFriend has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_closestVisibleHumanFriend;
            char m_closestVisibleHumanFriend[0x4]; // 0x6c24            
            source2sdk::server::IntervalTimer m_attentionInterval; // 0x6c28            
            // m_attacker has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_attacker;
            char m_attacker[0x4]; // 0x6c38            
            float m_attackedTimestamp; // 0x6c3c            
            source2sdk::server::IntervalTimer m_burnedByFlamesTimer; // 0x6c40            
            std::int32_t m_lastVictimID; // 0x6c50            
            bool m_isAimingAtEnemy; // 0x6c54            
            bool m_isRapidFiring; // 0x6c55            
            uint8_t _pad6c56[0x2]; // 0x6c56
            source2sdk::server::IntervalTimer m_equipTimer; // 0x6c58            
            source2sdk::server::CountdownTimer m_zoomTimer; // 0x6c68            
            source2sdk::entity2::GameTime_t m_fireWeaponTimestamp; // 0x6c80            
            uint8_t _pad6c84[0x4]; // 0x6c84
            source2sdk::server::CountdownTimer m_lookForWeaponsOnGroundTimer; // 0x6c88            
            bool m_bIsSleeping; // 0x6ca0            
            bool m_isEnemySniperVisible; // 0x6ca1            
            uint8_t _pad6ca2[0x6]; // 0x6ca2
            source2sdk::server::CountdownTimer m_sawEnemySniperTimer; // 0x6ca8            
            uint8_t _pad6cc0[0xa0]; // 0x6cc0
            std::uint8_t m_enemyQueueIndex; // 0x6d60            
            std::uint8_t m_enemyQueueCount; // 0x6d61            
            std::uint8_t m_enemyQueueAttendIndex; // 0x6d62            
            bool m_isStuck; // 0x6d63            
            source2sdk::entity2::GameTime_t m_stuckTimestamp; // 0x6d64            
            Vector m_stuckSpot; // 0x6d68            
            uint8_t _pad6d74[0x4]; // 0x6d74
            source2sdk::server::CountdownTimer m_wiggleTimer; // 0x6d78            
            source2sdk::server::CountdownTimer m_stuckJumpTimer; // 0x6d90            
            source2sdk::entity2::GameTime_t m_nextCleanupCheckTimestamp; // 0x6da8            
            float m_avgVel[10]; // 0x6dac            
            std::int32_t m_avgVelIndex; // 0x6dd4            
            std::int32_t m_avgVelCount; // 0x6dd8            
            Vector m_lastOrigin; // 0x6ddc            
            uint8_t _pad6de8[0x4]; // 0x6de8
            float m_lastRadioRecievedTimestamp; // 0x6dec            
            float m_lastRadioSentTimestamp; // 0x6df0            
            // m_radioSubject has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CCSPlayerPawn> m_radioSubject;
            char m_radioSubject[0x4]; // 0x6df4            
            Vector m_radioPosition; // 0x6df8            
            float m_voiceEndTimestamp; // 0x6e04            
            uint8_t _pad6e08[0x8]; // 0x6e08
            std::int32_t m_lastValidReactionQueueFrame; // 0x6e10            
            uint8_t _pad6e14[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSBot because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CCSBot) == 0x6e18);
    };
};
