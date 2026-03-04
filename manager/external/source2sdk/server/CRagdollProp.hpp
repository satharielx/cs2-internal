#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/ragdoll_t.hpp"
#include "source2sdk/entity2/GameTime_t.hpp"
#include "source2sdk/server/CBaseAnimGraph.hpp"
namespace source2sdk
{
    namespace server
    {
        struct CBaseEntity;
    };
};
namespace source2sdk
{
    namespace server
    {
        struct CBasePlayerPawn;
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
        // Size: 0xb70
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "bool m_ragEnabled"
        // static metadata: MNetworkVarNames "Vector m_ragPos"
        // static metadata: MNetworkVarNames "QAngle m_ragAngles"
        // static metadata: MNetworkVarNames "float32 m_flBlendWeight"
        #pragma pack(push, 1)
        class CRagdollProp : public source2sdk::server::CBaseAnimGraph
        {
        public:
            uint8_t _pad0a20[0x10]; // 0xa20
            // metadata: MNotSaved
            source2sdk::client::ragdoll_t m_ragdoll; // 0xa30            
            bool m_bStartDisabled; // 0xa80            
            uint8_t _pad0a81[0x7]; // 0xa81
            // metadata: MNetworkEnable
            // m_ragEnabled has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CNetworkUtlVectorBase<bool> m_ragEnabled;
            char m_ragEnabled[0x18]; // 0xa88            
            // metadata: MNetworkEnable
            // metadata: MNetworkEncoder "coord"
            // m_ragPos has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CNetworkUtlVectorBase<Vector> m_ragPos;
            char m_ragPos[0x18]; // 0xaa0            
            // metadata: MNetworkEnable
            // metadata: MNetworkEncoder "qangle"
            // metadata: MNetworkBitCount "13"
            // m_ragAngles has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CNetworkUtlVectorBase<QAngle> m_ragAngles;
            char m_ragAngles[0x18]; // 0xab8            
            std::uint32_t m_lastUpdateTickCount; // 0xad0            
            bool m_allAsleep; // 0xad4            
            bool m_bFirstCollisionAfterLaunch; // 0xad5            
            uint8_t _pad0ad6[0x2]; // 0xad6
            // m_hDamageEntity has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hDamageEntity;
            char m_hDamageEntity[0x4]; // 0xad8            
            // m_hKiller has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBaseEntity> m_hKiller;
            char m_hKiller[0x4]; // 0xadc            
            // m_hPhysicsAttacker has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::server::CBasePlayerPawn> m_hPhysicsAttacker;
            char m_hPhysicsAttacker[0x4]; // 0xae0            
            source2sdk::entity2::GameTime_t m_flLastPhysicsInfluenceTime; // 0xae4            
            source2sdk::entity2::GameTime_t m_flFadeOutStartTime; // 0xae8            
            float m_flFadeTime; // 0xaec            
            VectorWS m_vecLastOrigin; // 0xaf0            
            source2sdk::entity2::GameTime_t m_flAwakeTime; // 0xafc            
            source2sdk::entity2::GameTime_t m_flLastOriginChangeTime; // 0xb00            
            uint8_t _pad0b04[0x4]; // 0xb04
            CUtlSymbolLarge m_strOriginClassName; // 0xb08            
            CUtlSymbolLarge m_strSourceClassName; // 0xb10            
            bool m_bHasBeenPhysgunned; // 0xb18            
            // metadata: MNotSaved
            bool m_bAllowStretch; // 0xb19            
            uint8_t _pad0b1a[0x2]; // 0xb1a
            // metadata: MNetworkEnable
            // metadata: MNetworkBitCount "8"
            // metadata: MNetworkMinValue "0,000000"
            // metadata: MNetworkMaxValue "1,000000"
            // metadata: MNetworkEncodeFlags "1"
            float m_flBlendWeight; // 0xb1c            
            float m_flDefaultFadeScale; // 0xb20            
            uint8_t _pad0b24[0x4]; // 0xb24
            // metadata: MNotSaved
            // m_ragdollMins has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<Vector> m_ragdollMins;
            char m_ragdollMins[0x18]; // 0xb28            
            // metadata: MNotSaved
            // m_ragdollMaxs has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<Vector> m_ragdollMaxs;
            char m_ragdollMaxs[0x18]; // 0xb40            
            // metadata: MNotSaved
            bool m_bShouldDeleteActivationRecord; // 0xb58            
            uint8_t _pad0b59[0x17];
            
            // Datamap fields:
            // void InputEnableMotion; // 0x0
            // void InputDisableMotion; // 0x0
            // void InputTurnOn; // 0x0
            // void InputTurnOff; // 0x0
            // float InputFadeAndRemove; // 0x0
            // void CRagdollPropSetDebrisThink; // 0x0
            // void CRagdollPropClearFlagsThink; // 0x0
            // void CRagdollPropFadeOutThink; // 0x0
            // void CRagdollPropSettleThink; // 0x0
            // void CRagdollPropAttachedItemsThink; // 0x0
            // void m_ragdoll.list; // 0xa30
            // void m_ragdoll.hierarchyJoints; // 0xa48
            // bool m_ragdoll.allowStretch; // 0xa78
            // float massScale; // 0x7fffffff
            // float buoyancyscale; // 0x7fffffff
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in CRagdollProp because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::server::CRagdollProp) == 0xb70);
    };
};
