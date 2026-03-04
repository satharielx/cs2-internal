#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/server/CFuncBrush.hpp"
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
        // Size: 0x770
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "CUtlString m_targetCamera"
        // static metadata: MNetworkVarNames "int m_nResolutionEnum"
        // static metadata: MNetworkVarNames "bool m_bRenderShadows"
        // static metadata: MNetworkVarNames "bool m_bUseUniqueColorTarget"
        // static metadata: MNetworkVarNames "CUtlString m_brushModelName"
        // static metadata: MNetworkVarNames "EHANDLE m_hTargetCamera"
        // static metadata: MNetworkVarNames "bool m_bEnabled"
        // static metadata: MNetworkVarNames "bool m_bDraw3DSkybox"
        #pragma pack(push, 1)
        class CFuncMonitor : public source2sdk::server::CFuncBrush
        {
        public:
            // metadata: MNetworkEnable
            CUtlString m_targetCamera; // 0x750            
            // metadata: MNetworkEnable
            std::int32_t m_nResolutionEnum; // 0x758            
            // metadata: MNetworkEnable
            bool m_bRenderShadows; // 0x75c            
            // metadata: MNetworkEnable
            bool m_bUseUniqueColorTarget; // 0x75d            
            uint8_t _pad075e[0x2]; // 0x75e
            // metadata: MNetworkEnable
            CUtlString m_brushModelName; // 0x760            
            // metadata: MNetworkEnable
            // m_hTargetCamera has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hTargetCamera;
            char m_hTargetCamera[0x4]; // 0x768            
            // metadata: MNetworkEnable
            bool m_bEnabled; // 0x76c            
            // metadata: MNetworkEnable
            bool m_bDraw3DSkybox; // 0x76d            
            bool m_bStartEnabled; // 0x76e            
            uint8_t _pad076f[0x1];
            
            // Datamap fields:
            // void InputToggle; // 0x0
            // void InputEnable; // 0x0
            // void InputDisable; // 0x0
            // CUtlSymbolLarge InputSetCamera; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CFuncMonitor because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CFuncMonitor) == 0x770);
    };
};
