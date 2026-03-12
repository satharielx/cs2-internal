/*
Module Name: Source 2 SDK schema-derived offsets
Authors: sathariel, martinmarinov
Product: Nephilimgate Multicheat
Tools used: imgui, a2x-cs2dumper
© 2026 sathariel & martinmarinov
*/

#pragma once

#include <cstddef>
#include <cstdint>

namespace cs2_dumper {
    namespace schemas {
        namespace client_dll {

            namespace C_BaseEntity {
                constexpr std::ptrdiff_t m_CBodyComponent   = 0x38;
                constexpr std::ptrdiff_t m_pGameSceneNode   = 0x338;
                constexpr std::ptrdiff_t m_iMaxHealth       = 0x350;
                constexpr std::ptrdiff_t m_iHealth          = 0x354;
                constexpr std::ptrdiff_t m_lifeState        = 0x35C;
                constexpr std::ptrdiff_t m_nSubclassID      = 0x388;
                constexpr std::ptrdiff_t m_iTeamNum         = 0x3F3;
                constexpr std::ptrdiff_t m_fFlags           = 0x400;
                constexpr std::ptrdiff_t m_hOwnerEntity     = 0x528;
            }

            namespace C_BaseModelEntity {
                constexpr std::ptrdiff_t m_vecViewOffset = 0xD58;
            }

            namespace C_BasePlayerPawn {
                constexpr std::ptrdiff_t m_pWeaponServices = 0x13D8;
                constexpr std::ptrdiff_t m_vOldOrigin      = 0x1588;
            }

            namespace CPlayer_WeaponServices {
                constexpr std::ptrdiff_t m_hMyWeapons    = 0x48;
                constexpr std::ptrdiff_t m_hActiveWeapon = 0x60;
            }

            namespace C_EconEntity {
                constexpr std::ptrdiff_t m_AttributeManager      = 0x1378;
                constexpr std::ptrdiff_t m_OriginalOwnerXuidLow  = 0x1848;
                constexpr std::ptrdiff_t m_OriginalOwnerXuidHigh = 0x184C;
                constexpr std::ptrdiff_t m_nFallbackPaintKit     = 0x1850;
                constexpr std::ptrdiff_t m_nFallbackSeed         = 0x1854;
                constexpr std::ptrdiff_t m_flFallbackWear        = 0x1858;
                constexpr std::ptrdiff_t m_nFallbackStatTrak     = 0x185C;
                constexpr std::ptrdiff_t m_hViewmodelAttachment  = 0x1880;
            }

            namespace C_AttributeContainer {
                constexpr std::ptrdiff_t m_Item = 0x50;
            }

            namespace C_EconItemView {
                constexpr std::ptrdiff_t m_bRestoreCustomMaterialAfterPrecache = 0x1B8;
                constexpr std::ptrdiff_t m_iItemDefinitionIndex = 0x1BA;
                constexpr std::ptrdiff_t m_iEntityQuality       = 0x1BC;
                constexpr std::ptrdiff_t m_iItemID              = 0x1C8;
                constexpr std::ptrdiff_t m_iItemIDHigh          = 0x1D0;
                constexpr std::ptrdiff_t m_iItemIDLow           = 0x1D4;
                constexpr std::ptrdiff_t m_iAccountID           = 0x1D8;
                constexpr std::ptrdiff_t m_bDisallowSOC         = 0x1E9;
                constexpr std::ptrdiff_t m_szCustomName         = 0x2F8;
            }

            namespace CCSPlayerController {
                constexpr std::ptrdiff_t m_pInventoryServices = 0x810;
                constexpr std::ptrdiff_t m_hPlayerPawn        = 0x90C;
            }

            namespace CCSPlayerController_InventoryServices {
                constexpr std::ptrdiff_t m_vecNetworkableLoadout = 0x40;
            }

            namespace C_CSPlayerPawnBase {
                constexpr std::ptrdiff_t m_flFlashDuration = 0x15F8;
            }

            namespace C_CSPlayerPawn {
                constexpr std::ptrdiff_t m_bNeedToReApplyGloves = 0x188D;
                constexpr std::ptrdiff_t m_EconGloves           = 0x1890;
                constexpr std::ptrdiff_t m_aimPunchAngle        = 0x16CC;
                constexpr std::ptrdiff_t m_entitySpottedState   = 0x26E0;
                constexpr std::ptrdiff_t m_iShotsFired          = 0x270C;
                constexpr std::ptrdiff_t m_ArmorValue           = 0x272C;
                constexpr std::ptrdiff_t m_iIDEntIndex          = 0x3EAC;
            }

            namespace EntitySpottedState_t {
                constexpr std::ptrdiff_t m_bSpotted       = 0x8;
                constexpr std::ptrdiff_t m_bSpottedByMask = 0xC;
            }

            namespace CSkeletonInstance {
                constexpr std::ptrdiff_t m_modelState = 0x160;
            }

            namespace CModelState {
                constexpr std::ptrdiff_t m_hModel        = 0xA0;
                constexpr std::ptrdiff_t m_MeshGroupMask = 0x220;
            }

            namespace CGameSceneNode {
                constexpr std::ptrdiff_t m_vecAbsOrigin = 0xD0;
                constexpr std::ptrdiff_t m_bDormant     = 0x10B;
            }

            namespace CBodyComponentBaseAnimGraph {
                constexpr std::ptrdiff_t m_animationController = 0x550;
            }

            namespace CBaseAnimGraphController {
                constexpr std::ptrdiff_t m_pAnimGraphInstance = 0x14B0;
            }

        } // namespace client_dll
    } // namespace schemas
} // namespace cs2_dumper
