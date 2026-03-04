#include "skins.h"
#include "interfaces.h"
#include "game_state.h"
#include "../sdk/mem.h"
#include <Windows.h>
#include <wininet.h>
#include <sstream>
#include <algorithm>
#include <chrono>
#include "../external/offsets/offsets.hpp"
#include "../sdk/source2sdk_offsets.h"
#include "debug_console.h"

#pragma comment(lib, "wininet.lib")

namespace skins {

    // ==================== OFFSET CHAIN ====================
    // EconItemView lives inside: weapon + m_AttributeManager + m_Item
    static constexpr std::ptrdiff_t ECON_ITEM_VIEW_BASE =
        cs2_dumper::schemas::client_dll::C_EconEntity::m_AttributeManager +
        cs2_dumper::schemas::client_dll::C_AttributeContainer::m_Item;

    // Absolute offsets from weapon entity base to C_EconItemView fields
    static constexpr std::ptrdiff_t OFF_ITEM_DEF_INDEX      = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex;
    static constexpr std::ptrdiff_t OFF_ENTITY_QUALITY       = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iEntityQuality;
    static constexpr std::ptrdiff_t OFF_ITEM_ID_HIGH         = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh;
    static constexpr std::ptrdiff_t OFF_ACCOUNT_ID           = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iAccountID;
    static constexpr std::ptrdiff_t OFF_DISALLOW_SOC         = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_bDisallowSOC;
    static constexpr std::ptrdiff_t OFF_RESTORE_MATERIAL     = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_bRestoreCustomMaterialAfterPrecache;
    static constexpr std::ptrdiff_t OFF_CUSTOM_NAME          = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_szCustomName;

    // Fallback fields sit directly on C_EconEntity (not inside EconItemView)
    static constexpr std::ptrdiff_t OFF_FALLBACK_PAINT    = cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackPaintKit;
    static constexpr std::ptrdiff_t OFF_FALLBACK_SEED     = cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackSeed;
    static constexpr std::ptrdiff_t OFF_FALLBACK_WEAR     = cs2_dumper::schemas::client_dll::C_EconEntity::m_flFallbackWear;
    static constexpr std::ptrdiff_t OFF_FALLBACK_STATTRAK = cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackStatTrak;
    static constexpr std::ptrdiff_t OFF_OWNER_XUID_LOW    = cs2_dumper::schemas::client_dll::C_EconEntity::m_OriginalOwnerXuidLow;

    // Entity / scene offsets
    static constexpr std::ptrdiff_t OFF_GAME_SCENE_NODE = cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode;
    static constexpr std::ptrdiff_t OFF_SUBCLASS_ID     = cs2_dumper::schemas::client_dll::C_BaseEntity::m_nSubclassID;
    static constexpr std::ptrdiff_t OFF_OWNER_ENTITY    = cs2_dumper::schemas::client_dll::C_BaseEntity::m_hOwnerEntity;

    // Scene node → skeleton → model state → mesh group mask
    static constexpr std::ptrdiff_t OFF_MODEL_STATE      = cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState;
    static constexpr std::ptrdiff_t OFF_MESH_GROUP_MASK  = cs2_dumper::schemas::client_dll::CModelState::m_MeshGroupMask;

    // Full item ID (uint64) and low part — needed for loadout-based item resolution
    static constexpr std::ptrdiff_t OFF_ITEM_ID =
        ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemID;
    static constexpr std::ptrdiff_t OFF_ITEM_ID_LOW =
        ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDLow;

    // Inventory / Loadout access (CCSPlayerController offsets)
    static constexpr std::ptrdiff_t OFF_INV_SERVICES =
        cs2_dumper::schemas::client_dll::CCSPlayerController::m_pInventoryServices;
    static constexpr std::ptrdiff_t OFF_LOADOUT_VEC =
        cs2_dumper::schemas::client_dll::CCSPlayerController_InventoryServices::m_vecNetworkableLoadout;
    static constexpr std::ptrdiff_t OFF_TEAM_NUM =
        cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum;

    // ==================== KNIFE DATA ====================
    struct KnifeData {
        int weapon_id;
        uint32_t subclass_hash;
        const char* model_path;
    };

    static const KnifeData g_knife_data[] = {
        { WEAPON_KNIFE_BAYONET,        3933374535u, "weapons/models/knife/knife_bayonet/weapon_knife_bayonet.vmdl" },
        { WEAPON_KNIFE_CLASSIC,        3787235507u, "weapons/models/knife/knife_css/weapon_knife_css.vmdl" },
        { WEAPON_KNIFE_FLIP,           4046390180u, "weapons/models/knife/knife_flip/weapon_knife_flip.vmdl" },
        { WEAPON_KNIFE_GUT,            2047704618u, "weapons/models/knife/knife_gut/weapon_knife_gut.vmdl" },
        { WEAPON_KNIFE_KARAMBIT,       1731408398u, "weapons/models/knife/knife_karambit/weapon_knife_karambit.vmdl" },
        { WEAPON_KNIFE_M9_BAYONET,     1638561588u, "weapons/models/knife/knife_m9/weapon_knife_m9.vmdl" },
        { WEAPON_KNIFE_TACTICAL,       2282479884u, "weapons/models/knife/knife_tactical/weapon_knife_tactical.vmdl" },
        { WEAPON_KNIFE_FALCHION,       3412259219u, "weapons/models/knife/knife_falchion/weapon_knife_falchion.vmdl" },
        { WEAPON_KNIFE_SURVIVAL_BOWIE, 2511498851u, "weapons/models/knife/knife_bowie/weapon_knife_bowie.vmdl" },
        { WEAPON_KNIFE_BUTTERFLY,      1353709123u, "weapons/models/knife/knife_butterfly/weapon_knife_butterfly.vmdl" },
        { WEAPON_KNIFE_PUSH,           4269888884u, "weapons/models/knife/knife_push/weapon_knife_push.vmdl" },
        { WEAPON_KNIFE_CORD,           1105782941u, "weapons/models/knife/knife_cord/weapon_knife_cord.vmdl" },
        { WEAPON_KNIFE_CANIS,          275962944u,  "weapons/models/knife/knife_canis/weapon_knife_canis.vmdl" },
        { WEAPON_KNIFE_URSUS,          1338637359u, "weapons/models/knife/knife_ursus/weapon_knife_ursus.vmdl" },
        { WEAPON_KNIFE_GYPSY_JACKKNIFE,3230445913u, "weapons/models/knife/knife_navaja/weapon_knife_navaja.vmdl" },
        { WEAPON_KNIFE_OUTDOOR,        3206681373u, "weapons/models/knife/knife_outdoor/weapon_knife_outdoor.vmdl" },
        { WEAPON_KNIFE_STILETTO,       2595277776u, "weapons/models/knife/knife_stiletto/weapon_knife_stiletto.vmdl" },
        { WEAPON_KNIFE_WIDOWMAKER,     4029975521u, "weapons/models/knife/knife_talon/weapon_knife_talon.vmdl" },
        { WEAPON_KNIFE_SKELETON,       365028728u,  "weapons/models/knife/knife_skeleton/weapon_knife_skeleton.vmdl" },
        { WEAPON_KNIFE_KUKRI,          3845286452u, "weapons/models/knife/knife_kukri/weapon_knife_kukri.vmdl" },
    };

    static const KnifeData* GetKnifeData(int weapon_id) {
        for (const auto& kd : g_knife_data) {
            if (kd.weapon_id == weapon_id) return &kd;
        }
        return nullptr;
    }

    // ==================== ENTRY TRACKING ====================
    static int g_last_knife_entry = 0;
    static bool g_knife_needs_update = false; // set true when knife selection changes, consumed once by KnifeChanger

    // ==================== ENGINE FUNCTION POINTERS ====================
    // Resolved once via pattern scanning. Used for knife model + skin swaps.

    // C_BaseModelEntity::SetModel(this, const char* szModelName) — sets entity model
    using fnSetModel = void(__fastcall*)(void* thisptr, const char* szModelName);
    static fnSetModel g_fnSetModel = nullptr;

