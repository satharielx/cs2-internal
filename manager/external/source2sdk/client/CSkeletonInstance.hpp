#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CGameSceneNode.hpp"
#include "source2sdk/client/CModelState.hpp"

// /////////////////////////////////////////////////////////////
// Module: client
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace client
    {
        // Registered alignment: unknown
        // Alignment: unknown
        // Standard-layout class: false
        // Size: 0x4d0
        // Has VTable
        // Construct disallowed
        // 
        // static metadata: MNetworkVarNames "CModelState m_modelState"
        // static metadata: MNetworkVarNames "bool m_bIsAnimationEnabled"
        // static metadata: MNetworkVarNames "bool m_bUseParentRenderBounds"
        // static metadata: MNetworkVarNames "CUtlStringToken m_materialGroup"
        // static metadata: MNetworkVarNames "uint8 m_nHitboxSet"
        #pragma pack(push, 1)
        class CSkeletonInstance : public source2sdk::client::CGameSceneNode
        {
        public:
            uint8_t _pad0150[0x10]; // 0x150
            // metadata: MNetworkEnable
            source2sdk::client::CModelState m_modelState; // 0x160            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bIsAnimationEnabled; // 0x430            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bUseParentRenderBounds; // 0x431            
            bool m_bDisableSolidCollisionsForHierarchy; // 0x432            
            // start of bitfield block
            // metadata: MNotSaved
            uint8_t m_bDirtyMotionType: 1;
            // metadata: MNotSaved
            uint8_t m_bIsGeneratingLatchedParentSpaceState: 1;
            // end of bitfield block// 2 bits
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "skeletonMaterialGroupChanged"
            CUtlStringToken m_materialGroup; // 0x434            
            // metadata: MNetworkEnable
            std::uint8_t m_nHitboxSet; // 0x438            
            uint8_t _pad0439[0x97];
            
            // Datamap fields:
            // Vector velocity; // 0x7fffffff
            // Vector rotationAxis; // 0x7fffffff
            // float rotationSpeed; // 0x7fffffff
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CSkeletonInstance because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::CSkeletonInstance) == 0x4d0);
    };
};
