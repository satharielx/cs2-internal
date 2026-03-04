#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CBaseAnimGraph.hpp"
#include "source2sdk/server/magnetted_objects_t.hpp"

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
        // Size: 0xa90
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CPhysMagnet : public source2sdk::server::CBaseAnimGraph
        {
        public:
            source2sdk::entity2::CEntityIOOutput m_OnMagnetAttach; // 0xa20            
            source2sdk::entity2::CEntityIOOutput m_OnMagnetDetach; // 0xa38            
            float m_massScale; // 0xa50            
            float m_forceLimit; // 0xa54            
            float m_torqueLimit; // 0xa58            
            uint8_t _pad0a5c[0x4]; // 0xa5c
            // m_MagnettedEntities has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<source2sdk::server::magnetted_objects_t> m_MagnettedEntities;
            char m_MagnettedEntities[0x18]; // 0xa60            
            bool m_bActive; // 0xa78            
            bool m_bHasHitSomething; // 0xa79            
            uint8_t _pad0a7a[0x2]; // 0xa7a
            float m_flTotalMass; // 0xa7c            
            float m_flRadius; // 0xa80            
            source2sdk::entity2::GameTime_t m_flNextSuckTime; // 0xa84            
            std::int32_t m_iMaxObjectsAttached; // 0xa88            
            uint8_t _pad0a8c[0x4];
            
            // Datamap fields:
            // void InputToggle; // 0x0
            // void InputTurnOn; // 0x0
            // void InputTurnOff; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CPhysMagnet because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CPhysMagnet) == 0xa90);
    };
};
