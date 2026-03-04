#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/server/CBaseModelEntity.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CInfoLadderDismount;
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
        // Size: 0x7c0
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "Vector m_vecLadderDir"
        // static metadata: MNetworkVarNames "VectorWS m_vecPlayerMountPositionTop"
        // static metadata: MNetworkVarNames "VectorWS m_vecPlayerMountPositionBottom"
        // static metadata: MNetworkVarNames "float m_flAutoRideSpeed"
        // static metadata: MNetworkVarNames "bool m_bFakeLadder"
        #pragma pack(push, 1)
        class CFuncLadder : public source2sdk::server::CBaseModelEntity
        {
        public:
            // metadata: MNetworkEnable
            // metadata: MNetworkEncoder "coord"
            Vector m_vecLadderDir; // 0x730            
            uint8_t _pad073c[0x4]; // 0x73c
            // metadata: MNotSaved
            // m_Dismounts has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<CHandle<source2sdk::server::CInfoLadderDismount>> m_Dismounts;
            char m_Dismounts[0x18]; // 0x740            
            Vector m_vecLocalTop; // 0x758            
            // metadata: MNetworkEnable
            // metadata: MNetworkEncoder "coord"
            VectorWS m_vecPlayerMountPositionTop; // 0x764            
            // metadata: MNetworkEnable
            // metadata: MNetworkEncoder "coord"
            VectorWS m_vecPlayerMountPositionBottom; // 0x770            
            // metadata: MNetworkEnable
            float m_flAutoRideSpeed; // 0x77c            
            bool m_bDisabled; // 0x780            
            // metadata: MNetworkEnable
            bool m_bFakeLadder; // 0x781            
            bool m_bHasSlack; // 0x782            
            uint8_t _pad0783[0x5]; // 0x783
            CUtlSymbolLarge m_surfacePropName; // 0x788            
            source2sdk::entity2::CEntityIOOutput m_OnPlayerGotOnLadder; // 0x790            
            source2sdk::entity2::CEntityIOOutput m_OnPlayerGotOffLadder; // 0x7a8            
            
            // Datamap fields:
            // void InputEnable; // 0x0
            // void InputDisable; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CFuncLadder because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CFuncLadder) == 0x7c0);
    };
};
