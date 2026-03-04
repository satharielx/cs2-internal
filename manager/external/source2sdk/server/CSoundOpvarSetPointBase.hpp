#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CBaseEntity.hpp"

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
        // Size: 0x550
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "string_t m_iszStackName"
        // static metadata: MNetworkVarNames "string_t m_iszOperatorName"
        // static metadata: MNetworkVarNames "string_t m_iszOpvarName"
        // static metadata: MNetworkVarNames "int m_iOpvarIndex"
        // static metadata: MNetworkVarNames "bool m_bUseAutoCompare"
        // static metadata: MNetworkVarNames "bool m_bFastRefresh"
        #pragma pack(push, 1)
        class CSoundOpvarSetPointBase : public source2sdk::server::CBaseEntity
        {
        public:
            bool m_bDisabled; // 0x4a8            
            uint8_t _pad04a9[0x3]; // 0x4a9
            CEntityHandle m_hSource; // 0x4ac            
            uint8_t _pad04b0[0x18]; // 0x4b0
            CUtlSymbolLarge m_iszSourceEntityName; // 0x4c8            
            uint8_t _pad04d0[0x50]; // 0x4d0
            // metadata: MNotSaved
            Vector m_vLastPosition; // 0x520            
            float m_flRefreshTime; // 0x52c            
            // metadata: MNetworkEnable
            CUtlSymbolLarge m_iszStackName; // 0x530            
            // metadata: MNetworkEnable
            CUtlSymbolLarge m_iszOperatorName; // 0x538            
            // metadata: MNetworkEnable
            CUtlSymbolLarge m_iszOpvarName; // 0x540            
            // metadata: MNetworkEnable
            std::int32_t m_iOpvarIndex; // 0x548            
            // metadata: MNetworkEnable
            bool m_bUseAutoCompare; // 0x54c            
            // metadata: MNetworkEnable
            bool m_bFastRefresh; // 0x54d            
            uint8_t _pad054e[0x2];
            
            // Datamap fields:
            // uint64_t InputSetEventGuid; // 0x0
            // CUtlSymbolLarge InputSetStackName; // 0x0
            // CUtlSymbolLarge InputSetOperatorName; // 0x0
            // CUtlSymbolLarge InputSetOpvarName; // 0x0
            // CUtlSymbolLarge InputSetOpvarIndex; // 0x0
            // CUtlSymbolLarge InputSetSourceEntity; // 0x0
            // void InputEnable; // 0x0
            // void InputDisable; // 0x0
            // void CSoundOpvarSetPointBaseSetOpvarThink; // 0x0
            // void m_nGUID; // 0x4b0
            // void m_hOpvarData; // 0x4d0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CSoundOpvarSetPointBase because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CSoundOpvarSetPointBase) == 0x550);
    };
};
