#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CAnimGraphControllerManager.hpp"
#include "source2sdk/server/CBaseModelEntity.hpp"
#include "source2sdk/server/PhysicsRagdollPose_t.hpp"
namespace source2sdk
{
    namespace client
    {
        struct CAnimGraphControllerBase;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct IChoreoServices;
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
        // Size: 0xa20
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
        class CBaseAnimGraph : public source2sdk::server::CBaseModelEntity
        {
        public:
            // metadata: MSaveOpsForField
            source2sdk::client::CAnimGraphControllerManager m_graphControllerManager; // 0x730            
            // metadata: MSaveOpsForField
            source2sdk::client::CAnimGraphControllerBase* m_pMainGraphController; // 0x7e0            
            // metadata: MNetworkEnable
            bool m_bInitiallyPopulateInterpHistory; // 0x7e8            
            uint8_t _pad07e9[0x7]; // 0x7e9
            // metadata: MSaveOpsForField
            source2sdk::client::IChoreoServices* m_pChoreoServices; // 0x7f0            
            // metadata: MNetworkEnable
            bool m_bAnimGraphUpdateEnabled; // 0x7f8            
            uint8_t _pad07f9[0x3]; // 0x7f9
            float m_flMaxSlopeDistance; // 0x7fc            
            // metadata: MNotSaved
            VectorWS m_vLastSlopeCheckPos; // 0x800            
            std::uint32_t m_nAnimGraphUpdateId; // 0x80c            
            // metadata: MNotSaved
            bool m_bAnimationUpdateScheduled; // 0x810            
            uint8_t _pad0811[0x3]; // 0x811
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            Vector m_vecForce; // 0x814            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            std::int32_t m_nForceBone; // 0x820            
            uint8_t _pad0824[0x14]; // 0x824
            // metadata: MNetworkEnable
            source2sdk::server::PhysicsRagdollPose_t m_RagdollPose; // 0x838            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnClientRagdollEnabledChanged"
            bool m_bRagdollEnabled; // 0x860            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "OnClientRagdollChanged"
            // metadata: MNotSaved
            bool m_bRagdollClientSide; // 0x861            
            uint8_t _pad0862[0xe]; // 0x862
            CTransform m_xParentedRagdollRootInEntitySpace; // 0x870            
            uint8_t _pad0890[0x190];
            
            // Datamap fields:
            // void m_pRagdollControl; // 0x830
            // void CBaseAnimGraphChoreoServicesThink; // 0x0
            // float InputSetPlaybackRate; // 0x0
            // CUtlSymbolLarge InputSetBodyGroup; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CBaseAnimGraph because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CBaseAnimGraph) == 0xa20);
    };
};
