#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_AttributeContainer.hpp"
#include "source2sdk/client/C_BaseFlex.hpp"
#include "source2sdk/client/C_EconEntity_AttachedModelData_t.hpp"
namespace source2sdk
{
    namespace client
    {
        struct CBaseAnimGraph;
    };
};
namespace source2sdk
{
    namespace client
    {
        struct C_BaseEntity;
    };
};

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
        // Size: 0x18c0
        // Has VTable
        // Construct allowed
        // MClassHasEntityLimitedDataDesc
        // 
        // static metadata: MNetworkVarNames "CAttributeContainer m_AttributeManager"
        // static metadata: MNetworkVarNames "uint32 m_OriginalOwnerXuidLow"
        // static metadata: MNetworkVarNames "uint32 m_OriginalOwnerXuidHigh"
        // static metadata: MNetworkVarNames "int m_nFallbackPaintKit"
        // static metadata: MNetworkVarNames "int m_nFallbackSeed"
        // static metadata: MNetworkVarNames "float m_flFallbackWear"
        // static metadata: MNetworkVarNames "int m_nFallbackStatTrak"
        #pragma pack(push, 1)
        class C_EconEntity : public source2sdk::client::C_BaseFlex
        {
        public:
            uint8_t _pad1350[0x10]; // 0x1350
            float m_flFlexDelayTime; // 0x1360            
            uint8_t _pad1364[0x4]; // 0x1364
            float* m_flFlexDelayedWeight; // 0x1368            
            bool m_bAttributesInitialized; // 0x1370            
            uint8_t _pad1371[0x7]; // 0x1371
            // metadata: MNetworkEnable
            source2sdk::client::C_AttributeContainer m_AttributeManager; // 0x1378            
            // metadata: MNetworkEnable
            std::uint32_t m_OriginalOwnerXuidLow; // 0x1848            
            // metadata: MNetworkEnable
            std::uint32_t m_OriginalOwnerXuidHigh; // 0x184c            
            // metadata: MNetworkEnable
            std::int32_t m_nFallbackPaintKit; // 0x1850            
            // metadata: MNetworkEnable
            std::int32_t m_nFallbackSeed; // 0x1854            
            // metadata: MNetworkEnable
            float m_flFallbackWear; // 0x1858            
            // metadata: MNetworkEnable
            std::int32_t m_nFallbackStatTrak; // 0x185c            
            bool m_bClientside; // 0x1860            
            bool m_bParticleSystemsCreated; // 0x1861            
            uint8_t _pad1862[0x6]; // 0x1862
            // m_vecAttachedParticles has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<std::int32_t> m_vecAttachedParticles;
            char m_vecAttachedParticles[0x18]; // 0x1868            
            // m_hViewmodelAttachment has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::CBaseAnimGraph> m_hViewmodelAttachment;
            char m_hViewmodelAttachment[0x4]; // 0x1880            
            std::int32_t m_iOldTeam; // 0x1884            
            bool m_bAttachmentDirty; // 0x1888            
            uint8_t _pad1889[0x3]; // 0x1889
            std::int32_t m_nUnloadedModelIndex; // 0x188c            
            std::int32_t m_iNumOwnerValidationRetries; // 0x1890            
            uint8_t _pad1894[0xc]; // 0x1894
            // m_hOldProvidee has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_BaseEntity> m_hOldProvidee;
            char m_hOldProvidee[0x4]; // 0x18a0            
            uint8_t _pad18a4[0x4]; // 0x18a4
            // m_vecAttachedModels has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CUtlVector<source2sdk::client::C_EconEntity_AttachedModelData_t> m_vecAttachedModels;
            char m_vecAttachedModels[0x18]; // 0x18a8            
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_EconEntity because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_EconEntity) == 0x18c0);
    };
};
