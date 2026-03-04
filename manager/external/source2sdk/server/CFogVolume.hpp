#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CServerOnlyModelEntity.hpp"

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
        // Size: 0x758
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CFogVolume : public source2sdk::server::CServerOnlyModelEntity
        {
        public:
            CUtlSymbolLarge m_fogName; // 0x730            
            CUtlSymbolLarge m_postProcessName; // 0x738            
            CUtlSymbolLarge m_colorCorrectionName; // 0x740            
            uint8_t _pad0748[0x8]; // 0x748
            bool m_bDisabled; // 0x750            
            // metadata: MNotSaved
            bool m_bInFogVolumesList; // 0x751            
            uint8_t _pad0752[0x6];
            
            // Datamap fields:
            // void InputEnable; // 0x0
            // void InputDisable; // 0x0
            // CHandle< CBaseEntity > m_hFogController; // 0x748
            // CHandle< CBaseEntity > m_hColorCorrectionController; // 0x74c
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CFogVolume because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CFogVolume) == 0x758);
    };
};
