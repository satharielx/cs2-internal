#include "CFunctionList.hpp"
#include "../../sdk/mem.h"
#include "../../sdk/CFunctionList_stub.hpp"
#include <Windows.h>
#include "../../manager/core/debug_console.h"

// Simple singleton
static CFunctionList g_FunctionList;

CFunctionList* GetFunctionList() {
    static bool inited = false;
    if (!inited) {
        g_FunctionList.OnInit();
        inited = true;
    }
    return &g_FunctionList;
}

bool CFunctionList::OnInit() {
    // Resolve patterns for all known functions. Prefer offset-based resolution
    // where available; fall back to pattern scanning using sdk::find_pattern.
    try {
        auto try_resolve = [](CBasePattern& bp, const char* module, const char* pattern) {
            bp.Set(bp.name ? bp.name : "", pattern, module);
            void* match = nullptr;
            try {
                match = sdk::find_pattern(module, pattern);
            } catch (...) { match = nullptr; }
            void* resolved = nullptr;
            if (match) {
                // If the pattern contains a CALL opcode token ("E8"), try to
                // resolve the call target (relative addressing): target = ip + 5 + rel32
                bool has_call_token = (strstr(pattern, "E8") != nullptr);
                uint8_t* p = reinterpret_cast<uint8_t*>(match);
                if (has_call_token) {
                    // search first 64 bytes of the match for 0xE8
                    for (size_t i = 0; i < 64; ++i) {
                        uint8_t op = 0;
                        // safe read: wrap in try/catch not available — assume readable
                        op = *(p + i);
                        if (op == 0xE8) {
                            int32_t rel = *reinterpret_cast<int32_t*>(p + i + 1);
                            uint8_t* target = p + i + 5 + rel;
                            resolved = reinterpret_cast<void*>(target);
                            break;
                        }
                    }
                }
                // If we didn't resolve to a call target, use the match address itself
                if (!resolved) resolved = match;
            }
            bp.SetResolved(resolved);
        };

        // helper macro to reduce verbosity
#define RESOLVE(name, mod, pat) try_resolve(g_FunctionList.name, mod, pat)

        // Map logical module tokens
        const char* CLIENT = "client.dll";
        const char* TIER0 = "tier0.dll";
        const char* PANORAMA = "panorama.dll";

        RESOLVE(CGameEntitySystem_GetBaseEntity, CLIENT, "4C 8D 49 10 81 FA ?? ?? 00 00 77 ?? 8B CA C1 F9 09");
        RESOLVE(CGameEntitySystem_GetLocalPlayerController, CLIENT, "E8 ? ? ? ? 48 8B E8 48 85 C0 74 ? 33 DB 39 1D");
        RESOLVE(CCSInventoryManager_Get, CLIENT, "E8 ? ? ? ? 48 8B D8 E8 ? ? ? ? 8B 70");
        RESOLVE(CCSInventoryManager_EquipItemInLoadout, CLIENT, "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 89 54 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 0F B7 FA");
        RESOLVE(CCSPlayerInventory_GetItemInLoadout, CLIENT, "40 55 48 83 EC ? 49 63 E8");
        RESOLVE(CGCClientSharedObjectCache_CreateBaseTypeCache, CLIENT, "E8 ? ? ? ? 41 8B D5 49 8B CD");
        RESOLVE(CGCClientSharedObjectCache_FindTypeCache, CLIENT, "4C 8B 49 18 44 8B D2 4C 63 41 10 4F 8D 1C C1 49 8B C3");
        RESOLVE(CreateSharedObjectSubclassEconItem, CLIENT, "48 83 EC ? B9 ? ? ? ? E8 ? ? ? ? 48 85 C0 74 ? 48 8D 0D ? ? ? ? C7 40");
        RESOLVE(CEconItemSchema_GetAttributeDefinitionInterface, CLIENT, "E8 ? ? ? ? 48 85 C0 74 ? E8 ? ? ? ? 0F B7 14 3B 48 8B C8 E8 ? ? ? ? 0F B6 48");
        RESOLVE(CEconItem_SetDynamicAttributeValueUint, CLIENT, "E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 49 8B C0 48 8B CA 48 8B D0");
        RESOLVE(IGameEvent_GetName, CLIENT, "8B 41 14 0F BA E0 1E 73 05 48 8D 41 18 C3");
        RESOLVE(IGameEvent_GetInt64, CLIENT, "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 01 41 8B F0");
        RESOLVE(IGameEvent_GetPlayerController, CLIENT, "48 83 EC 38 8B 02 4C 8D 44 24 20");
        RESOLVE(IGameEvent_GetString, CLIENT, "48 83 EC 38 8B 02 48 83 C1 58 89 44 24 20 8B 42 04 89 44 24 24 48 8B 42 08 48 8D 54 24 20 48 89 44 24 28 E8 ? ? ? ? 48 83 C4 38 C3 CC CC CC 33 C9");
        RESOLVE(IGameEvent_SetString, CLIENT, "48 83 EC 38 8B 02 48 83 C1 58 89 44 24 20 41 B1 1A");
        RESOLVE(C_BaseEntity_ComputeHitboxSurroundingBox, CLIENT, "48 89 5C 24 10 48 89 6C 24 18 56 57 41 56 B8 A0");
        RESOLVE(C_EconItemView_GetStaticData, CLIENT, "40 56 48 83 EC ? 48 89 5C 24 ? 48 8B F1 48 8B 1D ? ? ? ? 48 85 DB 75 ? B9 ? ? ? ? 48 89 7C 24 ? E8 ? ? ? ? 33 FF 48 8B D8 48 85 C0 74 ? 48 8D 05 ? ? ? ? 48 89 7B ? B9 ? ? ? ? 48 89 03 E8 ? ? ? ? 48 85 C0 74 ? 48 8B C8 E8 ? ? ? ? 48 8B F8 48 8D 05 ? ? ? ? 48 89 7B ? 48 89 03 EB ? 48 8B DF 48 8B 7C 24 ? 48 89 1D ? ? ? ? 48 8B 4B ? 48 8B 5C 24 ? 48 85 C9 75");
        RESOLVE(CSkeletonInstance_CalcWorldSpaceBones, CLIENT, "48 89 4C 24 ? 55 53 56 57 41 54 41 55 41 56 41 57 B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8D 6C 24 ? 48 8B 81");
        RESOLVE(C_BaseEntity_GetBoneIdByName, CLIENT, "E8 ? ? ? ? 48 8B CF 85 C0 78 ? 44 8B C0");
        RESOLVE(C_EconItemView_GetBasePlayerWeaponVData, CLIENT, "48 81 EC ? ? ? ? 48 85 C9 75 ? 33 C0 48 81 C4 ? ? ? ? C3 48 89 9C 24");
        RESOLVE(KeyValues3_LoadKV3, TIER0, "?LoadKV3@@YA_NPEAVKeyValues3@@PEAVCUtlString@@PEBDAEBUKV3ID_t@@2I@Z");
        RESOLVE(IGamePhysicsQuery_TraceShape, CLIENT, "48 89 5C 24 ? 48 89 4C 24 ? 55 57");
        RESOLVE(CCSGOInput_GetViewAngles, CLIENT, "E8 ? ? ? ? EB ? 48 8B 01 48 8D 54 24 ?");
        RESOLVE(CCSGOInput_SetViewAngles, CLIENT, "85 D2 75 ? 48 63 81");
        RESOLVE(LineGoesThroughSmoke, CLIENT, "48 8B C4 55 53 56 41 54");
        RESOLVE(FindHudElement, CLIENT, "4C 8B DC 53 48 83 EC 50 48 8B 05");
        RESOLVE(C_CSWeaponBaseGun_GetInaccuracy, CLIENT, "48 89 5C 24 10 55 56 57 48 81 EC ? ? ? ? 44 0F 29 84 24 80 00 00 00");
        RESOLVE(C_CSWeaponBaseGun_GetSpread, CLIENT, "48 83 EC ? 48 63 91 ? ? ? ? 48 8B 81 ? ? ? ? 0F 29 74 24 ? 85 D2");
        RESOLVE(SetLocalPlayerReady, CLIENT, "40 53 48 83 EC 20 48 8B DA 48 8D 15 ? ? ? ? 48 8B CB FF");
        RESOLVE(ScreenTransform, CLIENT, "48 89 5C 24 ? 57 48 83 EC ? 48 83 3D ? ? ? ? ? 48 8B DA");
        RESOLVE(CreateSubtickMoveStep, CLIENT, "E8 ? ? ? ? 48 8B D0 49 8D 4E 18 E8 ? ? ? ? 4C 8B C8 4C 8D 43 02 49 8B D1 48 8B CE E8 ? ? ? ? 48 8B D8 48 85 C0 0F 84 ? ? ? ? 48 3B 06 0F 83 ? ? ? ? 0F B7 00 66 C7 45 67 ? ? 66 3B 45 67 74 ? E9 ? ? ? ? 40 80 FF ? 0F 85 ? ? ? ? 41 83 4E 10 ?");
        RESOLVE(ProtobufAddToRepeatedPtrElement, CLIENT, "48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 48 8B FA 48 8B 49 ? 48 85 C9 74 ? 8B 01");
        RESOLVE(KeyValues_GetName, TIER0, "?GetName@KeyValues@@QEBAPEBDXZ");
        RESOLVE(KeyValues_GetFirstSubKey, TIER0, "?GetFirstSubKey@KeyValues@@QEAAPEAV1@XZ");
        RESOLVE(KeyValues_GetFirstTrueSubKey, TIER0, "?GetFirstTrueSubKey@KeyValues@@QEAAPEAV1@XZ");
        RESOLVE(KeyValues_GetNextTrueSubKey, TIER0, "?GetNextTrueSubKey@KeyValues@@QEAAPEAV1@XZ");
        RESOLVE(KeyValues_GetNextKey, TIER0, "?GetNextKey@KeyValues@@QEAAPEAV1@XZ");
        RESOLVE(KeyValues_FindKey, TIER0, "?FindKey@KeyValues@@QEBAPEBV1@PEBD@Z");
        RESOLVE(KeyValues_GetInt, TIER0, "?GetInt@KeyValues@@QEBAHPEBDH@Z");
        RESOLVE(KeyValues_GetFloat, TIER0, "?GetFloat@KeyValues@@QEBAMPEBDM@Z");
        RESOLVE(KeyValues_GetUint64, TIER0, "?GetUint64@KeyValues@@QEBA_KPEBD_K@Z");
        RESOLVE(KeyValues_FindKeyAndParent, TIER0, "?FindKeyAndParent@KeyValues@@QEAAPEAV1@PEBDPEAPEAV1@_N@Z");
        RESOLVE(CKeyValues_Internal_GetString, TIER0, "?Internal_GetString@CKeyValues_Data@@IEAAPEBDPEBDPEAD_K@Z");
        RESOLVE(KeyValues_Constructor, TIER0, "??0KeyValues@@QEAA@PEBDPEAVIKeyValuesSystem@@_N@Z");
        RESOLVE(KeyValues_Destructor, TIER0, "??1KeyValues@@QEAA@XZ");
        RESOLVE(KeyValues_LoadFromFile, TIER0, "?LoadFromFile@KeyValues@@QEAA_NPEAVIFileSystem@@PEBD1P6A_N1PEAX@Z21@Z");
        RESOLVE(KeyValues_Element, TIER0, "?Element@KeyValues@@QEAAPEAV1@H@Z");
        RESOLVE(KeyValues_Count, TIER0, "?Count@KeyValues@@QEBAHXZ");
        RESOLVE(C_EconItemView_GetCustomPaintKitIndex, CLIENT, "48 89 5C 24 ? 57 48 83 EC ? 8B 15 ? ? ? ? 48 8B F9 65 48 8B 04 25 ? ? ? ? B9 ? ? ? ? 48 8B 04 D0 8B 04 01 39 05 ? ? ? ? 0F 8F ? ? ? ? E8 ? ? ? ? 8B 58 ? 39 1D ? ? ? ? 74 ? E8 ? ? ? ? 48 8B 15 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 89 05 ? ? ? ? 89 1D ? ? ? ? EB ? 48 8B 05 ? ? ? ? 48 85 C0 74");
        RESOLVE(C_BaseModelEntity_SetModel, CLIENT, "40 53 48 83 EC ? 48 8B D9 4C 8B C2 48 8B 0D ? ? ? ? 48 8D 54 24 40");
        RESOLVE(CGameSceneNode_SetMeshGroupMask, CLIENT, "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8D 99 ? ? ? ? 48 8B 71");
        RESOLVE(C_CSWeaponBase_UpdateSubclass, CLIENT, "4C 8B DC 53 48 81 EC ? ? ? ? 48 8B 41");
        RESOLVE(C_CSWeaponBase_UpdateSkin, CLIENT, "48 89 5C 24 08 57 48 83 EC 20 8B DA 48 8B F9 E8 ? ? ? ? 48 8D 8F A0 1B 00 00 48 8B D7 E8 ? ? ? ? F6 C3 01 74 0A 33 D2 48 8B CF E8 ? ? ? ? 48 8D 8F 50 1C 00 00 48 8B D7 E8 ? ? ? ? 45 33 C0 BA 02 00 00 00 48 8B CF E8 ? ? ? ? 48 8B CF 48 8B 5C 24 30 48 83 C4 20 5F E9");
        RESOLVE(C_CSWeaponBase_UpdateCompositeMaterial, CLIENT, "E8 ? ? ? ? 48 8D 8B ? ? ? ? 48 89 BC 24");
        RESOLVE(C_BaseEntity_SetBodyGroup, CLIENT, "85 D2 0F 88 5C");
        RESOLVE(GetCUserCmdTick, CLIENT, "48 83 EC ? 4C 8B 0D ? ? ? ? 4C 8B DA");
        RESOLVE(GetCUserCmdArray, CLIENT, "48 89 4C 24 ? 41 56 41 57");
        RESOLVE(GetCUserCmdBySequenceNumber, CLIENT, "40 53 48 83 EC ? 8B DA E8 ? ? ? ? 4C 8B C0");
        RESOLVE(CEconItem_SerializeToProtoBufItem, CLIENT, "40 55 56 48 83 EC ? 48 8B 41 ? 48 8B F2");
        RESOLVE(CUIEngineSource2_RunScript, PANORAMA, "48 89 5C 24 ? 4C 89 4C 24 ? 48 89 54 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24");
        RESOLVE(CTraceFilter_Constructor, CLIENT, "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 0F B6 41 ? 33 FF 24");
        RESOLVE(CCSGO_HudWeaponSelection_ClearHudWeaponIcon, CLIENT, "E8 ? ? ? ? 8B F8 C6 84 24");
        RESOLVE(C_BaseEntity_GetHitBoxSet, CLIENT, "48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 8B DA 48 8B F9 E8 ? ? ? ? 48 8B F0");

        // Log resolved pointers
        auto log_res = [](const char* name, CBasePattern& bp) {
            uintptr_t addr = reinterpret_cast<uintptr_t>(bp.GetFunction());
            debug_console::Console::Get().Pointer(name, addr, addr, addr != 0);
        };

        log_res("C_BaseModelEntity::SetModel", C_BaseModelEntity_SetModel);
        log_res("C_CSWeaponBase::UpdateSubclass", C_CSWeaponBase_UpdateSubclass);
        log_res("C_CSWeaponBase::UpdateCompositeMaterial", C_CSWeaponBase_UpdateCompositeMaterial);

        // cleanup macro
#undef RESOLVE

        return true;
    } catch (...) {
        return false;
    }
}
