#pragma once

#include "source2sdk/source2gen/source2gen.hpp"
#include <cstddef>
#include <cstdint>
#include "source2sdk/client/CBaseAnimGraph.hpp"
#include "source2sdk/client/C_BaseFlex_Emphasized_Phoneme.hpp"
#include "source2sdk/client/SceneEventId_t.hpp"
#include "source2sdk/modellib/AttachmentHandle_t.hpp"

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
        // Size: 0x1350
        // Has VTable
        // Construct allowed
        // 
        // static metadata: MNetworkVarNames "float32 m_flexWeight"
        #pragma pack(push, 1)
        class C_BaseFlex : public source2sdk::client::CBaseAnimGraph
        {
        public:
            uint8_t _pad1168[0x8]; // 0x1168
            // metadata: MNetworkEnable
            // metadata: MNetworkBitCount "12"
            // metadata: MNetworkMinValue "0,000000"
            // metadata: MNetworkMaxValue "1,000000"
            // metadata: MNetworkEncodeFlags "1"
            // m_flexWeight has a template type with potentially unknown template parameters. You can try uncommenting the field below.
            // C_NetworkUtlVectorBase<float> m_flexWeight;
            char m_flexWeight[0x18]; // 0x1170            
            // metadata: MNetworkEnable
            // metadata: MNetworkEncoder "coord"
            // metadata: MNetworkChangeCallback "OnViewTargetChanged"
            VectorWS m_vLookTargetPosition; // 0x1188            
            uint8_t _pad1194[0xdc]; // 0x1194
            // metadata: MNotSaved
            std::int32_t m_nLastFlexUpdateFrameCount; // 0x1270            
            // metadata: MNotSaved
            Vector m_CachedViewTarget; // 0x1274            
            source2sdk::client::SceneEventId_t m_nNextSceneEventId; // 0x1280            
            // metadata: MNotSaved
            source2sdk::modellib::AttachmentHandle_t m_iMouthAttachment; // 0x1284            
            // metadata: MNotSaved
            source2sdk::modellib::AttachmentHandle_t m_iEyeAttachment; // 0x1285            
            // metadata: MNotSaved
            bool m_bResetFlexWeightsOnModelChange; // 0x1286            
            uint8_t _pad1287[0x19]; // 0x1287
            // metadata: MNotSaved
            std::int32_t m_nEyeOcclusionRendererBone; // 0x12a0            
            // metadata: MNotSaved
            matrix3x4_t m_mEyeOcclusionRendererCameraToBoneTransform; // 0x12a4            
            // metadata: MNotSaved
            Vector m_vEyeOcclusionRendererHalfExtent; // 0x12d4            
            uint8_t _pad12e0[0x10]; // 0x12e0
            // metadata: MNotSaved
            source2sdk::client::C_BaseFlex_Emphasized_Phoneme m_PhonemeClasses[3]; // 0x12f0            
            
            // Datamap fields:
            // void m_PhonemeClasses; // 0x12f0
        };
        #pragma pack(pop)
        
        // Cannot assert offsets of fields in C_BaseFlex because it is not a standard-layout class
        
        static_assert(sizeof(source2sdk::client::C_BaseFlex) == 0x1350);
    };
};