    // C_CSWeaponBase::UpdateSubclass(this) — triggers engine subclass reload after m_nSubclassID change
    using fnUpdateSubClass = void(__fastcall*)(void* thisptr);
    static fnUpdateSubClass g_fnUpdateSubClass = nullptr;

    // C_CSWeaponBase::UpdateComposite(this, bool bForce) — updates composite skin texture
    using fnUpdateComposite = void(__fastcall*)(void* thisptr, bool bForce);
    static fnUpdateComposite g_fnUpdateComposite = nullptr;

    // C_CSWeaponBase::UpdateComposite (secondary variant)
    static fnUpdateComposite g_fnUpdateCompositeSec = nullptr;

    // RegenerateWeaponSkins — scans all weapons and re-applies composite materials/paintkits
    using fnRegenerateWeaponSkins = void(__fastcall*)();
    static fnRegenerateWeaponSkins g_fnRegenerateWeaponSkins = nullptr;

    static bool g_engine_funcs_resolved = false;

    // Helper: try a single pattern in a module, return nullptr on failure (no throw)
    static std::uint8_t* TryPattern(const char* module, const char* pattern) {
        try {
            return sdk::find_pattern(module, pattern);
        }
        catch (...) {
            return nullptr;
        }
    }

    // Find SetModel by locating a known string reference in client.dll
    static fnSetModel FindSetModelViaStringRef() {
        HMODULE hClient = GetModuleHandleA("client.dll");
        if (!hClient) return nullptr;

        auto* dos = reinterpret_cast<PIMAGE_DOS_HEADER>(hClient);
        auto* nt = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(hClient) + dos->e_lfanew);
        uint8_t* base = reinterpret_cast<uint8_t*>(hClient);
        DWORD size = nt->OptionalHeader.SizeOfImage;

        const char* target_str = "weapons/models/defuser/defuser.vmdl";
        size_t target_len = strlen(target_str);
        uint8_t* str_addr = nullptr;

        for (DWORD i = 0; i < size - target_len; i++) {
            if (memcmp(base + i, target_str, target_len) == 0) {
                str_addr = base + i;
                break;
            }
        }
        if (!str_addr) return nullptr;

        uint8_t* lea_site = nullptr;
        for (DWORD i = 0; i < size - 7; i++) {
            uint8_t* ip = base + i;
            if (ip[0] == 0x48 && ip[1] == 0x8D && (ip[2] & 0xC7) == 0x05) {
                int32_t disp = *reinterpret_cast<int32_t*>(ip + 3);
                uint8_t* resolved = ip + 7 + disp;
                if (resolved == str_addr) {
                    lea_site = ip;
                    break;
                }
            }
        }
        if (!lea_site) return nullptr;

