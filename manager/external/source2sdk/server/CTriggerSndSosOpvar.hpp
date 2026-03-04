#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CBaseTrigger.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CBaseEntity;
    };
};

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
        // Size: 0xbf0
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CTriggerSndSosOpvar : public source2sdk::server::CBaseTrigger
        {
        public:
            // metadata: MNotSaved
            // m_hTouchingPlayers has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<CHandle<source2sdk::server::CBaseEntity>> m_hTouchingPlayers;
            char m_hTouchingPlayers[0x18]; // 0x890            
            // metadata: MNotSaved
            Vector m_flPosition; // 0x8a8            
            float m_flCenterSize; // 0x8b4            
            float m_flMinVal; // 0x8b8            
            float m_flMaxVal; // 0x8bc            
            CUtlSymbolLarge m_opvarName; // 0x8c0            
            CUtlSymbolLarge m_stackName; // 0x8c8            
            CUtlSymbolLarge m_operatorName; // 0x8d0            
            bool m_bVolIs2D; // 0x8d8            
            // metadata: MNotSaved
            char m_opvarNameChar[256]; // 0x8d9            
            // metadata: MNotSaved
            char m_stackNameChar[256]; // 0x9d9            
            // metadata: MNotSaved
            char m_operatorNameChar[256]; // 0xad9            
            uint8_t _pad0bd9[0x3]; // 0xbd9
            // metadata: MNotSaved
            Vector m_VecNormPos; // 0xbdc            
            // metadata: MNotSaved
            float m_flNormCenterSize; // 0xbe8            
            uint8_t _pad0bec[0x4];
            
            // Datamap fields:
            // void CTriggerSndSosOpvarSndSosTriggerOpvarWaitOver; // 0x0
            // void m_opvarNameChar; // 0x8d9
            // void m_stackNameChar; // 0x9d9
            // void m_operatorNameChar; // 0xad9
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CTriggerSndSosOpvar because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CTriggerSndSosOpvar) == 0xbf0);
    };
};
