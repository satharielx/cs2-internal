#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_CSPlayerPawnBase.hpp"

// /////////////////////////////////////////////////////////////
// Module: client
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace client
    {
        // Registered alignment: 0x8
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x1658
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
        class C_CSObserverPawn : public source2sdk::client::C_CSPlayerPawnBase
        {
        public:
            CEntityHandle m_hDetectParentChange; // 0x1650            
            uint8_t _pad1654[0x4];
            
            // Datamap fields:
            // CCSObserver_ObserverServices m_pObserverServices; // 0x13f0
            // CCSObserver_MovementServices m_pMovementServices; // 0x1418
            // CCSObserver_UseServices m_pUseServices; // 0x1400
            // CCSObserver_CameraServices m_pCameraServices; // 0x1410
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_CSObserverPawn because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_CSObserverPawn) == 0x1658);
    };
};
