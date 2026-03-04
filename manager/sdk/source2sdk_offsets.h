#pragma once
// =============================================================================
// Source2SDK-Derived Offsets
// =============================================================================
// Drop-in replacement for external/offsets/client_dll.hpp (schema offsets only).
// All values are taken from the source2sdk struct definitions located at
//   external/source2sdk/client/*.hpp
// When the SDK is updated, re-derive these values from the struct comments.
//
// NOTE: offsets.hpp (dwEntityList, dwLocalPlayerPawn, etc.) is still required
//       for global/module-level addresses — this header only covers *schema*
//       (per-class field) offsets.
// =============================================================================

#include <cstddef>
#include <cstdint>

namespace cs2_dumper {
    namespace schemas {
        namespace client_dll {

            // source2sdk/client/C_BaseEntity.hpp
            namespace C_BaseEntity {
                constexpr std::ptrdiff_t m_CBodyComponent   = 0x38;  // CBodyComponent*
                constexpr std::ptrdiff_t m_pGameSceneNode   = 0x338; // CGameSceneNode*
                constexpr std::ptrdiff_t m_iMaxHealth       = 0x350; // int32
                constexpr std::ptrdiff_t m_iHealth          = 0x354; // int32
                constexpr std::ptrdiff_t m_lifeState        = 0x35C; // uint8
                constexpr std::ptrdiff_t m_nSubclassID      = 0x388; // CUtlStringToken
                constexpr std::ptrdiff_t m_iTeamNum         = 0x3F3; // uint8
                constexpr std::ptrdiff_t m_fFlags           = 0x400; // uint32
                constexpr std::ptrdiff_t m_hOwnerEntity     = 0x528; // CHandle<C_BaseEntity>
            }

            // source2sdk/client/C_BaseModelEntity.hpp
            namespace C_BaseModelEntity {
                constexpr std::ptrdiff_t m_vecViewOffset = 0xD58; // CNetworkViewOffsetVector
            }

            // source2sdk/client/C_BasePlayerPawn.hpp
            namespace C_BasePlayerPawn {
                constexpr std::ptrdiff_t m_pWeaponServices = 0x13D8; // CPlayer_WeaponServices*
                constexpr std::ptrdiff_t m_vOldOrigin      = 0x1588; // Vector
            }

            // source2sdk/client/CPlayer_WeaponServices.hpp
            namespace CPlayer_WeaponServices {
                constexpr std::ptrdiff_t m_hMyWeapons    = 0x48; // C_NetworkUtlVectorBase<CHandle>
                constexpr std::ptrdiff_t m_hActiveWeapon = 0x60; // CHandle<C_BasePlayerWeapon>
            }

            // source2sdk/client/C_EconEntity.hpp
            namespace C_EconEntity {
                constexpr std::ptrdiff_t m_AttributeManager      = 0x1378; // C_AttributeContainer
                constexpr std::ptrdiff_t m_OriginalOwnerXuidLow  = 0x1848; // uint32
                constexpr std::ptrdiff_t m_OriginalOwnerXuidHigh = 0x184C; // uint32
                constexpr std::ptrdiff_t m_nFallbackPaintKit     = 0x1850; // int32
                constexpr std::ptrdiff_t m_nFallbackSeed         = 0x1854; // int32
                constexpr std::ptrdiff_t m_flFallbackWear        = 0x1858; // float32
                constexpr std::ptrdiff_t m_nFallbackStatTrak     = 0x185C; // int32
                constexpr std::ptrdiff_t m_hViewmodelAttachment  = 0x1880; // CHandle<CBaseAnimGraph>
            }

            // source2sdk/client/C_AttributeContainer.hpp
            namespace C_AttributeContainer {
                constexpr std::ptrdiff_t m_Item = 0x50; // C_EconItemView
            }

