#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/entity2/CEntityIOOutput.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CBaseToggle.hpp"
#include "source2sdk/server/locksound_t.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CBaseModelEntity;
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
        // Size: 0x8b0
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "CHandle< CBaseModelEntity> m_glowEntity"
        // static metadata: MNetworkVarNames "bool m_usable"
        // static metadata: MNetworkVarNames "string_t m_szDisplayText"
        #pragma pack(push, 1)
        class CBaseButton : public source2sdk::server::CBaseToggle
        {
        public:
            QAngle m_angMoveEntitySpace; // 0x7b0            
            bool m_fStayPushed; // 0x7bc            
            bool m_fRotating; // 0x7bd            
            uint8_t _pad07be[0x2]; // 0x7be
            // metadata: MNotSaved
            source2sdk::server::locksound_t m_ls; // 0x7c0            
            CUtlSymbolLarge m_sUseSound; // 0x7e0            
            CUtlSymbolLarge m_sLockedSound; // 0x7e8            
            CUtlSymbolLarge m_sUnlockedSound; // 0x7f0            
            CUtlSymbolLarge m_sOverrideAnticipationName; // 0x7f8            
            bool m_bLocked; // 0x800            
            bool m_bDisabled; // 0x801            
            uint8_t _pad0802[0x2]; // 0x802
            source2sdk::entity2::GameTime_t m_flUseLockedTime; // 0x804            
            bool m_bSolidBsp; // 0x808            
            uint8_t _pad0809[0x7]; // 0x809
            source2sdk::entity2::CEntityIOOutput m_OnDamaged; // 0x810            
            source2sdk::entity2::CEntityIOOutput m_OnPressed; // 0x828            
            source2sdk::entity2::CEntityIOOutput m_OnUseLocked; // 0x840            
            source2sdk::entity2::CEntityIOOutput m_OnIn; // 0x858            
            source2sdk::entity2::CEntityIOOutput m_OnOut; // 0x870            
            // metadata: MNotSaved
            std::int32_t m_nState; // 0x888            
            CEntityHandle m_hConstraint; // 0x88c            
            CEntityHandle m_hConstraintParent; // 0x890            
            // metadata: MNotSaved
            bool m_bForceNpcExclude; // 0x894            
            uint8_t _pad0895[0x3]; // 0x895
            CUtlSymbolLarge m_sGlowEntity; // 0x898            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            // m_glowEntity has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseModelEntity> m_glowEntity;
            char m_glowEntity[0x4]; // 0x8a0            
            // metadata: MNetworkEnable
            bool m_usable; // 0x8a4            
            uint8_t _pad08a5[0x3]; // 0x8a5
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            CUtlSymbolLarge m_szDisplayText; // 0x8a8            
            
            // Datamap fields:
            //  m_ls.sLockedSound; // 0x7c8
            //  m_ls.sUnlockedSound; // 0x7d0
            // void CBaseButtonButtonTouch; // 0x0
            // void CBaseButtonButtonSpark; // 0x0
            // void CBaseButtonTriggerAndWait; // 0x0
            // void CBaseButtonButtonReturn; // 0x0
            // void CBaseButtonButtonBackHome; // 0x0
            // void CBaseButtonButtonUse; // 0x0
            // void CBaseButtonActivateTouch; // 0x0
            // void InputLock; // 0x0
            // void InputUnlock; // 0x0
            // void InputPress; // 0x0
            // void InputPressIn; // 0x0
            // void InputPressOut; // 0x0
            // void InputEnable; // 0x0
            // void InputDisable; // 0x0
            // CUtlString displaytext; // 0x7fffffff
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CBaseButton because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CBaseButton) == 0x8b0);
    };
};
