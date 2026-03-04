#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseTrigger.hpp"
#include "source2sdk/client/TimedEvent.hpp"

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
        // Size: 0xfa8
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class C_Precipitation : public source2sdk::client::C_BaseTrigger
        {
        public:
            // metadata: MNotSaved
            float m_flDensity; // 0xf58            
            uint8_t _pad0f5c[0xc]; // 0xf5c
            // metadata: MNotSaved
            float m_flParticleInnerDist; // 0xf68            
            uint8_t _pad0f6c[0x4]; // 0xf6c
            // metadata: MNotSaved
            char* m_pParticleDef; // 0xf70            
            uint8_t _pad0f78[0x20]; // 0xf78
            // metadata: MNotSaved
            source2sdk::client::TimedEvent m_tParticlePrecipTraceTimer[1]; // 0xf98            
            // metadata: MNotSaved
            bool m_bActiveParticlePrecipEmitter[1]; // 0xfa0            
            // metadata: MNotSaved
            bool m_bParticlePrecipInitialized; // 0xfa1            
            // metadata: MNotSaved
            bool m_bHasSimulatedSinceLastSceneObjectUpdate; // 0xfa2            
            uint8_t _pad0fa3[0x1]; // 0xfa3
            // metadata: MNotSaved
            std::int32_t m_nAvailableSheetSequencesMaxIndex; // 0xfa4            
            
            // Datamap fields:
            // bool m_bActiveParticlePrecipEmitter; // 0xfa0
            // void m_tParticlePrecipTraceTimer; // 0xf98
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_Precipitation because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_Precipitation) == 0xfa8);
    };
};
