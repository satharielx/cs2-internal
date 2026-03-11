#include "skins.h"
#include "interfaces.h"
#include "game_state.h"
#include "../sdk/mem.h"
#include <Windows.h>
#include <wininet.h>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include "../external/offsets/offsets.hpp"
#include "../sdk/source2sdk_offsets.h"
#include "debug_console.h"
#include "../../CS2/SDK/CFunctionList.hpp"

enum ScanType {
    NORMAL_SCAN,
    CALL_SCAN
};

enum Module {
    CLIENT,
    TIER0,
    ENGINEDLL
};

//DO NOT CHANGE THESE MACROS MANUALLY UPDATED

// C_BaseModelEntity_SetModel(C_BaseModelEntity* entity, const char* modelSzName);
#define SET_MODEL_SIGNATURE "40 53 48 83 EC ? 48 8B D9 4C 8B C2 48 8B 0D ? ? ? ? 48 8D 54 24 40"
#define SEARCH_TYPE_SET_MODEL NORMAL_SCAN
#define LOCATION_SET_MODEL CLIENT

#define SET_MESH_GROUP_MASK_SIGNATURE "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8D 99 ? ? ? ? 48 8B 71"
#define SEARCH_TYPE_SET_MESH_GROUP_MASK NORMAL_SCAN
#define LOCATION_SET_MESH_GROUP_MASK CLIENT

// C_CSWeaponBase_UpdateSubclass(C_CSWeaponBase* weapon);
#define UPDATE_SUBCLASS_SIGNATURE "4C 8B DC 53 48 81 EC ? ? ? ? 48 8B 41"
#define SEARCH_TYPE_UPDATE_SUBCLASS NORMAL_SCAN
#define LOCATION_UPDATE_SUBCLASS CLIENT

// C_CSWeaponBase_UpdateCompositeMaterial(C_CSWeaponBase* weapon);
#define UPDATE_COMPOSITE_MATERIAL_SIGNATURE "E8 ? ? ? ? 48 8D 8B ? ? ? ? 48 89 BC 24"
#define SEARCH_TYPE_UPDATE_COMPOSITE_MATERIAL CALL_SCAN
#define LOCATION_UPDATE_COMPOSITE_MATERIAL CLIENT


// C_CSWeaponBase_UpdateSkin(C_CSWeaponBase* weapon);
#define UPDATE_SKIN_SIGNATURE "48 89 5C 24 08 57 48 83 EC 20 8B DA 48 8B F9 E8 ? ? ? ? 48 8D 8F A0 1B 00 00 48 8B D7 E8 ? ? ? ? F6 C3 01 74 0A 33 D2 48 8B CF E8 ? ? ? ? 48 8D 8F 50 1C 00 00 48 8B D7 E8 ? ? ? ? 45 33 C0 BA 02 00 00 00 48 8B CF E8 ? ? ? ? 48 8B CF 48 8B 5C 24 30 48 83 C4 20 5F E9"
#define SEARCH_TYPE_UPDATE_SKIN NORMAL_SCAN
#define LOCATION_UPDATE_SKIN CLIENT

// CCSGO_HudWeaponSelection_ClearHudWeaponIcon(CHudWeaponSelection* thisptr);
#define CLEAR_HUD_WEAPON_ICON_SIGNATURE "E8 ? ? ? ? 8B F8 C6 84 24"
#define SEARCH_TYPE_CLEAR_HUD_WEAPON_ICON CALL_SCAN
#define LOCATION_CLEAR_HUD_WEAPON_ICON CLIENT

// C_EconItemView_GetCustomPaintKitIndex(C_EconItemView* thisptr);
#define GET_CUSTOM_PAINTKIT_INDEX "48 89 5C 24 ? 57 48 83 EC ? 8B 15 ? ? ? ? 48 8B F9 65 48 8B 04 25 ? ? ? ? B9 ? ? ? ? 48 8B 04 D0 8B 04 01 39 05 ? ? ? ? 0F 8F ? ? ? ? E8 ? ? ? ? 8B 58 ? 39 1D ? ? ? ? 74 ? E8 ? ? ? ? 48 8B 15 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 89 05 ? ? ? ? 89 1D ? ? ? ? EB ? 48 8B 05 ? ? ? ? 48 85 C0 74"
#define SEARCH_TYPE_GET_CUSTOM_PAINTKIT_INDEX NORMAL_SCAN
#define LOCATION_GET_CUSTOM_PAINTKIT_INDEX CLIENT

#define EQUIP_ITEM_IN_LOADOUT_SIGNATURE "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 89 54 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 0F B7 FA"
#define SEARCH_TYPE_EQUIP_ITEM_IN_LOADOUT NORMAL_SCAN
#define LOCATION_EQUIP_ITEM_IN_LOADOUT CLIENT

#pragma comment(lib, "wininet.lib")

namespace skins {

    // ==================== SAFE POINTER CHECK ====================
    // VirtualQuery-based, no try/catch — safe to call near SEH blocks.
    static bool IsReadablePtr(uintptr_t ptr) {
        if (ptr == 0 || ptr < 0x10000 || ptr == 0xFFFFFFFFFFFFFFFFull) return false;
        MEMORY_BASIC_INFORMATION mbi{};
        if (!VirtualQuery(reinterpret_cast<LPCVOID>(ptr), &mbi, sizeof(mbi))) return false;
        if (mbi.State != MEM_COMMIT) return false;
        const DWORD bad = PAGE_NOACCESS | PAGE_GUARD;
        return (mbi.Protect & bad) == 0;
    }

    // ==================== OFFSET CHAIN ====================
    static constexpr std::ptrdiff_t ECON_ITEM_VIEW_BASE =
        cs2_dumper::schemas::client_dll::C_EconEntity::m_AttributeManager +
        cs2_dumper::schemas::client_dll::C_AttributeContainer::m_Item;

    static constexpr std::ptrdiff_t OFF_ITEM_DEF_INDEX = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex;
    static constexpr std::ptrdiff_t OFF_ENTITY_QUALITY = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iEntityQuality;
    static constexpr std::ptrdiff_t OFF_ITEM_ID_HIGH = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh;
    static constexpr std::ptrdiff_t OFF_ITEM_ID_LOW = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDLow;
    static constexpr std::ptrdiff_t OFF_ITEM_ID = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemID;
    static constexpr std::ptrdiff_t OFF_ACCOUNT_ID = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iAccountID;
    static constexpr std::ptrdiff_t OFF_DISALLOW_SOC = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_bDisallowSOC;
    static constexpr std::ptrdiff_t OFF_RESTORE_MATERIAL = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_bRestoreCustomMaterialAfterPrecache;
    static constexpr std::ptrdiff_t OFF_CUSTOM_NAME = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_szCustomName;

    static constexpr std::ptrdiff_t OFF_FALLBACK_PAINT = cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackPaintKit;
    static constexpr std::ptrdiff_t OFF_FALLBACK_SEED = cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackSeed;
    static constexpr std::ptrdiff_t OFF_FALLBACK_WEAR = cs2_dumper::schemas::client_dll::C_EconEntity::m_flFallbackWear;
    static constexpr std::ptrdiff_t OFF_FALLBACK_STATTRAK = cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackStatTrak;
    static constexpr std::ptrdiff_t OFF_OWNER_XUID_LOW = cs2_dumper::schemas::client_dll::C_EconEntity::m_OriginalOwnerXuidLow;

    static constexpr std::ptrdiff_t OFF_GAME_SCENE_NODE = cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode;
    static constexpr std::ptrdiff_t OFF_SUBCLASS_ID = cs2_dumper::schemas::client_dll::C_BaseEntity::m_nSubclassID;
    static constexpr std::ptrdiff_t OFF_OWNER_ENTITY = cs2_dumper::schemas::client_dll::C_BaseEntity::m_hOwnerEntity;
    static constexpr std::ptrdiff_t OFF_TEAM_NUM = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum;

