#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/C_BaseModelEntity.hpp"
#include "source2sdk/client/C_RopeKeyframe_CPhysicsDelegate.hpp"
#include "source2sdk/modellib/AttachmentHandle_t.hpp"
#include "source2sdk/resourcesystem/InfoForResourceTypeIMaterial2.hpp"
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
        // Size: 0x11f8
        // Has VTable
        // Construct allowed
        // MNetworkNoBase
        // 
        // static metadata: MNetworkIncludeByUserGroup "Origin"
        // static metadata: MNetworkIncludeByName "CGameSceneNode::m_hParent"
        // static metadata: MNetworkIncludeByName "CGameSceneNode::m_hierarchyAttachName"
        // static metadata: MNetworkIncludeByName "m_nMinCPULevel"
        // static metadata: MNetworkIncludeByName "m_nMaxCPULevel"
        // static metadata: MNetworkIncludeByName "m_nMinGPULevel"
        // static metadata: MNetworkIncludeByName "m_nMaxGPULevel"
        // static metadata: MNetworkVarNames "float32 m_flScrollSpeed"
        // static metadata: MNetworkVarNames "uint16 m_RopeFlags"
        // static metadata: MNetworkVarNames "HMaterialStrong m_iRopeMaterialModelIndex"
        // static metadata: MNetworkVarNames "uint8 m_nSegments"
        // static metadata: MNetworkVarNames "CHandle< C_BaseEntity> m_hStartPoint"
        // static metadata: MNetworkVarNames "CHandle< C_BaseEntity> m_hEndPoint"
        // static metadata: MNetworkVarNames "AttachmentHandle_t m_iStartAttachment"
        // static metadata: MNetworkVarNames "AttachmentHandle_t m_iEndAttachment"
        // static metadata: MNetworkVarNames "uint8 m_Subdiv"
        // static metadata: MNetworkVarNames "int16 m_RopeLength"
        // static metadata: MNetworkVarNames "int16 m_Slack"
        // static metadata: MNetworkVarNames "float32 m_TextureScale"
        // static metadata: MNetworkVarNames "uint8 m_fLockedPoints"
        // static metadata: MNetworkVarNames "uint8 m_nChangeCount"
        // static metadata: MNetworkVarNames "float32 m_Width"
        // static metadata: MNetworkVarNames "bool m_bConstrainBetweenEndpoints"
        #pragma pack(push, 1)
        class C_RopeKeyframe : public source2sdk::client::C_BaseModelEntity
        {
        public:
            uint8_t _pad0e88[0x8]; // 0xe88
            // metadata: MNotSaved
            // m_LinksTouchingSomething has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CBitVec<10> m_LinksTouchingSomething;
            char m_LinksTouchingSomething[0x4]; // 0xe90            
            // metadata: MNotSaved
            std::int32_t m_nLinksTouchingSomething; // 0xe94            
            // metadata: MNotSaved
            bool m_bApplyWind; // 0xe98            
            uint8_t _pad0e99[0x3]; // 0xe99
            // metadata: MNotSaved
            std::int32_t m_fPrevLockedPoints; // 0xe9c            
            // metadata: MNotSaved
            std::int32_t m_iForcePointMoveCounter; // 0xea0            
            // metadata: MNotSaved
            bool m_bPrevEndPointPos[2]; // 0xea4            
            uint8_t _pad0ea6[0x2]; // 0xea6
            // metadata: MNotSaved
            Vector m_vPrevEndPointPos[2]; // 0xea8            
            // metadata: MNotSaved
            float m_flCurScroll; // 0xec0            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_flScrollSpeed; // 0xec4            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            std::uint16_t m_RopeFlags; // 0xec8            
            uint8_t _pad0eca[0x6]; // 0xeca
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            // m_iRopeMaterialModelIndex has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CStrongHandle<source2sdk::resourcesystem::InfoForResourceTypeIMaterial2> m_iRopeMaterialModelIndex;
            char m_iRopeMaterialModelIndex[0x8]; // 0xed0            
            uint8_t _pad0ed8[0x270]; // 0xed8
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            std::uint8_t m_nSegments; // 0x1148            
            uint8_t _pad1149[0x3]; // 0x1149
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            // m_hStartPoint has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_BaseEntity> m_hStartPoint;
            char m_hStartPoint[0x4]; // 0x114c            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            // m_hEndPoint has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CHandle<source2sdk::client::C_BaseEntity> m_hEndPoint;
            char m_hEndPoint[0x4]; // 0x1150            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            source2sdk::modellib::AttachmentHandle_t m_iStartAttachment; // 0x1154            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            source2sdk::modellib::AttachmentHandle_t m_iEndAttachment; // 0x1155            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            std::uint8_t m_Subdiv; // 0x1156            
            uint8_t _pad1157[0x1]; // 0x1157
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "RecomputeSprings"
            // metadata: MNotSaved
            std::int16_t m_RopeLength; // 0x1158            
            // metadata: MNetworkEnable
            // metadata: MNetworkChangeCallback "RecomputeSprings"
            // metadata: MNotSaved
            std::int16_t m_Slack; // 0x115a            
            // metadata: MNetworkEnable
            // metadata: MNetworkBitCount "10"
            // metadata: MNetworkMinValue "0,100000"
            // metadata: MNetworkMaxValue "10,000000"
            // metadata: MNotSaved
            float m_TextureScale; // 0x115c            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            std::uint8_t m_fLockedPoints; // 0x1160            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            std::uint8_t m_nChangeCount; // 0x1161            
            uint8_t _pad1162[0x2]; // 0x1162
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            float m_Width; // 0x1164            
            // metadata: MNotSaved
            source2sdk::client::C_RopeKeyframe_CPhysicsDelegate m_PhysicsDelegate; // 0x1168            
            // metadata: MNotSaved
            // m_hMaterial has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // CStrongHandle<source2sdk::resourcesystem::InfoForResourceTypeIMaterial2> m_hMaterial;
            char m_hMaterial[0x8]; // 0x1178            
            // metadata: MNotSaved
            std::int32_t m_TextureHeight; // 0x1180            
            // metadata: MNotSaved
            Vector m_vecImpulse; // 0x1184            
            // metadata: MNotSaved
            Vector m_vecPreviousImpulse; // 0x1190            
            // metadata: MNotSaved
            float m_flCurrentGustTimer; // 0x119c            
            // metadata: MNotSaved
            float m_flCurrentGustLifetime; // 0x11a0            
            // metadata: MNotSaved
            float m_flTimeToNextGust; // 0x11a4            
            // metadata: MNotSaved
            Vector m_vWindDir; // 0x11a8            
            // metadata: MNotSaved
            Vector m_vColorMod; // 0x11b4            
            // metadata: MNotSaved
            Vector m_vCachedEndPointAttachmentPos[2]; // 0x11c0            
            // metadata: MNotSaved
            QAngle m_vCachedEndPointAttachmentAngle[2]; // 0x11d8            
            // metadata: MNetworkEnable
            // metadata: MNotSaved
            bool m_bConstrainBetweenEndpoints; // 0x11f0            
            // start of bitfield block
            // metadata: MNotSaved
            uint8_t m_bEndPointAttachmentPositionsDirty: 1;
            // metadata: MNotSaved
            uint8_t m_bEndPointAttachmentAnglesDirty: 1;
            // metadata: MNotSaved
            uint8_t m_bNewDataThisFrame: 1;
            // metadata: MNotSaved
            uint8_t m_bPhysicsInitted: 1;
            // end of bitfield block// 4 bits
            uint8_t _pad11f2[0x6];
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_RopeKeyframe because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_RopeKeyframe) == 0x11f8);
    };
};
