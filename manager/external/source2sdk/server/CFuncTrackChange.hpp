#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CFuncPlatRot.hpp"
#include "source2sdk/server/TRAIN_CODE.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CFuncTrackTrain;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CPathTrack;
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
        // Size: 0x838
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CFuncTrackChange : public source2sdk::server::CFuncPlatRot
        {
        public:
            // metadata: MClassPtr
            source2sdk::server::CPathTrack* m_trackTop; // 0x7f8            
            // metadata: MClassPtr
            source2sdk::server::CPathTrack* m_trackBottom; // 0x800            
            // metadata: MClassPtr
            source2sdk::server::CFuncTrackTrain* m_train; // 0x808            
            CUtlSymbolLarge m_trackTopName; // 0x810            
            CUtlSymbolLarge m_trackBottomName; // 0x818            
            CUtlSymbolLarge m_trainName; // 0x820            
            source2sdk::server::TRAIN_CODE m_code; // 0x828            
            std::int32_t m_targetState; // 0x82c            
            std::int32_t m_use; // 0x830            
            uint8_t _pad0834[0x4];
            
            // Datamap fields:
            // void CFuncTrackChangeFind; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CFuncTrackChange because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CFuncTrackChange) == 0x838);
    };
};
