#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_PointEntity.hpp"
namespace source2sdk
{
    namespace client
    {
        struct CPathWithDynamicNodes;
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
        // Registered alignment: 0x10
        // Alignment: 0x10
        // Standard-layout class: false
        // Size: 0x660
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "Vector m_vInTangentLocal"
        // static metadata: MNetworkVarNames "Vector m_vOutTangentLocal"
        // static metadata: MNetworkVarNames "CUtlString m_strParentPathUniqueID"
        // static metadata: MNetworkVarNames "CUtlString m_strPathNodeParameter"
        #pragma pack(push, 1)
        class CPathNode : public source2sdk::client::C_PointEntity
        {
        public:
            // metadata: MNetworkEnable
            Vector m_vInTangentLocal; // 0x608            
            // metadata: MNetworkEnable
            Vector m_vOutTangentLocal; // 0x614            
            // metadata: MNetworkEnable
            CUtlString m_strParentPathUniqueID; // 0x620            
            // metadata: MNetworkEnable
            CUtlString m_strPathNodeParameter; // 0x628            
            CTransform m_xWSPrevParent; // 0x630            
            // m_hPath has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::CPathWithDynamicNodes> m_hPath;
            char m_hPath[0x4]; // 0x650            
            uint8_t _pad0654[0xc];
            
            // Datamap fields:
            // void CPathNodeParentedMoveThink; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CPathNode because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::CPathNode) == 0x660);
    };
};
