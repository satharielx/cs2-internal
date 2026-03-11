#pragma once

#include <vector>
#include "../../sdk/CFunctionList_stub.hpp"

#define DECLARATE_CS2_FUNCTION_SDK_FASTCALL(Ret,Function,Param,UsingParam,CallParam)\
inline Ret Function Param\
{\
    using Fn = Ret ( __fastcall* ) UsingParam;\
    Fn Original = static_cast<Fn>( GetFunctionList()->##Function##.GetFunction() );\
    return Original##CallParam##;\
}

class CFunctionList final
{
public:
    auto OnInit() -> bool;

public:
    // Only declare members; initialization and resolution are the responsibility
    // of the real implementation (OnInit).
    CBasePattern CGameEntitySystem_GetBaseEntity;
    CBasePattern CGameEntitySystem_GetLocalPlayerController;
    CBasePattern CCSInventoryManager_Get;

    CBasePattern CCSInventoryManager_EquipItemInLoadout;
    CBasePattern CCSPlayerInventory_GetItemInLoadout;

    CBasePattern CGCClientSharedObjectCache_CreateBaseTypeCache;
    CBasePattern CGCClientSharedObjectCache_FindTypeCache;

    CBasePattern CreateSharedObjectSubclassEconItem;

    CBasePattern CEconItemSchema_GetAttributeDefinitionInterface;
    CBasePattern CEconItem_SetDynamicAttributeValueUint;
    CBasePattern IGameEvent_GetName;
    CBasePattern IGameEvent_GetInt64;
    CBasePattern IGameEvent_GetPlayerController;
    CBasePattern IGameEvent_GetString;
    CBasePattern IGameEvent_SetString;
    CBasePattern C_BaseEntity_ComputeHitboxSurroundingBox;
    CBasePattern C_EconItemView_GetStaticData;
    CBasePattern CSkeletonInstance_CalcWorldSpaceBones;
    CBasePattern C_BaseEntity_GetBoneIdByName;
    CBasePattern C_EconItemView_GetBasePlayerWeaponVData;
    CBasePattern KeyValues3_LoadKV3;
    CBasePattern IGamePhysicsQuery_TraceShape;
    CBasePattern CCSGOInput_GetViewAngles;
    CBasePattern CCSGOInput_SetViewAngles;
    CBasePattern LineGoesThroughSmoke;
    CBasePattern FindHudElement;
    CBasePattern C_CSWeaponBaseGun_GetInaccuracy;
    CBasePattern C_CSWeaponBaseGun_GetSpread;
    CBasePattern SetLocalPlayerReady;
    CBasePattern ScreenTransform;
    CBasePattern CreateSubtickMoveStep;
    CBasePattern ProtobufAddToRepeatedPtrElement;
    CBasePattern KeyValues_GetName;
    CBasePattern KeyValues_GetFirstSubKey;
    CBasePattern KeyValues_GetFirstTrueSubKey;
    CBasePattern KeyValues_GetNextTrueSubKey;
    CBasePattern KeyValues_GetNextKey;
    CBasePattern KeyValues_FindKey;
    CBasePattern KeyValues_GetInt;
    CBasePattern KeyValues_GetFloat;
    CBasePattern KeyValues_GetUint64;
    CBasePattern KeyValues_FindKeyAndParent;
    CBasePattern CKeyValues_Internal_GetString;
    CBasePattern KeyValues_Constructor;
    CBasePattern KeyValues_Destructor;
    CBasePattern KeyValues_LoadFromFile;
    CBasePattern KeyValues_Element;
    CBasePattern KeyValues_Count;
    CBasePattern C_EconItemView_GetCustomPaintKitIndex;
    CBasePattern C_BaseModelEntity_SetModel;
    CBasePattern CGameSceneNode_SetMeshGroupMask;
    CBasePattern C_CSWeaponBase_UpdateSubclass;
    CBasePattern C_CSWeaponBase_UpdateSkin;
    CBasePattern C_CSWeaponBase_UpdateCompositeMaterial;
    CBasePattern C_BaseEntity_SetBodyGroup;
    CBasePattern GetCUserCmdTick;
    CBasePattern GetCUserCmdArray;
    CBasePattern GetCUserCmdBySequenceNumber;
    CBasePattern CEconItem_SerializeToProtoBufItem;
    CBasePattern CUIEngineSource2_RunScript;
    CBasePattern CTraceFilter_Constructor;
    CBasePattern CCSGO_HudWeaponSelection_ClearHudWeaponIcon;
    CBasePattern C_BaseEntity_GetHitBoxSet;
};

// Provide a weak default GetFunctionList implementation that returns nullptr.
// Real project should supply a proper definition that returns a populated list.
inline CFunctionList* GetFunctionList() { return nullptr; }
