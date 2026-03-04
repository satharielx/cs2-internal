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
        // Size: 0x6b8
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class C_SoundEventEntity : public source2sdk::client::C_BaseEntity
        {
        public:
            bool m_bStartOnSpawn; // 0x608            
            bool m_bToLocalPlayer; // 0x609            
            bool m_bStopOnNew; // 0x60a            
            bool m_bSaveRestore; // 0x60b            
            bool m_bSavedIsPlaying; // 0x60c            
            uint8_t _pad060d[0x3]; // 0x60d
            float m_flSavedElapsedTime; // 0x610            
            uint8_t _pad0614[0x4]; // 0x614
            CUtlSymbolLarge m_iszSourceEntityName; // 0x618            
            CUtlSymbolLarge m_iszAttachmentName; // 0x620            
            // m_onGUIDChanged has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<std::uint64_t,std::uint64_t> m_onGUIDChanged;
            char m_onGUIDChanged[0x20]; // 0x628            
            source2sdk::entity2::CEntityIOOutput m_onSoundFinished; // 0x648            
            float m_flClientCullRadius; // 0x660            
            uint8_t _pad0664[0x2c]; // 0x664
            CUtlSymbolLarge m_iszSoundName; // 0x690            
            uint8_t _pad0698[0x14]; // 0x698
            CEntityHandle m_hSource; // 0x6ac            
            std::int32_t m_nEntityIndexSelection; // 0x6b0            
            // start of bitfield block
            // metadata: MNotSaved
            uint8_t m_bClientSideOnly: 1;
            // end of bitfield block// 1 bits
            uint8_t _pad06b5[0x3];
            
            // Datamap fields:
            // CUtlSymbolLarge InputSetSoundName; // 0x0
            // CUtlSymbolLarge InputSetSourceEntity; // 0x0
            // CUtlSymbolLarge InputStartSoundOnAllClients; // 0x0
            // int32_t InputStartSoundOnSingleClient; // 0x0
            // bool InputPauseSound; // 0x0
            // bool InputUnPauseSound; // 0x0
            // void InputStopSound; // 0x0
            // void m_nGUID; // 0x698
            // void C_SoundEventEntitySoundFinishedThink; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_SoundEventEntity because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_SoundEventEntity) == 0x6b8);
    };
};
