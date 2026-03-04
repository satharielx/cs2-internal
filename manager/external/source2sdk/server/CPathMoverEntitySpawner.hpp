#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CLogicalEntity.hpp"
#include "source2sdk/server/CPathMoverEntitySpawn.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CFuncMover;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CPathMover;
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
        // Size: 0x508
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CPathMoverEntitySpawner : public source2sdk::server::CLogicalEntity
        {
        public:
            CUtlSymbolLarge m_szSpawnTemplates[4]; // 0x4a8            
            std::int32_t m_nSpawnIndex; // 0x4c8            
            // m_hPathMover has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CPathMover> m_hPathMover;
            char m_hPathMover[0x4]; // 0x4cc            
            float m_flSpawnFrequencySeconds; // 0x4d0            
            float m_flSpawnFrequencyDistToNearestMover; // 0x4d4            
            // m_mapSpawnedMoverTemplates has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlHashtable<CHandle<source2sdk::server::CFuncMover>,source2sdk::server::CPathMoverEntitySpawn> m_mapSpawnedMoverTemplates;
            char m_mapSpawnedMoverTemplates[0x20]; // 0x4d8            
            std::int32_t m_nMaxActive; // 0x4f8            
            source2sdk::entity2::GameTime_t m_flLastSpawnTime; // 0x4fc            
            bool m_bEnabled; // 0x500            
            uint8_t _pad0501[0x7];
            
            // Datamap fields:
            // void CPathMoverEntitySpawnerSpawnThink; // 0x0
            // void InputEnable; // 0x0
            // void InputDisable; // 0x0
            // void InputToggle; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CPathMoverEntitySpawner because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CPathMoverEntitySpawner) == 0x508);
    };
};
