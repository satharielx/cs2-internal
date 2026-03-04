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
        // Registered alignment: 0x8
        // Alignment: 0x8
        // Standard-layout class: false
        // Size: 0x850
        // Has VTable
        // Construct allowed
        // MNetworkNoBase
        // 
        // static metadata: MNetworkIncludeByUserGroup "Origin"
        // static metadata: MNetworkVarNames "float32 m_MinFalloff"
        // static metadata: MNetworkVarNames "float32 m_MaxFalloff"
        // static metadata: MNetworkVarNames "float32 m_flFadeInDuration"
        // static metadata: MNetworkVarNames "float32 m_flFadeOutDuration"
        // static metadata: MNetworkVarNames "float32 m_flMaxWeight"
        // static metadata: MNetworkVarNames "float32 m_flCurWeight"
        // static metadata: MNetworkVarNames "char m_netlookupFilename"
        // static metadata: MNetworkVarNames "bool m_bEnabled"
        // static metadata: MNetworkVarNames "bool m_bMaster"
        // static metadata: MNetworkVarNames "bool m_bClientSide"
        // static metadata: MNetworkVarNames "bool m_bExclusive"
        #pragma pack(push, 1)
        class C_ColorCorrection : public source2sdk::client::C_BaseEntity
        {
        public:
            // metadata: MNotSaved
            Vector m_vecOrigin; // 0x608            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_MinFalloff; // 0x614            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_MaxFalloff; // 0x618            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flFadeInDuration; // 0x61c            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flFadeOutDuration; // 0x620            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flMaxWeight; // 0x624            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flCurWeight; // 0x628            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            char m_netlookupFilename[512]; // 0x62c            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bEnabled; // 0x82c            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bMaster; // 0x82d            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bClientSide; // 0x82e            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bExclusive; // 0x82f            
            // metadata: MNotSaved
            bool m_bEnabledOnClient[1]; // 0x830            
            uint8_t _pad0831[0x3]; // 0x831
            // metadata: MNotSaved
            float m_flCurWeightOnClient[1]; // 0x834            
            // metadata: MNotSaved
            bool m_bFadingIn[1]; // 0x838            
            uint8_t _pad0839[0x3]; // 0x839
            // metadata: MNotSaved
            float m_flFadeStartWeight[1]; // 0x83c            
            // metadata: MNotSaved
            float m_flFadeStartTime[1]; // 0x840            
            // metadata: MNotSaved
            float m_flFadeDuration[1]; // 0x844            
            uint8_t _pad0848[0x8];
            
            // Datamap fields:
            // bool m_bEnabledOnClient; // 0x830
            // float m_flCurWeightOnClient; // 0x834
            // bool m_bFadingIn; // 0x838
            // float m_flFadeStartWeight; // 0x83c
            // float m_flFadeStartTime; // 0x840
            // float m_flFadeDuration; // 0x844
            // void m_netlookupFilename; // 0x62c
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_ColorCorrection because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_ColorCorrection) == 0x850);
    };
};
