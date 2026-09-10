#include "skins.h"
#include "interfaces.h"
#include "game_state.h"
#include "../sdk/mem.h"
#include "../sdk/feature_support.h"
#include <cmath>
#include "pattern_resolver.h"
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

// DO NOT CHANGE THESE MACROS MANUALLY UPDATED

// C_BaseModelEntity_SetModel(C_BaseModelEntity* entity, const char* modelSzName);
#define SET_MODEL_SIGNATURE "40 53 48 83 EC ? 48 8B D9 4C 8B C2 48 8B 0D ? ? ? ? 48 8D 54 24 40"
#define SEARCH_TYPE_SET_MODEL NORMAL_SCAN
#define LOCATION_SET_MODEL CLIENT

#define SET_MESH_GROUP_MASK_SIGNATURE "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8D 99"
#define SEARCH_TYPE_SET_MESH_GROUP_MASK NORMAL_SCAN
#define LOCATION_SET_MESH_GROUP_MASK CLIENT

// C_CSWeaponBase_UpdateSubclass(C_CSWeaponBase* weapon);
#define UPDATE_SUBCLASS_SIGNATURE "4C 8B DC 53 48 81 EC ? ? ? ? 48 8B 41 10 48 8B D9 8B 50 30 C1 EA 04"
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
#define CLEAR_HUD_WEAPON_ICON_SIGNATURE "E8 ? ? ? ? 8B F8 C6 84 24 ? ? ? ? ?"
#define SEARCH_TYPE_CLEAR_HUD_WEAPON_ICON CALL_SCAN
#define LOCATION_CLEAR_HUD_WEAPON_ICON CLIENT

// C_EconItemView_GetCustomPaintKitIndex(C_EconItemView* thisptr);
#define GET_CUSTOM_PAINTKIT_INDEX "48 89 5C 24 ? 57 48 83 EC ? 8B 15 ? ? ? ? 48 8B F9 65 48 8B 04 25 ? ? ? ? B9 ? ? ? ? 48 8B 04 D0 8B 04 01 39 05 ? ? ? ? 0F 8F ? ? ? ? E8 ? ? ? ? 8B 58 ? 39 1D ? ? ? ? 74 ? E8 ? ? ? ? 48 8B 15 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 89 05 ? ? ? ? 89 1D ? ? ? ? EB ? 48 8B 05 ? ? ? ? 48 85 C0 74"
#define SEARCH_TYPE_GET_CUSTOM_PAINTKIT_INDEX NORMAL_SCAN
#define LOCATION_GET_CUSTOM_PAINTKIT_INDEX CLIENT

#define EQUIP_ITEM_IN_LOADOUT_SIGNATURE "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 89 54 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 0F B7 FA"
#define SEARCH_TYPE_EQUIP_ITEM_IN_LOADOUT NORMAL_SCAN
#define LOCATION_EQUIP_ITEM_IN_LOADOUT CLIENT

#define REGEN_WEAPON_SKINS_SIGNATURE "40 55 53 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 44 0F B6 FA 48 8B D9 BA ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ?"
#define SEARCH_TYPE_REGEN_WEAPON_SKINS NORMAL_SCAN
#define LOCATION_REGEN_WEAPON_SKINS CLIENT

