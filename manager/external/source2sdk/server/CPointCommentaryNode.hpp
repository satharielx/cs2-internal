#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CBaseAnimGraph.hpp"
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
        // Registered alignment: 0x10
        // Alignment: 0x10
        // Standard-layout class: false
        // Size: 0xb00
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "string_t m_iszCommentaryFile"
        // static metadata: MNetworkVarNames "CHandle< CBaseEntity> m_hViewPosition"
        // static metadata: MNetworkVarNames "bool m_bActive"
        // static metadata: MNetworkVarNames "GameTime_t m_flStartTime"
        // static metadata: MNetworkVarNames "float32 m_flStartTimeInCommentary"
        // static metadata: MNetworkVarNames "string_t m_iszTitle"
        // static metadata: MNetworkVarNames "string_t m_iszSpeakers"
        // static metadata: MNetworkVarNames "int m_iNodeNumber"
        // static metadata: MNetworkVarNames "int m_iNodeNumberMax"
        // static metadata: MNetworkVarNames "bool m_bListenedTo"
        #pragma pack(push, 1)
        class CPointCommentaryNode : public source2sdk::server::CBaseAnimGraph
        {
        public:
            CUtlSymbolLarge m_iszPreCommands; // 0xa20            
            CUtlSymbolLarge m_iszPostCommands; // 0xa28            
            // metadata: MNetworkEnable
            CUtlSymbolLarge m_iszCommentaryFile; // 0xa30            
            CUtlSymbolLarge m_iszViewTarget; // 0xa38            
            // m_hViewTarget has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hViewTarget;
            char m_hViewTarget[0x4]; // 0xa40            
            // m_hViewTargetAngles has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hViewTargetAngles;
            char m_hViewTargetAngles[0x4]; // 0xa44            
            CUtlSymbolLarge m_iszViewPosition; // 0xa48            
            // metadata: MNetworkEnable
            // m_hViewPosition has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hViewPosition;
            char m_hViewPosition[0x4]; // 0xa50            
            // m_hViewPositionMover has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hViewPositionMover;
            char m_hViewPositionMover[0x4]; // 0xa54            
            bool m_bPreventMovement; // 0xa58            
            bool m_bUnderCrosshair; // 0xa59            
            bool m_bUnstoppable; // 0xa5a            
            uint8_t _pad0a5b[0x1]; // 0xa5b
            source2sdk::entity2::GameTime_t m_flFinishedTime; // 0xa5c            
            Vector m_vecFinishOrigin; // 0xa60            
            QAngle m_vecOriginalAngles; // 0xa6c            
            QAngle m_vecFinishAngles; // 0xa78            
            bool m_bPreventChangesWhileMoving; // 0xa84            
            bool m_bDisabled; // 0xa85            
            uint8_t _pad0a86[0x2]; // 0xa86
            VectorWS m_vecTeleportOrigin; // 0xa88            
            source2sdk::entity2::GameTime_t m_flAbortedPlaybackAt; // 0xa94            
            source2sdk::entity2::CEntityIOOutput m_pOnCommentaryStarted; // 0xa98            
            source2sdk::entity2::CEntityIOOutput m_pOnCommentaryStopped; // 0xab0            
            // metadata: MNetworkEnable
            bool m_bActive; // 0xac8            
            uint8_t _pad0ac9[0x3]; // 0xac9
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_flStartTime; // 0xacc            
            // metadata: MNetworkEnable
            float m_flStartTimeInCommentary; // 0xad0            
            uint8_t _pad0ad4[0x4]; // 0xad4
            // metadata: MNetworkEnable
            CUtlSymbolLarge m_iszTitle; // 0xad8            
            // metadata: MNetworkEnable
            CUtlSymbolLarge m_iszSpeakers; // 0xae0            
            // metadata: MNetworkEnable
            std::int32_t m_iNodeNumber; // 0xae8            
            // metadata: MNetworkEnable
            std::int32_t m_iNodeNumberMax; // 0xaec            
            // metadata: MNetworkEnable
            bool m_bListenedTo; // 0xaf0            
            uint8_t _pad0af1[0xf];
            
            // Datamap fields:
            // void InputStartCommentary; // 0x0
            // void InputStartUnstoppableCommentary; // 0x0
            // void InputEnable; // 0x0
            // void InputDisable; // 0x0
            // void CPointCommentaryNodeSpinThink; // 0x0
            // void CPointCommentaryNodeUpdateViewThink; // 0x0
            // void CPointCommentaryNodeUpdateViewPostThink; // 0x0
            // void CPointCommentaryNodeAcculumatePlayTimeThink; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CPointCommentaryNode because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CPointCommentaryNode) == 0xb00);
    };
};
