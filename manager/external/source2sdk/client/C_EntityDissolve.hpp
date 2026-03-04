#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseModelEntity.hpp"
#include "source2sdk/client/EntityDisolveType_t.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"

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
        // Size: 0xed0
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "GameTime_t m_flStartTime"
        // static metadata: MNetworkVarNames "float32 m_flFadeInStart"
        // static metadata: MNetworkVarNames "float32 m_flFadeInLength"
        // static metadata: MNetworkVarNames "float32 m_flFadeOutModelStart"
        // static metadata: MNetworkVarNames "float32 m_flFadeOutModelLength"
        // static metadata: MNetworkVarNames "float32 m_flFadeOutStart"
        // static metadata: MNetworkVarNames "float32 m_flFadeOutLength"
        // static metadata: MNetworkVarNames "EntityDisolveType_t m_nDissolveType"
        // static metadata: MNetworkVarNames "Vector m_vDissolverOrigin"
        // static metadata: MNetworkVarNames "uint32 m_nMagnitude"
        #pragma pack(push, 1)
        class C_EntityDissolve : public source2sdk::client::C_BaseModelEntity
        {
        public:
            uint8_t _pad0e88[0x8]; // 0xe88
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            source2sdk::entity2::GameTime_t m_flStartTime; // 0xe90            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flFadeInStart; // 0xe94            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flFadeInLength; // 0xe98            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flFadeOutModelStart; // 0xe9c            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flFadeOutModelLength; // 0xea0            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flFadeOutStart; // 0xea4            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flFadeOutLength; // 0xea8            
            // metadata: MNotSaved
            source2sdk::entity2::GameTime_t m_flNextSparkTime; // 0xeac            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            source2sdk::client::EntityDisolveType_t m_nDissolveType; // 0xeb0            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            Vector m_vDissolverOrigin; // 0xeb4            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            std::uint32_t m_nMagnitude; // 0xec0            
            // metadata: MNotSaved
            bool m_bCoreExplode; // 0xec4            
            // metadata: MNotSaved
            bool m_bLinkedToServerEnt; // 0xec5            
            uint8_t _pad0ec6[0xa];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_EntityDissolve because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_EntityDissolve) == 0xed0);
    };
};
