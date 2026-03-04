#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/SolidType_t.hpp"
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/entity2/GameTick_t.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CBaseModelEntity.hpp"
#include "source2sdk/server/CFuncMover_FollowConstraint_t.hpp"
#include "source2sdk/server/CFuncMover_FollowEntityDirection_t.hpp"
#include "source2sdk/server/CFuncMover_Move_t.hpp"
#include "source2sdk/server/CFuncMover_OrientationUpdate_t.hpp"
#include "source2sdk/server/CFuncMover_TransitionToPathNodeAction_t.hpp"
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
        struct CMoverPathNode;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CPathMover;
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
        // Size: 0x998
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CFuncMover : public source2sdk::server::CBaseModelEntity
        {
        public:
            CUtlSymbolLarge m_iszPathName; // 0x730            
            // m_hPathMover has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CPathMover> m_hPathMover;
            char m_hPathMover[0x4]; // 0x738            
            // m_hPrevPathMover has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CPathMover> m_hPrevPathMover;
            char m_hPrevPathMover[0x4]; // 0x73c            
            CUtlSymbolLarge m_iszPathNodeStart; // 0x740            
            CUtlSymbolLarge m_iszPathNodeEnd; // 0x748            
            source2sdk::server::CFuncMover_Move_t m_eMoveType; // 0x750            
            bool m_bIsReversing; // 0x754            
            uint8_t _pad0755[0x3]; // 0x755
            float m_flStartSpeed; // 0x758            
            float m_flPathLocation; // 0x75c            
            float m_flT; // 0x760            
            std::int32_t m_nCurrentNodeIndex; // 0x764            
            std::int32_t m_nPreviousNodeIndex; // 0x768            
            source2sdk::client::SolidType_t m_eSolidType; // 0x76c            
            bool m_bIsMoving; // 0x76d            
            uint8_t _pad076e[0x2]; // 0x76e
            float m_flTimeToReachMaxSpeed; // 0x770            
            float m_flDistanceToReachMaxSpeed; // 0x774            
            float m_flTimeToReachZeroSpeed; // 0x778            
            float m_flComputedDistanceToReachMaxSpeed; // 0x77c            
            float m_flComputedDistanceToReachZeroSpeed; // 0x780            
            float m_flStartCurveScale; // 0x784            
            float m_flStopCurveScale; // 0x788            
            float m_flDistanceToReachZeroSpeed; // 0x78c            
            source2sdk::entity2::GameTime_t m_flTimeMovementStart; // 0x790            
            source2sdk::entity2::GameTime_t m_flTimeMovementStop; // 0x794            
            // m_hStopAtNode has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CMoverPathNode> m_hStopAtNode;
            char m_hStopAtNode[0x4]; // 0x798            
            float m_flPathLocationToBeginStop; // 0x79c            
            float m_flPathLocationStart; // 0x7a0            
            float m_flBeginStopT; // 0x7a4            
            CUtlSymbolLarge m_iszStartForwardSound; // 0x7a8            
            CUtlSymbolLarge m_iszLoopForwardSound; // 0x7b0            
            CUtlSymbolLarge m_iszStopForwardSound; // 0x7b8            
            CUtlSymbolLarge m_iszStartReverseSound; // 0x7c0            
            CUtlSymbolLarge m_iszLoopReverseSound; // 0x7c8            
            CUtlSymbolLarge m_iszStopReverseSound; // 0x7d0            
            CUtlSymbolLarge m_iszArriveAtDestinationSound; // 0x7d8            
            uint8_t _pad07e0[0x18]; // 0x7e0
            source2sdk::entity2::CEntityIOOutput m_OnMovementEnd; // 0x7f8            
            bool m_bStartAtClosestPoint; // 0x810            
            bool m_bStartAtEnd; // 0x811            
            bool m_bStartFollowingClosestMover; // 0x812            
            uint8_t _pad0813[0x1]; // 0x813
            source2sdk::server::CFuncMover_OrientationUpdate_t m_eOrientationUpdate; // 0x814            
            source2sdk::entity2::GameTime_t m_flTimeStartOrientationChange; // 0x818            
            float m_flTimeToBlendToNewOrientation; // 0x81c            
            float m_flDurationBlendToNewOrientationRan; // 0x820            
            std::int32_t m_nOriginalOrientationIndex; // 0x824            
            bool m_bCreateMovableNavMesh; // 0x828            
            bool m_bAllowMovableNavMeshDockingOnEntireEntity; // 0x829            
            uint8_t _pad082a[0x6]; // 0x82a
            // m_OnNodePassed has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<CUtlString,char*> m_OnNodePassed;
            char m_OnNodePassed[0x20]; // 0x830            
            CUtlSymbolLarge m_iszOrientationMatchEntityName; // 0x850            
            // m_hOrientationMatchEntity has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hOrientationMatchEntity;
            char m_hOrientationMatchEntity[0x4]; // 0x858            
            float m_flTimeToTraverseToNextNode; // 0x85c            
            Vector m_vLerpToNewPosStartInPathEntitySpace; // 0x860            
            Vector m_vLerpToNewPosEndInPathEntitySpace; // 0x86c            
            float m_flLerpToPositionT; // 0x878            
            float m_flLerpToPositionDeltaT; // 0x87c            
            source2sdk::entity2::CEntityIOOutput m_OnLerpToPositionComplete; // 0x880            
            bool m_bIsPaused; // 0x898            
            uint8_t _pad0899[0x3]; // 0x899
            source2sdk::server::CFuncMover_TransitionToPathNodeAction_t m_eTransitionedToPathNodeAction; // 0x89c            
            std::int32_t m_nDelayedTeleportToNode; // 0x8a0            
            bool m_bIsVerboseLogging; // 0x8a4            
            uint8_t _pad08a5[0x3]; // 0x8a5
            // m_hFollowEntity has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hFollowEntity;
            char m_hFollowEntity[0x4]; // 0x8a8            
            float m_flFollowDistance; // 0x8ac            
            float m_flFollowMinimumSpeed; // 0x8b0            
            float m_flCurFollowEntityT; // 0x8b4            
            float m_flCurFollowSpeed; // 0x8b8            
            uint8_t _pad08bc[0x4]; // 0x8bc
            CUtlSymbolLarge m_strOrientationFaceEntityName; // 0x8c0            
            // m_hOrientationFaceEntity has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hOrientationFaceEntity;
            char m_hOrientationFaceEntity[0x4]; // 0x8c8            
            uint8_t _pad08cc[0x4]; // 0x8cc
            source2sdk::entity2::CEntityIOOutput m_OnStart; // 0x8d0            
            source2sdk::entity2::CEntityIOOutput m_OnStartForward; // 0x8e8            
            source2sdk::entity2::CEntityIOOutput m_OnStartReverse; // 0x900            
            source2sdk::entity2::CEntityIOOutput m_OnStop; // 0x918            
            source2sdk::entity2::CEntityIOOutput m_OnStopped; // 0x930            
            bool m_bNextNodeReturnsCurrent; // 0x948            
            bool m_bStartedMoving; // 0x949            
            uint8_t _pad094a[0x1e]; // 0x94a
            source2sdk::server::CFuncMover_FollowEntityDirection_t m_eFollowEntityDirection; // 0x968            
            // m_hFollowMover has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CFuncMover> m_hFollowMover;
            char m_hFollowMover[0x4]; // 0x96c            
            CUtlSymbolLarge m_iszFollowMoverEntityName; // 0x970            
            float m_flFollowMoverDistance; // 0x978            
            float m_flFollowMoverCalculatedDistance; // 0x97c            
            float m_flFollowMoverSpringStrength; // 0x980            
            bool m_bFollowConstraintsInitialized; // 0x984            
            uint8_t _pad0985[0x3]; // 0x985
            source2sdk::server::CFuncMover_FollowConstraint_t m_eFollowConstraint; // 0x988            
            float m_flFollowMoverSpeed; // 0x98c            
            float m_flFollowMoverVelocity; // 0x990            
            source2sdk::entity2::GameTick_t m_nTickMovementRan; // 0x994            
            
            // Datamap fields:
            // void InputStart; // 0x0
            // void InputStartForward; // 0x0
            // void InputStartReverse; // 0x0
            // void InputStop; // 0x0
            // void InputStopImmediate; // 0x0
            // void InputToggle; // 0x0
            // void InputToggleDirection; // 0x0
            // void InputPause; // 0x0
            // void InputUnpause; // 0x0
            // CUtlSymbolLarge InputTeleportToPathNode; // 0x0
            // CUtlSymbolLarge InputMoveToPathNode; // 0x0
            // CUtlSymbolLarge InputTransitionToPathNode; // 0x0
            // CUtlSymbolLarge InputTransitionToPathNodeStartForward; // 0x0
            // CUtlSymbolLarge InputTransitionToPathNodeStartReverse; // 0x0
            // float InputSetSpeed; // 0x0
            // float InputSetSpeedImmediate; // 0x0
            // float InputSetTimeToReachMaxSpeed; // 0x0
            // float InputSetTimeToReachZeroSpeed; // 0x0
            // float InputSetTimeToBlendToNewOrientation; // 0x0
            // int32_t InputSetOrientationMode; // 0x0
            // CUtlSymbolLarge InputSetFollowEntity; // 0x0
            // CUtlSymbolLarge InputSetFollowEntityForward; // 0x0
            // CUtlSymbolLarge InputSetFollowEntityReverse; // 0x0
            // CUtlSymbolLarge InputSetFaceEntity; // 0x0
            // float InputSetFollowDistance; // 0x0
            // float InputSetFollowMinimumSpeed; // 0x0
            // float InputSetTimeToTraverseToNextNode; // 0x0
            // int32_t InputSetMoveType; // 0x0
            // CUtlSymbolLarge InputSetFollowMoverEntity; // 0x0
            // void InputSetFollowMoverEntityToClosestOnSpline; // 0x0
            // void InputDeleteFollowMoverSpringConstraint; // 0x0
            // void CFuncMoverLerpToNewPosition; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CFuncMover because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CFuncMover) == 0x998);
    };
};
