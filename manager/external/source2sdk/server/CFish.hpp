#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CBaseAnimGraph.hpp"
#include "source2sdk/server/CountdownTimer.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CFishPool;
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
        // Registered alignment: 0x10
        // Alignment: 0x10
        // Standard-layout class: false
        // Size: 0xb30
        // Has VTable
        // Construct allowed
        // MNetworkNoBase
        // 
        // static metadata: MNetworkIncludeByName "m_nModelIndex"
        // static metadata: MNetworkIncludeByName "m_lifeState"
        // static metadata: MNetworkVarNames "float32 m_x"
        // static metadata: MNetworkVarNames "float32 m_y"
        // static metadata: MNetworkVarNames "float32 m_z"
        // static metadata: MNetworkVarNames "float32 m_angle"
        // static metadata: MNetworkVarNames "Vector m_poolOrigin"
        // static metadata: MNetworkVarNames "float32 m_waterLevel"
        #pragma pack(push, 1)
        class CFish : public source2sdk::server::CBaseAnimGraph
        {
        public:
            // m_pool has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CFishPool> m_pool;
            char m_pool[0x4]; // 0xa20            
            std::uint32_t m_id; // 0xa24            
            // metadata: MNetworkEnable
            // metadata: MNetworkSerializer "fish_pos_x"
            // metadata: MNotSaved
            float m_x; // 0xa28            
            // metadata: MNetworkEnable
            // metadata: MNetworkSerializer "fish_pos_y"
            // metadata: MNotSaved
            float m_y; // 0xa2c            
            // metadata: MNetworkEnable
            // metadata: MNetworkSerializer "fish_pos_z"
            // metadata: MNotSaved
            float m_z; // 0xa30            
            // metadata: MNetworkEnable
            // metadata: MNetworkSerializer "angle_normalize_positive"
            // metadata: MNetworkBitCount "7"
            // metadata: MNetworkMinValue "0,000000"
            // metadata: MNetworkMaxValue "360,000000"
            // metadata: MNetworkEncodeFlags "1"
            float m_angle; // 0xa34            
            float m_angleChange; // 0xa38            
            Vector m_forward; // 0xa3c            
            Vector m_perp; // 0xa48            
            // metadata: MNetworkEnable
            // metadata: MNetworkEncoder "coord"
            Vector m_poolOrigin; // 0xa54            
            // metadata: MNetworkEnable
            float m_waterLevel; // 0xa60            
            float m_speed; // 0xa64            
            float m_desiredSpeed; // 0xa68            
            float m_calmSpeed; // 0xa6c            
            float m_panicSpeed; // 0xa70            
            float m_avoidRange; // 0xa74            
            // metadata: MNotSaved
            source2sdk::server::CountdownTimer m_turnTimer; // 0xa78            
            bool m_turnClockwise; // 0xa90            
            uint8_t _pad0a91[0x7]; // 0xa91
            // metadata: MNotSaved
            source2sdk::server::CountdownTimer m_goTimer; // 0xa98            
            // metadata: MNotSaved
            source2sdk::server::CountdownTimer m_moveTimer; // 0xab0            
            // metadata: MNotSaved
            source2sdk::server::CountdownTimer m_panicTimer; // 0xac8            
            // metadata: MNotSaved
            source2sdk::server::CountdownTimer m_disperseTimer; // 0xae0            
            // metadata: MNotSaved
            source2sdk::server::CountdownTimer m_proximityTimer; // 0xaf8            
            // metadata: MNotSaved
            // m_visible has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<source2sdk::server::CFish*> m_visible;
            char m_visible[0x18]; // 0xb10            
            uint8_t _pad0b28[0x8];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CFish because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CFish) == 0xb30);
    };
};
