#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CGameSceneNode.hpp"
#include "source2sdk/server/CModelState.hpp"

// /////////////////////////////////////////////////////////////
// Module: server
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace server
    {
        // Registered alignment: unknown
        // Alignment: unknown
        // Standard-layout class: false
        // Size: 0x430
        // Has VTable
        // Construct disallowed
        // 
        // static metadata: MNetworkVarNames "CModelState m_modelState"
        // static metadata: MNetworkVarNames "bool m_bIsAnimationEnabled"
        // static metadata: MNetworkVarNames "bool m_bUseParentRenderBounds"
        // static metadata: MNetworkVarNames "CUtlStringToken m_materialGroup"
        // static metadata: MNetworkVarNames "uint8 m_nHitboxSet"
        #pragma pack(push, 1)
        class CSkeletonInstance : public source2sdk::server::CGameSceneNode
        {
        public:
            uint8_t _pad0130[0x10]; // 0x130
            // metadata: MNetworkEnable
            source2sdk::server::CModelState m_modelState; // 0x140            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bIsAnimationEnabled; // 0x390            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bUseParentRenderBounds; // 0x391            
            bool m_bDisableSolidCollisionsForHierarchy; // 0x392            
            // start of bitfield block
            // metadata: MNotSaved
            uint8_t m_bDirtyMotionType: 1;
            // metadata: MNotSaved
            uint8_t m_bIsGeneratingLatchedParentSpaceState: 1;
            // end of bitfield block// 2 bits
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "skeletonMaterialGroupChanged"
            CUtlStringToken m_materialGroup; // 0x394            
            // metadata: MNetworkEnable
            std::uint8_t m_nHitboxSet; // 0x398            
            uint8_t _pad0399[0x5b]; // 0x399
            bool m_bForceServerConstraintsEnabled; // 0x3f4            
            uint8_t _pad03f5[0x3b];
            
            // Datamap fields:
            // Vector velocity; // 0x7fffffff
            // Vector rotationAxis; // 0x7fffffff
            // float rotationSpeed; // 0x7fffffff
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CSkeletonInstance because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CSkeletonInstance) == 0x430);
    };
};
