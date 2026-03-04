#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseEntity.hpp"

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
        // Size: 0x640
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "int m_iActiveIssueIndex"
        // static metadata: MNetworkVarNames "int m_iOnlyTeamToVote"
        // static metadata: MNetworkVarNames "int m_nVoteOptionCount"
        // static metadata: MNetworkVarNames "int m_nPotentialVotes"
        // static metadata: MNetworkVarNames "bool m_bIsYesNoVote"
        #pragma pack(push, 1)
        class C_VoteController : public source2sdk::client::C_BaseEntity
        {
        public:
            uint8_t _pad0608[0x10]; // 0x608
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "RecvProxy_VoteType"
            std::int32_t m_iActiveIssueIndex; // 0x618            
            // metadata: MNetworkEnable
            std::int32_t m_iOnlyTeamToVote; // 0x61c            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "RecvProxy_VoteOption"
            std::int32_t m_nVoteOptionCount[5]; // 0x620            
            // metadata: MNetworkEnable
            std::int32_t m_nPotentialVotes; // 0x634            
            bool m_bVotesDirty; // 0x638            
            bool m_bTypeDirty; // 0x639            
            // metadata: MNetworkEnable
            bool m_bIsYesNoVote; // 0x63a            
            uint8_t _pad063b[0x5];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_VoteController because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_VoteController) == 0x640);
    };
};