        for (int offset = 0; offset < 32; offset++) {
            uint8_t* check = lea_site + 7 + offset;
            if (check[0] == 0xE8) {
                int32_t disp = *reinterpret_cast<int32_t*>(check + 1);
                uint8_t* call_target = check + 5 + disp;
                if (call_target >= base && call_target < base + size)
                    return reinterpret_cast<fnSetModel>(call_target);
            }
        }
        return nullptr;
    }

    static void ResolveEngineFunctions() {
        if (g_engine_funcs_resolved) return;
        g_engine_funcs_resolved = true;

        auto& con = debug_console::Console::Get();

        // --- C_BaseModelEntity::SetModel: sets entity model ---
        auto sm_addr = TryPattern("client.dll", "48 89 5C 24 10 48 89 7C 24 20 55 48");
        if (sm_addr) {
            g_fnSetModel = reinterpret_cast<fnSetModel>(sm_addr);
            con.Success("[KNIFE] SetModel found via pattern at 0x%llX", reinterpret_cast<uintptr_t>(sm_addr));
        }
        if (!g_fnSetModel) {
            g_fnSetModel = FindSetModelViaStringRef();
            if (g_fnSetModel)
                con.Success("[KNIFE] SetModel found via string xref at 0x%llX", reinterpret_cast<uintptr_t>(g_fnSetModel));
        }
        if (!g_fnSetModel)
            con.Warning("[KNIFE] SetModel NOT found");

        // --- C_CSWeaponBase::UpdateSubclass: triggers engine subclass reload after m_nSubclassID change ---
        auto usc_addr = TryPattern("client.dll", "4C 8B DC 53 48 81 EC ? ? ? ? 48 8B 41");
        if (usc_addr) {
            g_fnUpdateSubClass = reinterpret_cast<fnUpdateSubClass>(usc_addr);
            con.Success("[KNIFE] UpdateSubclass found at 0x%llX", reinterpret_cast<uintptr_t>(usc_addr));
        }
        else {
            con.Warning("[KNIFE] UpdateSubclass NOT found");
        }

        // --- C_CSWeaponBase::UpdateComposite: refreshes composite skin texture ---
        auto comp_addr = TryPattern("client.dll", "48 89 5C 24 ? 57 48 83 EC ? 8B DA 48 8B F9 E8 ? ? ? ? 48 8D 8F");
        if (comp_addr) {
            g_fnUpdateComposite = reinterpret_cast<fnUpdateComposite>(comp_addr);
            con.Success("[KNIFE] UpdateComposite found at 0x%llX", reinterpret_cast<uintptr_t>(comp_addr));
        }
        else {
            con.Warning("[KNIFE] UpdateComposite NOT found");
        }

        // --- C_CSWeaponBase::UpdateComposite (secondary): handles additional composite layers ---
        auto comp2_addr = TryPattern("client.dll", "48 89 5C 24 ? 57 48 83 EC ? 8B DA 48 8B F9 E8 ? ? ? ? 48 8D 8F ? ? ? ? 48 8B D7");
        if (comp2_addr) {
            g_fnUpdateCompositeSec = reinterpret_cast<fnUpdateComposite>(comp2_addr);
            con.Success("[KNIFE] UpdateCompositeSec found at 0x%llX", reinterpret_cast<uintptr_t>(comp2_addr));
        }
        else {
            con.Warning("[KNIFE] UpdateCompositeSec NOT found");
        }

        // --- RegenerateWeaponSkins: forces engine to re-evaluate composite materials for all weapons ---
        auto regen_addr = TryPattern("client.dll", "48 83 EC ? E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 48 8B 10");
        if (regen_addr) {
            g_fnRegenerateWeaponSkins = reinterpret_cast<fnRegenerateWeaponSkins>(regen_addr);
            con.Success("[KNIFE] RegenerateWeaponSkins found at 0x%llX", reinterpret_cast<uintptr_t>(regen_addr));
        }
        else {
            con.Warning("[KNIFE] RegenerateWeaponSkins NOT found");
        }
    }

    // ==================== RATE LIMITING ====================
    static auto g_last_apply_time = std::chrono::steady_clock::now();
    static constexpr int APPLY_INTERVAL_MS = 250;
    static std::map<int, int> g_applied_skins; // weapon_id -> paint_kit already applied

    // Global cooldown for ForceFullUpdate to prevent prediction spam
    static auto g_last_full_update_time = std::chrono::steady_clock::now();
    static constexpr int FULL_UPDATE_COOLDOWN_MS = 5000;

    static bool ShouldApply() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - g_last_apply_time).count();
        if (elapsed < APPLY_INTERVAL_MS) return false;
        g_last_apply_time = now;
        return true;
    }

    // ==================== NETWORK RE-UPDATE ====================
    static bool ForceFullUpdate() {
        uintptr_t engine = (uintptr_t)GetModuleHandleA("engine2.dll");
        if (!engine) return false;

        uintptr_t net_client = *(uintptr_t*)(engine + cs2_dumper::offsets::engine2_dll::dwNetworkGameClient);
        if (!net_client || !sdk::is_valid_ptr(net_client)) return false;

        int* delta_tick = (int*)(net_client + cs2_dumper::offsets::engine2_dll::dwNetworkGameClient_deltaTick);
        if (*delta_tick != -1) {
            *delta_tick = -1;
        }
        return true;
    }

    // ==================== SCENE NODE HELPERS ====================
    static uintptr_t GetSceneNode(uintptr_t entity) {
        uintptr_t node = *(uintptr_t*)(entity + OFF_GAME_SCENE_NODE);
        if (!node || !sdk::is_valid_ptr(node)) return 0;
        return node;
    }

    static void SetMeshGroupMask(uintptr_t scene_node, uint64_t mask) {
        if (!scene_node) return;
        // scene_node is a CSkeletonInstance (inherits CGameSceneNode)
        // m_modelState is at CSkeletonInstance::m_modelState, m_MeshGroupMask inside that
        *reinterpret_cast<uint64_t*>(scene_node + OFF_MODEL_STATE + OFF_MESH_GROUP_MASK) = mask;
    }

    // ==================== LOADOUT / INVENTORY SYSTEM ====================
    // Reads the player's equipped loadout via CCSPlayerController -> m_pInventoryServices
    // -> m_vecNetworkableLoadout to obtain valid item IDs that the game recognizes.
    // This bridges the raw-memory approach with the game's inventory system.

    struct LoadoutItem {
        uintptr_t item_view;    // C_EconItemView*
        uint16_t def_index;     // m_iItemDefinitionIndex from the item view
        uint64_t item_id;       // m_iItemID (full uint64)
        uint32_t item_id_high;  // m_iItemIDHigh
        uint32_t item_id_low;   // m_iItemIDLow
        uint32_t account_id;    // m_iAccountID
        uint16_t team;          // 2=T, 3=CT
        uint16_t slot;          // loadout slot (0=melee, 1-2=pistols, etc.)
    };

    static uint8_t GetLocalTeam() {
        uintptr_t controller = game_state::GetLocalController();
        if (!controller || !sdk::is_valid_ptr(controller)) return 0;
        return *(uint8_t*)(controller + OFF_TEAM_NUM);
    }

    static std::vector<LoadoutItem> ReadLoadout() {
        std::vector<LoadoutItem> items;

        try {
            uintptr_t controller = game_state::GetLocalController();
            if (!controller || !sdk::is_valid_ptr(controller)) return items;

            uintptr_t inv_services = *(uintptr_t*)(controller + OFF_INV_SERVICES);
            if (!inv_services || !sdk::is_valid_ptr(inv_services)) return items;

            uintptr_t vec_base = inv_services + OFF_LOADOUT_VEC;

            // Source 2 CUtlVector layout: m_pMemory at +0x00, m_nAllocationCount at +0x08, m_nGrowSize at +0x0C, m_Size at +0x10
            uintptr_t data = *(uintptr_t*)(vec_base + 0x00);
            if (!data || !sdk::is_valid_ptr(data)) return items;

            int count = *(int*)(vec_base + 0x10);
            if (count <= 0 || count > 256) return items;

            for (int i = 0; i < count; i++) {
                // NetworkedLoadoutSlot_t: pItem(+0x00, 8B), team(+0x08, 2B), slot(+0x0A, 2B)
                // Stride = 16 bytes (aligned to pointer)
                uintptr_t slot_base = data + (i * 16);

                uintptr_t item_view = *(uintptr_t*)(slot_base + 0x0);
                if (!item_view || !sdk::is_valid_ptr(item_view)) continue;

                LoadoutItem li{};
                li.item_view = item_view;
                li.team = *(uint16_t*)(slot_base + 0x08);
                li.slot = *(uint16_t*)(slot_base + 0x0A);

                li.def_index = *(uint16_t*)(item_view +
                    cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex);
                li.item_id = *(uint64_t*)(item_view +
                    cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemID);
                li.item_id_high = *(uint32_t*)(item_view +
                    cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh);
                li.item_id_low = *(uint32_t*)(item_view +
                    cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDLow);
                li.account_id = *(uint32_t*)(item_view +
                    cs2_dumper::schemas::client_dll::C_EconItemView::m_iAccountID);

                items.push_back(li);
            }
        }
        catch (...) {}

        return items;
    }

    // Find loadout item matching a weapon def_index for the given team
    static const LoadoutItem* FindLoadoutItem(const std::vector<LoadoutItem>& loadout,
                                               uint16_t def_index, uint8_t team) {
        for (const auto& item : loadout) {
            if (item.team == team && item.def_index == def_index)
                return &item;
        }
        return nullptr;
    }

    // Find the melee slot (slot 0) for the given team — used for knife item IDs
    static const LoadoutItem* FindMeleeItem(const std::vector<LoadoutItem>& loadout,
                                             uint8_t team) {
        for (const auto& item : loadout) {
            if (item.team == team && item.slot == 0)
                return &item;
        }
        return nullptr;
    }

    // Copy item IDs from a loadout item to a weapon entity's EconItemView.
    // If no loadout item is available, falls back to legacy m_iItemIDHigh = -1 mode.
    static void CopyLoadoutItemToWeapon(uintptr_t weapon, const LoadoutItem* li) {
        if (!li || li->item_id == 0) {
            // No loadout item available — use legacy fallback
            uint32_t original_xuid = *reinterpret_cast<uint32_t*>(weapon + OFF_OWNER_XUID_LOW);
            *reinterpret_cast<int*>(weapon + OFF_ITEM_ID_HIGH) = -1;
            *reinterpret_cast<uint32_t*>(weapon + OFF_ACCOUNT_ID) = original_xuid;
            return;
        }

        // Write the real item IDs from the inventory loadout
        *reinterpret_cast<uint64_t*>(weapon + OFF_ITEM_ID) = li->item_id;
        *reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_HIGH) = li->item_id_high;
        *reinterpret_cast<uint32_t*>(weapon + OFF_ITEM_ID_LOW) = li->item_id_low;
        *reinterpret_cast<uint32_t*>(weapon + OFF_ACCOUNT_ID) = li->account_id;
    }

    // ==================== WEAPON ITERATION ====================
    static std::vector<uintptr_t> GetPlayerWeapons() {
        std::vector<uintptr_t> weapons;

        if (!game_state::IsInGame()) return weapons;

        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return weapons;

        try {
            uintptr_t local_pawn = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
            if (!local_pawn || !sdk::is_valid_ptr(local_pawn)) return weapons;

            uintptr_t entity_list = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwEntityList);
            if (!entity_list || !sdk::is_valid_ptr(entity_list)) return weapons;

            uintptr_t weapon_services = *(uintptr_t*)(local_pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pWeaponServices);
            if (!weapon_services || !sdk::is_valid_ptr(weapon_services)) return weapons;

            uintptr_t weapons_array = weapon_services + cs2_dumper::schemas::client_dll::CPlayer_WeaponServices::m_hMyWeapons;

            for (int i = 0; i < 64; i++) {
                uint32_t weapon_handle = *(uint32_t*)(weapons_array + (i * 0x4));
                if (!weapon_handle || weapon_handle == 0xFFFFFFFF) continue;

                uintptr_t list_entry = *(uintptr_t*)(entity_list + 0x8 * ((weapon_handle & 0x7FFF) >> 9) + 16);
                if (!list_entry || !sdk::is_valid_ptr(list_entry)) continue;

                uintptr_t weapon = *(uintptr_t*)(list_entry + 112 * (weapon_handle & 0x1FF));
                if (weapon && sdk::is_valid_ptr(weapon)) {
                    weapons.push_back(weapon);
                }
            }
        }
        catch (...) {}

        return weapons;
    }

    // Get active weapon entity of local player
    static uintptr_t GetActiveWeapon() {
        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return 0;

        try {
            uintptr_t local_pawn = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
            if (!local_pawn || !sdk::is_valid_ptr(local_pawn)) return 0;

            uintptr_t entity_list = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwEntityList);
            if (!entity_list || !sdk::is_valid_ptr(entity_list)) return 0;

            uintptr_t weapon_services = *(uintptr_t*)(local_pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pWeaponServices);
            if (!weapon_services || !sdk::is_valid_ptr(weapon_services)) return 0;

            uint32_t active_handle = *(uint32_t*)(weapon_services + cs2_dumper::schemas::client_dll::CPlayer_WeaponServices::m_hActiveWeapon);
            if (!active_handle || active_handle == 0xFFFFFFFF) return 0;

            uintptr_t list_entry = *(uintptr_t*)(entity_list + 0x8 * ((active_handle & 0x7FFF) >> 9) + 16);
            if (!list_entry || !sdk::is_valid_ptr(list_entry)) return 0;

            uintptr_t weapon = *(uintptr_t*)(list_entry + 112 * (active_handle & 0x1FF));
            if (weapon && sdk::is_valid_ptr(weapon)) return weapon;
        }
        catch (...) {}
        return 0;
    }

    // Resolve a CHandle through the entity list. Returns entity pointer or 0.
    static uintptr_t ResolveHandle(uintptr_t entity_list, uint32_t handle) {
        if (!handle || handle == 0xFFFFFFFF || !entity_list) return 0;
        try {
            uintptr_t list_entry = *(uintptr_t*)(entity_list + 0x8 * ((handle & 0x7FFF) >> 9) + 16);
            if (!list_entry || !sdk::is_valid_ptr(list_entry)) return 0;
            uintptr_t ent = *(uintptr_t*)(list_entry + 112 * (handle & 0x1FF));
            if (ent && sdk::is_valid_ptr(ent)) return ent;
        }
        catch (...) {}
        return 0;
    }

    // Get the arms entity (C_CS2HudModelArms) for the local player.
    // In CS2, the first-person weapon is NOT a separate viewmodel entity.
    // The weapon's m_nSubclassID + UpdateSubclass controls which model/animations
    // are used in first person. The arms entity renders the hands.
    static uintptr_t GetArmsEntity() {
        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return 0;

        try {
            uintptr_t local_pawn = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
            if (!local_pawn || !sdk::is_valid_ptr(local_pawn)) return 0;

            uintptr_t entity_list = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwEntityList);
            if (!entity_list || !sdk::is_valid_ptr(entity_list)) return 0;

            // m_hHudModelArms on C_CSPlayerPawn at offset 0x2400
            constexpr std::ptrdiff_t OFF_HUD_MODEL_ARMS = 0x2400;
            uint32_t arms_handle = *(uint32_t*)(local_pawn + OFF_HUD_MODEL_ARMS);
            return ResolveHandle(entity_list, arms_handle);
        }
        catch (...) {}
        return 0;
    }

    static uint16_t GetWeaponDefIndex(uintptr_t weapon_addr) {
        return *reinterpret_cast<uint16_t*>(weapon_addr + OFF_ITEM_DEF_INDEX);
    }

    static int GetOwnerEntryIndex(uintptr_t weapon_addr) {
        uint32_t handle = *(uint32_t*)(weapon_addr + OFF_OWNER_ENTITY);
        return handle & 0x7FFF;
    }

    // ==================== SKIN APPLICATION ====================

    static void ApplySkinWithLoadout(uintptr_t weapon, int weapon_id, const LoadoutItem* loadout_item) {
        if (!weapon) return;

        auto it = user_skins.find(weapon_id);
        if (it == user_skins.end()) return;

        PlayerSkinConfig& cfg = it->second;

        try {
            // Copy real item IDs from the inventory loadout (or legacy fallback)
            CopyLoadoutItemToWeapon(weapon, loadout_item);

            // Write fallback paint/seed/wear — engine renders from these
            *reinterpret_cast<int*>(weapon + OFF_FALLBACK_PAINT) = cfg.paint_kit;
            *reinterpret_cast<int*>(weapon + OFF_FALLBACK_SEED) = cfg.seed;
            *reinterpret_cast<float*>(weapon + OFF_FALLBACK_WEAR) = cfg.wear;

            // StatTrak
            if (cfg.stattrak) {
                *reinterpret_cast<int*>(weapon + OFF_FALLBACK_STATTRAK) = cfg.stattrak_count;
                *reinterpret_cast<int*>(weapon + OFF_ENTITY_QUALITY) = 9;
            }
            else {
                *reinterpret_cast<int*>(weapon + OFF_FALLBACK_STATTRAK) = -1;
                if (!IsKnife(weapon_id))
                    *reinterpret_cast<int*>(weapon + OFF_ENTITY_QUALITY) = 4;
            }

            // SOC bypass + material restore
            *reinterpret_cast<bool*>(weapon + OFF_DISALLOW_SOC) = false;
            *reinterpret_cast<bool*>(weapon + OFF_RESTORE_MATERIAL) = true;

            // Custom name tag
            if (!cfg.name_tag.empty()) {
                char* name_ptr = reinterpret_cast<char*>(weapon + OFF_CUSTOM_NAME);
                strncpy_s(name_ptr, 161, cfg.name_tag.c_str(), _TRUNCATE);
            }

            // Mesh group mask: select the painted model variant for paint kit rendering
            if (cfg.paint_kit > 0) {
                uintptr_t scene_node = GetSceneNode(weapon);
                if (scene_node)
                    SetMeshGroupMask(scene_node, 2);
            }
        }
        catch (...) {}
    }

    // Public wrapper — preserves the old API for external callers
    void ApplySkin(void* weapon, int weapon_id) {
        ApplySkinWithLoadout(reinterpret_cast<uintptr_t>(weapon), weapon_id, nullptr);
    }

    // ==================== KNIFE CHANGER ====================
    // Runs EVERY FRAME (not rate-limited).
    // In CS2, the first-person weapon model is driven by m_nSubclassID on the weapon entity.
    // UpdateSubclass tells the engine to reload the weapon type (model + animations).
    // SetModel sets the world model (third person). There is NO separate viewmodel entity.
    // NO ForceFullUpdate — it resets networkable fields back to server values.
    static void KnifeChanger(const std::vector<LoadoutItem>& loadout, uint8_t local_team) {
        ResolveEngineFunctions();

        if (!game_state::IsInGame()) return;

        const KnifeData* kd = GetKnifeData(selected_knife_id);
        if (!kd) return;

        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return;

        uintptr_t local_pawn = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
        if (!local_pawn || !sdk::is_valid_ptr(local_pawn)) return;

        uintptr_t entity_list = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwEntityList);
        if (!entity_list || !sdk::is_valid_ptr(entity_list)) return;

        uintptr_t weapon_services = *(uintptr_t*)(local_pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pWeaponServices);
        if (!weapon_services || !sdk::is_valid_ptr(weapon_services)) return;

        uintptr_t weapons_array = weapon_services + cs2_dumper::schemas::client_dll::CPlayer_WeaponServices::m_hMyWeapons;

        // Get the melee slot item from the inventory loadout for valid item IDs
        const LoadoutItem* melee = FindMeleeItem(loadout, local_team);

        // Determine paint kit for mesh group mask and composite refresh
        auto skin_cfg = user_skins.find(selected_knife_id);
        int paint_kit = 0;
        uint64_t mesh_mask = 1; // 1 = vanilla, 2 = painted
        if (skin_cfg != user_skins.end() && skin_cfg->second.paint_kit > 0) {
            paint_kit = skin_cfg->second.paint_kit;
            mesh_mask = 2;
        }

        for (int i = 0; i < 64; i++) {
            uint32_t weapon_handle = *(uint32_t*)(weapons_array + (i * 0x4));
            if (!weapon_handle || weapon_handle == 0xFFFFFFFF) continue;

            uintptr_t list_entry = *(uintptr_t*)(entity_list + 0x8 * ((weapon_handle & 0x7FFF) >> 9) + 16);
            if (!list_entry || !sdk::is_valid_ptr(list_entry)) continue;

            uintptr_t weapon = *(uintptr_t*)(list_entry + 112 * (weapon_handle & 0x1FF));
            if (!weapon || !sdk::is_valid_ptr(weapon)) continue;

            uint16_t def_index = GetWeaponDefIndex(weapon);
            if (!IsDefaultKnife(def_index) && def_index != static_cast<uint16_t>(kd->weapon_id))
                continue;

            // Use inventory loadout item IDs (or legacy fallback)
            CopyLoadoutItemToWeapon(weapon, melee);

            // SOC bypass + quality
            *reinterpret_cast<bool*>(weapon + OFF_DISALLOW_SOC) = false;
            *reinterpret_cast<bool*>(weapon + OFF_RESTORE_MATERIAL) = true;
            *reinterpret_cast<int*>(weapon + OFF_ENTITY_QUALITY) = 3;

            // Apply knife skin from config
            if (skin_cfg != user_skins.end()) {
                *reinterpret_cast<int*>(weapon + OFF_FALLBACK_PAINT) = skin_cfg->second.paint_kit;
                *reinterpret_cast<float*>(weapon + OFF_FALLBACK_WEAR) = skin_cfg->second.wear;
                *reinterpret_cast<int*>(weapon + OFF_FALLBACK_SEED) = skin_cfg->second.seed;
                *reinterpret_cast<int*>(weapon + OFF_FALLBACK_STATTRAK) = skin_cfg->second.stattrak ? skin_cfg->second.stattrak_count : -1;
            }
            else {
                *reinterpret_cast<int*>(weapon + OFF_FALLBACK_PAINT) = 0;
                *reinterpret_cast<float*>(weapon + OFF_FALLBACK_WEAR) = 0.0001f;
                *reinterpret_cast<int*>(weapon + OFF_FALLBACK_SEED) = 0;
                *reinterpret_cast<int*>(weapon + OFF_FALLBACK_STATTRAK) = -1;
            }

            // Write item definition + subclass every frame
            *reinterpret_cast<uint16_t*>(weapon + OFF_ITEM_DEF_INDEX) = static_cast<uint16_t>(kd->weapon_id);
            *reinterpret_cast<uint32_t*>(weapon + OFF_SUBCLASS_ID) = kd->subclass_hash;

            // SetModel on weapon every frame (world model / third person)
            if (g_fnSetModel)
                g_fnSetModel(reinterpret_cast<void*>(weapon), kd->model_path);

            // Mesh group mask on weapon every frame
            uintptr_t weapon_scene = GetSceneNode(weapon);
            if (weapon_scene)
                SetMeshGroupMask(weapon_scene, mesh_mask);

            // On knife change: UpdateSubclass + UpdateComposite + RegenerateWeaponSkins
            if (g_knife_needs_update) {
                auto& con = debug_console::Console::Get();
                con.Debug("[KNIFE] --- Per-weapon update ---");
                con.Debug("[KNIFE]   weapon=0x%llX  old_def=%d  new_def=%d", weapon, def_index, kd->weapon_id);
                con.Debug("[KNIFE]   subclass=0x%X  model=%s  mesh=%llu  paint=%d",
                    kd->subclass_hash, kd->model_path, mesh_mask, paint_kit);

                // Verify writes
                uint16_t v_def = GetWeaponDefIndex(weapon);
                uint32_t v_sub = *reinterpret_cast<uint32_t*>(weapon + OFF_SUBCLASS_ID);
                con.Debug("[KNIFE]   VERIFY def=%d(exp %d) sub=0x%X(exp 0x%X)",
                    v_def, kd->weapon_id, v_sub, kd->subclass_hash);

                if (g_fnUpdateSubClass) {
                    g_fnUpdateSubClass(reinterpret_cast<void*>(weapon));
                    con.Debug("[KNIFE]   UpdateSubclass OK");
                } else {
                    con.Warning("[KNIFE]   UpdateSubclass NOT FOUND");
                }

                if (g_fnUpdateComposite) {
                    g_fnUpdateComposite(reinterpret_cast<void*>(weapon), true);
                    con.Debug("[KNIFE]   UpdateComposite OK");
                } else {
                    con.Warning("[KNIFE]   UpdateComposite NOT FOUND");
                }
                if (g_fnUpdateCompositeSec) {
                    g_fnUpdateCompositeSec(reinterpret_cast<void*>(weapon), true);
                    con.Debug("[KNIFE]   UpdateCompositeSec OK");
                } else {
                    con.Warning("[KNIFE]   UpdateCompositeSec NOT FOUND");
                }
            }
        }

        // One-time actions on knife change
        if (g_knife_needs_update) {
            auto& con = debug_console::Console::Get();

            if (g_fnRegenerateWeaponSkins) {
                g_fnRegenerateWeaponSkins();
                con.Debug("[KNIFE] RegenerateWeaponSkins OK");
            }

            // NO ForceFullUpdate — it resets networkable fields (defindex, subclass) to server values

            uintptr_t arms = GetArmsEntity();
            con.Success("[KNIFE] === CHANGE SUMMARY ===");
            con.Success("[KNIFE]   target      = %s (id=%d)", GetWeaponName(kd->weapon_id), kd->weapon_id);
            con.Success("[KNIFE]   model       = %s", kd->model_path);
            con.Success("[KNIFE]   subclass    = 0x%X", kd->subclass_hash);
            con.Success("[KNIFE]   SetModel    @ 0x%llX (%s)", (uintptr_t)g_fnSetModel, g_fnSetModel ? "OK" : "MISSING");
            con.Success("[KNIFE]   UpdateSub   @ 0x%llX (%s)", (uintptr_t)g_fnUpdateSubClass, g_fnUpdateSubClass ? "OK" : "MISSING");
            con.Success("[KNIFE]   Composite   @ 0x%llX (%s)", (uintptr_t)g_fnUpdateComposite, g_fnUpdateComposite ? "OK" : "MISSING");
            con.Success("[KNIFE]   CompositeSec@ 0x%llX (%s)", (uintptr_t)g_fnUpdateCompositeSec, g_fnUpdateCompositeSec ? "OK" : "MISSING");
            con.Success("[KNIFE]   RegenSkins  @ 0x%llX (%s)", (uintptr_t)g_fnRegenerateWeaponSkins, g_fnRegenerateWeaponSkins ? "OK" : "MISSING");
            con.Success("[KNIFE]   arms entity = 0x%llX (%s)", arms, arms ? "OK" : "NONE");
            con.Success("[KNIFE]   melee loadout = %s", melee ? "YES" : "NO (legacy)");

            g_knife_needs_update = false;
            g_last_knife_entry = 0;
        }
    }

    // ==================== PUBLIC API ====================

    void ApplyAllSkins() {
        if (!game_state::IsInGame()) return;

        // Read the inventory loadout once per frame — provides valid item IDs
        // that the game's inventory system recognizes for each equipped weapon.
        std::vector<LoadoutItem> loadout;
        uint8_t local_team = 0;
        try {
            loadout = ReadLoadout();
            local_team = GetLocalTeam();
        }
        catch (...) {}

        try {
            // Knife changer runs EVERY FRAME (no rate limit).
            // Uses loadout melee item IDs for inventory-backed validation.
            KnifeChanger(loadout, local_team);
        }
        catch (...) {}

        // Weapon skins are rate-limited
        if (!ShouldApply()) return;

        try {
            auto weapons = GetPlayerWeapons();
            bool changed = false;

            for (auto weapon_addr : weapons) {
                if (!weapon_addr) continue;

                uint16_t def_index = GetWeaponDefIndex(weapon_addr);

                // Skip knives (handled above) and grenades
                if (IsKnife(def_index) || IsDefaultKnife(def_index)) continue;
                if (def_index >= WEAPON_FLASHBANG && def_index <= WEAPON_INCGRENADE) continue;
                if (def_index == WEAPON_C4 || def_index == WEAPON_TASER) continue;

                auto cfg_it = user_skins.find(def_index);
                if (cfg_it != user_skins.end()) {
                    auto applied_it = g_applied_skins.find(def_index);
                    bool needs_update = (applied_it == g_applied_skins.end() || applied_it->second != cfg_it->second.paint_kit);

                    // Find the matching loadout item for inventory-based item ID
                    const LoadoutItem* li = FindLoadoutItem(loadout, def_index, local_team);
                    ApplySkinWithLoadout(weapon_addr, def_index, li);

                    if (needs_update) {
                        g_applied_skins[def_index] = cfg_it->second.paint_kit;
                        changed = true;
                    }
                }
            }

            if (changed) {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - g_last_full_update_time).count();
                if (elapsed >= FULL_UPDATE_COOLDOWN_MS) {
                    ForceFullUpdate();
                    g_last_full_update_time = now;
                }
            }
        }
        catch (...) {}
    }

    void ApplyKnife() {
        if (!game_state::IsInGame()) return;
        g_last_knife_entry = 0;
        g_knife_needs_update = true; // trigger one-time ForceFullUpdate for model load
    }

    void ApplyGloves() {
        if (!game_state::IsInGame()) return;
        if (selected_glove_kit <= 0) return;

        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return;

        try {
            uintptr_t local_pawn = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
            if (!local_pawn || !sdk::is_valid_ptr(local_pawn)) return;

            // m_EconGloves is an embedded C_EconItemView directly in the pawn
            uintptr_t glove_view = local_pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_EconGloves;

            *reinterpret_cast<bool*>(glove_view + cs2_dumper::schemas::client_dll::C_EconItemView::m_bDisallowSOC) = false;
            *reinterpret_cast<bool*>(glove_view + cs2_dumper::schemas::client_dll::C_EconItemView::m_bRestoreCustomMaterialAfterPrecache) = true;
            *reinterpret_cast<int*>(glove_view + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh) = -1;
            *reinterpret_cast<int*>(glove_view + cs2_dumper::schemas::client_dll::C_EconItemView::m_iAccountID) = 0;

            *reinterpret_cast<bool*>(local_pawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_bNeedToReApplyGloves) = true;

            ForceFullUpdate();

            debug_console::Console::Get().Success("[GLOVES] Applied glove kit %d", selected_glove_kit);
        }
        catch (...) {
            debug_console::Console::Get().Error("[GLOVES] Exception during apply");
        }
    }

    // ==================== HTTP + JSON (skin database) ====================

    static std::string HTTPGet(const std::string& url) {
        std::string response;
        HINTERNET hInternet = InternetOpenA(
            "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36",
            INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (!hInternet) {
            debug_console::Console::Get().Error("[HTTP] InternetOpenA failed (err=%lu)", GetLastError());
            return "";
        }

        DWORD flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE;
        if (url.find("https://") == 0)
            flags |= INTERNET_FLAG_SECURE;

        HINTERNET hConnect = InternetOpenUrlA(
            hInternet, url.c_str(), NULL, 0, flags, 0);
        if (!hConnect) {
            debug_console::Console::Get().Error("[HTTP] InternetOpenUrlA failed (err=%lu)", GetLastError());
            InternetCloseHandle(hInternet);
            return "";
        }

        char buffer[8192];
        DWORD bytesRead = 0;
        while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0)
            response.append(buffer, bytesRead);

        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        debug_console::Console::Get().Success("[HTTP] Got %zu bytes from %s", response.size(), url.c_str());
        return response;
    }

    static int ExtractIntValue(const std::string& obj, const std::string& key) {
        std::string search = "\"" + key + "\":";
        size_t pos = obj.find(search);
        if (pos == std::string::npos) return 0;
        pos += search.length();
        while (pos < obj.length() && (obj[pos] == ' ' || obj[pos] == '\t' || obj[pos] == '\n' || obj[pos] == '\r')) pos++;
        size_t end = pos;
        while (end < obj.length() && std::isdigit(obj[end])) end++;
        if (end > pos) { try { return std::stoi(obj.substr(pos, end - pos)); } catch (...) {} }
        return 0;
    }

    static std::string ExtractStringValue(const std::string& obj, const std::string& key) {
        std::string search = "\"" + key + "\":";
        size_t pos = obj.find(search);
        if (pos == std::string::npos) return "";
        pos += search.length();
        // Skip whitespace between : and opening quote
        while (pos < obj.length() && (obj[pos] == ' ' || obj[pos] == '\t' || obj[pos] == '\n' || obj[pos] == '\r')) pos++;
        if (pos >= obj.length() || obj[pos] != '"') return "";
        pos++; // skip opening quote
        size_t end = obj.find("\"", pos);
        if (end != std::string::npos) return obj.substr(pos, end - pos);
        return "";
    }

    static bool ExtractBoolValue(const std::string& obj, const std::string& key) {
        std::string search = "\"" + key + "\":";
        size_t pos = obj.find(search);
        if (pos == std::string::npos) return false;
        pos += search.length();
        while (pos < obj.length() && (obj[pos] == ' ' || obj[pos] == '\t' || obj[pos] == '\n' || obj[pos] == '\r')) pos++;
        return obj.substr(pos, 4) == "true";
    }

    // Extracts an int value that may be bare (123) or quoted ("123")
    static int ExtractIntOrQuotedInt(const std::string& obj, const std::string& key) {
        std::string search = "\"" + key + "\":";
        size_t pos = obj.find(search);
        if (pos == std::string::npos) return 0;
        pos += search.length();
        while (pos < obj.length() && (obj[pos] == ' ' || obj[pos] == '\t' || obj[pos] == '\n' || obj[pos] == '\r')) pos++;
        if (pos < obj.length() && obj[pos] == '"') pos++; // skip optional quote
        size_t end = pos;
        while (end < obj.length() && std::isdigit(obj[end])) end++;
        if (end > pos) { try { return std::stoi(obj.substr(pos, end - pos)); } catch (...) {} }
        return 0;
    }

    // ==================== DATABASE LOADING ====================

    // Source: Nereziel/cs2-WeaponPaints (flat JSON, no nesting)
    // skins_en.json fields: weapon_defindex (int), weapon_name (string), paint (int|string), paint_name (string)
    // gloves_en.json fields: weapon_defindex (int), paint (int|string), paint_name (string)

    void LoadSkinsFromAPI() {
        skin_database.clear();
        skins_by_weapon.clear();
        glove_database.clear();

        debug_console::Console::Get().Success("[SKINS] Loading skin database from API...");

        try {
            // Load weapon + knife skins (all in one file, flat JSON)
            std::string skins_url = "https://raw.githubusercontent.com/Nereziel/cs2-WeaponPaints/main/website/data/skins_en.json";
            std::string skins_data = HTTPGet(skins_url);

            debug_console::Console::Get().Success("[SKINS] Skins response: %zu bytes", skins_data.size());

            if (!skins_data.empty()) {
                size_t pos = 0;
                while ((pos = skins_data.find("{", pos)) != std::string::npos) {
                    try {
                        size_t end = skins_data.find("}", pos);
                        if (end == std::string::npos) break;
                        std::string obj = skins_data.substr(pos, end - pos + 1);

                        int weapon_id = ExtractIntValue(obj, "weapon_defindex");
                        int paint_kit = ExtractIntOrQuotedInt(obj, "paint");
                        std::string paint_name = ExtractStringValue(obj, "paint_name");

                        if (paint_kit > 0 && weapon_id > 0 && !paint_name.empty()) {
                            // Extract skin name from "AK-47 | Searing Rage" -> "Searing Rage"
                            std::string skin_name = paint_name;
                            std::string weapon_display = GetWeaponName(weapon_id);
                            size_t sep = paint_name.find(" | ");
                            if (sep != std::string::npos) {
                                skin_name = paint_name.substr(sep + 3);
                                // Use the weapon part from paint_name for display if GetWeaponName returns Unknown
                                if (std::string(weapon_display).find("Unknown") != std::string::npos) {
                                    weapon_display = paint_name.substr(0, sep);
                                    // Strip leading star symbol
                                    if (weapon_display.size() > 4 && weapon_display[0] == '\xe2')
                                        weapon_display = weapon_display.substr(4);
                                }
                            }

                            SkinInfo skin(paint_kit, skin_name, weapon_display, weapon_id, RARITY_COMMON, true);
                            skin_database.push_back(skin);
                            skins_by_weapon[weapon_display].push_back(skin);
                        }
                        pos = end + 1;
                    }
                    catch (...) { pos++; }
                }
            }

            debug_console::Console::Get().Success("[SKINS] Parsed %zu skins from API", skin_database.size());

            // Load gloves (separate flat JSON file)
            std::string gloves_url = "https://raw.githubusercontent.com/Nereziel/cs2-WeaponPaints/main/website/data/gloves_en.json";
            std::string gloves_data = HTTPGet(gloves_url);

            debug_console::Console::Get().Success("[SKINS] Gloves response: %zu bytes", gloves_data.size());

            if (!gloves_data.empty()) {
                size_t pos = 0;
                while ((pos = gloves_data.find("{", pos)) != std::string::npos) {
                    try {
                        size_t end = gloves_data.find("}", pos);
                        if (end == std::string::npos) break;
                        std::string obj = gloves_data.substr(pos, end - pos + 1);

                        int paint_kit = ExtractIntOrQuotedInt(obj, "paint");
                        std::string paint_name = ExtractStringValue(obj, "paint_name");

                        if (paint_kit > 0 && !paint_name.empty()) {
                            // Extract glove skin name from "★ Hand Wraps | Spruce DDPAT" -> "Spruce DDPAT"
                            std::string glove_name = paint_name;
                            size_t sep = paint_name.find(" | ");
                            if (sep != std::string::npos)
                                glove_name = paint_name.substr(sep + 3);
                            glove_database.push_back(GloveInfo(paint_kit, glove_name));
                        }
                        pos = end + 1;
                    }
                    catch (...) { pos++; }
                }
            }

            debug_console::Console::Get().Success("[SKINS] Parsed %zu gloves from API", glove_database.size());

            if (skin_database.empty() && glove_database.empty()) {
                debug_console::Console::Get().Error("[SKINS] API returned no data, using fallback");
                InitializeSkinDatabase();
            }
        }
        catch (...) {
            debug_console::Console::Get().Error("[SKINS] Exception during API load, using fallback");
            InitializeSkinDatabase();
        }
    }

    void InitializeSkinDatabase() {
        // Fallback with verified paint kit IDs from cs2-WeaponPaints
        skin_database.push_back(SkinInfo(282, "Redline", "AK-47", WEAPON_AK47, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(180, "Vulcan", "AK-47", WEAPON_AK47, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(490, "Aquamarine Revenge", "AK-47", WEAPON_AK47, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(524, "Neon Revolution", "AK-47", WEAPON_AK47, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(675, "Bloodsport", "AK-47", WEAPON_AK47, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(730, "Empress", "AK-47", WEAPON_AK47, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(174, "Boom", "AWP", WEAPON_AWP, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(279, "Asiimov", "AWP", WEAPON_AWP, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(344, "Dragon Lore", "AWP", WEAPON_AWP, RARITY_CONTRABAND, true));
        skin_database.push_back(SkinInfo(475, "Hyper Beast", "AWP", WEAPON_AWP, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(309, "Howl", "M4A4", WEAPON_M4A1, RARITY_CONTRABAND, true));
        skin_database.push_back(SkinInfo(255, "Asiimov", "M4A4", WEAPON_M4A1, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(588, "Desolate Space", "M4A4", WEAPON_M4A1, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(360, "Cyrex", "M4A1-S", WEAPON_M4A1_SILENCER, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(587, "Golden Coil", "M4A1-S", WEAPON_M4A1_SILENCER, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(351, "Hypnotic", "Desert Eagle", WEAPON_DEAGLE, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(37, "Blaze", "Desert Eagle", WEAPON_DEAGLE, RARITY_ANCIENT, false));
        skin_database.push_back(SkinInfo(277, "Orion", "USP-S", WEAPON_USP_SILENCER, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(504, "Kill Confirmed", "USP-S", WEAPON_USP_SILENCER, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(38, "Fade", "Glock-18", WEAPON_GLOCK, RARITY_ANCIENT, false));
        skin_database.push_back(SkinInfo(353, "Water Elemental", "Glock-18", WEAPON_GLOCK, RARITY_LEGENDARY, true));

        for (const auto& skin : skin_database)
            skins_by_weapon[skin.weapon_name].push_back(skin);

        glove_database.push_back(GloveInfo(10006, "Superconductor"));
        glove_database.push_back(GloveInfo(10007, "Arid"));
        glove_database.push_back(GloveInfo(10015, "Pandora's Box"));
        glove_database.push_back(GloveInfo(10016, "Foundation"));
        glove_database.push_back(GloveInfo(10018, "Vice"));
        glove_database.push_back(GloveInfo(10024, "Emerald Web"));
    }

    bool IsDatabaseLoaded() {
        return !skin_database.empty() || !glove_database.empty();
    }

    // ==================== BATCH OPERATIONS ====================

    void ClearAllSkins() {
        user_skins.clear();
        g_applied_skins.clear();
        selected_glove_kit = 0;
        g_last_knife_entry = 0;
        debug_console::Console::Get().Info("[SKIN] All configurations cleared");
    }

    void ApplyDefaultPreset() {
        PlayerSkinConfig ak; ak.weapon_id = WEAPON_AK47; ak.paint_kit = 282; ak.wear = 0.01f;
        user_skins[WEAPON_AK47] = ak;
        PlayerSkinConfig awp; awp.weapon_id = WEAPON_AWP; awp.paint_kit = 344; awp.wear = 0.01f;
        user_skins[WEAPON_AWP] = awp;
        PlayerSkinConfig deagle; deagle.weapon_id = WEAPON_DEAGLE; deagle.paint_kit = 37; deagle.wear = 0.01f;
        user_skins[WEAPON_DEAGLE] = deagle;
        debug_console::Console::Get().Success("[SKIN] Default preset loaded");
    }

    void ApplyFactoryNewAll() {
        for (auto& pair : user_skins) pair.second.wear = 0.0f;
        debug_console::Console::Get().Info("[SKIN] All skins set to Factory New");
    }

    void ApplyRandomSeeds() {
        for (auto& pair : user_skins) pair.second.seed = rand() % 1000;
        debug_console::Console::Get().Info("[SKIN] All pattern seeds randomized");
    }

    // ==================== NAME / RARITY ====================

    const char* GetWeaponName(int weapon_id) {
        switch (weapon_id) {
        case WEAPON_AK47: return "AK-47";
        case WEAPON_M4A1: return "M4A4";
        case WEAPON_M4A1_SILENCER: return "M4A1-S";
        case WEAPON_AWP: return "AWP";
        case WEAPON_DEAGLE: return "Desert Eagle";
        case WEAPON_GLOCK: return "Glock-18";
        case WEAPON_USP_SILENCER: return "USP-S";
        case WEAPON_P250: return "P250";
        case WEAPON_FIVESEVEN: return "Five-SeveN";
        case WEAPON_TEC9: return "Tec-9";
        case WEAPON_CZ75A: return "CZ75-Auto";
        case WEAPON_REVOLVER: return "R8 Revolver";
        case WEAPON_ELITE: return "Dual Berettas";
        case WEAPON_HKP2000: return "P2000";
        case WEAPON_NOVA: return "Nova";
        case WEAPON_XM1014: return "XM1014";
        case WEAPON_SAWEDOFF: return "Sawed-Off";
        case WEAPON_MAG7: return "MAG-7";
        case WEAPON_M249: return "M249";
        case WEAPON_NEGEV: return "Negev";
        case WEAPON_MAC10: return "MAC-10";
        case WEAPON_MP9: return "MP9";
        case WEAPON_MP7: return "MP7";
        case WEAPON_MP5SD: return "MP5-SD";
        case WEAPON_UMP45: return "UMP-45";
        case WEAPON_P90: return "P90";
        case WEAPON_BIZON: return "PP-Bizon";
        case WEAPON_FAMAS: return "FAMAS";
        case WEAPON_GALILAR: return "Galil AR";
        case WEAPON_AUG: return "AUG";
        case WEAPON_SG556: return "SG 553";
        case WEAPON_SSG08: return "SSG 08";
        case WEAPON_SCAR20: return "SCAR-20";
        case WEAPON_G3SG1: return "G3SG1";
        case WEAPON_KNIFE_BAYONET: return "Bayonet";
        case WEAPON_KNIFE_CLASSIC: return "Classic Knife";
        case WEAPON_KNIFE_FLIP: return "Flip Knife";
        case WEAPON_KNIFE_GUT: return "Gut Knife";
        case WEAPON_KNIFE_KARAMBIT: return "Karambit";
        case WEAPON_KNIFE_M9_BAYONET: return "M9 Bayonet";
        case WEAPON_KNIFE_TACTICAL: return "Huntsman Knife";
        case WEAPON_KNIFE_FALCHION: return "Falchion Knife";
        case WEAPON_KNIFE_SURVIVAL_BOWIE: return "Bowie Knife";
        case WEAPON_KNIFE_BUTTERFLY: return "Butterfly Knife";
        case WEAPON_KNIFE_PUSH: return "Shadow Daggers";
        case WEAPON_KNIFE_CORD: return "Paracord Knife";
        case WEAPON_KNIFE_CANIS: return "Survival Knife";
        case WEAPON_KNIFE_URSUS: return "Ursus Knife";
        case WEAPON_KNIFE_GYPSY_JACKKNIFE: return "Navaja Knife";
        case WEAPON_KNIFE_OUTDOOR: return "Nomad Knife";
        case WEAPON_KNIFE_STILETTO: return "Stiletto Knife";
        case WEAPON_KNIFE_WIDOWMAKER: return "Talon Knife";
        case WEAPON_KNIFE_SKELETON: return "Skeleton Knife";
        case WEAPON_KNIFE_KUKRI: return "Kukri Knife";
        case WEAPON_KNIFE_CT: return "CT Knife";
        case WEAPON_KNIFE_T: return "T Knife";
        case WEAPON_TASER: return "Zeus x27";
        default: return "Unknown";
        }
    }

    const char* GetRarityName(SkinRarity rarity) {
        switch (rarity) {
        case RARITY_COMMON: return "Consumer Grade";
        case RARITY_UNCOMMON: return "Industrial Grade";
        case RARITY_RARE: return "Mil-Spec Grade";
        case RARITY_MYTHICAL: return "Restricted";
        case RARITY_LEGENDARY: return "Classified";
        case RARITY_ANCIENT: return "Covert";
        case RARITY_CONTRABAND: return "Contraband";
        default: return "Unknown";
        }
    }

    const float* GetRarityColor(SkinRarity rarity) {
        static float colors[][4] = {
            {0.7f, 0.7f, 0.7f, 1.0f},
            {0.4f, 0.6f, 0.9f, 1.0f},
            {0.3f, 0.4f, 0.8f, 1.0f},
            {0.5f, 0.3f, 0.8f, 1.0f},
            {0.8f, 0.3f, 0.6f, 1.0f},
            {0.9f, 0.2f, 0.2f, 1.0f},
            {0.95f, 0.8f, 0.1f, 1.0f}
        };
        if (rarity >= 0 && rarity <= RARITY_CONTRABAND) return colors[rarity];
        return colors[0];
    }

    // ==================== DEBUG READBACK ====================

    std::vector<WeaponDebugInfo> GetCurrentWeaponsDebugInfo() {
        std::vector<WeaponDebugInfo> result;

        if (!game_state::IsInGame()) return result;

        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (!client) return result;

        try {
            uintptr_t local_pawn = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
            if (!local_pawn || !sdk::is_valid_ptr(local_pawn)) return result;

            uintptr_t entity_list = *(uintptr_t*)(client + cs2_dumper::offsets::client_dll::dwEntityList);
            if (!entity_list || !sdk::is_valid_ptr(entity_list)) return result;

            uintptr_t weapon_services = *(uintptr_t*)(local_pawn + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pWeaponServices);
            if (!weapon_services || !sdk::is_valid_ptr(weapon_services)) return result;

            // Resolve active weapon handle for comparison
            uint32_t active_handle = *(uint32_t*)(weapon_services + cs2_dumper::schemas::client_dll::CPlayer_WeaponServices::m_hActiveWeapon);
            uintptr_t active_weapon = 0;
            if (active_handle && active_handle != 0xFFFFFFFF) {
                uintptr_t le = *(uintptr_t*)(entity_list + 0x8 * ((active_handle & 0x7FFF) >> 9) + 16);
                if (le && sdk::is_valid_ptr(le)) {
                    active_weapon = *(uintptr_t*)(le + 112 * (active_handle & 0x1FF));
                }
            }

            uintptr_t weapons_array = weapon_services + cs2_dumper::schemas::client_dll::CPlayer_WeaponServices::m_hMyWeapons;

            for (int i = 0; i < 64; i++) {
                uint32_t weapon_handle = *(uint32_t*)(weapons_array + (i * 0x4));
                if (!weapon_handle || weapon_handle == 0xFFFFFFFF) continue;

                uintptr_t list_entry = *(uintptr_t*)(entity_list + 0x8 * ((weapon_handle & 0x7FFF) >> 9) + 16);
                if (!list_entry || !sdk::is_valid_ptr(list_entry)) continue;

                uintptr_t w = *(uintptr_t*)(list_entry + 112 * (weapon_handle & 0x1FF));
                if (!w || !sdk::is_valid_ptr(w)) continue;

                WeaponDebugInfo info;
                info.address = w;
                info.is_active = (w == active_weapon);

                info.def_index         = *reinterpret_cast<uint16_t*>(w + OFF_ITEM_DEF_INDEX);
                info.entity_quality    = *reinterpret_cast<int*>(w + OFF_ENTITY_QUALITY);
                info.item_id_high      = *reinterpret_cast<int*>(w + OFF_ITEM_ID_HIGH);
                info.item_id           = *reinterpret_cast<uint64_t*>(w + OFF_ITEM_ID);
                info.account_id        = *reinterpret_cast<uint32_t*>(w + OFF_ACCOUNT_ID);
                info.disallow_soc      = *reinterpret_cast<bool*>(w + OFF_DISALLOW_SOC);
                info.restore_material  = *reinterpret_cast<bool*>(w + OFF_RESTORE_MATERIAL);
                info.fallback_paint_kit = *reinterpret_cast<int*>(w + OFF_FALLBACK_PAINT);
                info.fallback_seed     = *reinterpret_cast<int*>(w + OFF_FALLBACK_SEED);
                info.fallback_wear     = *reinterpret_cast<float*>(w + OFF_FALLBACK_WEAR);
                info.fallback_stattrak = *reinterpret_cast<int*>(w + OFF_FALLBACK_STATTRAK);
                info.owner_xuid_low    = *reinterpret_cast<uint32_t*>(w + OFF_OWNER_XUID_LOW);
                info.subclass_id       = *reinterpret_cast<uint32_t*>(w + OFF_SUBCLASS_ID);
                info.loadout_matched   = (info.item_id != 0 && info.item_id_high != -1);

                char* name_src = reinterpret_cast<char*>(w + OFF_CUSTOM_NAME);
                strncpy_s(info.custom_name, name_src, sizeof(info.custom_name) - 1);
                info.custom_name[sizeof(info.custom_name) - 1] = '\0';

                result.push_back(info);
            }
        }
        catch (...) {}

        return result;
    }

    bool IsSetModelAvailable() {
        return g_fnSetModel != nullptr;
    }

    int GetLoadoutItemCount() {
        try {
            auto loadout = ReadLoadout();
            return static_cast<int>(loadout.size());
        }
        catch (...) { return 0; }
    }
}
