#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseEntity.hpp"
#include "source2sdk/entity2/CEntityIOOutput.hpp"

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
        // Size: 0x698
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CEnvSoundscape : public source2sdk::client::C_BaseEntity
        {
        public:
            source2sdk::entity2::CEntityIOOutput m_OnPlay; // 0x608            
            float m_flRadius; // 0x620            
            uint8_t _pad0624[0x4]; // 0x624
            CUtlSymbolLarge m_soundEventName; // 0x628            
            bool m_bOverrideWithEvent; // 0x630            
            uint8_t _pad0631[0x3]; // 0x631
            // metadata: MNotSaved
            std::int32_t m_soundscapeIndex; // 0x634            
            // metadata: MNotSaved
            std::int32_t m_soundscapeEntityListId; // 0x638            
            uint8_t _pad063c[0x4]; // 0x63c
            CUtlSymbolLarge m_positionNames[8]; // 0x640            
            // m_hProxySoundscape has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::CEnvSoundscape> m_hProxySoundscape;
            char m_hProxySoundscape[0x4]; // 0x680            
            bool m_bDisabled; // 0x684            
            uint8_t _pad0685[0x3]; // 0x685
            CUtlSymbolLarge m_soundscapeName; // 0x688            
            // metadata: MNotSaved
            std::uint32_t m_soundEventHash; // 0x690            
            uint8_t _pad0694[0x4];
            
            // Datamap fields:
            // void InputEnable; // 0x0
            // void InputDisable; // 0x0
            // void InputToggleEnabled; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CEnvSoundscape because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::CEnvSoundscape) == 0x698);
    };
};
