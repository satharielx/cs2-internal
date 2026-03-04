#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CAnimGraphControllerManager.hpp"
#include "source2sdk/client/C_BaseModelEntity.hpp"
#include "source2sdk/client/PhysicsRagdollPose_t.hpp"
namespace source2sdk
{
    namespace client
    {
        struct CAnimGraphControllerBase;
    };
};

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
        // Size: 0x1168
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkIncludeByName "m_bClientSideRagdoll"
        // static metadata: MNetworkVarNames "bool m_bInitiallyPopulateInterpHistory"
        // static metadata: MNetworkVarNames "bool m_bAnimGraphUpdateEnabled"
        // static metadata: MNetworkVarNames "Vector m_vecForce"
        // static metadata: MNetworkVarNames "int32 m_nForceBone"
        // static metadata: MNetworkVarNames "PhysicsRagdollPose_t m_RagdollPose"
        // static metadata: MNetworkVarNames "bool m_bRagdollEnabled"
        // static metadata: MNetworkVarNames "bool m_bRagdollClientSide"
        #pragma pack(push, 1)
        class CBaseAnimGraph : public source2sdk::client::C_BaseModelEntity
        {
        public:
            // metadata: MSaveOpsForField
            source2sdk::client::CAnimGraphControllerManager m_graphControllerManager; // 0xe88            
            // metadata: MSaveOpsForField
            source2sdk::client::CAnimGraphControllerBase* m_pMainGraphController; // 0xf38            
            // metadata: MNetworkEnable
            bool m_bInitiallyPopulateInterpHistory; // 0xf40            
            uint8_t _pad0f41[0x1]; // 0xf41
            bool m_bSuppressAnimEventSounds; // 0xf42            
            uint8_t _pad0f43[0xd]; // 0xf43
            // metadata: MNetworkEnable
            bool m_bAnimGraphUpdateEnabled; // 0xf50            
            uint8_t _pad0f51[0x3]; // 0xf51
            float m_flMaxSlopeDistance; // 0xf54            
            // metadata: MNotSaved
            VectorWS m_vLastSlopeCheckPos; // 0xf58            
            std::uint32_t m_nAnimGraphUpdateId; // 0xf64            
            // metadata: MNotSaved
            bool m_bAnimationUpdateScheduled; // 0xf68            
            uint8_t _pad0f69[0x3]; // 0xf69
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            Vector m_vecForce; // 0xf6c            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            std::int32_t m_nForceBone; // 0xf78            
            uint8_t _pad0f7c[0x4]; // 0xf7c
            // metadata: MNotSaved
            source2sdk::client::CBaseAnimGraph* m_pClientsideRagdoll; // 0xf80            
            // metadata: MNotSaved
            bool m_bBuiltRagdoll; // 0xf88            
            uint8_t _pad0f89[0x17]; // 0xf89
            // metadata: MNetworkEnable
            source2sdk::client::PhysicsRagdollPose_t m_RagdollPose; // 0xfa0            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnClientRagdollEnabledChanged"
            bool m_bRagdollEnabled; // 0xfe8            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnClientRagdollChanged"
            // metadata: MNotSaved
            bool m_bRagdollClientSide; // 0xfe9            
            uint8_t _pad0fea[0xe]; // 0xfea
            // metadata: MNotSaved
            bool m_bHasAnimatedMaterialAttributes; // 0xff8            
            uint8_t _pad0ff9[0x16f];
            
            // Datamap fields:
            // void m_pRagdollControl; // 0xf98
            // float InputSetPlaybackRate; // 0x0
            // CUtlSymbolLarge InputSetBodyGroup; // 0x0
            // bool InputDisableAnimEventSounds; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CBaseAnimGraph because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::CBaseAnimGraph) == 0x1168);
    };
};
