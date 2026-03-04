#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C4LightEffect_t.hpp"
#include "source2sdk/client/C_CSWeaponBase.hpp"
#include "source2sdk/client/EntitySpottedState_t.hpp"
#include "source2sdk/client/ParticleIndex_t.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"

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
        // Size: 0x1f80
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "bool m_bStartedArming"
        // static metadata: MNetworkVarNames "GameTime_t m_fArmedTime"
        // static metadata: MNetworkVarNames "bool m_bBombPlacedAnimation"
        // static metadata: MNetworkVarNames "bool m_bIsPlantingViaUse"
        // static metadata: MNetworkVarNames "EntitySpottedState_t m_entitySpottedState"
        #pragma pack(push, 1)
        class C_C4 : public source2sdk::client::C_CSWeaponBase
        {
        public:
            source2sdk::client::ParticleIndex_t m_activeLightParticleIndex; // 0x1f40            
            source2sdk::client::C4LightEffect_t m_eActiveLightEffect; // 0x1f44            
            // metadata: MNetworkEnable
            bool m_bStartedArming; // 0x1f48            
            uint8_t _pad1f49[0x3]; // 0x1f49
            // metadata: MNetworkEnable
            source2sdk::entity2::GameTime_t m_fArmedTime; // 0x1f4c            
            // metadata: MNetworkEnable
            bool m_bBombPlacedAnimation; // 0x1f50            
            // metadata: MNetworkEnable
            bool m_bIsPlantingViaUse; // 0x1f51            
            uint8_t _pad1f52[0x6]; // 0x1f52
            // metadata: MNetworkEnable
            source2sdk::client::EntitySpottedState_t m_entitySpottedState; // 0x1f58            
            std::int32_t m_nSpotRules; // 0x1f70            
            bool m_bPlayedArmingBeeps[7]; // 0x1f74            
            bool m_bBombPlanted; // 0x1f7b            
            uint8_t _pad1f7c[0x4];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_C4 because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_C4) == 0x1f80);
    };
};
