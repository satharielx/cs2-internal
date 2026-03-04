#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
namespace source2sdk
{
    namespace client
    {
        struct C_CSPlayerPawn;
    };
};

// /////////////////////////////////////////////////////////////
// Module: client
// Created using source2gen - github.com/neverlosecc/source2gen
// /////////////////////////////////////////////////////////////

namespace source2sdk
{
    namespace client
    {
        // Registered alignment: unknown
        // Alignment: 0x4
        // Standard-layout class: true
        // Size: 0x158
        // Has VTable
        // 
        // static metadata: MNetworkVarNames "int m_nMatchSeed"
        // static metadata: MNetworkVarNames "bool m_bBlockersPresent"
        // static metadata: MNetworkVarNames "bool m_bRoundInProgress"
        // static metadata: MNetworkVarNames "int m_iFirstSecondHalfRound"
        // static metadata: MNetworkVarNames "int m_iBombSite"
        // static metadata: MNetworkVarNames "CHandle< CCSPlayerPawn> m_hBombPlanter"
        #pragma pack(push, 1)
        class C_RetakeGameRules
        {
        public:
            uint8_t _pad0000[0x138]; // 0x0
            // metadata: MNetworkEnable
            std::int32_t m_nMatchSeed; // 0x138            
            // metadata: MNetworkEnable
            bool m_bBlockersPresent; // 0x13c            
            // metadata: MNetworkEnable
            bool m_bRoundInProgress; // 0x13d            
            uint8_t _pad013e[0x2]; // 0x13e
            // metadata: MNetworkEnable
            std::int32_t m_iFirstSecondHalfRound; // 0x140            
            // metadata: MNetworkEnable
            std::int32_t m_iBombSite; // 0x144            
            // metadata: MNetworkEnable
            // m_hBombPlanter has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_CSPlayerPawn> m_hBombPlanter;
            char m_hBombPlanter[0x4]; // 0x148            
            uint8_t _pad014c[0xc];
        };
        #pragma pack(pop)
        
        static_assert(offsetof(source2sdk::client::C_RetakeGameRules, m_nMatchSeed) == 0x138);
        static_assert(offsetof(source2sdk::client::C_RetakeGameRules, m_bBlockersPresent) == 0x13c);
        static_assert(offsetof(source2sdk::client::C_RetakeGameRules, m_bRoundInProgress) == 0x13d);
        static_assert(offsetof(source2sdk::client::C_RetakeGameRules, m_iFirstSecondHalfRound) == 0x140);
        static_assert(offsetof(source2sdk::client::C_RetakeGameRules, m_iBombSite) == 0x144);
        static_assert(offsetof(source2sdk::client::C_RetakeGameRules, m_hBombPlanter) == 0x148);
        
        static_assert(sizeof(source2sdk::client::C_RetakeGameRules) == 0x158);
    };
};
