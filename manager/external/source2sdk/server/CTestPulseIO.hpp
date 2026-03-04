#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/server/CLogicalEntity.hpp"

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
        // Size: 0x590
        // Has VTable
        // Construct allowed
        #pragma pack(push, 1)
        class CTestPulseIO : public source2sdk::server::CLogicalEntity
        {
        public:
            source2sdk::entity2::CEntityIOOutput m_OnVariantVoid; // 0x4a8            
            // m_OnVariantBool has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<bool,bool> m_OnVariantBool;
            char m_OnVariantBool[0x20]; // 0x4c0            
            // m_OnVariantInt has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<std::int32_t,std::int32_t> m_OnVariantInt;
            char m_OnVariantInt[0x20]; // 0x4e0            
            // m_OnVariantFloat has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<float,float> m_OnVariantFloat;
            char m_OnVariantFloat[0x20]; // 0x500            
            // m_OnVariantString has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<CUtlSymbolLarge,CUtlSymbolLarge> m_OnVariantString;
            char m_OnVariantString[0x20]; // 0x520            
            // m_OnVariantColor has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<Color,Color> m_OnVariantColor;
            char m_OnVariantColor[0x20]; // 0x540            
            // m_OnVariantVector has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CEntityOutputTemplate<Vector,Vector> m_OnVariantVector;
            char m_OnVariantVector[0x28]; // 0x560            
            bool m_bAllowEmptyInputs; // 0x588            
            uint8_t _pad0589[0x7];
            
            // Datamap fields:
            // void InputVariantVoid; // 0x0
            // bool InputVariantBool; // 0x0
            // int32_t InputVariantInt; // 0x0
            // float InputVariantFloat; // 0x0
            // CUtlSymbolLarge InputVariantString; // 0x0
            // Color InputVariantColor; // 0x0
            // Vector InputVariantVector; // 0x0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CTestPulseIO because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CTestPulseIO) == 0x590);
    };
};