    static constexpr std::ptrdiff_t OFF_MODEL_STATE = cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState;
    static constexpr std::ptrdiff_t OFF_MESH_GROUP_MASK = cs2_dumper::schemas::client_dll::CModelState::m_MeshGroupMask;

    static constexpr std::ptrdiff_t OFF_INV_SERVICES =
        cs2_dumper::schemas::client_dll::CCSPlayerController::m_pInventoryServices;
    static constexpr std::ptrdiff_t OFF_LOADOUT_VEC =
        cs2_dumper::schemas::client_dll::CCSPlayerController_InventoryServices::m_vecNetworkableLoadout;

    // ==================== KNIFE DATA ====================
    struct KnifeData { int weapon_id; unsigned long subclass_hash; const char* model_path; };

    static const KnifeData g_knife_data[] = {
        { WEAPON_KNIFE_BAYONET,         3933374535ul, "weapons/models/knife/knife_bayonet/weapon_knife_bayonet.vmdl" },
        { WEAPON_KNIFE_CLASSIC,         3787235507ul, "weapons/models/knife/knife_css/weapon_knife_css.vmdl" },
        { WEAPON_KNIFE_FLIP,            4046390180ul, "weapons/models/knife/knife_flip/weapon_knife_flip.vmdl" },
        { WEAPON_KNIFE_GUT,             2047704618ul, "weapons/models/knife/knife_gut/weapon_knife_gut.vmdl" },
        { WEAPON_KNIFE_KARAMBIT,        1731408398ul, "weapons/models/knife/knife_karambit/weapon_knife_karambit.vmdl" },
        { WEAPON_KNIFE_M9_BAYONET,      1638561588ul, "weapons/models/knife/knife_m9/weapon_knife_m9.vmdl" },
        { WEAPON_KNIFE_TACTICAL,        2282479884ul, "weapons/models/knife/knife_tactical/weapon_knife_tactical.vmdl" },
        { WEAPON_KNIFE_FALCHION,        3412259219ul, "weapons/models/knife/knife_falchion/weapon_knife_falchion.vmdl" },
        { WEAPON_KNIFE_SURVIVAL_BOWIE,  2511498851ul, "weapons/models/knife/knife_bowie/weapon_knife_bowie.vmdl" },
        { WEAPON_KNIFE_BUTTERFLY,       1353709123ul, "weapons/models/knife/knife_butterfly/weapon_knife_butterfly.vmdl" },
        { WEAPON_KNIFE_PUSH,            4269888884ul, "weapons/models/knife/knife_push/weapon_knife_push.vmdl" },
        { WEAPON_KNIFE_CORD,            1105782941ul, "weapons/models/knife/knife_cord/weapon_knife_cord.vmdl" },
        { WEAPON_KNIFE_CANIS,           275962944ul,  "weapons/models/knife/knife_canis/weapon_knife_canis.vmdl" },
        { WEAPON_KNIFE_URSUS,           1338637359ul, "weapons/models/knife/knife_ursus/weapon_knife_ursus.vmdl" },
        { WEAPON_KNIFE_GYPSY_JACKKNIFE, 3230445913ul, "weapons/models/knife/knife_navaja/weapon_knife_navaja.vmdl" },
        { WEAPON_KNIFE_OUTDOOR,         3206681373ul, "weapons/models/knife/knife_outdoor/weapon_knife_outdoor.vmdl" },
        { WEAPON_KNIFE_STILETTO,        2595277776ul, "weapons/models/knife/knife_stiletto/weapon_knife_stiletto.vmdl" },
        { WEAPON_KNIFE_WIDOWMAKER,      4029975521ul, "weapons/models/knife/knife_talon/weapon_knife_talon.vmdl" },
        { WEAPON_KNIFE_SKELETON,        365028728ul,  "weapons/models/knife/knife_skeleton/weapon_knife_skeleton.vmdl" },
        { WEAPON_KNIFE_KUKRI,           3845286452ul, "weapons/models/knife/knife_kukri/weapon_knife_kukri.vmdl" },
    };

    static const KnifeData* GetKnifeData(int weapon_id) {
        for (const auto& kd : g_knife_data)
            if (kd.weapon_id == weapon_id) return &kd;
        return nullptr;
    }

    // ==================== ENGINE FUNCTION POINTERS ====================
     
    //C_BaseModelEntity_SetModel(C_BaseModelEntity* pBaseModelEntity, const char* szModelName)
    using fnSetModel = void(__fastcall*)(void*, const char*);

    //C_CSWeaponBase_UpdateSubclass(C_CSWeaponBase* pWeaponBase)
    using fnUpdateSubClass = void(__fastcall*)(void*);

    //C_CSWeaponBase_UpdateCompositeMaterial(CCompositeMaterialOwner* pCompositeMaterialOwner, bool unk1)
    using fnUpdateComposite = void(__fastcall*)(void*, bool);

    //C_CSWeaponBase_UpdateSkin(C_CSWeaponBase* pWeaponBase , bool Update)
	using fnUpdateSkin = void(__fastcall*)(void*, bool);

	//CHudWeaponSelection_ClearHudWeaponIcon(CHudWeaponSelection* thisptr, int unk1 , int64_t unk2);
	using fnClearHudWeaponIcon = void(__fastcall*)(void*, int, int64_t);

    //CCSInventoryManager_EquipItemInLoadout(CCSInventoryManager* pCCSInventoryManager , int iTeam , int iSlot , uint64_t iItemID)
	using fnEquipItemInLoadout = bool(__fastcall*)(void*, int, int, uint64_t);

    //CGameSceneNode_SetMeshGroupMask( CGameSceneNode* pGameSceneNode, uint64_t MeshGroupMask)
	using fnSetMeshGroupMask = void(__fastcall*)(void*, uint64_t);
	using fnUpdateCompositeSec = void(__fastcall*)(void*, bool);

    static fnSetModel        g_fnSetModel = nullptr;
    static fnUpdateSubClass  g_fnUpdateSubClass = nullptr;
    static fnUpdateComposite g_fnUpdateComposite = nullptr;
	static fnUpdateSkin       g_fnUpdateSkin = nullptr;
	static fnUpdateCompositeSec g_fnUpdateCompositeSec = nullptr;
	static fnClearHudWeaponIcon g_fnClearHudWeaponIcon = nullptr;
	static fnEquipItemInLoadout g_fnEquipItemInLoadout = nullptr;
    static fnSetMeshGroupMask g_fnSetMeshGroupMask = nullptr;
    static bool              g_engine_funcs_resolved = false;

    static std::uint8_t* TryPattern(const char* module, const char* pattern) {
        try { return sdk::find_pattern(module, pattern); }
        catch (...) { return nullptr; }
    }

