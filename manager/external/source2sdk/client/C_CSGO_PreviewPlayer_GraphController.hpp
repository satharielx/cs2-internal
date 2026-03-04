#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CAnimGraphControllerBase.hpp"

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
        // Size: 0x200
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MGetKV3ClassDefaults
        #pragma pack(push, 1)
        class C_CSGO_PreviewPlayer_GraphController : public source2sdk::client::CAnimGraphControllerBase
        {
        public:
            // m_pszCharacterMode has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CAnimGraphParamRef<char*> m_pszCharacterMode;
            char m_pszCharacterMode[0x30]; // 0x90            
            // m_pszTeamPreviewVariant has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CAnimGraphParamRef<char*> m_pszTeamPreviewVariant;
            char m_pszTeamPreviewVariant[0x30]; // 0xc0            
            // m_pszTeamPreviewPosition has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CAnimGraphParamRef<char*> m_pszTeamPreviewPosition;
            char m_pszTeamPreviewPosition[0x30]; // 0xf0            
            // m_pszEndOfMatchCelebration has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CAnimGraphParamRef<char*> m_pszEndOfMatchCelebration;
            char m_pszEndOfMatchCelebration[0x30]; // 0x120            
            // m_nTeamPreviewRandom has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CAnimGraphParamRef<std::int32_t> m_nTeamPreviewRandom;
            char m_nTeamPreviewRandom[0x28]; // 0x150            
            // m_pszWeaponState has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CAnimGraphParamRef<char*> m_pszWeaponState;
            char m_pszWeaponState[0x30]; // 0x178            
            // m_pszWeaponType has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CAnimGraphParamRef<char*> m_pszWeaponType;
            char m_pszWeaponType[0x30]; // 0x1a8            
            // m_bCT has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CAnimGraphParamRef<bool> m_bCT;
            char m_bCT[0x28]; // 0x1d8            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_CSGO_PreviewPlayer_GraphController because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_CSGO_PreviewPlayer_GraphController) == 0x200);
    };
};
