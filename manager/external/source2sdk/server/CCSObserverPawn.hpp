#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CCSPlayerPawnBase.hpp"

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
        // Size: 0xea0
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarTypeOverride "CCSObserver_ObserverServices m_pObserverServices"
        // static metadata: MNetworkIncludeByName "m_pObserverServices"
        // static metadata: MNetworkVarTypeOverride "CCSObserver_MovementServices m_pMovementServices"
        // static metadata: MNetworkIncludeByName "m_pMovementServices"
        // static metadata: MNetworkVarTypeOverride "CCSObserver_CameraServices m_pCameraServices"
        // static metadata: MNetworkIncludeByName "m_pCameraServices"
        // static metadata: MNetworkVarTypeOverride "CCSObserver_UseServices m_pUseServices"
        // static metadata: MNetworkIncludeByName "m_pUseServices"
        #pragma pack(push, 1)
        class CCSObserverPawn : public source2sdk::server::CCSPlayerPawnBase
        {
        public:
            uint8_t _pad0e70[0x30];
            // Datamap fields:
            // CCSObserver_ObserverServices m_pObserverServices; // 0xb88
            // CCSObserver_MovementServices m_pMovementServices; // 0xbb0
            // CCSObserver_UseServices m_pUseServices; // 0xb98
            // CCSObserver_CameraServices m_pCameraServices; // 0xba8
        };
        #pragma pack(pop)
        
        
        static_assert(sizeof(source2sdk::server::CCSObserverPawn) == 0xea0);
    };
};
