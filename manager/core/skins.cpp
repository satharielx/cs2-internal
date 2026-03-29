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

#define REGEN_WEAPON_SKINS_SIGNATURE "48 83 EC ? E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 48 8B 10"
#define SEARCH_TYPE_REGEN_WEAPON_SKINS NORMAL_SCAN
#define LOCATION_REGEN_WEAPON_SKINS CLIENT

#pragma comment(lib, "wininet.lib")

namespace skins {

    // ==================== SAFE POINTER CHECK ====================
    static bool IsReadablePtr(uintptr_t ptr) {
        if (ptr == 0 || ptr < 0x10000 || ptr == 0xFFFFFFFFFFFFFFFFull) return false;
        MEMORY_BASIC_INFORMATION mbi{};
        if (!VirtualQuery(reinterpret_cast<LPCVOID>(ptr), &mbi, sizeof(mbi))) return false;
        if (mbi.State != MEM_COMMIT) return false;
        const DWORD bad = PAGE_NOACCESS | PAGE_GUARD;
        return (mbi.Protect & bad) == 0;
    }

    static bool SafeReadInt(uintptr_t addr, int& out) {
        if (!IsReadablePtr(addr)) return false;
        out = *reinterpret_cast<int*>(addr);
        return true;
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
    static constexpr std::ptrdiff_t OFF_HEALTH = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth;

    static constexpr std::ptrdiff_t OFF_MODEL_STATE = cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState;
    static constexpr std::ptrdiff_t OFF_MESH_GROUP_MASK = cs2_dumper::schemas::client_dll::CModelState::m_MeshGroupMask;

    static constexpr std::ptrdiff_t OFF_INV_SERVICES =
        cs2_dumper::schemas::client_dll::CCSPlayerController::m_pInventoryServices;
    static constexpr std::ptrdiff_t OFF_LOADOUT_VEC =
        cs2_dumper::schemas::client_dll::CCSPlayerController_InventoryServices::m_vecNetworkableLoadout;

    // ==================== KNIFE DATA ====================
    struct KnifeData { int weapon_id; uint32_t subclass_hash; const char* model_path; };

    static const KnifeData g_knife_data[] = {
        { WEAPON_KNIFE_BAYONET,         3933374535, "weapons/models/knife/knife_bayonet/weapon_knife_bayonet.vmdl" },
        { WEAPON_KNIFE_CLASSIC,         3787235507, "weapons/models/knife/knife_css/weapon_knife_css.vmdl" },
        { WEAPON_KNIFE_FLIP,            4046390180, "weapons/models/knife/knife_flip/weapon_knife_flip.vmdl" },
        { WEAPON_KNIFE_GUT,             2047704618, "weapons/models/knife/knife_gut/weapon_knife_gut.vmdl" },
        { WEAPON_KNIFE_KARAMBIT,        1731408398, "weapons/models/knife/knife_karambit/weapon_knife_karambit.vmdl" },
        { WEAPON_KNIFE_M9_BAYONET,      1638561588, "weapons/models/knife/knife_m9/weapon_knife_m9.vmdl" },
        { WEAPON_KNIFE_TACTICAL,        2282479884, "weapons/models/knife/knife_tactical/weapon_knife_tactical.vmdl" },
        { WEAPON_KNIFE_FALCHION,        3412259219, "weapons/models/knife/knife_falchion/weapon_knife_falchion.vmdl" },
        { WEAPON_KNIFE_SURVIVAL_BOWIE,  2511498851, "weapons/models/knife/knife_bowie/weapon_knife_bowie.vmdl" },
        { WEAPON_KNIFE_BUTTERFLY,       1353709123, "weapons/models/knife/knife_butterfly/weapon_knife_butterfly.vmdl" },
        { WEAPON_KNIFE_PUSH,            4269888884, "weapons/models/knife/knife_push/weapon_knife_push.vmdl" },
        { WEAPON_KNIFE_CORD,            1105782941, "weapons/models/knife/knife_cord/weapon_knife_cord.vmdl" },
        { WEAPON_KNIFE_CANIS,           275962944,  "weapons/models/knife/knife_canis/weapon_knife_canis.vmdl" },
        { WEAPON_KNIFE_URSUS,           1338637359, "weapons/models/knife/knife_ursus/weapon_knife_ursus.vmdl" },
        { WEAPON_KNIFE_GYPSY_JACKKNIFE, 3230445913, "weapons/models/knife/knife_navaja/weapon_knife_navaja.vmdl" },
        { WEAPON_KNIFE_OUTDOOR,         3206681373, "weapons/models/knife/knife_outdoor/weapon_knife_outdoor.vmdl" },
        { WEAPON_KNIFE_STILETTO,        2595277776, "weapons/models/knife/knife_stiletto/weapon_knife_stiletto.vmdl" },
        { WEAPON_KNIFE_WIDOWMAKER,      4029975521, "weapons/models/knife/knife_talon/weapon_knife_talon.vmdl" },
        { WEAPON_KNIFE_SKELETON,        365028728,  "weapons/models/knife/knife_skeleton/weapon_knife_skeleton.vmdl" },
        { WEAPON_KNIFE_KUKRI,           3845286452, "weapons/models/knife/knife_kukri/weapon_knife_kukri.vmdl" },
    };

    static const KnifeData* GetKnifeData(int weapon_id) {
        for (const auto& kd : g_knife_data)
            if (kd.weapon_id == weapon_id) return &kd;
        return nullptr;
    }

    // ==================== ENGINE FUNCTION POINTERS ====================

    using fnSetModel = void(__fastcall*)(void*, const char*);
    using fnUpdateSubClass = void(__fastcall*)(void*);
    using fnUpdateComposite = void(__fastcall*)(void*, bool);
    using fnUpdateSkin = void(__fastcall*)(void*, bool);
    using fnClearHudWeaponIcon = void(__fastcall*)(void*, int, int64_t);
    using fnEquipItemInLoadout = bool(__fastcall*)(void*, int, int, uint64_t);
    using fnSetMeshGroupMask = void(__fastcall*)(void*, uint64_t);
    using fnUpdateCompositeSec = void(__fastcall*)(void*, bool);

    static fnSetModel           g_fnSetModel = nullptr;
    static fnUpdateSubClass     g_fnUpdateSubClass = nullptr;
    static fnUpdateComposite    g_fnUpdateComposite = nullptr;
    static fnUpdateSkin         g_fnUpdateSkin = nullptr;
    static fnUpdateCompositeSec g_fnUpdateCompositeSec = nullptr;
    static fnClearHudWeaponIcon g_fnClearHudWeaponIcon = nullptr;
    static fnEquipItemInLoadout g_fnEquipItemInLoadout = nullptr;
    static fnSetMeshGroupMask   g_fnSetMeshGroupMask = nullptr;
    static bool                 g_engine_funcs_resolved = false;

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
            if (fnUpdateSkinResolved) {
                g_fnUpdateSkin = reinterpret_cast<fnUpdateSkin>(sdk::GetCA(reinterpret_cast<uintptr_t>(fnUpdateSkinResolved)));
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

    // ==================== ENTITY FLAGS ====================
    enum e_entity_flags : uint32_t {
        ef_is_invalid_ehandle = 0x1,
        ef_spawn_in_progress = 0x2,
        ef_in_staging_list = 0x4,
        ef_in_post_data_update = 0x8,
        ef_delete_in_progress = 0x10,
        ef_in_stasis = 0x20,
    };

    struct CEntityIdentity {
        uintptr_t _pad0[2];
        uint32_t m_flags;

        uint32_t flags() const { return m_flags; }
    };

    static CEntityIdentity* GetEntityIdentity(uintptr_t ent) {
        if (!ent || !IsReadablePtr(ent)) return nullptr;
        uintptr_t identity_ptr = *(uintptr_t*)(ent + 0x10);
        return (identity_ptr && IsReadablePtr(identity_ptr)) 
            ? reinterpret_cast<CEntityIdentity*>(identity_ptr) 
            : nullptr;
    }

    static bool IsEntityInStagingList(uintptr_t ent) {
        CEntityIdentity* identity = GetEntityIdentity(ent);
        return identity && (identity->flags() & ef_in_staging_list);
    }

	// ==================== SAFE CALLERS ====================
	// Wrapper functions for game engine calls - all perform null/bounds checks
	// Prevents crashes from invalid pointers during aggressive memory writes

	// SetModel(entity, model_path) - changes 3D model for weapon
	// Critical: Must only call at stage 7 when skeleton instance (m_pSkeletonInstance) valid
	// SetModel invalidates all bone caches; next stage 7 will recalculate transforms
	// If called during skeleton NULL state, causes heap corruption
	static void CallSetModel(uintptr_t ent, const char* model) {
		if (!ent || !IsReadablePtr(ent) || !model) return;
		if (!g_fnSetModel || reinterpret_cast<uintptr_t>(g_fnSetModel) <= 0x10000) return;
		g_fnSetModel(reinterpret_cast<void*>(ent), model);
		//debug_console::Console::Get().Info("[SKINS] Called SetModel for entity %p with model %s", reinterpret_cast<void*>(ent), model);
	}

	// UpdateSubclass(entity) - rebuilds weapon subclass vftable
	// Vftable pointer at entity+OFF_WEAPON_VEH_PTR
	// After def_index change, old vftable stale; UpdateSubclass re-queries SDK registry
	static void CallUpdateSubclassSafe(uintptr_t ent) {
		if (!ent || !IsReadablePtr(ent)) return;
		if (!g_fnUpdateSubClass || reinterpret_cast<uintptr_t>(g_fnUpdateSubClass) <= 0x10000) { 
			//debug_console::Console::Get().Warning("[SKINS] UpdateSubclass function pointer invalid, cannot call UpdateSubclass for entity %p", reinterpret_cast<void*>(ent));
			return; 
		}
		g_fnUpdateSubClass(reinterpret_cast<void*>(ent));
		//debug_console::Console::Get().Info("[SKINS] Called UpdateSubclass for entity %p", reinterpret_cast<void*>(ent));
	}

	// UpdateComposite(entity, force) - flushes composite material render cache
	// force=true bypasses change detection, forces immediate recalculation
	// Must call after changing paint_kit/quality for effects to render
	static void CallUpdateComposite(uintptr_t ent, bool force) {
		if (!ent || !IsReadablePtr(ent)) return;
		if (g_fnUpdateComposite && reinterpret_cast<uintptr_t>(g_fnUpdateComposite) > 0x10000)
			g_fnUpdateComposite(reinterpret_cast<void*>(ent), force);
	}

	// UpdateSkin(entity, force) - updates skin material properties
	// Applies paint_kit texture/shader parameters to model
	static void CallUpdateSkin(uintptr_t ent, bool force) {
		if (!ent || !IsReadablePtr(ent)) return;
		if (g_fnUpdateSkin && reinterpret_cast<uintptr_t>(g_fnUpdateSkin) > 0x10000)
			g_fnUpdateSkin(reinterpret_cast<void*>(ent), force);
	}

	// ClearHudWeaponIcon(hud_selection, unk1, unk2) - removes weapon icon from HUD cache
	// unk1, unk2 appear to be flags and item index
	static void CallClearHudWeaponIcon(uintptr_t hud_weapon_selection, int unk1, int64_t unk2) {
		if (!hud_weapon_selection || !IsReadablePtr(hud_weapon_selection)) return;
		if (g_fnClearHudWeaponIcon && reinterpret_cast<uintptr_t>(g_fnClearHudWeaponIcon) > 0x10000)
			g_fnClearHudWeaponIcon(reinterpret_cast<void*>(hud_weapon_selection), unk1, unk2);
	}

	// EquipItemInLoadout(inventory_mgr, team, slot, itemID) - writes loadout to shared object cache
	// Server syncs inventory_mgr state; modifying directly bypasses server checks
	static bool CallEquipItemInLoadout(uintptr_t inventory_manager, int team, int slot, uint64_t itemID) {
		if (!inventory_manager || !IsReadablePtr(inventory_manager)) return false;
		if (g_fnEquipItemInLoadout && reinterpret_cast<uintptr_t>(g_fnEquipItemInLoadout) > 0x10000)
			return g_fnEquipItemInLoadout(reinterpret_cast<void*>(inventory_manager), team, slot, itemID);
		return false;
	}

	// ==================== SCENE NODE ====================
	// Scene node (m_pGameSceneNode) - root of entity's transform hierarchy
	// Contains model state, transform matrices, mesh groups, material data
	// Must re-fetch after SetModel call (can be deallocated/reallocated)
	static uintptr_t GetSceneNode(uintptr_t ent) {
		if (!ent || !IsReadablePtr(ent)) return 0;
		// Off-by-one check: m_pGameSceneNode offset changes per Source2 version
		uintptr_t n = *(uintptr_t*)(ent + OFF_GAME_SCENE_NODE);
		return (n && IsReadablePtr(n)) ? n : 0;
	}

	// SetMeshGroupMask(scene_node, mask) - enables/disables mesh groups for rendering
	// mask=2 typically means "visible" (0=hidden, 1=lod2, 2=normal, 3=lod0)
	// Used to show/hide weapon parts based on distance/quality
	static void SetMeshGroupMask(uintptr_t scene_node, uint64_t mask) {
		if (!scene_node || !IsReadablePtr(scene_node)) return;
		// Try vfunc call first, fallback to direct memory write
		if (g_fnSetMeshGroupMask && reinterpret_cast<uintptr_t>(g_fnSetMeshGroupMask) > 0x10000) {
			g_fnSetMeshGroupMask(reinterpret_cast<void*>(scene_node), mask);
			return;
		}
		// Direct memory write: CGameSceneNode->m_nRenderAttributeCount + OFF_MESH_GROUP_MASK
		*reinterpret_cast<uint64_t*>(scene_node + OFF_MODEL_STATE + OFF_MESH_GROUP_MASK) = mask;
	}

	// IsSetModelSafe(entity) - validates entity state before SetModel call
	// Checks: entity readable, scene node exists, vtable valid, destructor valid
	// Stage 7 guarantee: entity won't be deleted during render phase
	static bool IsSetModelSafe(uintptr_t ent) {
		if (!ent || !IsReadablePtr(ent)) return false;

		// Fetch scene node - if NULL or invalid, skeleton not ready
		uintptr_t node = GetSceneNode(ent);

        if (!node || !IsReadablePtr(node)) return false;

        uintptr_t* vptr = *reinterpret_cast<uintptr_t**>(node);
        if (!vptr || !IsReadablePtr(reinterpret_cast<uintptr_t>(vptr))) return false;

        uintptr_t fn = vptr[0];
        return (fn && IsReadablePtr(fn));
    }

    // ==================== ENTITY HELPERS ====================
    // Core entity resolution functions - walk entity hierarchy to find specific entities

    // GetLocalPawn() - fetch client.dll::dwLocalPlayerPawn pointer
    // Returns C_CSPlayerPawn entity (local player character)
    // Offset: client.dll + 0x22F0FB8 (from cs2-dumper)
    // All weapon resolution flows through this pawn pointer
    static uintptr_t GetLocalPawn() {
        uintptr_t base = (uintptr_t)GetModuleHandleA("client.dll");
        if (!base) return 0;
        uintptr_t p = *(uintptr_t*)(base + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
        return (p && IsReadablePtr(p)) ? p : 0;
    }

    // GetEntityList() - fetch client.dll::dwEntityList (entity array registry)
    // Returns pointer to entity list array
    // Offset: client.dll + 0x24ABF98 (from cs2-dumper)
    // Required by ResolveHandle to convert entity handles to pointers
    static uintptr_t GetEntityList() {
        uintptr_t base = (uintptr_t)GetModuleHandleA("client.dll");
        if (!base) return 0;
        uintptr_t l = *(uintptr_t*)(base + cs2_dumper::offsets::client_dll::dwEntityList);
        return (l && IsReadablePtr(l)) ? l : 0;
    }

    // ResolveHandle(entity_list, handle) - convert entity handle to entity pointer
    // Entity handles are 32-bit indices split across two level lookup table
    // First level: (h >> 9) * 8 + 16 (bucket index)
    // Second level: (h & 0x1FF) * 112 (entity offset within bucket)
    // Sentinel: h=0 or h=0xFFFFFFFF means invalid/unequipped
    static uintptr_t ResolveHandle(uintptr_t el, uint32_t h) {
        if (!h || h == 0xFFFFFFFF || !el) return 0;
        try {
            // First lookup: get bucket from list
            uintptr_t le = *(uintptr_t*)(el + 0x8 * ((h & 0x7FFF) >> 9) + 16);
            if (!le || !IsReadablePtr(le)) return 0;
            // Second lookup: get entity from bucket
            uintptr_t e = *(uintptr_t*)(le + 112 * (h & 0x1FF));
            return (e && IsReadablePtr(e)) ? e : 0;
        }
        catch (...) { return 0; }
    }

    // GetActiveWeapon() - resolve player's currently equipped weapon
    // Walks: pawn -> CPlayer_WeaponServices -> m_hActiveWeapon handle -> resolve to entity
    // Returns active weapon entity pointer
    // Handle stored at: pawn + m_pWeaponServices + m_hActiveWeapon offset
    static uintptr_t GetActiveWeapon() {
        uintptr_t pawn = GetLocalPawn(), list = GetEntityList();
        if (!pawn || !list) return 0;
        try {
            // Fetch weapon services from pawn
            uintptr_t ws = *(uintptr_t*)(pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pWeaponServices);
            if (!ws || !IsReadablePtr(ws)) return 0;
            // Read active weapon handle (0xFFFFFFFF if no weapon equipped)
            uint32_t h = *(uint32_t*)(ws + cs2_dumper::schemas::client_dll::CPlayer_WeaponServices::m_hActiveWeapon);
            return ResolveHandle(list, h);
        }
        catch (...) { return 0; }
    }

    // GetAllWeapons() - enumerate all weapons player owns
    // Walks: pawn -> CPlayer_WeaponServices -> m_hMyWeapons array (handles)
    // Array length fixed at 64 slots (m16_hMyWeapons is CHandle array)
    // Each handle resolved via ResolveHandle
    // Returns vector of weapon entity pointers (empty entries skipped)
    static std::vector<uintptr_t> GetAllWeapons() {
        std::vector<uintptr_t> result;
        uintptr_t pawn = GetLocalPawn(), list = GetEntityList();
        if (!pawn || !list) return result;
        try {
            // Fetch weapon services from pawn
            uintptr_t ws = *(uintptr_t*)(pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pWeaponServices);
            if (!ws || !IsReadablePtr(ws)) return result;
            // Iterate weapon handles array (64 slots, each CHandle is 4 bytes)
            uintptr_t arr = ws + cs2_dumper::schemas::client_dll::CPlayer_WeaponServices::m_hMyWeapons;
            for (int i = 0; i < 64; i++) {
                uint32_t h = *(uint32_t*)(arr + i * 4);
                if (!h || h == 0xFFFFFFFF) continue;  // Skip empty slots
                uintptr_t w = ResolveHandle(list, h);
                if (w) result.push_back(w);
            }
        }
        catch (...) {}
        return result;
    }

    // GetArmsEntity() - fetch first-person arms model entity
    // Arms stored as separate entity in pawn (third-person separates arms from body)
    // Offset: OFF_HUD_MODEL_ARMS = 0x2400 (arms entity handle)
    // Used to sync mesh visibility with weapon (when knife shown, arms shown)
    static uintptr_t GetArmsEntity() {
        uintptr_t pawn = GetLocalPawn(), list = GetEntityList();
        if (!pawn || !list) return 0;
        try {
            constexpr std::ptrdiff_t OFF_HUD_MODEL_ARMS = 0x2400;
            uint32_t h = *(uint32_t*)(pawn + OFF_HUD_MODEL_ARMS);
            return ResolveHandle(list, h);
        }
        catch (...) { return 0; }

    }

    // GetDefIndex(weapon) - read m_iItemDefinitionIndex from weapon entity
    // def_index determines weapon type (530=knife, 533=ghost knife, etc)
    // Offset: weapon + OFF_ITEM_DEF_INDEX (C_BaseEntity::m_iItemDefinitionIndex)
    static uint16_t GetDefIndex(uintptr_t w) {
        return *reinterpret_cast<uint16_t*>(w + OFF_ITEM_DEF_INDEX);
    }

    // GetLocalTeam() - read team number from player pawn (TEAM_T=2, TEAM_CT=3)
    // Off_TEAM_NUM: C_BaseEntity::m_iTeamNum (1=unassigned, 2=terrorist, 3=counter-terrorist)
    // Used to lookup team-specific loadout items
    static uint8_t GetLocalTeam() {
        uintptr_t p = GetLocalPawn();
        return p ? *(uint8_t*)(p + OFF_TEAM_NUM) : 0;
    }

    // ==================== INVENTORY BACKING ====================
    // Tracks custom items added via UI (item IDs, paint kits, etc)
    struct AddedItemInfo {
        uint64_t id;        // 64-bit inventory item ID (unique per user/item)
        float    paintKit;  // Paint kit ID (0=default, 1-1000=various skins)
        float    paintSeed; // Random seed for texture generation (0-1000)
        float    paintWear; // Wear factor (0.0=factory new, 1.0=well worn)
        bool     legacy;    // Whether to use legacy paint system (fallback)
    };

    static std::vector<AddedItemInfo> g_vecAddedItemsIDs;

    // AddEconItemToList - register custom item to inventory system
    // Prevents duplicate entries (removes old entry before adding)
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

    static const LoadoutItem* FindLoadoutByDefIndex(const std::vector<LoadoutItem>& lo, uint16_t def, uint8_t team) {
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
    void ApplyWeaponSkins() {
        ResolveEngineFunctions();
        if (!game_state::IsInGame()) return;

        uintptr_t weapon = GetActiveWeapon();
        if (!weapon) return;

        uint16_t def_index = GetDefIndex(weapon);
        if (IsKnife(def_index) || IsDefaultKnife(def_index)) return;

        std::vector<LoadoutItem> loadout;
        try { loadout = ReadLoadout(); }
        catch (...) {}
        uint8_t team = GetLocalTeam();

        const LoadoutItem* li = FindLoadoutByDefIndex(loadout, def_index, team);
        if (!li || li->item_id == 0) return;

        auto it = std::find_if(g_vecAddedItemsIDs.begin(), g_vecAddedItemsIDs.end(),
            [&](const AddedItemInfo& i) { return i.id == li->item_id; });
        if (it == g_vecAddedItemsIDs.end()) return;

        const AddedItemInfo& info = *it;

        *reinterpret_cast<uint64_t*>(weapon + OFF_ITEM_ID) = li->item_id;
        *reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_HIGH) = li->item_id_high;
        *reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_LOW) = li->item_id_low;
        *reinterpret_cast<uint32_t*>(weapon + OFF_ACCOUNT_ID) = li->account_id;
        *reinterpret_cast<bool*>(weapon + OFF_DISALLOW_SOC) = false;
        *reinterpret_cast<bool*>(weapon + OFF_RESTORE_MATERIAL) = true;

        if (info.legacy) {
            *reinterpret_cast<int*>(weapon + OFF_FALLBACK_PAINT) = (int)info.paintKit;
            *reinterpret_cast<int*>(weapon + OFF_FALLBACK_SEED) = (int)info.paintSeed;
            *reinterpret_cast<float*>(weapon + OFF_FALLBACK_WEAR) = info.paintWear;
            *reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_HIGH) = (uint32_t)-1;
            *reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_LOW) = (uint32_t)-1;
        }

        uintptr_t node = GetSceneNode(weapon);
        if (node) SetMeshGroupMask(node, info.legacy ? 2ULL : 1ULL);

        try { CallUpdateComposite(weapon, true); }
        catch (...) {}
        try {
            if (g_fnUpdateCompositeSec && reinterpret_cast<uintptr_t>(g_fnUpdateCompositeSec) > 0x10000)
                g_fnUpdateCompositeSec(reinterpret_cast<void*>(weapon), true);
        }
        catch (...) {}
    }

