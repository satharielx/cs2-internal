#pragma once
#include "../../external/offsets/client_dll.hpp"
#include <cstddef>

namespace skins::offsets {
    // Offset chain.
    inline constexpr std::ptrdiff_t ECON_ITEM_VIEW_BASE =
        cs2_dumper::schemas::client_dll::C_EconEntity::m_AttributeManager +
        cs2_dumper::schemas::client_dll::C_AttributeContainer::m_Item;

    inline constexpr std::ptrdiff_t OFF_ITEM_DEF_INDEX = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemDefinitionIndex;
    inline constexpr std::ptrdiff_t OFF_ENTITY_QUALITY = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iEntityQuality;
    inline constexpr std::ptrdiff_t OFF_ITEM_ID_HIGH = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDHigh;
    inline constexpr std::ptrdiff_t OFF_ITEM_ID_LOW = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemIDLow;
    inline constexpr std::ptrdiff_t OFF_ITEM_ID = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iItemID;
    inline constexpr std::ptrdiff_t OFF_ACCOUNT_ID = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_iAccountID;
    inline constexpr std::ptrdiff_t OFF_DISALLOW_SOC = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_bDisallowSOC;
    inline constexpr std::ptrdiff_t OFF_RESTORE_MATERIAL = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_bRestoreCustomMaterialAfterPrecache;
    inline constexpr std::ptrdiff_t OFF_CUSTOM_NAME = ECON_ITEM_VIEW_BASE + cs2_dumper::schemas::client_dll::C_EconItemView::m_szCustomName;

    inline constexpr std::ptrdiff_t OFF_FALLBACK_PAINT = cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackPaintKit;
    inline constexpr std::ptrdiff_t OFF_FALLBACK_SEED = cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackSeed;
    inline constexpr std::ptrdiff_t OFF_FALLBACK_WEAR = cs2_dumper::schemas::client_dll::C_EconEntity::m_flFallbackWear;
    inline constexpr std::ptrdiff_t OFF_FALLBACK_STATTRAK = cs2_dumper::schemas::client_dll::C_EconEntity::m_nFallbackStatTrak;
    inline constexpr std::ptrdiff_t OFF_OWNER_XUID_LOW = cs2_dumper::schemas::client_dll::C_EconEntity::m_OriginalOwnerXuidLow;

    inline constexpr std::ptrdiff_t OFF_GAME_SCENE_NODE = cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode;
    inline constexpr std::ptrdiff_t OFF_SUBCLASS_ID = cs2_dumper::schemas::client_dll::C_BaseEntity::m_nSubclassID;
    inline constexpr std::ptrdiff_t OFF_OWNER_ENTITY = cs2_dumper::schemas::client_dll::C_BaseEntity::m_hOwnerEntity;
    inline constexpr std::ptrdiff_t OFF_TEAM_NUM = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum;
    inline constexpr std::ptrdiff_t OFF_HEALTH = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth;

    inline constexpr std::ptrdiff_t OFF_MODEL_STATE = cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState;
    inline constexpr std::ptrdiff_t OFF_MESH_GROUP_MASK = cs2_dumper::schemas::client_dll::CModelState::m_MeshGroupMask;

    inline constexpr std::ptrdiff_t OFF_INV_SERVICES =
        cs2_dumper::schemas::client_dll::CCSPlayerController::m_pInventoryServices;
    inline constexpr std::ptrdiff_t OFF_LOADOUT_VEC =
        cs2_dumper::schemas::client_dll::CCSPlayerController_InventoryServices::m_vecNetworkableLoadout;

}
