#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/Hull_t.hpp"
#include "source2sdk/server/CBaseFlex.hpp"
#include "source2sdk/server/CMovementStatsProperty.hpp"
#include "source2sdk/server/RelationshipOverride_t.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CEconWearable;
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
        // Size: 0xb70
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkExcludeByUserGroup "LocalPlayerExclusive"
        // static metadata: MNetworkVarNames "CHandle< CEconWearable > m_hMyWearables"
        #pragma pack(push, 1)
        class CBaseCombatCharacter : public source2sdk::server::CBaseFlex
        {
        public:
            bool m_bForceServerRagdoll; // 0xab0            
            uint8_t _pad0ab1[0x7]; // 0xab1
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            // m_hMyWearables has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CNetworkUtlVectorBase<CHandle<source2sdk::server::CEconWearable>> m_hMyWearables;
            char m_hMyWearables[0x18]; // 0xab8            
            float m_impactEnergyScale; // 0xad0            
            bool m_bApplyStressDamage; // 0xad4            
            bool m_bDeathEventsDispatched; // 0xad5            
            uint8_t _pad0ad6[0x42]; // 0xad6
            // metadata: MNotSaved
            // m_pVecRelationships has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<source2sdk::server::RelationshipOverride_t>* m_pVecRelationships;
            char m_pVecRelationships[0x8]; // 0xb18            
            CUtlSymbolLarge m_strRelationships; // 0xb20            
            source2sdk::client::Hull_t m_eHull; // 0xb28            
            std::uint32_t m_nNavHullIdx; // 0xb2c            
            source2sdk::server::CMovementStatsProperty m_movementStats; // 0xb30            
            
            // Datamap fields:
            // CUtlSymbolLarge InputSetRelationship; // 0x0
            // void m_hLastValidNavArea; // 0xae8
            // void m_hCurrentNavArea; // 0xad8
            // void m_hCurrentNavAreaBlocked; // 0xaf8
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CBaseCombatCharacter because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CBaseCombatCharacter) == 0xb70);
    };
};