            // source2sdk/client/C_EconItemView.hpp
            namespace C_EconItemView {
                constexpr std::ptrdiff_t m_bRestoreCustomMaterialAfterPrecache = 0x1B8; // bool
                constexpr std::ptrdiff_t m_iItemDefinitionIndex = 0x1BA; // uint16
                constexpr std::ptrdiff_t m_iEntityQuality       = 0x1BC; // int32
                constexpr std::ptrdiff_t m_iItemID              = 0x1C8; // uint64
                constexpr std::ptrdiff_t m_iItemIDHigh          = 0x1D0; // uint32
                constexpr std::ptrdiff_t m_iItemIDLow           = 0x1D4; // uint32
                constexpr std::ptrdiff_t m_iAccountID           = 0x1D8; // uint32
                constexpr std::ptrdiff_t m_bDisallowSOC         = 0x1E9; // bool
                constexpr std::ptrdiff_t m_szCustomName         = 0x2F8; // char[161]
            }

            // source2sdk/client/CCSPlayerController.hpp
            namespace CCSPlayerController {
                constexpr std::ptrdiff_t m_pInventoryServices = 0x810; // CCSPlayerController_InventoryServices*
                constexpr std::ptrdiff_t m_hPlayerPawn        = 0x90C; // CHandle<C_CSPlayerPawn>
            }

            // source2sdk/client/CCSPlayerController_InventoryServices.hpp
            namespace CCSPlayerController_InventoryServices {
                constexpr std::ptrdiff_t m_vecNetworkableLoadout = 0x40; // CUtlVector<NetworkedLoadoutSlot_t>
            }

            // source2sdk/client/C_CSPlayerPawnBase.hpp
            namespace C_CSPlayerPawnBase {
                constexpr std::ptrdiff_t m_flFlashDuration = 0x15F8; // float32
            }

            // source2sdk/client/C_CSPlayerPawn.hpp
            namespace C_CSPlayerPawn {
                constexpr std::ptrdiff_t m_bNeedToReApplyGloves = 0x188D; // bool
                constexpr std::ptrdiff_t m_EconGloves           = 0x1890; // C_EconItemView
                constexpr std::ptrdiff_t m_aimPunchAngle        = 0x16CC; // QAngle
                constexpr std::ptrdiff_t m_entitySpottedState   = 0x26E0; // EntitySpottedState_t
                constexpr std::ptrdiff_t m_iShotsFired          = 0x270C; // int32
                constexpr std::ptrdiff_t m_ArmorValue           = 0x272C; // int32
                constexpr std::ptrdiff_t m_iIDEntIndex          = 0x3EAC; // CEntityIndex
            }

            // source2sdk/client/EntitySpottedState_t.hpp
            namespace EntitySpottedState_t {
                constexpr std::ptrdiff_t m_bSpotted       = 0x8;  // bool
                constexpr std::ptrdiff_t m_bSpottedByMask = 0xC;  // uint32[2]
            }

            // source2sdk/client/CSkeletonInstance.hpp
            namespace CSkeletonInstance {
                constexpr std::ptrdiff_t m_modelState = 0x160; // CModelState
            }

            // source2sdk/client/CModelState.hpp
            namespace CModelState {
                constexpr std::ptrdiff_t m_hModel        = 0xA0;  // CStrongHandle
                constexpr std::ptrdiff_t m_MeshGroupMask = 0x220; // uint64
            }

            // source2sdk/client/CGameSceneNode.hpp
            namespace CGameSceneNode {
                constexpr std::ptrdiff_t m_vecAbsOrigin = 0xD0;  // VectorWS
                constexpr std::ptrdiff_t m_bDormant     = 0x10B; // bool
            }

            // source2sdk/client/CBodyComponentBaseAnimGraph.hpp
            namespace CBodyComponentBaseAnimGraph {
                constexpr std::ptrdiff_t m_animationController = 0x550; // CBaseAnimGraphController
            }

            // source2sdk/client/CBaseAnimGraphController.hpp
            namespace CBaseAnimGraphController {
                constexpr std::ptrdiff_t m_pAnimGraphInstance = 0x14B0; // CSmartPtr<IAnimationGraphInstance>
            }

        } // namespace client_dll
    } // namespace schemas
} // namespace cs2_dumper
