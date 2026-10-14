#include "aim_profiler.h"
#include "skins.h"
#include "skins/signatures.h"
#include "skins/offsets.h"
#include "skins/knife_data.h"
#include "skin_models.h"
#include "skin_hud.h"
#include "interfaces.h"
#include "game_state.h"
#include "../sdk/mem.h"
#include "../sdk/feature_support.h"
#include <cmath>
#include "pattern_resolver.h"
#include <Windows.h>
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

namespace skins {

    // Safe pointer check.
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

    using namespace offsets;

    using detail::GetKnifeData;

    // Engine function pointers.

    using fnSetModel = void(__fastcall*)(void*, const char*);
    using fnUpdateSubClass = void(__fastcall*)(void*);
    using fnUpdateComposite = void(__fastcall*)(void*, bool);
    using fnClearHudWeaponIcon = skin_hud::Clear;
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
    using fnBuildSkin = void(__fastcall*)(void*, void*, int64_t, int, char, char, void*);
    using fnCreatePaintKit = void*(__fastcall*)(void*);
    using fnSetBodyGroup = void(__fastcall*)(void*, const char*, unsigned int);
    static fnBuildSkin g_fnBuildSkin = nullptr;
    static fnCreatePaintKit g_fnCreatePaintKit = nullptr;
    static fnSetBodyGroup g_fnSetBodyGroup = nullptr;
    static void(__fastcall* g_fnRegenerateSkins)() = nullptr;
    static uintptr_t g_paintKitSchema = 0;
    static bool                 g_engine_funcs_resolved = false;

    // FindHudElement function pointer
    static void* (*g_fnFindHudElement)(const char* szHudName) = nullptr;

    static void ResolveFindHudElement() {
        if (g_fnFindHudElement) return;
        uint8_t* pattern = sdk::find_pattern("client.dll", signatures::find_hud_element);
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

        auto fnSetModelResolved = sdk::find_pattern("client.dll", signatures::SET_MODEL_SIGNATURE);
        if (fnSetModelResolved) {
            g_fnSetModel = reinterpret_cast<fnSetModel>(fnSetModelResolved);
            con.Success("[SKINS] SetModel at 0x%llX", reinterpret_cast<uintptr_t>(fnSetModelResolved));
        }
        else con.Warning("[SKINS] SetModel NOT found");

        auto fnUpdateSubClassResolved = sdk::find_pattern("client.dll", signatures::UPDATE_SUBCLASS_SIGNATURE);
        if (fnUpdateSubClassResolved) {
            g_fnUpdateSubClass = reinterpret_cast<fnUpdateSubClass>(fnUpdateSubClassResolved);
            con.Success("[SKINS] UpdateSubclass at 0x%llX", reinterpret_cast<uintptr_t>(fnUpdateSubClassResolved));
        }
        else con.Warning("[SKINS] UpdateSubclass NOT found");

        auto fnUpdateCompositeResolved = sdk::find_pattern("client.dll", signatures::UPDATE_COMPOSITE_MATERIAL_SIGNATURE);
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
            sdk::find_pattern("client.dll", signatures::REGEN_WEAPON_SKINS_SIGNATURE));
        if (!g_compositeOwnerOffset) con.Warning("[SKINS] Material owner layout unavailable; refresh disabled");

        auto fnClearHudWeaponIconResolved = sdk::find_pattern("client.dll", signatures::CLEAR_HUD_WEAPON_ICON_SIGNATURE);
        if (fnClearHudWeaponIconResolved) {
            g_fnClearHudWeaponIcon = reinterpret_cast<fnClearHudWeaponIcon>(sdk::GetCA(reinterpret_cast<uintptr_t>(fnClearHudWeaponIconResolved)));
            con.Success("[SKINS] ClearHudWeaponIcon at 0x%llX", reinterpret_cast<uintptr_t>(g_fnClearHudWeaponIcon));
        }
        else con.Warning("[SKINS] ClearHudWeaponIcon NOT found");