	// ==================== FRAME STAGE TRACKING ====================
	// Frame stage is passed via FrameStageNotify hook callback (see Hook_FrameStageNotify.cpp)
	// Thread-local storage avoids race conditions in multithreaded render pipeline
	// Only stage 7 (POST_RENDER_END) is safe for SetModel and skeleton operations
	thread_local static int t_currentFrameStage = -1;

	int GetCurrentFrameStage() {
		return t_currentFrameStage;
	}

	void SetCurrentFrameStage(int stage) {
		t_currentFrameStage = stage;
	}

	// ==================== APPLY KNIFE SKINS ====================
	// 
	// [CRITICAL] Called ONLY from FrameStageNotify stage 7 callback 
	// (registered in hooks.cpp via RegisterOnFrameStageNotify)
	//
	// Algorithm:
	// 1. Detect new weapon pointer (weapon swap/equip) - reset all counters, skip frame
	// 2. Cheap writes every frame: item IDs, paint kit, def_index (server resets each tick)
	// 3. Type change detection: if def_index differs, trigger expensive operation burst
	// 4. Execute model update sequence: UpdateSubclass → SetMeshGroupMask → UpdateComposite → SetModel
	//
	// Stage 7 guarantee: Entity won't be deleted (post-render, pre-cleanup)
	// Skeleton instance guaranteed valid (created at stage 5, valid until cleanup)
	// Scene node may be reallocated - must re-fetch after SetModel
	//
	// Burst counters (s_subclassRefreshFrames, s_compositeRefreshFrames):
	// Old implementation - not used anymore, kept for future reference
	// New: s_setModelDelayFrames allows UpdateSubclass cache to settle before SetModel

