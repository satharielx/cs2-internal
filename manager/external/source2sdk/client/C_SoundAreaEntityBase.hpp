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
        // Registered alignment: unknown
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x630
        // Has VTable
        // Is Abstract
        // 
        // static metadata: MNetworkVarNames "bool m_bDisabled"
        // static metadata: MNetworkVarNames "string_t m_iszSoundAreaType"
        // static metadata: MNetworkVarNames "Vector m_vPos"
        #pragma pack(push, 1)
        class C_SoundAreaEntityBase : public source2sdk::client::C_BaseEntity
        {
        public:
            // metadata: MNetworkEnable
            bool m_bDisabled; // 0x608            
            uint8_t _pad0609[0x7]; // 0x609
            // metadata: MNotSaved
            bool m_bWasEnabled; // 0x610            
            uint8_t _pad0611[0x7]; // 0x611
            // metadata: MNetworkEnable
            CUtlSymbolLarge m_iszSoundAreaType; // 0x618            
            // metadata: MNetworkEnable
            Vector m_vPos; // 0x620            
            uint8_t _pad062c[0x4];
            
            // Datamap fields:
            // void m_nGUID; // 0x60c
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_SoundAreaEntityBase because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_SoundAreaEntityBase) == 0x630);
    };
};