    static void ResolveEngineFunctions() {
        if (g_engine_funcs_resolved) return;
        g_engine_funcs_resolved = true;
        auto& con = debug_console::Console::Get();
        auto fnSetModelResolved = sdk::find_pattern("client.dll", SET_MODEL_SIGNATURE);
        switch (SEARCH_TYPE_SET_MODEL) {
            case NORMAL_SCAN:
                if (fnSetModelResolved) {
                    g_fnSetModel = reinterpret_cast<fnSetModel>(fnSetModelResolved);
                    con.Success("[SKINS] SetModel at 0x%llX", reinterpret_cast<uintptr_t>(fnSetModelResolved));
                }
                else con.Warning("[SKINS] SetModel NOT found");
                break;
            case CALL_SCAN:
                
                if (fnSetModelResolved) {
                    g_fnSetModel = reinterpret_cast<fnSetModel>(sdk::GetCA(reinterpret_cast<uintptr_t>(fnSetModelResolved)));
                    con.Success("[SKINS] SetModel at 0x%llX", reinterpret_cast<uintptr_t>(fnSetModelResolved));
                }
                else con.Warning("[SKINS] SetModel NOT found");
                break;
        }
        auto fnUpdateSubClassResolved = sdk::find_pattern("client.dll", UPDATE_SUBCLASS_SIGNATURE);
        switch (SEARCH_TYPE_UPDATE_SUBCLASS) {
            case NORMAL_SCAN:
                
                if (fnUpdateSubClassResolved) {
                    g_fnUpdateSubClass = reinterpret_cast<fnUpdateSubClass>(fnUpdateSubClassResolved);
                    con.Success("[SKINS] UpdateSubclass at 0x%llX", reinterpret_cast<uintptr_t>(fnUpdateSubClassResolved));
                }
                else con.Warning("[SKINS] UpdateSubclass NOT found");
                break;
            case CALL_SCAN:
                
                if (fnUpdateSubClassResolved) {
                    g_fnUpdateSubClass = reinterpret_cast<fnUpdateSubClass>(sdk::GetCA(reinterpret_cast<uintptr_t>(fnUpdateSubClassResolved)));
                    con.Success("[SKINS] UpdateSubclass at 0x%llX", reinterpret_cast<uintptr_t>(fnUpdateSubClassResolved));
                }
                else con.Warning("[SKINS] UpdateSubclass NOT found");
                break;
        }
        auto fnUpdateCompositeResolved = sdk::find_pattern("client.dll", UPDATE_COMPOSITE_MATERIAL_SIGNATURE);
        switch (SEARCH_TYPE_UPDATE_COMPOSITE_MATERIAL) {
            case NORMAL_SCAN:
                
                if (fnUpdateCompositeResolved) {
                    g_fnUpdateComposite = reinterpret_cast<fnUpdateComposite>(fnUpdateCompositeResolved);
                    con.Success("[SKINS] UpdateComposite at 0x%llX", reinterpret_cast<uintptr_t>(fnUpdateCompositeResolved));
                }
                else con.Warning("[SKINS] UpdateComposite NOT found");
                break;
            case CALL_SCAN:
                
                if (fnUpdateCompositeResolved) {
                    g_fnUpdateComposite = reinterpret_cast<fnUpdateComposite>(sdk::GetCA(reinterpret_cast<uintptr_t>(fnUpdateCompositeResolved)));
                    con.Success("[SKINS] UpdateComposite at 0x%llX", reinterpret_cast<uintptr_t>(fnUpdateCompositeResolved));
                }
                else con.Warning("[SKINS] UpdateComposite NOT found");
                break;

        }
        auto fnUpdateSkinResolved = sdk::find_pattern("client.dll", UPDATE_SKIN_SIGNATURE);
        switch (SEARCH_TYPE_UPDATE_SKIN) {
            case NORMAL_SCAN:
                
                if (fnUpdateSkinResolved) {
                    g_fnUpdateSkin = reinterpret_cast<fnUpdateSkin>(fnUpdateSkinResolved);
                    con.Success("[SKINS] UpdateSkin at 0x%llX", reinterpret_cast<uintptr_t>(fnUpdateSkinResolved));
                }
                else con.Warning("[SKINS] UpdateSkin NOT found");
                break;
            case CALL_SCAN:
                auto fnUpdateSkinResolved = sdk::find_pattern("client.dll", UPDATE_SKIN_SIGNATURE);
                if (fnUpdateSkinResolved) {
                    g_fnUpdateComposite = reinterpret_cast<fnUpdateSkin>(sdk::GetCA(reinterpret_cast<uintptr_t>(fnUpdateSkinResolved)));
                    con.Success("[SKINS] UpdateSkin at 0x%llX", reinterpret_cast<uintptr_t>(fnUpdateSkinResolved));
                }
                else con.Warning("[SKINS] UpdateSkin NOT found");
                break;

        }
        auto fnClearHudWeaponIconResolved = sdk::find_pattern("client.dll", CLEAR_HUD_WEAPON_ICON_SIGNATURE);
        switch (SEARCH_TYPE_CLEAR_HUD_WEAPON_ICON) {
            case NORMAL_SCAN:
                
                if (fnClearHudWeaponIconResolved) {
                    g_fnClearHudWeaponIcon = reinterpret_cast<fnClearHudWeaponIcon>(fnClearHudWeaponIconResolved);
                    con.Success("[SKINS] ClearHudWeaponIcon at 0x%llX", reinterpret_cast<uintptr_t>(fnClearHudWeaponIconResolved));
                }
                else con.Warning("[SKINS] ClearHudWeaponIcon NOT found");
                break;
            case CALL_SCAN:
                
                if (fnClearHudWeaponIconResolved) {
                    g_fnClearHudWeaponIcon = reinterpret_cast<fnClearHudWeaponIcon>(sdk::GetCA(reinterpret_cast<uintptr_t>(fnClearHudWeaponIconResolved)));
                    con.Success("[SKINS] ClearHudWeaponIcon at 0x%llX", reinterpret_cast<uintptr_t>(fnClearHudWeaponIconResolved));
                }
                else con.Warning("[SKINS] ClearHudWeaponIcon NOT found");
                break;
        }
        auto fnEquipItemInLoadoutResolved = sdk::find_pattern("client.dll", EQUIP_ITEM_IN_LOADOUT_SIGNATURE);
        switch (SEARCH_TYPE_EQUIP_ITEM_IN_LOADOUT) {
            case NORMAL_SCAN:
                
                if (fnEquipItemInLoadoutResolved) {
                    g_fnEquipItemInLoadout = reinterpret_cast<fnEquipItemInLoadout>(fnEquipItemInLoadoutResolved);
                    con.Success("[SKINS] EquipItemInLoadout at 0x%llX", reinterpret_cast<uintptr_t>(fnEquipItemInLoadoutResolved));
                }
                else con.Warning("[SKINS] EquipItemInLoadout NOT found");
                break;
            case CALL_SCAN:
                
                if (fnEquipItemInLoadoutResolved) {
                    g_fnEquipItemInLoadout = reinterpret_cast<fnEquipItemInLoadout>(sdk::GetCA(reinterpret_cast<uintptr_t>(fnEquipItemInLoadoutResolved)));
                    con.Success("[SKINS] EquipItemInLoadout at 0x%llX", reinterpret_cast<uintptr_t>(fnEquipItemInLoadoutResolved));
                }
                else con.Warning("[SKINS] EquipItemInLoadout NOT found");
                break;
        }
    }

    // ==================== SAFE CALLERS ====================
    // No C++ objects inside __try frames — required for MSVC SEH without /EHa.

    static void CallSetModel(uintptr_t ent, const char* model) {
        if (!ent || !IsReadablePtr(ent) || !model) return;
        if (!g_fnSetModel || reinterpret_cast<uintptr_t>(g_fnSetModel) <= 0x10000) return;
        g_fnSetModel(reinterpret_cast<void*>(ent), model);
    }

    static void CallUpdateSubclassSafe(uintptr_t ent) {
        if (!ent || !IsReadablePtr(ent)) return;
        if (!g_fnUpdateSubClass || reinterpret_cast<uintptr_t>(g_fnUpdateSubClass) <= 0x10000) return;
        g_fnUpdateSubClass(reinterpret_cast<void*>(ent));
    }

    static void CallUpdateComposite(uintptr_t ent, bool force) {
        if (!ent || !IsReadablePtr(ent)) return;
        if (g_fnUpdateComposite && reinterpret_cast<uintptr_t>(g_fnUpdateComposite) > 0x10000)
            g_fnUpdateComposite(reinterpret_cast<void*>(ent), force);
    }

    static void CallUpdateSkin(uintptr_t ent, bool force) {
        if (!ent || !IsReadablePtr(ent)) return;
        if (g_fnUpdateSkin && reinterpret_cast<uintptr_t>(g_fnUpdateSkin) > 0x10000)
            g_fnUpdateSkin(reinterpret_cast<void*>(ent), force);
	}

