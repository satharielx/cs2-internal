#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CCSGameModeRules.hpp"

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
        // Size: 0x48
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "CUtlString m_WeaponSequence"
        #pragma pack(push, 1)
        class CCSGameModeRules_ArmsRace : public source2sdk::client::CCSGameModeRules
        {
        public:
            // metadata: MNetworkEnable
            // m_WeaponSequence has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // C_NetworkUtlVectorBase<CUtlString> m_WeaponSequence;
            char m_WeaponSequence[0x18]; // 0x30            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CCSGameModeRules_ArmsRace because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::CCSGameModeRules_ArmsRace) == 0x48);
    };
};