	// Static state tracking
	static uint16_t  s_lastKnifeDefIndex = 0;      // Previous def_index to detect type changes
	static uintptr_t s_lastWeaponPtr = 0;          // Track weapon entity pointer
	static int       s_lastHealth = 0;              // UNUSED - kept for reference
	static int       s_subclassRefreshFrames = 0;  // UNUSED - kept for reference (old burst logic)
	static int       s_compositeRefreshFrames = 0; // UNUSED - kept for reference (old burst logic)
	static int       s_respawnDelayFrames = 0;     // UNUSED - kept for reference
	static bool      s_engineFunctionsResolved = false; // One-time function pointer resolution
	static int       s_setModelDelayFrames = 0;    // Delay SetModel 1 frame after UpdateSubclass

    static int should_apply_set_model = true;
	void ApplyKnifeSkins() {
		// Resolve function pointers on first call: g_fnSetModel, g_fnUpdateSubClass, etc
		// These are pattern-scanned from client.dll in ResolveEngineFunctions()

       
		if (!s_engineFunctionsResolved) {
			ResolveEngineFunctions();
			s_engineFunctionsResolved = true;
		}
		if (!game_state::IsInGame()) return;

		// [SAFETY CHECK] This function should ONLY be called at stage 7
		// If called at wrong stage, bail immediately (prevents skeleton NULL corruption)
		if (t_currentFrameStage != 7) {
			return;
		}

		// Fetch active weapon (knife) entity pointer from player's weapon service
		// Entity address may change on weapon swap/equip
		uintptr_t weapon = GetActiveWeapon();
		if (!weapon) return;

		// Read m_iItemDefinitionIndex from entity
		// Offset: weapon + OFF_ITEM_DEF_INDEX (from cs2-dumper)
		uint16_t def_index = GetDefIndex(weapon);
		if (!IsKnife(def_index) && !IsDefaultKnife(def_index)) return;

		// ========== NEW WEAPON DETECTION ==========
		// When player equips new weapon or swaps: weapon pointer changes
		// Must reset all state and skip this frame (next frame safe to process)
		// Prevents calling functions on stale/reallocated entity
		if (weapon != s_lastWeaponPtr) {
			s_lastKnifeDefIndex = 0;      // Force type-change detection
			s_subclassRefreshFrames = 0;
			s_compositeRefreshFrames = 0;
			s_setModelDelayFrames = 0;
            should_apply_set_model = true;
			s_lastWeaponPtr = weapon;
			return;  // Skip processing this frame - wait for stable state
		}

		// Read shared object loadout (inventory manager)
		// Contains synchronized weapon configs (paint kits, item IDs, etc)
		std::vector<LoadoutItem> loadout;
		try { loadout = ReadLoadout(); }
		catch (...) {}
		uint8_t team = GetLocalTeam();

		// Determine target knife def_index (from loadout or user selection)
		uint16_t knifeDefIndex = 0;
		const LoadoutItem* melee = FindMeleeItem(loadout, team);
		if (melee && melee->def_index >= 500)
			knifeDefIndex = melee->def_index;
		else if (selected_knife_id >= 500)
			knifeDefIndex = static_cast<uint16_t>(selected_knife_id);
		else
			return;

		// Fetch knife metadata: model path, subclass hash, etc
		const KnifeData* kd = GetKnifeData(knifeDefIndex);
		if (!kd) return;

		// ========== CHEAP WRITES: Every frame ==========
		// Item ID and ownership (accountID) written every frame
		// Server may reset these on next tick, so persistent writes necessary
		// Off_ITEM_ID: C_EconItemView::m_iItemID (64-bit)
		// Off_ITEM_ID_HIGH/LOW: Split 64-bit ID across 32-bit reads (inventory syncing)
		// Off_ACCOUNT_ID: C_EconItemView::m_iAccountID (owner of item)
		// Off_DISALLOW_SOC: m_bDisallowSOC (block server-side modification)
		// Off_RESTORE_MATERIAL: m_bRestoreCustomMaterialAfterPrecache (reload composite on next UpdateComposite)
		if (melee && melee->item_id != 0) {
			// Loadout item exists: write its IDs
			*reinterpret_cast<uint64_t*>(weapon + OFF_ITEM_ID) = melee->item_id;
			*reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_HIGH) = melee->item_id_high;
			*reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_LOW) = melee->item_id_low;
			*reinterpret_cast<uint32_t*>(weapon + OFF_ACCOUNT_ID) = melee->account_id;
		} else {
			// No loadout item: write sentinel values (server won't override)
			*reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_HIGH) = (uint32_t)-1;
			*reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_LOW) = (uint32_t)-1;
		}
		// Always allow engine to override material/SOC flags
		*reinterpret_cast<bool*>(weapon + OFF_DISALLOW_SOC) = false;
		*reinterpret_cast<bool*>(weapon + OFF_RESTORE_MATERIAL) = true;

		// Paint kit / skin config: lookup user's selected paint for this knife
		// Off_FALLBACK_PAINT: C_EconItemView::m_nFallbackPaintKit (legacy paint kit ID)
		// Off_FALLBACK_WEAR: C_EconItemView::m_flFallbackWear (0.0-1.0 wear factor)
		// Off_FALLBACK_SEED: C_EconItemView::m_nFallbackSeed (RNG seed for texture generation)
		// Off_FALLBACK_STATTRAK: C_EconItemView::m_nFallbackStatTrak (stat trak count, -1=disabled)
		auto skin_cfg = user_skins.find(knifeDefIndex);
		if (skin_cfg != user_skins.end()) {
			*reinterpret_cast<int*>(weapon + OFF_FALLBACK_PAINT) = skin_cfg->second.paint_kit;
			*reinterpret_cast<float*>(weapon + OFF_FALLBACK_WEAR) = skin_cfg->second.wear;
			*reinterpret_cast<int*>(weapon + OFF_FALLBACK_SEED) = skin_cfg->second.seed;
			*reinterpret_cast<int*>(weapon + OFF_FALLBACK_STATTRAK) =
				skin_cfg->second.stattrak ? skin_cfg->second.stattrak_count : -1;
		}

		// ========== SUBCLASS + DEF_INDEX: Rewrite every frame ==========
		// Server resets m_iItemDefinitionIndex each tick (used for weapon selection logic)
		// Subclass hash (C_BaseEntity::m_pNSubclassID + 0x08) must match def_index for vftable lookup
		// After these writes, UpdateSubclass() must be called to re-query SDK registry
		// (Wrong vftable = wrong material/model system behavior)
		*reinterpret_cast<uint16_t*>(weapon + OFF_ITEM_DEF_INDEX) = knifeDefIndex;
		*reinterpret_cast<uint32_t*>(weapon + OFF_SUBCLASS_ID) = kd->subclass_hash;

		// ========== TYPE CHANGE DETECTION ==========
		// If def_index differs from last frame, trigger expensive operations
		// Sets counter to 1, which delays SetModel by 1 frame (allows UpdateSubclass cache to settle)
		if (s_lastKnifeDefIndex != knifeDefIndex) {
			s_subclassRefreshFrames = 5;  // UNUSED - legacy burst logic
			s_compositeRefreshFrames = 3; // UNUSED - legacy burst logic
			s_setModelDelayFrames = 1;    // Delay SetModel 1 frame

			s_lastKnifeDefIndex = knifeDefIndex;
		}

		// ========== STAGE 7 EXECUTION: Model Update Sequence ==========
		// Critical order: UpdateSubclass -> SetMeshGroupMask -> UpdateComposite -> SetModel
		// Each step depends on previous (UpdateSubclass invalidates vftable, UpdateComposite reads new vtable)
		// SetModel must be LAST because it reallocates scene node

		// Decrement delay counter until SetModel ready
		if (s_setModelDelayFrames > 0) {
			s_setModelDelayFrames--;
		}

		// When delay elapsed and subclass refresh >= 0: execute full sequence
		if (s_setModelDelayFrames == 0 && s_subclassRefreshFrames >= 0) {
			// Step 1: UpdateSubclass (vfunc 0 on entity vtable)
			// Rebuilds vftable pointer from SDK registry (m_pNSubclassID->vtable)
			// Critical: old vtable stale after def_index change
			try {
				CallUpdateSubclassSafe(weapon);
			} catch (...) { }

			// Step 2: SetMeshGroupMask for weapon (visibility control)
			// mask=2 enables rendering (0=hidden, 1=lod2, 2=normal, 3=lod0)
			// Ensures weapon parts visible after model change
			try {
				uintptr_t node = GetSceneNode(weapon);
				if (node) {
					SetMeshGroupMask(node, 2);
				}
			} catch (...) { }

			// Step 2b: SetMeshGroupMask for player arms model
			// Arms are separate entity (first-person hands)
			// Must also show arms when knife displayed
			try {
				uintptr_t arms = GetArmsEntity();
				if (arms) {
					uintptr_t arms_node = GetSceneNode(arms);
					if (arms_node) {
						SetMeshGroupMask(arms_node, 2);
					}
				}
			} catch (...) { }

			// Step 3: UpdateComposite vfunc 7 (material composite rebuild)
			// Rebuilds material composite from new subclass vftable
			// Reads paint_kit values set earlier and generates textures
			/*try {
				sdk::CallVFunc<7, void*>(reinterpret_cast<void*>(weapon), 1);
			} catch (...) { }*/

			// Step 4: UpdateComposite vfunc 105 (secondary material update)
			// Some Source2 entities use vfunc 105 as secondary update
			// Ensures all material LOD variants updated
			/*try {
				sdk::CallVFunc<105, void*>(reinterpret_cast<void*>(weapon), 1);
			} catch (...) { }*/

			// Step 5: SetModel (LAST - after all updates)
			// Changes 3D model pointer (m_pModelData)
			// Invalidates all bone caches; next render recalculates transforms
			// Must be last because scene node may be deallocated/reallocated
			// Re-fetch scene node after this call if further operations needed
			try {

				if (should_apply_set_model && IsSetModelSafe(weapon)) {

					CallSetModel(weapon, kd->model_path);
					should_apply_set_model = false;
				}
			} catch (...) { }
		}
	}

    // ==================== APPLY GLOVES ====================
    // Gloves are stored directly on player pawn, not in weapon list
    // Offset: C_CSPlayerPawn::m_EconGloves (C_EconItemView structure)
    // Glove def_index always 5028, paint kit applied via material system
    void ApplyGloves() {
        if (!game_state::IsInGame()) return;
        uintptr_t pawn = GetLocalPawn();
        if (!pawn) return;

        // Gloves stored inline on pawn (not separate entity like weapons)
        uintptr_t gv = pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_EconGloves;

        std::vector<LoadoutItem> loadout;
        try { loadout = ReadLoadout(); }
        catch (...) {}
        uint8_t team = GetLocalTeam();

        // Find gloves in loadout (slot 41 = glove slot)
        const LoadoutItem* gl = nullptr;
        for (const auto& li : loadout)
            if (li.team == team && li.slot == 41) { gl = &li; break; }

        // Apply gloves if found in loadout
        if (gl && gl->item_id != 0) {
            *reinterpret_cast<uint16_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex) = gl->def_index;
            *reinterpret_cast<uint64_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemID) = gl->item_id;
            *reinterpret_cast<uint32_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh) = gl->item_id_high;
            *reinterpret_cast<uint32_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDLow) = gl->item_id_low;
            *reinterpret_cast<uint32_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iAccountID) = gl->account_id;
            *reinterpret_cast<int*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iEntityQuality) = 3;
            *reinterpret_cast<bool*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_bDisallowSOC) = false;
            *reinterpret_cast<bool*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_bRestoreCustomMaterialAfterPrecache) = true;
        }
        // Fallback to generic gloves if user selected paint kit
        else if (selected_glove_kit > 0) {
            *reinterpret_cast<uint16_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex) = 5028;
            *reinterpret_cast<uint32_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh) = (uint32_t)-1;
            *reinterpret_cast<uint32_t*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDLow) = (uint32_t)-1;
            *reinterpret_cast<int*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iEntityQuality) = 3;
            *reinterpret_cast<bool*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_bDisallowSOC) = false;
            *reinterpret_cast<bool*>(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_bRestoreCustomMaterialAfterPrecache) = true;
        }
        else return;

        // Flag glove system to reapply material on next render
        *reinterpret_cast<bool*>(pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_bNeedToReApplyGloves) = true;
    }

    static auto g_last_apply = std::chrono::steady_clock::now();

    // ==================== MAIN TICK ====================
    // ApplyAllSkins() - called every frame from main game loop
    // Runs weapon skin system (gloves, equipment skins, etc)
    // 
    // [IMPORTANT] ApplyKnifeSkins is NOT called here anymore
    // Knife skins are now handled by FrameStageNotify stage 7 callback only
    // (registered in hooks.cpp via RegisterOnFrameStageNotify)
    // Calling it here would break stage-aware execution and cause SetModel at wrong stage
    void ApplyAllSkins() {
        if (!game_state::IsInGame()) return;

        // NOTE: ApplyKnifeSkins is now called ONLY from FrameStageNotify stage 7 callback
        // (via Hook_FrameStageNotify.cpp RegisterOnFrameStageNotify)
        // Do NOT call it here — it breaks stage-aware execution!

        uintptr_t localPawn = GetLocalPawn();
        int health = 0;
        if (localPawn)
            SafeReadInt(localPawn + OFF_HEALTH, health);

        if (health <= 0) {
            should_apply_set_model = true;
            return;
        }

        auto now = std::chrono::steady_clock::now();
        bool should_apply =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - g_last_apply).count() >= 600;
        if (!should_apply) return;
        g_last_apply = now;

        // ---- Death detection: reset knife state ----
        if (s_lastHealth > 0 && health <= 0) {
            s_lastKnifeDefIndex = 0;
            s_lastWeaponPtr = 0;
            s_subclassRefreshFrames = 0;
            s_compositeRefreshFrames = 0;
            s_respawnDelayFrames = 3;
            debug_console::Console::Get().Info(
                "[KNIFE] Death detected (health %d -> %d) — delaying knife apply for %d frames",
                s_lastHealth, health, s_respawnDelayFrames);
        }
        s_lastHealth = health;



        if (s_respawnDelayFrames > 0) {
            s_respawnDelayFrames--;
            debug_console::Console::Get().Debug(
                "[KNIFE] Respawn delay active, frames remaining: %d", s_respawnDelayFrames);
            return;
        }



        /*try { ApplyWeaponSkins(); } catch (...) {}*/
    }

    void ApplyKnife() {
        should_apply_set_model = true;
        s_lastKnifeDefIndex = 0;
        s_lastWeaponPtr = 0;
        s_subclassRefreshFrames = 0;
        s_compositeRefreshFrames = 0;
        s_respawnDelayFrames = 0;
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
                int  wid = ExtractInt(obj, "weapon_defindex");
                int  pk = ExtractInt(obj, "paint");
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
                int  pk = ExtractInt(obj, "paint");
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
        user_skins.clear();
        g_vecAddedItemsIDs.clear();
        selected_glove_kit = 0;
        s_lastKnifeDefIndex = 0;
        s_lastWeaponPtr = 0;
        s_subclassRefreshFrames = 0;
        s_compositeRefreshFrames = 0;
        s_respawnDelayFrames = 0;
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
        case WEAPON_AK47:                  return "AK-47";
        case WEAPON_M4A1:                  return "M4A4";
        case WEAPON_M4A1_SILENCER:         return "M4A1-S";
        case WEAPON_AWP:                   return "AWP";
        case WEAPON_DEAGLE:                return "Desert Eagle";
        case WEAPON_GLOCK:                 return "Glock-18";
        case WEAPON_USP_SILENCER:          return "USP-S";
        case WEAPON_P250:                  return "P250";
        case WEAPON_FIVESEVEN:             return "Five-SeveN";
        case WEAPON_TEC9:                  return "Tec-9";
        case WEAPON_CZ75A:                 return "CZ75-Auto";
        case WEAPON_REVOLVER:              return "R8 Revolver";
        case WEAPON_ELITE:                 return "Dual Berettas";
        case WEAPON_HKP2000:               return "P2000";
        case WEAPON_NOVA:                  return "Nova";
        case WEAPON_XM1014:                return "XM1014";
        case WEAPON_SAWEDOFF:              return "Sawed-Off";
        case WEAPON_MAG7:                  return "MAG-7";
        case WEAPON_M249:                  return "M249";
        case WEAPON_NEGEV:                 return "Negev";
        case WEAPON_MAC10:                 return "MAC-10";
        case WEAPON_MP9:                   return "MP9";
        case WEAPON_MP7:                   return "MP7";
        case WEAPON_MP5SD:                 return "MP5-SD";
        case WEAPON_UMP45:                 return "UMP-45";
        case WEAPON_P90:                   return "P90";
        case WEAPON_BIZON:                 return "PP-Bizon";
        case WEAPON_FAMAS:                 return "FAMAS";
        case WEAPON_GALILAR:               return "Galil AR";
        case WEAPON_AUG:                   return "AUG";
        case WEAPON_SG556:                 return "SG 553";
        case WEAPON_SSG08:                 return "SSG 08";
        case WEAPON_SCAR20:                return "SCAR-20";
        case WEAPON_G3SG1:                 return "G3SG1";
        case WEAPON_KNIFE_BAYONET:         return "Bayonet";
        case WEAPON_KNIFE_CLASSIC:         return "Classic Knife";
        case WEAPON_KNIFE_FLIP:            return "Flip Knife";
        case WEAPON_KNIFE_GUT:             return "Gut Knife";
        case WEAPON_KNIFE_KARAMBIT:        return "Karambit";
        case WEAPON_KNIFE_M9_BAYONET:      return "M9 Bayonet";
        case WEAPON_KNIFE_TACTICAL:        return "Huntsman Knife";
        case WEAPON_KNIFE_FALCHION:        return "Falchion Knife";
        case WEAPON_KNIFE_SURVIVAL_BOWIE:  return "Bowie Knife";
        case WEAPON_KNIFE_BUTTERFLY:       return "Butterfly Knife";
        case WEAPON_KNIFE_PUSH:            return "Shadow Daggers";
        case WEAPON_KNIFE_CORD:            return "Paracord Knife";
        case WEAPON_KNIFE_CANIS:           return "Survival Knife";
        case WEAPON_KNIFE_URSUS:           return "Ursus Knife";
        case WEAPON_KNIFE_GYPSY_JACKKNIFE: return "Navaja Knife";
        case WEAPON_KNIFE_OUTDOOR:         return "Nomad Knife";
        case WEAPON_KNIFE_STILETTO:        return "Stiletto Knife";
        case WEAPON_KNIFE_WIDOWMAKER:      return "Talon Knife";
        case WEAPON_KNIFE_SKELETON:        return "Skeleton Knife";
        case WEAPON_KNIFE_KUKRI:           return "Kukri Knife";
        case WEAPON_KNIFE_CT:              return "CT Knife";
        case WEAPON_KNIFE_T:               return "T Knife";
        case WEAPON_TASER:                 return "Zeus x27";
        default:                           return "Unknown";
        }
    }

    const char* GetRarityName(SkinRarity r) {
        switch (r) {
        case RARITY_COMMON:     return "Consumer Grade";
        case RARITY_UNCOMMON:   return "Industrial Grade";
        case RARITY_RARE:       return "Mil-Spec Grade";
        case RARITY_MYTHICAL:   return "Restricted";
        case RARITY_LEGENDARY:  return "Classified";
        case RARITY_ANCIENT:    return "Covert";
        case RARITY_CONTRABAND: return "Contraband";
        default:                return "Unknown";
        }
    }

    const float* GetRarityColor(SkinRarity r) {
        static float c[][4] = {
            {0.7f,0.7f,0.7f,1.f},  {0.4f,0.6f,0.9f,1.f}, {0.3f,0.4f,0.8f,1.f},
            {0.5f,0.3f,0.8f,1.f},  {0.8f,0.3f,0.6f,1.f}, {0.9f,0.2f,0.2f,1.f},
            {0.95f,0.8f,0.1f,1.f}
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