    static void CallClearHudWeaponIcon(uintptr_t hud_weapon_selection, int unk1, int64_t unk2) {
        if (!hud_weapon_selection || !IsReadablePtr(hud_weapon_selection)) return;
        if (g_fnClearHudWeaponIcon && reinterpret_cast<uintptr_t>(g_fnClearHudWeaponIcon) > 0x10000)
            g_fnClearHudWeaponIcon(reinterpret_cast<void*>(hud_weapon_selection), unk1, unk2);
	}

    static bool CallEquipItemInLoadout(uintptr_t inventory_manager, int team, int slot, uint64_t itemID) {
        if (!inventory_manager || !IsReadablePtr(inventory_manager)) return false;
        if (g_fnEquipItemInLoadout && reinterpret_cast<uintptr_t>(g_fnEquipItemInLoadout) > 0x10000)
            return g_fnEquipItemInLoadout(reinterpret_cast<void*>(inventory_manager), team, slot, itemID);
        return false;
	}

    // ==================== SCENE NODE ====================
    static uintptr_t GetSceneNode(uintptr_t ent) {
        uintptr_t n = *(uintptr_t*)(ent + OFF_GAME_SCENE_NODE);
        return (n && IsReadablePtr(n)) ? n : 0;
    }

    static void SetMeshGroupMask(uintptr_t scene_node, uint64_t mask) {
        if (!scene_node || !IsReadablePtr(scene_node)) return;
       
            if (g_fnSetMeshGroupMask && reinterpret_cast<uintptr_t>(g_fnSetMeshGroupMask) > 0x10000) {
                g_fnSetMeshGroupMask(reinterpret_cast<void*>(scene_node), mask);
                return;
            }
        
        *reinterpret_cast<uint64_t*>(scene_node + OFF_MODEL_STATE + OFF_MESH_GROUP_MASK) = mask;
    }

    // ==================== ENTITY HELPERS ====================
    static uintptr_t GetLocalPawn() {
        uintptr_t base = (uintptr_t)GetModuleHandleA("client.dll");
        if (!base) return 0;
        uintptr_t p = *(uintptr_t*)(base + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
        return (p && IsReadablePtr(p)) ? p : 0;
    }

    static uintptr_t GetEntityList() {
        uintptr_t base = (uintptr_t)GetModuleHandleA("client.dll");
        if (!base) return 0;
        uintptr_t l = *(uintptr_t*)(base + cs2_dumper::offsets::client_dll::dwEntityList);
        return (l && IsReadablePtr(l)) ? l : 0;
    }

    static uintptr_t ResolveHandle(uintptr_t el, uint32_t h) {
        if (!h || h == 0xFFFFFFFF || !el) return 0;
        try {
            uintptr_t le = *(uintptr_t*)(el + 0x8 * ((h & 0x7FFF) >> 9) + 16);
            if (!le || !IsReadablePtr(le)) return 0;
            uintptr_t e = *(uintptr_t*)(le + 112 * (h & 0x1FF));
            return (e && IsReadablePtr(e)) ? e : 0;
        }
        catch (...) { return 0; }
    }

    static uintptr_t GetActiveWeapon() {
        uintptr_t pawn = GetLocalPawn(), list = GetEntityList();
        if (!pawn || !list) return 0;
        try {
            uintptr_t ws = *(uintptr_t*)(pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pWeaponServices);
            if (!ws || !IsReadablePtr(ws)) return 0;
            uint32_t h = *(uint32_t*)(ws + cs2_dumper::schemas::client_dll::CPlayer_WeaponServices::m_hActiveWeapon);
            return ResolveHandle(list, h);
        }
        catch (...) { return 0; }
    }

    static std::vector<uintptr_t> GetAllWeapons() {
        std::vector<uintptr_t> result;
        uintptr_t pawn = GetLocalPawn(), list = GetEntityList();
        if (!pawn || !list) return result;
        try {
            uintptr_t ws = *(uintptr_t*)(pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pWeaponServices);
            if (!ws || !IsReadablePtr(ws)) return result;
            uintptr_t arr = ws + cs2_dumper::schemas::client_dll::CPlayer_WeaponServices::m_hMyWeapons;
            for (int i = 0; i < 64; i++) {
                uint32_t h = *(uint32_t*)(arr + i * 4);
                if (!h || h == 0xFFFFFFFF) continue;
                uintptr_t w = ResolveHandle(list, h);
                if (w) result.push_back(w);
            }
        }
        catch (...) {}
        return result;
    }

    static uintptr_t GetArmsEntity() {
        uintptr_t pawn = GetLocalPawn(), list = GetEntityList();
        if (!pawn || !list) return 0;
        try {
            constexpr std::ptrdiff_t OFF_HUD_MODEL_ARMS = 0x2400;
            uint32_t h = *(uint32_t*)(pawn + 0x2400);
            return ResolveHandle(list, h);
        }
        catch (...) { return 0; }
    }

    static uint16_t GetDefIndex(uintptr_t w) {
        return *reinterpret_cast<uint16_t*>(w + OFF_ITEM_DEF_INDEX);
    }

    static uint8_t GetLocalTeam() {
        uintptr_t p = GetLocalPawn();
        return p ? *(uint8_t*)(p + OFF_TEAM_NUM) : 0;
    }

    // ==================== INVENTORY BACKING ====================
    // Mirrors reference g_vecAddedItemsIDs: tracks items injected via AddSkinToInventory.
    // ApplyWeaponSkins looks up active weapon's loadout item_id in this list to apply skin data.

    struct AddedItemInfo {
        uint64_t id;
        float    paintKit;
        float    paintSeed;
        float    paintWear;
        bool     legacy;
    };

    static std::vector<AddedItemInfo> g_vecAddedItemsIDs;

    void AddEconItemToList(uint64_t itemID, float paintKit, float paintSeed, float paintWear, bool legacy) {
        g_vecAddedItemsIDs.erase(
            std::remove_if(g_vecAddedItemsIDs.begin(), g_vecAddedItemsIDs.end(),
                [itemID](const AddedItemInfo& i) { return i.id == itemID; }),
            g_vecAddedItemsIDs.end());
        g_vecAddedItemsIDs.push_back({ itemID, paintKit, paintSeed, paintWear, legacy });
        debug_console::Console::Get().Success("[SKINS] Registered item id=%llu kit=%.0f legacy=%d",
            itemID, paintKit, (int)legacy);
    }

    void RemoveEconItemFromList(uint64_t itemID) {
        g_vecAddedItemsIDs.erase(
            std::remove_if(g_vecAddedItemsIDs.begin(), g_vecAddedItemsIDs.end(),
                [itemID](const AddedItemInfo& i) { return i.id == itemID; }),
            g_vecAddedItemsIDs.end());
    }

    char* GetWeaponModelName(uintptr_t weapon) {
		uintptr_t weaponData = *(uintptr_t*)(weapon + cs2_dumper::schemas::client_dll::C_BaseEntity::m_nSubclassID + 0x08);
		char* modelName = weaponData ? (char*)(weaponData + 0x640) : nullptr;
		return (modelName && IsReadablePtr((uintptr_t)modelName)) ? modelName : nullptr;
    }

    // ==================== LOADOUT READING ====================
    struct LoadoutItem {
        uintptr_t item_view;
        uint16_t  def_index;
        uint64_t  item_id;
        uint32_t  item_id_high;
        uint32_t  item_id_low;
        uint32_t  account_id;
        uint16_t  team;
        uint16_t  slot;
    };

    static std::vector<LoadoutItem> ReadLoadout() {
        std::vector<LoadoutItem> items;
        try {
            uintptr_t ctrl = game_state::GetLocalController();
            if (!ctrl || !IsReadablePtr(ctrl)) return items;
            uintptr_t svc = *(uintptr_t*)(ctrl + OFF_INV_SERVICES);
            if (!svc || !IsReadablePtr(svc)) return items;
            uintptr_t vec = svc + OFF_LOADOUT_VEC;
            uintptr_t data = *(uintptr_t*)(vec);
            if (!data || !IsReadablePtr(data)) return items;
            int count = *(int*)(vec + 0x10);
            if (count <= 0 || count > 256) return items;
            for (int i = 0; i < count; i++) {
                uintptr_t sb = data + (i * 16);
                uintptr_t iv = *(uintptr_t*)(sb);
                if (!iv || !IsReadablePtr(iv)) continue;
                LoadoutItem li{};
                li.item_view = iv;
                li.team = *(uint16_t*)(sb + 0x08);
                li.slot = *(uint16_t*)(sb + 0x0A);
                li.def_index = *(uint16_t*)(iv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex);
                li.item_id = *(uint64_t*)(iv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemID);
                li.item_id_high = *(uint32_t*)(iv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh);
                li.item_id_low = *(uint32_t*)(iv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDLow);
                li.account_id = *(uint32_t*)(iv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iAccountID);
                items.push_back(li);
            }
        }
        catch (...) {}
        return items;
    }

    static const LoadoutItem* FindLoadoutByDefIndex(const std::vector<LoadoutItem>& lo,
        uint16_t def, uint8_t team) {
        for (const auto& li : lo)
            if (li.team == team && li.def_index == def) return &li;
        return nullptr;
    }

    static const LoadoutItem* FindMeleeItem(const std::vector<LoadoutItem>& lo, uint8_t team) {
        for (const auto& li : lo)
            if (li.team == team && li.slot == 0) return &li;
        return nullptr;
    }

    // ==================== UPDATESUBCLASS QUEUE ====================
    static std::mutex             s_subclass_mutex;
    static std::vector<uintptr_t> s_subclass_queue;

    void EnqueueUpdateSubclass(uintptr_t weapon) {
        if (!weapon) return;
        std::lock_guard<std::mutex> lk(s_subclass_mutex);
        s_subclass_queue.push_back(weapon);
        debug_console::Console::Get().Debug("[KNIFE] Enqueued UpdateSubclass for 0x%llX", weapon);
    }

    void ProcessQueuedUpdateSubclass() {
        uintptr_t jobs[64]; int count = 0;
        {
            std::lock_guard<std::mutex> lk(s_subclass_mutex);
            for (auto w : s_subclass_queue) if (count < 64) jobs[count++] = w;
            s_subclass_queue.clear();
        }
        for (int i = 0; i < count; i++) {
            if (!jobs[i] || !IsReadablePtr(jobs[i])) continue;
            debug_console::Console::Get().Debug("[KNIFE] ProcessQueued: UpdateSubclass for 0x%llX", jobs[i]);
            CallUpdateSubclassSafe(jobs[i]);
        }
    }

    // ==================== APPLY WEAPON SKINS ====================
    // Mirrors reference ApplyWeaponSkins:
    //   1. Active weapon only, skip knives
    //   2. Get loadout slot item for the weapon def_index + team
    //   3. Find item in g_vecAddedItemsIDs by item_id
    //   4. Copy item IDs from loadout -> weapon EconItemView
    //   5. Set legacy fallback fields if skin.legacy == true
    //   6. UpdateComposite + mesh group mask

    void ApplyWeaponSkins() {
        ResolveEngineFunctions();
        if (!game_state::IsInGame()) return;

        uintptr_t weapon = GetActiveWeapon();
        if (!weapon) return;

        uint16_t def_index = GetDefIndex(weapon);
        if (IsKnife(def_index) || IsDefaultKnife(def_index)) return; // knives handled separately

        std::vector<LoadoutItem> loadout;
        try { loadout = ReadLoadout(); }
        catch (...) {}
        uint8_t team = GetLocalTeam();

        // Reference: pInventory->GetItemInLoadout(pWeapon->m_iOriginalTeamNumber(), nSlot)
        const LoadoutItem* li = FindLoadoutByDefIndex(loadout, def_index, team);
        if (!li || li->item_id == 0) return;

        // Reference: look up in g_vecAddedItemsIDs
        auto it = std::find_if(g_vecAddedItemsIDs.begin(), g_vecAddedItemsIDs.end(),
            [&](const AddedItemInfo& i) { return i.id == li->item_id; });
        if (it == g_vecAddedItemsIDs.end()) return;

        const AddedItemInfo& info = *it;

        // Reference: copy item IDs from loadout item to pWeaponItemView
        *reinterpret_cast<uint64_t*>(weapon + OFF_ITEM_ID) = li->item_id;
        *reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_HIGH) = li->item_id_high;
        *reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_LOW) = li->item_id_low;
        *reinterpret_cast<uint32_t*>(weapon + OFF_ACCOUNT_ID) = li->account_id;
        *reinterpret_cast<bool*>(weapon + OFF_DISALLOW_SOC) = false;
        *reinterpret_cast<bool*>(weapon + OFF_RESTORE_MATERIAL) = true;