#define UPDATE_BODY_GROUP_CHOICE "E8 ? ? ? ? 4C 8B AC 24 ? ? ? ? 48 8B BC 24"
#define SEARCH_TYPE_UPDATE_BODY_GROUP_CHOICE CALL_SCAN
#define LOCATION_UPDATE_BODY_GROUP_CHOICE CLIENT

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
        out = sdk::read_value<int>(addr);
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
    using fnClearHudWeaponIcon = void(__fastcall*)(void*, int, int64_t);
    using fnEquipItemInLoadout = bool(__fastcall*)(void*, int, int, uint64_t);
    using fnSetMeshGroupMask = void(__fastcall*)(void*, uint64_t);
    using fnUpdateCompositeSec = void(__fastcall*)(void*, bool);

    static fnSetModel           g_fnSetModel = nullptr;
    static fnUpdateSubClass     g_fnUpdateSubClass = nullptr;
    static fnUpdateComposite    g_fnUpdateComposite = nullptr;
    static uintptr_t g_compositeOwnerOffset = 0;
    static fnUpdateCompositeSec g_fnUpdateCompositeSec = nullptr;
    static fnClearHudWeaponIcon g_fnClearHudWeaponIcon = nullptr;
    static fnEquipItemInLoadout g_fnEquipItemInLoadout = nullptr;
    static fnSetMeshGroupMask   g_fnSetMeshGroupMask = nullptr;
    using fnSetAttribute = void(__fastcall*)(void*, const char*, float);
    static fnSetAttribute g_fnSetAttribute = nullptr;
    static bool                 g_engine_funcs_resolved = false;

    // FindHudElement function pointer
    static void* (*g_fnFindHudElement)(const char* szHudName) = nullptr;

    static void ResolveFindHudElement() {
        if (g_fnFindHudElement) return;
        uint8_t* pattern = sdk::find_pattern("client.dll", "4C 8B DC 53 48 83 EC ? 48 8B 05");
        if (pattern) {
            g_fnFindHudElement = reinterpret_cast<void* (*)(const char*)>(pattern);
            debug_console::Console::Get().Success("[SKINS] FindHudElement at 0x%llX", (uintptr_t)pattern);
        }
        else {
            debug_console::Console::Get().Warning("[SKINS] FindHudElement pattern NOT found");
        }
    }

    static std::uint8_t* TryPattern(const char* module, const char* pattern) {
        try { return sdk::find_pattern(module, pattern); }
        catch (...) { return nullptr; }
    }

    static void ResolveEngineFunctions() {
        if (g_engine_funcs_resolved) return;
        g_engine_funcs_resolved = true;
        auto& con = debug_console::Console::Get();

        auto fnSetModelResolved = sdk::find_pattern("client.dll", SET_MODEL_SIGNATURE);
        if (fnSetModelResolved) {
            g_fnSetModel = reinterpret_cast<fnSetModel>(fnSetModelResolved);
            con.Success("[SKINS] SetModel at 0x%llX", reinterpret_cast<uintptr_t>(fnSetModelResolved));
        }
        else con.Warning("[SKINS] SetModel NOT found");

        auto fnUpdateSubClassResolved = sdk::find_pattern("client.dll", UPDATE_SUBCLASS_SIGNATURE);
        if (fnUpdateSubClassResolved) {
            g_fnUpdateSubClass = reinterpret_cast<fnUpdateSubClass>(fnUpdateSubClassResolved);
            con.Success("[SKINS] UpdateSubclass at 0x%llX", reinterpret_cast<uintptr_t>(fnUpdateSubClassResolved));
        }
        else con.Warning("[SKINS] UpdateSubclass NOT found");

        auto fnUpdateCompositeResolved = sdk::find_pattern("client.dll", UPDATE_COMPOSITE_MATERIAL_SIGNATURE);
        if (fnUpdateCompositeResolved) {
            // The call site adds the embedded material owner before calling.
            const auto call = reinterpret_cast<uintptr_t>(fnUpdateCompositeResolved);
            unsigned char prefix[9]{};
            if (sdk::read_memory(call - 9, prefix) && prefix[0] == 0x48 &&
                prefix[1] == 0x81 && prefix[2] == 0xC1 && prefix[7] == 0xB2 && prefix[8] == 1) {
                const auto offset = sdk::read_value<uint32_t>(call - 6);
                if (offset && offset < 0x10000) g_compositeOwnerOffset = offset;
            }
            g_fnUpdateComposite = reinterpret_cast<fnUpdateComposite>(sdk::GetCA(reinterpret_cast<uintptr_t>(fnUpdateCompositeResolved)));
            con.Success("[SKINS] UpdateComposite at 0x%llX", reinterpret_cast<uintptr_t>(fnUpdateCompositeResolved));
        }
        else con.Warning("[SKINS] UpdateComposite NOT found");

        g_fnUpdateCompositeSec = reinterpret_cast<fnUpdateCompositeSec>(
            sdk::find_pattern("client.dll", REGEN_WEAPON_SKINS_SIGNATURE));
        if (!g_compositeOwnerOffset) con.Warning("[SKINS] Material owner layout unavailable; refresh disabled");

        auto fnClearHudWeaponIconResolved = sdk::find_pattern("client.dll", CLEAR_HUD_WEAPON_ICON_SIGNATURE);
        if (fnClearHudWeaponIconResolved) {
            g_fnClearHudWeaponIcon = reinterpret_cast<fnClearHudWeaponIcon>(sdk::GetCA(reinterpret_cast<uintptr_t>(fnClearHudWeaponIconResolved)));
            con.Success("[SKINS] ClearHudWeaponIcon at 0x%llX", reinterpret_cast<uintptr_t>(g_fnClearHudWeaponIcon));
        }
        else con.Warning("[SKINS] ClearHudWeaponIcon NOT found");

        auto fnEquipItemInLoadoutResolved = sdk::find_pattern("client.dll", EQUIP_ITEM_IN_LOADOUT_SIGNATURE);
        if (fnEquipItemInLoadoutResolved) {
            g_fnEquipItemInLoadout = reinterpret_cast<fnEquipItemInLoadout>(fnEquipItemInLoadoutResolved);
            con.Success("[SKINS] EquipItemInLoadout at 0x%llX", reinterpret_cast<uintptr_t>(fnEquipItemInLoadoutResolved));
        }
        else con.Warning("[SKINS] EquipItemInLoadout NOT found");

        g_fnSetMeshGroupMask = reinterpret_cast<fnSetMeshGroupMask>(sdk::find_pattern("client.dll", SET_MESH_GROUP_MASK_SIGNATURE));
        if (auto call = sdk::find_pattern("client.dll", "E8 ? ? ? ? 66 41 0F 6E D4"))
            g_fnSetAttribute = reinterpret_cast<fnSetAttribute>(sdk::GetCA(reinterpret_cast<uintptr_t>(call)));
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
        uintptr_t _pad0[6];
        uint32_t m_flags;
        uint32_t flags() const { return sdk::read_value<uint32_t>(reinterpret_cast<uintptr_t>(this) + 0x30); }
    };

    static CEntityIdentity* GetEntityIdentity(uintptr_t ent) {
        if (!ent || !IsReadablePtr(ent)) return nullptr;
        uintptr_t identity_ptr = sdk::read_value<uintptr_t>(ent + 0x10);
        return (identity_ptr && IsReadablePtr(identity_ptr))
            ? reinterpret_cast<CEntityIdentity*>(identity_ptr)
            : nullptr;
    }

    static bool IsEntityInStagingList(uintptr_t ent) {
        CEntityIdentity* identity = GetEntityIdentity(ent);
        return identity && (identity->flags() & ef_in_staging_list);
    }

    // ==================== SAFE CALLERS ====================
    static void CallSetModel(uintptr_t ent, const char* model) {
        if (!ent || !IsReadablePtr(ent) || !model) return;
        if (!g_fnSetModel || reinterpret_cast<uintptr_t>(g_fnSetModel) <= 0x10000) return;
        g_fnSetModel(reinterpret_cast<void*>(ent), model);
        debug_console::Console::Get().Info("[SKINS] Called SetModel for entity %p with model %s", reinterpret_cast<void*>(ent), model);
    }

    static void CallUpdateSubclassSafe(uintptr_t ent) {
        if (!ent || !IsReadablePtr(ent)) return;
        if (!g_fnUpdateSubClass || reinterpret_cast<uintptr_t>(g_fnUpdateSubClass) <= 0x10000) {
            debug_console::Console::Get().Warning("[SKINS] UpdateSubclass function pointer invalid, cannot call UpdateSubclass for entity %p", reinterpret_cast<void*>(ent));
            return;
        }
        g_fnUpdateSubClass(reinterpret_cast<void*>(ent));
        debug_console::Console::Get().Info("[SKINS] Called UpdateSubclass for entity %p", reinterpret_cast<void*>(ent));
    }

    static void CallUpdateComposite(uintptr_t ent, bool force) {
        if (!ent || !IsReadablePtr(ent) || !g_fnUpdateComposite || !g_compositeOwnerOffset) return;
        g_fnUpdateComposite(reinterpret_cast<void*>(ent + g_compositeOwnerOffset), force);
        if (g_fnUpdateCompositeSec) g_fnUpdateCompositeSec(reinterpret_cast<void*>(ent), force);
    }

    static bool CallEquipItemInLoadout(uintptr_t inventory_manager, int team, int slot, uint64_t itemID) {
        if (!inventory_manager || !IsReadablePtr(inventory_manager)) return false;
        if (g_fnEquipItemInLoadout && reinterpret_cast<uintptr_t>(g_fnEquipItemInLoadout) > 0x10000)
            return g_fnEquipItemInLoadout(reinterpret_cast<void*>(inventory_manager), team, slot, itemID);
        return false;
    }

    // ==================== SCENE NODE ====================
    static uintptr_t GetSceneNode(uintptr_t ent) {
        if (!ent || !IsReadablePtr(ent)) return 0;
        uintptr_t n = sdk::read_value<uintptr_t>(ent + OFF_GAME_SCENE_NODE);
        return (n && IsReadablePtr(n)) ? n : 0;
    }

    static void SetMeshGroupMask(uintptr_t scene_node, uint64_t mask) {
        if (!scene_node || !IsReadablePtr(scene_node)) return;
        if (g_fnSetMeshGroupMask && reinterpret_cast<uintptr_t>(g_fnSetMeshGroupMask) > 0x10000) {
            g_fnSetMeshGroupMask(reinterpret_cast<void*>(scene_node), mask);
            return;
        }
        sdk::write_memory<uint64_t>(scene_node + OFF_MODEL_STATE + OFF_MESH_GROUP_MASK, mask);
    }

    static bool IsSetModelSafe(uintptr_t ent) {
        if (!ent || !IsReadablePtr(ent)) return false;
        uintptr_t node = GetSceneNode(ent);
        if (!node || !IsReadablePtr(node)) return false;
        uintptr_t* vptr = reinterpret_cast<uintptr_t*>(sdk::read_value<uintptr_t>(node));
        if (!vptr || !IsReadablePtr(reinterpret_cast<uintptr_t>(vptr))) return false;
        uintptr_t fn = sdk::read_value<uintptr_t>(reinterpret_cast<uintptr_t>(vptr));
        return (fn && IsReadablePtr(fn));
    }

    // ==================== ENTITY HELPERS ====================
    uintptr_t GetLocalPawn() {
        return game_state::GetLocalPawnRaw();
    }

    uintptr_t GetEntityList() {
        return game_state::GetEntityList();
    }

    uintptr_t ResolveHandle(uintptr_t el, uint32_t h) {
        return sdk::entity_from_handle(el, h);
    }

    uintptr_t GetActiveWeapon() {
        uintptr_t pawn = GetLocalPawn(), list = GetEntityList();
        if (!pawn || !list) return 0;
        try {
            uintptr_t ws = sdk::read_value<uintptr_t>(pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pWeaponServices);
            if (!ws || !IsReadablePtr(ws)) return 0;
            uint32_t h = sdk::read_value<uint32_t>(ws + cs2_dumper::schemas::client_dll::CPlayer_WeaponServices::m_hActiveWeapon);
            return ResolveHandle(list, h);
        }
        catch (...) { return 0; }
    }

    static std::vector<uintptr_t> GetAllWeapons() {
        std::vector<uintptr_t> result;
        const uintptr_t pawn = GetLocalPawn(), list = GetEntityList();
        if (!pawn || !list) return result;
        const auto ws = sdk::read_value<uintptr_t>(pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pWeaponServices);
        if (!ws) return result;
        const auto vec = ws + cs2_dumper::schemas::client_dll::CPlayer_WeaponServices::m_hMyWeapons;
        sdk::VectorView view{};
        if (!sdk::read_vector(vec, view, 64)) return result;
        for (int i = 0; i < view.count; ++i) {
            uint32_t handle = UINT32_MAX;
            if (!sdk::read_memory(view.data + sizeof(handle) * i, handle)) break;
            if (const auto weapon = ResolveHandle(list, handle)) result.push_back(weapon);
        }
        return result;
    }

    WeaponReadDiagnostics GetWeaponReadDiagnostics() {
        WeaponReadDiagnostics info{};
        info.material_refresh_available = g_fnUpdateComposite && g_compositeOwnerOffset && g_fnUpdateCompositeSec;
        const auto state = game_state::GetSnapshot();
        info.pawn = state.pawn;
        info.entity_list = state.entity_list;
        if (!info.pawn) { info.status = "Local pawn unavailable"; return info; }
        const auto services_slot = info.pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pWeaponServices;
        if (!sdk::read_memory(services_slot, info.services) || !sdk::is_valid_ptr(info.services)) {
            info.status = "Weapon-services pointer is null/unreadable"; return info;
        }
        sdk::read_memory(info.services + cs2_dumper::schemas::client_dll::CPlayer_WeaponServices::m_hActiveWeapon, info.active_handle);
        info.active = ResolveHandle(info.entity_list, info.active_handle);
        const auto vec = info.services + cs2_dumper::schemas::client_dll::CPlayer_WeaponServices::m_hMyWeapons;
        sdk::read_memory(vec, info.count);
        sdk::read_memory(vec + 8, info.data);
        sdk::VectorView view{};
        if (!info.entity_list) info.status = "Entity list unavailable";
        else if (!sdk::read_vector(vec, view, 64)) info.status = "Weapon vector is unreadable or has invalid bounds";
        else if (!view.count) info.status = "Weapon vector contains zero handles";
        else if (GetAllWeapons().empty()) info.status = "Weapon handles do not resolve (entity layout/serial mismatch)";
        else info.status = "Weapon handles resolved";
        return info;
    }

    static uintptr_t GetArmsEntity() {
        uintptr_t pawn = GetLocalPawn(), list = GetEntityList();
        if (!pawn || !list) return 0;
        try {
            constexpr std::ptrdiff_t OFF_HUD_MODEL_ARMS = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_hHudModelArms;
            uint32_t h = sdk::read_value<uint32_t>(pawn + OFF_HUD_MODEL_ARMS);
            return ResolveHandle(list, h);
        }
        catch (...) { return 0; }
    }

    uint16_t GetDefIndex(uintptr_t w) {
        return sdk::read_value<uint16_t>(w + OFF_ITEM_DEF_INDEX);
    }

    static uint8_t GetLocalTeam() {
        uintptr_t p = GetLocalPawn();
        return p ? sdk::read_value<uint8_t>(p + OFF_TEAM_NUM) : 0;
    }

    // ==================== INVENTORY BACKING ====================
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
        uintptr_t weaponData = sdk::read_value<uintptr_t>(weapon + cs2_dumper::schemas::client_dll::C_BaseEntity::m_nSubclassID + 0x08);
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
            uintptr_t svc = sdk::read_value<uintptr_t>(ctrl + OFF_INV_SERVICES);
            if (!svc || !IsReadablePtr(svc)) return items;
            uintptr_t vec = svc + OFF_LOADOUT_VEC;
            uintptr_t data = sdk::read_value<uintptr_t>(vec + 8);
            if (!data || !IsReadablePtr(data)) return items;
            int count = sdk::read_value<int>(vec);
            if (count <= 0 || count > 256) return items;
            for (int i = 0; i < count; i++) {
                uintptr_t sb = data + (i * 0xC8); // generated NetworkedLoadoutSlot_t size
                uintptr_t iv = sdk::read_value<uintptr_t>(sb);
                if (!iv || !IsReadablePtr(iv)) continue;
                LoadoutItem li{};
                li.item_view = iv;
                li.team = sdk::read_value<uint16_t>(sb + 0x08);
                li.slot = sdk::read_value<uint16_t>(sb + 0x0A);
                li.def_index = sdk::read_value<uint16_t>(iv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex);
                li.item_id = sdk::read_value<uint64_t>(iv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemID);
                li.item_id_high = sdk::read_value<uint32_t>(iv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh);
                li.item_id_low = sdk::read_value<uint32_t>(iv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDLow);
                li.account_id = sdk::read_value<uint32_t>(iv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iAccountID);
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
        if (!game_state::IsInGame() || GetCurrentFrameStage() != 6) return;
        for (const auto weapon : GetAllWeapons())
            ApplySkin(reinterpret_cast<void*>(weapon), GetDefIndex(weapon));
    }

    // ==================== FRAME STAGE TRACKING ====================
    thread_local static int t_currentFrameStage = -1;

    int GetCurrentFrameStage() {
        return t_currentFrameStage;
    }

    void SetCurrentFrameStage(int stage) {
        t_currentFrameStage = stage;
    }

    // ==================== APPLY KNIFE SKINS ====================
    static uint16_t  s_lastKnifeDefIndex = 0;
    static uintptr_t s_lastWeaponPtr = 0;
    static int       s_subclassRefreshFrames = 0;
    static int       s_setModelDelayFrames = 0;
    static bool      s_engineFunctionsResolved = false;

    void ApplyKnifeSkins() {
        if (!s_engineFunctionsResolved) {
            ResolveEngineFunctions();
            s_engineFunctionsResolved = true;
        }
        if (!game_state::IsInGame()) return;

        // Use stage 6, matching the reference inventory callback.
        if (GetCurrentFrameStage() != 6) return;

        // --- Death detection ---
        static int s_lastHealth = 0;
        uintptr_t localPawn = GetLocalPawn();
        int health = 0;
        if (localPawn)
            SafeReadInt(localPawn + OFF_HEALTH, health);

        if (s_lastHealth > 0 && health <= 0) {
            s_lastKnifeDefIndex = 0;
            s_lastWeaponPtr = 0;
            s_subclassRefreshFrames = 0;
            s_setModelDelayFrames = 0;
            // Also clear any fake item ID we might have stored
            static uint64_t lastFakeID = 0;
            if (lastFakeID != 0) {
                RemoveEconItemFromList(lastFakeID);
                lastFakeID = 0;
            }
            debug_console::Console::Get().Info("[KNIFE] Death detected, resetting state");
        }
        s_lastHealth = health;

        if (health <= 0) return;  // Don't process while dead

        uintptr_t weapon = GetActiveWeapon();
        if (!weapon) return;

        uint16_t def_index = GetDefIndex(weapon);
        if (!IsKnife(def_index) && !IsDefaultKnife(def_index)) return;

        // New weapon detection
        if (weapon != s_lastWeaponPtr) {
            s_lastKnifeDefIndex = 0;
            s_subclassRefreshFrames = 0;
            s_setModelDelayFrames = 0;
            s_lastWeaponPtr = weapon;
            return;
        }

        std::vector<LoadoutItem> loadout;
        try { loadout = ReadLoadout(); }
        catch (...) {}
        uint8_t team = GetLocalTeam();

        uint16_t knifeDefIndex = 0;
        const LoadoutItem* melee = FindMeleeItem(loadout, team);
        if (selected_knife_id < 500 && melee && melee->def_index >= 500)
            knifeDefIndex = melee->def_index;
        else if (selected_knife_id >= 500)
            knifeDefIndex = static_cast<uint16_t>(selected_knife_id);
        else
            return;

        const KnifeData* kd = GetKnifeData(knifeDefIndex);
        if (!kd) return;

        // --- Cheap writes every frame ---
        if (melee && melee->item_id != 0 && user_skins.find(knifeDefIndex) == user_skins.end()) {
            sdk::write_memory<uint64_t>(weapon + OFF_ITEM_ID, melee->item_id);
            sdk::write_memory<uint32_t>(weapon + OFF_ITEM_ID_HIGH, melee->item_id_high);
            sdk::write_memory<uint32_t>(weapon + OFF_ITEM_ID_LOW, melee->item_id_low);
            sdk::write_memory<uint32_t>(weapon + OFF_ACCOUNT_ID, melee->account_id);
        }
        else {
            sdk::write_memory(weapon + OFF_ITEM_ID_HIGH, UINT32_MAX);
        }

        // Common writes (definition index, subclass, fallback skin)
        sdk::write_memory<bool>(weapon + OFF_DISALLOW_SOC, false);
        sdk::write_memory<bool>(weapon + OFF_RESTORE_MATERIAL, true);



        sdk::write_memory<uint16_t>(weapon + OFF_ITEM_DEF_INDEX, knifeDefIndex);
        sdk::write_memory<uint32_t>(weapon + OFF_SUBCLASS_ID, kd->subclass_hash);

        // Type change detection (used for model update)
        if (s_lastKnifeDefIndex != knifeDefIndex) {
            s_subclassRefreshFrames = 5;
            s_setModelDelayFrames = 1;
            s_lastKnifeDefIndex = knifeDefIndex;
        }

        if (s_setModelDelayFrames > 0) {
            s_setModelDelayFrames--;
        }

        // Run model update sequence only once per knife type change
        if (s_setModelDelayFrames == 0 && s_subclassRefreshFrames >= 0) {
            // Step 1: UpdateSubclass
            try { CallUpdateSubclassSafe(weapon); }
            catch (...) {}

            // Step 2: Mesh group masks
            try {
                uintptr_t node = GetSceneNode(weapon);
                if (node) SetMeshGroupMask(node, 2);
                uintptr_t arms = GetArmsEntity();
                if (arms) {
                    uintptr_t arms_node = GetSceneNode(arms);
                    if (arms_node) SetMeshGroupMask(arms_node, 2);
                }
            }
            catch (...) {}

            // Step 3: UpdateComposite (vfunc 7)
            //try { sdk::CallVFunc<7, void*>(reinterpret_cast<void*>(weapon), 1); }
           // catch (...) {}
            // Step 4: UpdateComposite (vfunc 105)
            //try { sdk::CallVFunc<105, void*>(reinterpret_cast<void*>(weapon), 1); }
           // catch (...) {}

            // Step 5: SetModel (last)
            try {
                if (IsSetModelSafe(weapon)) {
                    CallSetModel(weapon, kd->model_path);
                    const auto attachment = ResolveHandle(GetEntityList(), sdk::read_value<uint32_t>(weapon));
                    if (attachment && IsSetModelSafe(attachment)) CallSetModel(attachment, kd->model_path);
                }
            }
            catch (...) {}

            // Prevent this block from running again until next knife change
            s_subclassRefreshFrames = -1;
        }
    }

    // ==================== APPLY GLOVES ====================
    static uintptr_t s_glovePawn = 0;
    static int s_glovePaint = 0;
    void ApplyGloves() {
        if (!game_state::IsInGame() || GetCurrentFrameStage() != 6) return;
        const uintptr_t pawn = GetLocalPawn();
        if (!pawn || sdk::read_value<int>(pawn + OFF_HEALTH) <= 0 || selected_glove_kit <= 0) {
            s_glovePawn = 0; return;
        }
        const auto it = std::find_if(glove_database.begin(), glove_database.end(),
            [](const GloveInfo& glove) { return glove.paint_kit == selected_glove_kit; });
        if (it == glove_database.end() || it->weapon_id <= 0 || !g_fnSetAttribute) return;
        const auto gv = pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_EconGloves;
        const auto def = static_cast<uint16_t>(it->weapon_id);
        const auto defAddress = gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex;
        if (s_glovePawn == pawn && s_glovePaint == selected_glove_kit && sdk::read_value<uint16_t>(defAddress) == def) return;
        if (!sdk::write_memory(defAddress, def)) return;
        sdk::write_memory(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh, UINT32_MAX);
        sdk::write_memory(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iEntityQuality, 3);
        sdk::write_memory(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_bDisallowSOC, false);
        sdk::write_memory(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_bRestoreCustomMaterialAfterPrecache, true);
        g_fnSetAttribute(reinterpret_cast<void*>(gv), "set item texture prefab", static_cast<float>(selected_glove_kit));
        g_fnSetAttribute(reinterpret_cast<void*>(gv), "set item texture wear", 0.01f);
        g_fnSetAttribute(reinterpret_cast<void*>(gv), "set item texture seed", 0.0f);
        sdk::write_memory(pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_bNeedToReApplyGloves, true);
        s_glovePawn = pawn; s_glovePaint = selected_glove_kit;
    }



    // ==================== MAIN TICK ====================
    void ApplyAllSkins() {
        if (!game_state::IsInGame() || GetCurrentFrameStage() != 6) return;
        const auto pawn = GetLocalPawn();
        if (!pawn || sdk::read_value<int>(pawn + OFF_HEALTH) <= 0) { s_glovePawn = 0; return; }
        ResolveEngineFunctions();
        ApplyKnifeSkins();
        for (const auto weapon : GetAllWeapons())
            ApplySkin(reinterpret_cast<void*>(weapon), GetDefIndex(weapon));
        ApplyGloves();
    }

    void ApplyKnife() {
        //should_apply_set_model = true;
        s_lastKnifeDefIndex = 0;
        s_lastWeaponPtr = 0;
        s_subclassRefreshFrames = 0;
        s_setModelDelayFrames = 0;
        debug_console::Console::Get().Success("[KNIFE] Selection changed, refreshing next frame");
    }

    // ==================== HTTP ====================
    static std::string HTTPGet(const std::string& url) {
        std::string r;
        HINTERNET h = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (!h) return r;
        DWORD timeout = 5000;
        InternetSetOptionA(h, INTERNET_OPTION_CONNECT_TIMEOUT, &timeout, sizeof(timeout));
        InternetSetOptionA(h, INTERNET_OPTION_RECEIVE_TIMEOUT, &timeout, sizeof(timeout));
        DWORD flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE;
        if (url.find("https://") == 0) flags |= INTERNET_FLAG_SECURE;
        HINTERNET hu = InternetOpenUrlA(h, url.c_str(), NULL, 0, flags, 0);
        if (hu) {
            char buf[8192]; DWORD rd = 0;
            while (r.size() < 16 * 1024 * 1024 && InternetReadFile(hu, buf, sizeof(buf) - 1, &rd) && rd > 0) r.append(buf, rd);
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
                    glove_database.push_back(GloveInfo(pk, gn, ExtractInt(obj, "weapon_defindex")));
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
        skin_database.clear(); skins_by_weapon.clear(); glove_database.clear();
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
        glove_database.push_back(GloveInfo(10018, "Sport Gloves | Superconductor", 5030));
        glove_database.push_back(GloveInfo(10037, "Sport Gloves | Pandora's Box", 5030));
        glove_database.push_back(GloveInfo(10048, "Sport Gloves | Vice", 5030));
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
        s_setModelDelayFrames = 0;
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
                info.def_index = sdk::read_value<uint16_t>(w + OFF_ITEM_DEF_INDEX);
                info.entity_quality = sdk::read_value<int>(w + OFF_ENTITY_QUALITY);
                info.item_id_high = sdk::read_value<int>(w + OFF_ITEM_ID_HIGH);
                info.item_id = sdk::read_value<uint64_t>(w + OFF_ITEM_ID);
                info.account_id = sdk::read_value<uint32_t>(w + OFF_ACCOUNT_ID);
                info.disallow_soc = sdk::read_value<bool>(w + OFF_DISALLOW_SOC);
                info.restore_material = sdk::read_value<bool>(w + OFF_RESTORE_MATERIAL);
                info.fallback_paint_kit = sdk::read_value<int>(w + OFF_FALLBACK_PAINT);
                info.fallback_seed = sdk::read_value<int>(w + OFF_FALLBACK_SEED);
                info.fallback_wear = sdk::read_value<float>(w + OFF_FALLBACK_WEAR);
                info.fallback_stattrak = sdk::read_value<int>(w + OFF_FALLBACK_STATTRAK);
                info.owner_xuid_low = sdk::read_value<uint32_t>(w + OFF_OWNER_XUID_LOW);
                info.subclass_id = sdk::read_value<uint32_t>(w + OFF_SUBCLASS_ID);
                info.loadout_matched = (info.item_id != 0 && info.item_id_high != -1);
                sdk::read_memory(w + OFF_CUSTOM_NAME, info.custom_name);
                info.custom_name[sizeof(info.custom_name) - 1] = '\0';
                result.push_back(info);
            }
        }
        catch (...) {}
        return result;
    }

    bool IsSetModelAvailable() { return g_fnSetModel != nullptr; }
    int  GetLoadoutItemCount() { try { return (int)ReadLoadout().size(); } catch (...) { return 0; } }

    struct SavedSkin {
        uint32_t handle = UINT32_MAX, high = 0;
        int paint = 0, seed = 0, stat = 0;
        float wear = 0;
        bool disallow = false, restore = false;
        char name[161]{};
    };
    static std::map<uintptr_t, SavedSkin> saved_skins;

    void ApplySkin(void* weapon, int weapon_id) {
        if (!weapon) return;
        const uintptr_t w = reinterpret_cast<uintptr_t>(weapon);
        const auto identity = sdk::read_value<uintptr_t>(w + 0x10);
        uint32_t handle = UINT32_MAX;
        if (!identity || !sdk::read_memory(identity + 0x10, handle) || handle == UINT32_MAX) return;
        auto saved = saved_skins.find(w);
        if (saved != saved_skins.end() && saved->second.handle != handle) {
            saved_skins.erase(saved); saved = saved_skins.end();
        }
        const auto it = user_skins.find(weapon_id);
        if (it == user_skins.end()) {
            if (saved == saved_skins.end()) return;
            const auto& original = saved->second;
            bool restored = sdk::write_memory(w + OFF_FALLBACK_PAINT, original.paint) &&
                sdk::write_memory(w + OFF_FALLBACK_SEED, original.seed) &&
                sdk::write_memory(w + OFF_FALLBACK_WEAR, original.wear) &&
                sdk::write_memory(w + OFF_FALLBACK_STATTRAK, original.stat) &&
                sdk::write_memory(w + OFF_CUSTOM_NAME, original.name) &&
                sdk::write_memory(w + OFF_ITEM_ID_HIGH, original.high) &&
                sdk::write_memory(w + OFF_DISALLOW_SOC, original.disallow) &&
                sdk::write_memory(w + OFF_RESTORE_MATERIAL, original.restore);
            if (!restored) return;
            if (g_fnSetAttribute) {
                auto* view = reinterpret_cast<void*>(w + ECON_ITEM_VIEW_BASE);
                g_fnSetAttribute(view, "set item texture prefab", static_cast<float>(original.paint));
                g_fnSetAttribute(view, "set item texture wear", original.wear);
                g_fnSetAttribute(view, "set item texture seed", static_cast<float>(original.seed));
            }
            CallUpdateComposite(w, true);
            saved_skins.erase(saved);
            return;
        }
        if (saved == saved_skins.end()) {
            SavedSkin original{}; original.handle = handle;
            if (!sdk::read_memory(w + OFF_FALLBACK_PAINT, original.paint) ||
                !sdk::read_memory(w + OFF_FALLBACK_SEED, original.seed) ||
                !sdk::read_memory(w + OFF_FALLBACK_WEAR, original.wear) ||
                !sdk::read_memory(w + OFF_FALLBACK_STATTRAK, original.stat) ||
                !sdk::read_memory(w + OFF_CUSTOM_NAME, original.name) ||
                !sdk::read_memory(w + OFF_ITEM_ID_HIGH, original.high) ||
                !sdk::read_memory(w + OFF_DISALLOW_SOC, original.disallow) ||
                !sdk::read_memory(w + OFF_RESTORE_MATERIAL, original.restore)) return;
            saved_skins.emplace(w, original);
        }
        const auto& cfg = it->second;
        const int paint = (std::max)(0, cfg.paint_kit);
        const int seed = (std::clamp)(cfg.seed, 0, 1000);
        const float wear = std::isfinite(cfg.wear) ? (std::clamp)(cfg.wear, 0.0f, 1.0f) : 0.0f;
        const int stat = cfg.stattrak ? (std::max)(0, cfg.stattrak_count) : -1;
        char name[161]{}; // C_EconItemView::m_szCustomName
        std::memcpy(name, cfg.name_tag.data(), (std::min)(cfg.name_tag.size(), sizeof(name) - 1));
        char previous[sizeof(name)]{};
        const bool name_read = sdk::read_memory(w + OFF_CUSTOM_NAME, previous);
        const bool changed = sdk::read_value<int>(w + OFF_FALLBACK_PAINT) != paint ||
            sdk::read_value<int>(w + OFF_FALLBACK_SEED) != seed ||
            sdk::read_value<float>(w + OFF_FALLBACK_WEAR) != wear ||
            sdk::read_value<int>(w + OFF_FALLBACK_STATTRAK) != stat ||
            sdk::read_value<uint32_t>(w + OFF_ITEM_ID_HIGH) != UINT32_MAX ||
            !name_read || std::memcmp(name, previous, sizeof(name)) != 0;
        if (!changed) return;
        if (!sdk::write_memory(w + OFF_FALLBACK_PAINT, paint) ||
            !sdk::write_memory(w + OFF_FALLBACK_SEED, seed) ||
            !sdk::write_memory(w + OFF_FALLBACK_WEAR, wear) ||
            !sdk::write_memory(w + OFF_FALLBACK_STATTRAK, stat) ||
            !sdk::write_memory(w + OFF_CUSTOM_NAME, name)) return;
        sdk::write_memory(w + OFF_ITEM_ID_HIGH, UINT32_MAX);
        sdk::write_memory(w + OFF_DISALLOW_SOC, false);
        sdk::write_memory(w + OFF_RESTORE_MATERIAL, true);
        if (g_fnSetAttribute) {
            void* view = reinterpret_cast<void*>(w + ECON_ITEM_VIEW_BASE);
            g_fnSetAttribute(view, "set item texture prefab", static_cast<float>(paint));
            g_fnSetAttribute(view, "set item texture wear", wear);
            g_fnSetAttribute(view, "set item texture seed", static_cast<float>(seed));
        }
        CallUpdateComposite(w, true);
    }
}
