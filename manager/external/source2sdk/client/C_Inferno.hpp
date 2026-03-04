#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseModelEntity.hpp"
#include "source2sdk/client/ParticleIndex_t.hpp"
#include "source2sdk/resourcesystem/InfoForResourceTypeIParticleSnapshot.hpp"

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
        // Size: 0x8490
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "Vector m_firePositions"
        // static metadata: MNetworkVarNames "Vector m_fireParentPositions"
        // static metadata: MNetworkVarNames "bool m_bFireIsBurning"
        // static metadata: MNetworkVarNames "Vector m_BurnNormal"
        // static metadata: MNetworkVarNames "int m_fireCount"
        // static metadata: MNetworkVarNames "int m_nInfernoType"
        // static metadata: MNetworkVarNames "float m_nFireLifetime"
        // static metadata: MNetworkVarNames "bool m_bInPostEffectTime"
        // static metadata: MNetworkVarNames "int m_nFireEffectTickBegin"
        #pragma pack(push, 1)
        class C_Inferno : public source2sdk::client::C_BaseModelEntity
        {
        public:
            uint8_t _pad0e88[0x40]; // 0xe88
            source2sdk::client::ParticleIndex_t m_nfxFireDamageEffect; // 0xec8            
            uint8_t _pad0ecc[0x4]; // 0xecc
            // m_hInfernoPointsSnapshot has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CStrongHandle<source2sdk::resourcesystem::InfoForResourceTypeIParticleSnapshot> m_hInfernoPointsSnapshot;
            char m_hInfernoPointsSnapshot[0x8]; // 0xed0            
            // m_hInfernoFillerPointsSnapshot has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CStrongHandle<source2sdk::resourcesystem::InfoForResourceTypeIParticleSnapshot> m_hInfernoFillerPointsSnapshot;
            char m_hInfernoFillerPointsSnapshot[0x8]; // 0xed8            
            // m_hInfernoOutlinePointsSnapshot has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CStrongHandle<source2sdk::resourcesystem::InfoForResourceTypeIParticleSnapshot> m_hInfernoOutlinePointsSnapshot;
            char m_hInfernoOutlinePointsSnapshot[0x8]; // 0xee0            
            // m_hInfernoClimbingOutlinePointsSnapshot has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CStrongHandle<source2sdk::resourcesystem::InfoForResourceTypeIParticleSnapshot> m_hInfernoClimbingOutlinePointsSnapshot;
            char m_hInfernoClimbingOutlinePointsSnapshot[0x8]; // 0xee8            
            // m_hInfernoDecalsSnapshot has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CStrongHandle<source2sdk::resourcesystem::InfoForResourceTypeIParticleSnapshot> m_hInfernoDecalsSnapshot;
            char m_hInfernoDecalsSnapshot[0x8]; // 0xef0            
            // metadata: MNetworkEnable
            Vector m_firePositions[64]; // 0xef8            
            // metadata: MNetworkEnable
            Vector m_fireParentPositions[64]; // 0x11f8            
            // metadata: MNetworkEnable
            bool m_bFireIsBurning[64]; // 0x14f8            
            // metadata: MNetworkEnable
            Vector m_BurnNormal[64]; // 0x1538            
            // metadata: MNetworkEnable
            std::int32_t m_fireCount; // 0x1838            
            // metadata: MNetworkEnable
            std::int32_t m_nInfernoType; // 0x183c            
            // metadata: MNetworkEnable
            float m_nFireLifetime; // 0x1840            
            // metadata: MNetworkEnable
            bool m_bInPostEffectTime; // 0x1844            
            uint8_t _pad1845[0x3]; // 0x1845
            std::int32_t m_lastFireCount; // 0x1848            
            // metadata: MNetworkEnable
            std::int32_t m_nFireEffectTickBegin; // 0x184c            
            uint8_t _pad1850[0x6c00]; // 0x1850
            std::int32_t m_drawableCount; // 0x8450            
            bool m_blosCheck; // 0x8454            
            uint8_t _pad8455[0x3]; // 0x8455
            std::int32_t m_nlosperiod; // 0x8458            
            float m_maxFireHalfWidth; // 0x845c            
            float m_maxFireHeight; // 0x8460            
            Vector m_minBounds; // 0x8464            
            Vector m_maxBounds; // 0x8470            
            float m_flLastGrassBurnThink; // 0x847c            
            uint8_t _pad8480[0x10];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_Inferno because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_Inferno) == 0x8490);
    };
};