        // Reference: if (info.legacy && s_regenPending) set fallback + itemIDHigh=-1
        if (info.legacy) {
            *reinterpret_cast<int*>(weapon + OFF_FALLBACK_PAINT) = (int)info.paintKit;
            *reinterpret_cast<int*>(weapon + OFF_FALLBACK_SEED) = (int)info.paintSeed;
            *reinterpret_cast<float*>(weapon + OFF_FALLBACK_WEAR) = info.paintWear;
            *reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_HIGH) = (uint32_t)-1;
            *reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_LOW) = (uint32_t)-1;
        }

        // Reference: mask = info.legacy ? 2 : 1
        uintptr_t node = GetSceneNode(weapon);
        if (node) SetMeshGroupMask(node, info.legacy ? 2ULL : 1ULL);

        // Reference: pWeapon->UpdateComposite(1); pWeapon->UpdateCompositeSec(1);
        try { CallUpdateComposite(weapon, true); }
        catch (...) {}
        try {
            if (g_fnUpdateCompositeSec && reinterpret_cast<uintptr_t>(g_fnUpdateCompositeSec) > 0x10000)
                g_fnUpdateCompositeSec(reinterpret_cast<void*>(weapon), true);
        }
        catch (...) {}
    }

    // ==================== APPLY KNIFE SKINS ====================
    // Mirrors reference ApplyKnifeSkins exactly:
    //   1. Active weapon must be a knife
    //   2. Get melee loadout item — verify def_index >= 500
    //   3. ONE TIME on def_index change: write def_index + subclass + UpdateSubclass
    //   4. EVERY FRAME: set_model + mesh mask + UpdateComposite + UpdateCompositeSec

    static uint16_t s_lastKnifeDefIndex = 0;

    void ApplyKnifeSkins() {
        ResolveEngineFunctions();
        if (!game_state::IsInGame()) return;

        uintptr_t weapon = GetActiveWeapon();
        if (!weapon) return;

        uint16_t def_index = GetDefIndex(weapon);
        if (!IsKnife(def_index) && !IsDefaultKnife(def_index)) return;

        std::vector<LoadoutItem> loadout;
        try { loadout = ReadLoadout(); }
        catch (...) {}
        uint8_t team = GetLocalTeam();

        // Determine target knife: melee loadout slot preferred, else config selection
        uint16_t knifeDefIndex = 0;
        const LoadoutItem* melee = FindMeleeItem(loadout, team);
        if (melee && melee->def_index >= 500)
            knifeDefIndex = melee->def_index;
        else if (selected_knife_id >= 500)
            knifeDefIndex = static_cast<uint16_t>(selected_knife_id);
        else
            return;

        // Reference: if (subclassMap.find(knifeDefIndex) == subclassMap.end()) return;
        const KnifeData* kd = GetKnifeData(knifeDefIndex);
        if (!kd) return;

        // Reference: copy item IDs from pInLoadout to pWeaponItemView
        if (melee && melee->item_id != 0) {
            *reinterpret_cast<uint64_t*>(weapon + OFF_ITEM_ID) = melee->item_id;
            *reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_HIGH) = melee->item_id_high;
            *reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_LOW) = melee->item_id_low;
            *reinterpret_cast<uint32_t*>(weapon + OFF_ACCOUNT_ID) = melee->account_id;
        }
        else {
            *reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_HIGH) = (uint32_t)-1;
            *reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_LOW) = (uint32_t)-1;
        }
        *reinterpret_cast<bool*>(weapon + OFF_DISALLOW_SOC) = false;
        *reinterpret_cast<bool*>(weapon + OFF_RESTORE_MATERIAL) = true;

        // Knife skin fallback fields from config
        auto skin_cfg = user_skins.find(knifeDefIndex);
        if (skin_cfg != user_skins.end()) {
            *reinterpret_cast<int*>(weapon + OFF_FALLBACK_PAINT) = skin_cfg->second.paint_kit;
            *reinterpret_cast<float*>(weapon + OFF_FALLBACK_WEAR) = skin_cfg->second.wear;
            *reinterpret_cast<int*>(weapon + OFF_FALLBACK_SEED) = skin_cfg->second.seed;
            *reinterpret_cast<int*>(weapon + OFF_FALLBACK_STATTRAK) =
                skin_cfg->second.stattrak ? skin_cfg->second.stattrak_count : -1;
        }

        // Reference: if (nLastKnifeDefIndex != knifeDefIndex) — ONE TIME on type change
        if (s_lastKnifeDefIndex != knifeDefIndex) {
            *reinterpret_cast<uint16_t*>(weapon + OFF_ITEM_DEF_INDEX) = knifeDefIndex;
            *reinterpret_cast<unsigned long*>(weapon + OFF_SUBCLASS_ID) = kd->subclass_hash;
            // Reference: pWeapon->UpdateSubClass();
			CallUpdateSubclassSafe(weapon);
            s_lastKnifeDefIndex = knifeDefIndex;
            debug_console::Console::Get().Success("[KNIFE] Type change -> def=%u subclass=0x%X model=%s",
                knifeDefIndex, kd->subclass_hash, kd->model_path);
        }

        // EVERY FRAME: re-write def_index + subclass (network resets them each tick)
        *reinterpret_cast<uint16_t*>(weapon + OFF_ITEM_DEF_INDEX) = knifeDefIndex;
        *reinterpret_cast<unsigned long*>(weapon + OFF_SUBCLASS_ID) = kd->subclass_hash;
         // Reference: pWeapon->set_model(knifeModel)
        try {

            CallSetModel(weapon, kd->model_path);

        }
        catch (...) {}

        // Also set model on arms (hands) entity
        // uintptr_t arms = GetArmsEntity();
        //if (arms) { try { CallSetModel(arms, kd->model_path); } catch (...) {} }

        // Reference: node->set_mesh_group_mask(2)
        uintptr_t node = GetSceneNode(weapon);
        if (node) SetMeshGroupMask(node, 2);
        uintptr_t arms = GetArmsEntity();
        if (arms) SetMeshGroupMask(GetSceneNode(arms), 2);
        //if (arms) { uintptr_t an = GetSceneNode(arms); if (an) SetMeshGroupMask(an, 2); }

        // Reference: pWeapon->UpdateComposite(1); pWeapon->UpdateCompositeSec(1);
        try {
            auto result = sdk::CallVFunc<7u, void*>(reinterpret_cast<void*>(weapon), 1);
            if (result)
                debug_console::Console::Get().Success("[KNIFE] Called UpdateComposite via vfunc");
            else
                debug_console::Console::Get().Warning("[KNIFE] Failed to call UpdateComposite via vfunc");
        }
        catch (...) {
            debug_console::Console::Get().Warning("[KNIFE] Failed to call UpdateComposite via vfunc");
        }
        try {
            auto result = sdk::CallVFunc<105u, void*>(reinterpret_cast<void*>(weapon), 1);
            if (result)
                debug_console::Console::Get().Success("[KNIFE] Called UpdateCompositeSec via vfunc");
            else
                debug_console::Console::Get().Warning("[KNIFE] Failed to call UpdateCompositeSec via vfunc");
        }
        catch (...) {
            debug_console::Console::Get().Warning("[KNIFE] Failed to call UpdateCompositeSec via vfunc");
        }
        
    }

    // ==================== APPLY GLOVES ====================
    void ApplyGloves() {
        if (!game_state::IsInGame()) return;
        uintptr_t pawn = GetLocalPawn();
        if (!pawn) return;

        uintptr_t gv = pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_EconGloves;

        std::vector<LoadoutItem> loadout;
        try { loadout = ReadLoadout(); }
        catch (...) {}
        uint8_t team = GetLocalTeam();

        // Find glove loadout slot (slot 41)
        const LoadoutItem* gl = nullptr;
        for (const auto& li : loadout)
            if (li.team == team && li.slot == 41) { gl = &li; break; }

        if (gl && gl->item_id != 0) {
            // Reference: copy from inventory loadout item to m_EconGloves
            *reinterpret_cast<uint16_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex) = gl->def_index;
            *reinterpret_cast<uint64_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemID) = gl->item_id;
            *reinterpret_cast<uint32_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh) = gl->item_id_high;
            *reinterpret_cast<uint32_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDLow) = gl->item_id_low;
            *reinterpret_cast<uint32_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iAccountID) = gl->account_id;
            *reinterpret_cast<int*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iEntityQuality) = 3;
            *reinterpret_cast<bool*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_bDisallowSOC) = false;
            *reinterpret_cast<bool*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_bRestoreCustomMaterialAfterPrecache) = true;
        }
        else if (selected_glove_kit > 0) {
            // Legacy fallback: no inventory item, write directly
            *reinterpret_cast<uint16_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex) = 5028;
            *reinterpret_cast<uint32_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh) = (uint32_t)-1;
            *reinterpret_cast<uint32_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDLow) = (uint32_t)-1;
            *reinterpret_cast<int*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iEntityQuality) = 3;
            *reinterpret_cast<bool*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_bDisallowSOC) = false;
            *reinterpret_cast<bool*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_bRestoreCustomMaterialAfterPrecache) = true;
        }
        else return;

        // Reference: local_player->m_bNeedToReApplyGloves() = true
        *reinterpret_cast<bool*>(pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_bNeedToReApplyGloves) = true;
    }

    // ==================== MAIN TICK ====================
    // Called from hkPresent every frame. Matches reference dllmain.cpp call order.

    void ApplyAllSkins() {
        if (!game_state::IsInGame()) return;

        uintptr_t localPawn = GetLocalPawn();
		int health = localPawn ? *(int*)(localPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth) : 0;
        if (health < 1) return;

        try { ApplyKnifeSkins(); }
        catch (...) {}

        // Reference: ApplyWeaponSkins — inventory-injection path
        // Rate-limited to avoid thrashing UpdateComposite every frame
        static auto g_last_apply = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        bool should_apply = std::chrono::duration_cast<std::chrono::milliseconds>(now - g_last_apply).count() >= 2000;
        if (!should_apply) return;
        g_last_apply = now;

        // Reference: ApplyKnifeSkins — every frame, no rate limit
        
        

        /*try { ApplyWeaponSkins(); }
        catch (...) {}*/

        // Config-based fallback path (user_skins map — for weapons not in g_vecAddedItemsIDs)
        /*try {
            auto weapons = GetAllWeapons();
            for (auto w : weapons) {
                if (!w) continue;
                uint16_t def = GetDefIndex(w);
                if (IsKnife(def) || IsDefaultKnife(def)) continue;
                if (def >= WEAPON_FLASHBANG && def <= WEAPON_INCGRENADE) continue;
                if (def == WEAPON_C4 || def == WEAPON_TASER) continue;

                auto cfg = user_skins.find(def);
                if (cfg == user_skins.end()) continue;

                try {
                    *reinterpret_cast<int*>(w + OFF_FALLBACK_PAINT) = cfg->second.paint_kit;
                    *reinterpret_cast<int*>(w + OFF_FALLBACK_SEED) = cfg->second.seed;
                    *reinterpret_cast<float*>(w + OFF_FALLBACK_WEAR) = cfg->second.wear;
                    *reinterpret_cast<bool*>(w + OFF_DISALLOW_SOC) = false;
                    *reinterpret_cast<bool*>(w + OFF_RESTORE_MATERIAL) = true;
                    if (cfg->second.stattrak) {
                        *reinterpret_cast<int*>(w + OFF_FALLBACK_STATTRAK) = cfg->second.stattrak_count;
                        *reinterpret_cast<int*>(w + OFF_ENTITY_QUALITY) = 9;
                    }
                    else {
                        *reinterpret_cast<int*>(w + OFF_FALLBACK_STATTRAK) = -1;
                    }
                    uintptr_t node = GetSceneNode(w);
                    if (node) SetMeshGroupMask(node, cfg->second.paint_kit > 0 ? 2ULL : 1ULL);
                    CallUpdateComposite(w, true);
                }
                catch (...) {}
            }
        }
        catch (...) {}*/
    }

    void ApplyKnife() {
        s_lastKnifeDefIndex = 0; // reset so next frame triggers type-change path
        debug_console::Console::Get().Success("[KNIFE] Selection changed, refreshing next frame");
    }

    // ==================== HTTP ====================
    static std::string HTTPGet(const std::string& url) {
        std::string r;
        HINTERNET h = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (!h) return r;
        DWORD flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE;
        if (url.find("https://") == 0) flags |= INTERNET_FLAG_SECURE;
        HINTERNET hu = InternetOpenUrlA(h, url.c_str(), NULL, 0, flags, 0);
        if (hu) {
            char buf[8192]; DWORD rd = 0;
            while (InternetReadFile(hu, buf, sizeof(buf) - 1, &rd) && rd > 0) r.append(buf, rd);
            InternetCloseHandle(hu);
        }
        InternetCloseHandle(h);
        return r;
    }

    static int ExtractInt(const std::string& obj, const std::string& key) {
        auto s = "\"" + key + "\":";
        auto p = obj.find(s); if (p == std::string::npos) return 0;
        p += s.size();
        while (p < obj.size() && !std::isdigit(obj[p]) && obj[p] != '-') p++;
        if (p < obj.size() && obj[p] == '"') p++;
        auto e = p;
        while (e < obj.size() && std::isdigit(obj[e])) e++;
        if (e > p) { try { return std::stoi(obj.substr(p, e - p)); } catch (...) {} }
        return 0;
    }

    static std::string ExtractStr(const std::string& obj, const std::string& key) {
        auto s = "\"" + key + "\":";
        auto p = obj.find(s); if (p == std::string::npos) return "";
        p += s.size();
        while (p < obj.size() && obj[p] != '"') p++;
        if (p >= obj.size()) return "";
        p++;
        auto e = obj.find('"', p);
        return (e != std::string::npos) ? obj.substr(p, e - p) : "";
    }

    // ==================== SKIN DATABASE ====================
    void LoadSkinsFromAPI() {
        skin_database.clear(); skins_by_weapon.clear(); glove_database.clear();
        debug_console::Console::Get().Success("[SKINS] Fetching skin database...");
        try {
            auto data = HTTPGet("https://raw.githubusercontent.com/Nereziel/cs2-WeaponPaints/main/website/data/skins_en.json");
            size_t pos = 0;
            while ((pos = data.find('{', pos)) != std::string::npos) {
                auto end = data.find('}', pos); if (end == std::string::npos) break;
                auto obj = data.substr(pos, end - pos + 1);
                int wid = ExtractInt(obj, "weapon_defindex");
                int pk = ExtractInt(obj, "paint");
                auto pn = ExtractStr(obj, "paint_name");
                if (wid > 0 && pk > 0 && !pn.empty()) {
                    auto sn = pn;
                    auto wd = GetWeaponName(wid);
                    auto sep = pn.find(" | ");
                    if (sep != std::string::npos) sn = pn.substr(sep + 3);
                    SkinInfo sk(pk, sn, wd, wid, RARITY_COMMON, true);
                    skin_database.push_back(sk);
                    skins_by_weapon[wd].push_back(sk);
                }
                pos = end + 1;
            }
            debug_console::Console::Get().Success("[SKINS] Loaded %zu weapon skins", skin_database.size());

            auto gdata = HTTPGet("https://raw.githubusercontent.com/Nereziel/cs2-WeaponPaints/main/website/data/gloves_en.json");
            pos = 0;
            while ((pos = gdata.find('{', pos)) != std::string::npos) {
                auto end = gdata.find('}', pos); if (end == std::string::npos) break;
                auto obj = gdata.substr(pos, end - pos + 1);
                int pk = ExtractInt(obj, "paint");
                auto pn = ExtractStr(obj, "paint_name");
                if (pk > 0 && !pn.empty()) {
                    auto gn = pn;
                    auto sep = pn.find(" | ");
                    if (sep != std::string::npos) gn = pn.substr(sep + 3);
                    glove_database.push_back(GloveInfo(pk, gn));
                }
                pos = end + 1;
            }
            debug_console::Console::Get().Success("[SKINS] Loaded %zu glove skins", glove_database.size());
            if (skin_database.empty() && glove_database.empty()) InitializeSkinDatabase();
        }
        catch (...) {
            debug_console::Console::Get().Error("[SKINS] API failed, using fallback");
            InitializeSkinDatabase();
        }
    }

    void InitializeSkinDatabase() {
        skin_database.push_back(SkinInfo(282, "Redline", "AK-47", WEAPON_AK47, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(180, "Vulcan", "AK-47", WEAPON_AK47, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(344, "Dragon Lore", "AWP", WEAPON_AWP, RARITY_CONTRABAND, true));
        skin_database.push_back(SkinInfo(279, "Asiimov", "AWP", WEAPON_AWP, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(309, "Howl", "M4A4", WEAPON_M4A1, RARITY_CONTRABAND, true));
        skin_database.push_back(SkinInfo(360, "Cyrex", "M4A1-S", WEAPON_M4A1_SILENCER, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(37, "Blaze", "Desert Eagle", WEAPON_DEAGLE, RARITY_ANCIENT, false));
        skin_database.push_back(SkinInfo(277, "Orion", "USP-S", WEAPON_USP_SILENCER, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(38, "Fade", "Glock-18", WEAPON_GLOCK, RARITY_ANCIENT, false));
        for (const auto& s : skin_database) skins_by_weapon[s.weapon_name].push_back(s);
        glove_database.push_back(GloveInfo(10006, "Superconductor"));
        glove_database.push_back(GloveInfo(10015, "Pandora's Box"));
        glove_database.push_back(GloveInfo(10018, "Vice"));
    }

    bool IsDatabaseLoaded() { return !skin_database.empty() || !glove_database.empty(); }

    // ==================== BATCH OPS ====================
    void ClearAllSkins() {
        user_skins.clear(); g_vecAddedItemsIDs.clear();
        selected_glove_kit = 0; s_lastKnifeDefIndex = 0;
        debug_console::Console::Get().Info("[SKIN] Cleared");
    }

    void ApplyDefaultPreset() {
        PlayerSkinConfig a; a.weapon_id = WEAPON_AK47;   a.paint_kit = 282; a.wear = 0.01f; user_skins[WEAPON_AK47] = a;
        PlayerSkinConfig b; b.weapon_id = WEAPON_AWP;    b.paint_kit = 344; b.wear = 0.01f; user_skins[WEAPON_AWP] = b;
        PlayerSkinConfig c; c.weapon_id = WEAPON_DEAGLE; c.paint_kit = 37;  c.wear = 0.01f; user_skins[WEAPON_DEAGLE] = c;
        debug_console::Console::Get().Success("[SKIN] Default preset loaded");
    }

    void ApplyFactoryNewAll() { for (auto& p : user_skins) p.second.wear = 0.f; }
    void ApplyRandomSeeds() { for (auto& p : user_skins) p.second.seed = rand() % 1000; }

    // ==================== NAMES / RARITIES ====================
    const char* GetWeaponName(int id) {
        switch (id) {
        case WEAPON_AK47: return "AK-47"; case WEAPON_M4A1: return "M4A4";
        case WEAPON_M4A1_SILENCER: return "M4A1-S"; case WEAPON_AWP: return "AWP";
        case WEAPON_DEAGLE: return "Desert Eagle"; case WEAPON_GLOCK: return "Glock-18";
        case WEAPON_USP_SILENCER: return "USP-S"; case WEAPON_P250: return "P250";
        case WEAPON_FIVESEVEN: return "Five-SeveN"; case WEAPON_TEC9: return "Tec-9";
        case WEAPON_CZ75A: return "CZ75-Auto"; case WEAPON_REVOLVER: return "R8 Revolver";
        case WEAPON_ELITE: return "Dual Berettas"; case WEAPON_HKP2000: return "P2000";
        case WEAPON_NOVA: return "Nova"; case WEAPON_XM1014: return "XM1014";
        case WEAPON_SAWEDOFF: return "Sawed-Off"; case WEAPON_MAG7: return "MAG-7";
        case WEAPON_M249: return "M249"; case WEAPON_NEGEV: return "Negev";
        case WEAPON_MAC10: return "MAC-10"; case WEAPON_MP9: return "MP9";
        case WEAPON_MP7: return "MP7"; case WEAPON_MP5SD: return "MP5-SD";
        case WEAPON_UMP45: return "UMP-45"; case WEAPON_P90: return "P90";
        case WEAPON_BIZON: return "PP-Bizon"; case WEAPON_FAMAS: return "FAMAS";
        case WEAPON_GALILAR: return "Galil AR"; case WEAPON_AUG: return "AUG";
        case WEAPON_SG556: return "SG 553"; case WEAPON_SSG08: return "SSG 08";
        case WEAPON_SCAR20: return "SCAR-20"; case WEAPON_G3SG1: return "G3SG1";
        case WEAPON_KNIFE_BAYONET: return "Bayonet"; case WEAPON_KNIFE_CLASSIC: return "Classic Knife";
        case WEAPON_KNIFE_FLIP: return "Flip Knife"; case WEAPON_KNIFE_GUT: return "Gut Knife";
        case WEAPON_KNIFE_KARAMBIT: return "Karambit"; case WEAPON_KNIFE_M9_BAYONET: return "M9 Bayonet";
        case WEAPON_KNIFE_TACTICAL: return "Huntsman Knife"; case WEAPON_KNIFE_FALCHION: return "Falchion Knife";
        case WEAPON_KNIFE_SURVIVAL_BOWIE: return "Bowie Knife"; case WEAPON_KNIFE_BUTTERFLY: return "Butterfly Knife";
        case WEAPON_KNIFE_PUSH: return "Shadow Daggers"; case WEAPON_KNIFE_CORD: return "Paracord Knife";
        case WEAPON_KNIFE_CANIS: return "Survival Knife"; case WEAPON_KNIFE_URSUS: return "Ursus Knife";
        case WEAPON_KNIFE_GYPSY_JACKKNIFE: return "Navaja Knife"; case WEAPON_KNIFE_OUTDOOR: return "Nomad Knife";
        case WEAPON_KNIFE_STILETTO: return "Stiletto Knife"; case WEAPON_KNIFE_WIDOWMAKER: return "Talon Knife";
        case WEAPON_KNIFE_SKELETON: return "Skeleton Knife"; case WEAPON_KNIFE_KUKRI: return "Kukri Knife";
        case WEAPON_KNIFE_CT: return "CT Knife"; case WEAPON_KNIFE_T: return "T Knife";
        case WEAPON_TASER: return "Zeus x27"; default: return "Unknown";
        }
    }

    const char* GetRarityName(SkinRarity r) {
        switch (r) {
        case RARITY_COMMON: return "Consumer Grade"; case RARITY_UNCOMMON: return "Industrial Grade";
        case RARITY_RARE: return "Mil-Spec Grade"; case RARITY_MYTHICAL: return "Restricted";
        case RARITY_LEGENDARY: return "Classified"; case RARITY_ANCIENT: return "Covert";
        case RARITY_CONTRABAND: return "Contraband"; default: return "Unknown";
        }
    }

    const float* GetRarityColor(SkinRarity r) {
        static float c[][4] = {
            {0.7f,0.7f,0.7f,1.f},{0.4f,0.6f,0.9f,1.f},{0.3f,0.4f,0.8f,1.f},
            {0.5f,0.3f,0.8f,1.f},{0.8f,0.3f,0.6f,1.f},{0.9f,0.2f,0.2f,1.f},{0.95f,0.8f,0.1f,1.f}
        };
        return (r >= 0 && r <= RARITY_CONTRABAND) ? c[r] : c[0];
    }

    // ==================== DEBUG READBACK ====================
    std::vector<WeaponDebugInfo> GetCurrentWeaponsDebugInfo() {
        std::vector<WeaponDebugInfo> result;
        if (!game_state::IsInGame()) return result;
        try {
            auto active = GetActiveWeapon();
            for (auto w : GetAllWeapons()) {
                if (!w) continue;
                WeaponDebugInfo info{};
                info.address = w;
                info.is_active = (w == active);
                info.def_index = *reinterpret_cast<uint16_t*>(w + OFF_ITEM_DEF_INDEX);
                info.entity_quality = *reinterpret_cast<int*>(w + OFF_ENTITY_QUALITY);
                info.item_id_high = *reinterpret_cast<int*>(w + OFF_ITEM_ID_HIGH);
                info.item_id = *reinterpret_cast<uint64_t*>(w + OFF_ITEM_ID);
                info.account_id = *reinterpret_cast<uint32_t*>(w + OFF_ACCOUNT_ID);
                info.disallow_soc = *reinterpret_cast<bool*>(w + OFF_DISALLOW_SOC);
                info.restore_material = *reinterpret_cast<bool*>(w + OFF_RESTORE_MATERIAL);
                info.fallback_paint_kit = *reinterpret_cast<int*>(w + OFF_FALLBACK_PAINT);
                info.fallback_seed = *reinterpret_cast<int*>(w + OFF_FALLBACK_SEED);
                info.fallback_wear = *reinterpret_cast<float*>(w + OFF_FALLBACK_WEAR);
                info.fallback_stattrak = *reinterpret_cast<int*>(w + OFF_FALLBACK_STATTRAK);
                info.owner_xuid_low = *reinterpret_cast<uint32_t*>(w + OFF_OWNER_XUID_LOW);
                info.subclass_id = *reinterpret_cast<uint32_t*>(w + OFF_SUBCLASS_ID);
                info.loadout_matched = (info.item_id != 0 && info.item_id_high != -1);
                strncpy_s(info.custom_name, reinterpret_cast<char*>(w + OFF_CUSTOM_NAME),
                    sizeof(info.custom_name) - 1);
                result.push_back(info);
            }
        }
        catch (...) {}
        return result;
    }

    bool IsSetModelAvailable() { return g_fnSetModel != nullptr; }
    int  GetLoadoutItemCount() { try { return (int)ReadLoadout().size(); } catch (...) { return 0; } }

    // Legacy single-weapon apply (still used by some UI paths)
    void ApplySkin(void* weapon, int weapon_id) {
        if (!weapon) return;
        auto it = user_skins.find(weapon_id);
        if (it == user_skins.end()) return;
        uintptr_t w = reinterpret_cast<uintptr_t>(weapon);
        *reinterpret_cast<int*>(w + OFF_FALLBACK_PAINT) = it->second.paint_kit;
        *reinterpret_cast<int*>(w + OFF_FALLBACK_SEED) = it->second.seed;
        *reinterpret_cast<float*>(w + OFF_FALLBACK_WEAR) = it->second.wear;
        *reinterpret_cast<bool*>(w + OFF_DISALLOW_SOC) = false;
        *reinterpret_cast<bool*>(w + OFF_RESTORE_MATERIAL) = true;
        CallUpdateComposite(w, true);
    }
}