        auto fnEquipItemInLoadoutResolved = sdk::find_pattern("client.dll", signatures::EQUIP_ITEM_IN_LOADOUT_SIGNATURE);
        if (fnEquipItemInLoadoutResolved) {
            g_fnEquipItemInLoadout = reinterpret_cast<fnEquipItemInLoadout>(fnEquipItemInLoadoutResolved);
            con.Success("[SKINS] EquipItemInLoadout at 0x%llX", reinterpret_cast<uintptr_t>(fnEquipItemInLoadoutResolved));
        }
        else con.Warning("[SKINS] EquipItemInLoadout NOT found");

        // Native material entry points used by the reference implementation.
        g_fnBuildSkin = reinterpret_cast<fnBuildSkin>(TryPattern("client.dll", signatures::build_skin));
        g_fnCreatePaintKit = reinterpret_cast<fnCreatePaintKit>(TryPattern("client.dll", signatures::create_paint_kit));
        if (auto call = TryPattern("client.dll", signatures::set_body_group_call))
            g_fnSetBodyGroup = reinterpret_cast<fnSetBodyGroup>(sdk::GetCA(reinterpret_cast<uintptr_t>(call)));
        g_fnRegenerateSkins = reinterpret_cast<decltype(g_fnRegenerateSkins)>(TryPattern("client.dll", signatures::regenerate_skins));
        if (!g_fnBuildSkin || !g_fnCreatePaintKit || !g_fnSetBodyGroup || !g_fnRegenerateSkins)
            con.Warning("[SKINS] Native material functions missing: weapon=%d glove=%d body=%d regen=%d",
                !!g_fnBuildSkin, !!g_fnCreatePaintKit, !!g_fnSetBodyGroup, !!g_fnRegenerateSkins);
        g_fnSetMeshGroupMask = reinterpret_cast<fnSetMeshGroupMask>(sdk::find_pattern("client.dll", signatures::SET_MESH_GROUP_MASK_SIGNATURE));
        if (auto call = sdk::find_pattern("client.dll", signatures::set_attribute_call))
            g_fnSetAttribute = reinterpret_cast<fnSetAttribute>(sdk::GetCA(reinterpret_cast<uintptr_t>(call)));
    }

    // Entity flags.
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

    // Safe callers.
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

    // Scene node.
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

    // Entity helpers.
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
        info.material_refresh_available = g_fnBuildSkin && g_fnRegenerateSkins;
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

    // Reference CEconItemSchema layout: paint-kit map at 0x2F0, 0x20-byte nodes.
    // Resolve the actual Source2Client interface; interfaces::client may be a hook address.
    static uintptr_t FindPaintKit(int id) {
        if (!g_paintKitSchema) {
            const auto module = GetModuleHandleA("client.dll");
            using Factory = void*(__cdecl*)(const char*, int*);
            const auto factory = module ? reinterpret_cast<Factory>(GetProcAddress(module, "CreateInterface")) : nullptr;
            const auto client = factory ? reinterpret_cast<uintptr_t>(factory("Source2Client002", nullptr)) : 0;
            const auto vtable = client ? sdk::read_value<uintptr_t>(client) : 0;
            using GetSystem = uintptr_t(__fastcall*)(void*);
            const auto getSystem = vtable ? sdk::read_value<GetSystem>(vtable + 129 * sizeof(uintptr_t)) : nullptr;
            if (!getSystem) return 0;
            const auto system = getSystem(reinterpret_cast<void*>(client));
            g_paintKitSchema = system ? sdk::read_value<uintptr_t>(system + 8) : 0;
        }
        if (!g_paintKitSchema) return 0;
        const auto map = g_paintKitSchema + 0x2F0;
        const int count = sdk::read_value<int>(map);
        const auto nodes = sdk::read_value<uintptr_t>(map + 8);
        if (!nodes || count <= 0 || count > 65536) return 0;
        for (int i = 0; i < count; ++i) {
            const auto kit = sdk::read_value<uintptr_t>(nodes + i * 0x20 + 0x18);
            int kitId = -1;
            if (kit && sdk::read_memory(kit, kitId) && kitId == id) return kit;
        }
        return 0;
    }

    // The HUD weapon is a child of the arms, with its owner handle pointing
    // back to the world weapon. Never refresh the arms themselves.
    static uintptr_t FindHudWeapon(uintptr_t weapon) {
        const auto list = GetEntityList();
        const auto armsNode = GetSceneNode(GetArmsEntity());
        if (!list || !armsNode) return 0;
        using namespace cs2_dumper::schemas::client_dll;
        auto node = sdk::read_value<uintptr_t>(armsNode + CGameSceneNode::m_pChild);
        for (int visited = 0; node && visited < 64; ++visited) {
            const auto entity = sdk::read_value<uintptr_t>(node + CGameSceneNode::m_pOwner);
            uint32_t owner = UINT32_MAX;
            if (entity && entity != weapon && sdk::read_memory(entity + OFF_OWNER_ENTITY, owner) &&
                ResolveHandle(list, owner) == weapon) return entity;
            node = sdk::read_value<uintptr_t>(node + CGameSceneNode::m_pNextSibling);
        }
        return 0;
    }

    static bool RefreshWeaponMaterials(uintptr_t weapon) {
        aim_profiler::Scope profile(IsKnife(GetDefIndex(weapon)) ? aim_profiler::KnifeMaterials : aim_profiler::OtherMaterials);
        // Knives also need their world and HUD paint materials rebuilt.
        if (!g_fnBuildSkin || !g_fnRegenerateSkins) return false;
        const int paint = sdk::read_value<int>(weapon + OFF_FALLBACK_PAINT);
        uint64_t mask = skin_models::Mask(GetDefIndex(weapon), paint);
        if (!mask) {
            const auto kit = paint > 0 ? FindPaintKit(paint) : 0;
            if (paint > 0 && !kit) return false;
            mask = kit && sdk::read_value<bool>(kit + 0xAE) ? 2ULL : 1ULL;
        }
        auto* item = reinterpret_cast<void*>(weapon + ECON_ITEM_VIEW_BASE);
        SetMeshGroupMask(GetSceneNode(weapon), mask);
        g_fnBuildSkin(item, reinterpret_cast<void*>(weapon), 0, 2048, 0, 0, nullptr);
        if (const auto hud = FindHudWeapon(weapon)) {
            SetMeshGroupMask(GetSceneNode(hud), mask);
            g_fnBuildSkin(item, reinterpret_cast<void*>(hud), 0, 2048, 0, 0, nullptr);
        }
        g_fnRegenerateSkins();
        return true;
    }

    uint16_t GetDefIndex(uintptr_t w) {
        return sdk::read_value<uint16_t>(w + OFF_ITEM_DEF_INDEX);
    }

    static uint8_t GetLocalTeam() {
        uintptr_t p = GetLocalPawn();
        return p ? sdk::read_value<uint8_t>(p + OFF_TEAM_NUM) : 0;
    }

    // Inventory backing.
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

    // Loadout reading.
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
        aim_profiler::Scope profile(aim_profiler::Loadout);
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

    // Updatesubclass queue.
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

    // Apply weapon skins.
    void ApplyWeaponSkins() {
        if (!game_state::IsInGame() || GetCurrentFrameStage() != 6) return;
        for (const auto weapon : GetAllWeapons())
            ApplySkin(reinterpret_cast<void*>(weapon), GetDefIndex(weapon));
    }

    // Frame stage tracking.
    thread_local static int t_currentFrameStage = -1;

    int GetCurrentFrameStage() {
        return t_currentFrameStage;
    }

    void SetCurrentFrameStage(int stage) {
        t_currentFrameStage = stage;
    }

    // Apply knife skins.
    static uint16_t  s_lastKnifeDefIndex = 0;
    static uintptr_t s_lastWeaponPtr = 0;
    static int       s_subclassRefreshFrames = 0;
    static int       s_setModelDelayFrames = 0;
    static bool      s_engineFunctionsResolved = false;
    static uint32_t s_lastKnifeHandle = UINT32_MAX;
    static void InvalidateSkinMaterial(uintptr_t weapon);

    template<class T> static bool WriteKnifeValue(uintptr_t address, T value) {
        T current{};
        if (!sdk::read_memory(address, current)) return false;
        return current == value || sdk::write_memory(address, value);
    }

    void ApplyKnifeSkins() {
        aim_profiler::Scope profile(aim_profiler::Knife);
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
        const auto identity = sdk::read_value<uintptr_t>(weapon + 0x10);
        const auto handle = identity ? sdk::read_value<uint32_t>(identity + 0x10) : UINT32_MAX;
        if (handle == UINT32_MAX) return;
        if (weapon != s_lastWeaponPtr || handle != s_lastKnifeHandle) {
            s_lastKnifeDefIndex = 0;
            s_subclassRefreshFrames = 0;
            s_setModelDelayFrames = 0;
            s_lastWeaponPtr = weapon;
            s_lastKnifeHandle = handle;
            return;
        }

        uint8_t team = GetLocalTeam();
        // Custom paint has no inventory dependency. Otherwise copy the loadout
        // at most four times a second, rather than scanning it every frame.
        static std::vector<LoadoutItem> loadout;
        static uintptr_t loadoutController = 0;
        static uint8_t loadoutTeam = 0;
        static ULONGLONG nextLoadoutRead = 0;
        const bool customPaint = selected_knife_id >= 500 && user_skins.find(selected_knife_id) != user_skins.end();
        const LoadoutItem* melee = nullptr;
        if (!customPaint) {
            const auto controller = game_state::GetLocalController();
            const auto now = GetTickCount64();
            if (controller != loadoutController || team != loadoutTeam || now >= nextLoadoutRead) {
                loadout = ReadLoadout();
                loadoutController = controller; loadoutTeam = team;
                nextLoadoutRead = now + 250;
            }
            melee = FindMeleeItem(loadout, team);
        }

        uint16_t knifeDefIndex = 0;
        if (selected_knife_id < 500 && melee && melee->def_index >= 500)
            knifeDefIndex = melee->def_index;
        else if (selected_knife_id >= 500)
            knifeDefIndex = static_cast<uint16_t>(selected_knife_id);
        else
            return;

        const detail::KnifeData* kd = GetKnifeData(knifeDefIndex);
        if (!kd) return;

        // Avoid VirtualQuery/WriteProcessMemory on unchanged knife fields.
        if (melee && melee->item_id != 0 && user_skins.find(knifeDefIndex) == user_skins.end()) {
            WriteKnifeValue<uint64_t>(weapon + OFF_ITEM_ID, melee->item_id);
            WriteKnifeValue<uint32_t>(weapon + OFF_ITEM_ID_HIGH, melee->item_id_high);
            WriteKnifeValue<uint32_t>(weapon + OFF_ITEM_ID_LOW, melee->item_id_low);
            WriteKnifeValue<uint32_t>(weapon + OFF_ACCOUNT_ID, melee->account_id);
        }
        else {
            WriteKnifeValue(weapon + OFF_ITEM_ID_HIGH, UINT32_MAX);
        }

        // Common writes (definition index, subclass, fallback skin)
        WriteKnifeValue<bool>(weapon + OFF_DISALLOW_SOC, false);
        WriteKnifeValue<bool>(weapon + OFF_RESTORE_MATERIAL, true);



        WriteKnifeValue<uint16_t>(weapon + OFF_ITEM_DEF_INDEX, knifeDefIndex);
        WriteKnifeValue<uint32_t>(weapon + OFF_SUBCLASS_ID, kd->subclass_hash);

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
            aim_profiler::Scope modelProfile(aim_profiler::KnifeModel);
            // The arms have their own visible groups. Preserve them before
            // changing the weapon; assigning 2 alone clears every other bit.
            const auto arms = GetArmsEntity();
            const auto arms_node = arms ? GetSceneNode(arms) : 0;
            uint64_t arms_mask = 0;
            const bool have_arms_mask = arms_node && arms_node != GetSceneNode(weapon) &&
                sdk::read_memory(arms_node + OFF_MODEL_STATE + OFF_MESH_GROUP_MASK, arms_mask);
            // Step 1: UpdateSubclass
            try { CallUpdateSubclassSafe(weapon); }
            catch (...) {}

            // Step 2: Mesh group masks
            try {
                uintptr_t node = GetSceneNode(weapon);
                const auto cfg = user_skins.find(selected_knife_id);
                const int paint = cfg != user_skins.end() ? cfg->second.paint_kit : 0;
                const auto mask = skin_models::Mask(selected_knife_id, paint);
                if (node) SetMeshGroupMask(node, mask ? mask : 2);
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
                }
            }
            catch (...) {}

            // Keep the arms update, but enable the requested group without
            // removing groups used by the hands/gloves. Apply after SetModel.
            if (have_arms_mask) SetMeshGroupMask(arms_node, arms_mask | uint64_t{2});
            InvalidateSkinMaterial(weapon);

            // Prevent this block from running again until next knife change
            s_subclassRefreshFrames = -1;
            skin_hud::Request();
        }
    }

    // Apply gloves.
    static uintptr_t s_glovePawn = 0;
    static int s_glovePaint = 0;
    static float s_gloveSpawnTime = -1.0f;
    static uint8_t s_gloveTeam = 0;
    static bool s_customGlovesApplied = false;
    void ApplyGloves() {
        if (!game_state::IsInGame() || GetCurrentFrameStage() != 7) return;
        const uintptr_t pawn = GetLocalPawn();
        if (!pawn || sdk::read_value<int>(pawn + OFF_HEALTH) <= 0) {
            s_glovePawn = 0; return;
        }
        const auto team = sdk::read_value<uint8_t>(pawn + OFF_TEAM_NUM);
        const auto spawnTime = sdk::read_value<float>(pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_flLastSpawnTimeIndex);
        const bool custom = selected_glove_kit > 0;
        if (!custom && !s_customGlovesApplied) return;
        const auto it = std::find_if(glove_database.begin(), glove_database.end(),
            [](const GloveInfo& glove) { return glove.paint_kit == selected_glove_kit; });
        if (!g_fnSetAttribute || !g_fnSetBodyGroup || !g_fnCreatePaintKit || (custom && (it == glove_database.end() || it->weapon_id <= 0))) return;
        if (!custom && team != 2 && team != 3) return;
        const auto gv = pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_EconGloves;
        const auto def = static_cast<uint16_t>(custom ? it->weapon_id : (team == 2 ? 5028 : 5029));
        const auto defAddress = gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex;
        if (s_glovePawn == pawn && s_glovePaint == selected_glove_kit && s_gloveSpawnTime == spawnTime && s_gloveTeam == team && sdk::read_value<uint16_t>(defAddress) == def) return;
        // Schema traversal is only needed when rebuilding the glove material.
        const auto paintKit = custom ? FindPaintKit(selected_glove_kit) : 0;
        if (custom && !paintKit) return;
        if (!sdk::write_memory(defAddress, def)) return;
        sdk::write_memory(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_bInitialized, false);
        sdk::write_memory(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh, custom ? UINT32_MAX : 0u);
        sdk::write_memory(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_iEntityQuality, custom ? 3 : 0);
        sdk::write_memory(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_bDisallowSOC, false);
        sdk::write_memory(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_bRestoreCustomMaterialAfterPrecache, true);
        g_fnSetAttribute(reinterpret_cast<void*>(gv), "set item texture prefab", static_cast<float>(custom ? selected_glove_kit : 0));
        g_fnSetAttribute(reinterpret_cast<void*>(gv), "set item texture wear", 0.01f);
        g_fnSetAttribute(reinterpret_cast<void*>(gv), "set item texture seed", 0.0f);
        if (custom) {
            const auto generated = reinterpret_cast<uintptr_t>(g_fnCreatePaintKit(reinterpret_cast<void*>(gv)));
            const auto name = sdk::read_value<uintptr_t>(paintKit + 8);
            if (!generated || !name || !sdk::write_memory(generated + 8, name)) {
                sdk::write_memory(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_bInitialized, true);
                return; // Retry on the next frame; do not cache a failed material build.
            }
        }
        g_fnSetBodyGroup(reinterpret_cast<void*>(pawn), "first_or_third_person", 1);
        sdk::write_memory(gv + cs2_dumper::schemas::client_dll::C_EconItemView::m_bInitialized, true);
        sdk::write_memory(pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_bNeedToReApplyGloves, true);
        s_glovePawn = pawn; s_glovePaint = selected_glove_kit;
        s_gloveSpawnTime = spawnTime; s_gloveTeam = team;
        s_customGlovesApplied = custom;
        skin_hud::Request();
    }



    // Main tick.
    void ApplyAllSkins() {
        aim_profiler::Scope profile(aim_profiler::Skins);
        const int stage = GetCurrentFrameStage();
        if (!game_state::IsInGame() || (stage != 6 && stage != 7)) return;
        const auto pawn = GetLocalPawn();
        if (!pawn || sdk::read_value<int>(pawn + OFF_HEALTH) <= 0) { s_glovePawn = 0; return; }
        ResolveEngineFunctions();
        // Keep knives at their original stage. Guns/gloves follow the reference's
        // network-update-end pass, before the original FrameStageNotify call.
        if (stage == 6) ApplyKnifeSkins();
        for (const auto weapon : GetAllWeapons()) {
            const auto definition = GetDefIndex(weapon);
            if (IsKnife(definition) == (stage == 6))
                ApplySkin(reinterpret_cast<void*>(weapon), definition);
        }
        if (stage == 7) ApplyGloves();
    }

    void ApplyKnife() {
        //should_apply_set_model = true;
        s_lastKnifeDefIndex = 0;
        s_lastWeaponPtr = 0;
        s_subclassRefreshFrames = 0;
        s_setModelDelayFrames = 0;
        debug_console::Console::Get().Success("[KNIFE] Selection changed, refreshing next frame");
    }

    // Batch ops.
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

    // Debug readback.
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
        bool materialReady = false;
        uintptr_t hudWeapon = 0;
    };
    static std::map<uintptr_t, SavedSkin> saved_skins;
    static void InvalidateSkinMaterial(uintptr_t weapon) {
        const auto it = saved_skins.find(weapon);
        if (it != saved_skins.end()) it->second.materialReady = false;
    }

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
            if (!RefreshWeaponMaterials(w)) return;
            saved_skins.erase(saved);
            skin_hud::Request();
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
        auto& state = saved_skins.at(w);
        const auto hud = FindHudWeapon(w);
        if (!changed && state.materialReady && state.hudWeapon == hud) return;
        if (!g_fnSetAttribute || !g_fnBuildSkin || !g_fnRegenerateSkins) return;
        sdk::write_memory(w + ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_bInitialized, true);
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
        const bool refreshNeeded = changed || !state.materialReady || state.hudWeapon != hud;
        state.materialReady = RefreshWeaponMaterials(w);
        state.hudWeapon = hud;
        if (state.materialReady && refreshNeeded) skin_hud::Request();
    }
}
