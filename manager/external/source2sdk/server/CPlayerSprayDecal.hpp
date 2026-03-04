#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CModelPointEntity.hpp"

// /////////////////////////////////////////////////////////////
// Module: server
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace server
    {
        // Registered alignment: 0x8
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x808
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "int m_nUniqueID"
        // static metadata: MNetworkVarNames "uint32 m_unAccountID"
        // static metadata: MNetworkVarNames "uint32 m_unTraceID"
        // static metadata: MNetworkVarNames "uint32 m_rtGcTime"
        // static metadata: MNetworkVarNames "Vector m_vecEndPos"
        // static metadata: MNetworkVarNames "Vector m_vecStart"
        // static metadata: MNetworkVarNames "Vector m_vecLeft"
        // static metadata: MNetworkVarNames "Vector m_vecNormal"
        // static metadata: MNetworkVarNames "int m_nPlayer"
        // static metadata: MNetworkVarNames "int m_nEntity"
        // static metadata: MNetworkVarNames "int m_nHitbox"
        // static metadata: MNetworkVarNames "float m_flCreationTime"
        // static metadata: MNetworkVarNames "int m_nTintID"
        // static metadata: MNetworkVarNames "uint8 m_nVersion"
        // static metadata: MNetworkVarNames "uint8 m_ubSignature"
        #pragma pack(push, 1)
        class CPlayerSprayDecal : public source2sdk::server::CModelPointEntity
        {
        public:
            // metadata: MNetworkEnable
            std::int32_t m_nUniqueID; // 0x730            
            // metadata: MNetworkEnable
            std::uint32_t m_unAccountID; // 0x734            
            // metadata: MNetworkEnable
            std::uint32_t m_unTraceID; // 0x738            
            // metadata: MNetworkEnable
            std::uint32_t m_rtGcTime; // 0x73c            
            // metadata: MNetworkEnable
            Vector m_vecEndPos; // 0x740            
            // metadata: MNetworkEnable
            Vector m_vecStart; // 0x74c            
            // metadata: MNetworkEnable
            Vector m_vecLeft; // 0x758            
            // metadata: MNetworkEnable
            Vector m_vecNormal; // 0x764            
            // metadata: MNetworkEnable
            std::int32_t m_nPlayer; // 0x770            
            // metadata: MNetworkEnable
            std::int32_t m_nEntity; // 0x774            
            // metadata: MNetworkEnable
            std::int32_t m_nHitbox; // 0x778            
            // metadata: MNetworkEnable
            float m_flCreationTime; // 0x77c            
            // metadata: MNetworkEnable
            std::int32_t m_nTintID; // 0x780            
            // metadata: MNetworkEnable
            std::uint8_t m_nVersion; // 0x784            
            // metadata: MNetworkEnable
            std::uint8_t m_ubSignature[128]; // 0x785            
            uint8_t _pad0805[0x3];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CPlayerSprayDecal because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CPlayerSprayDecal) == 0x808);
    };
};
