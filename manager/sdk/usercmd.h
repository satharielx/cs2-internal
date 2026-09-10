#pragma once
#include <cstdint>
#include <Windows.h>
#include <string>
#include "entity.h" // your existing QAngle, Vector definitions

namespace sdk {
    // Verified serializer layout in the installed client: entry +0x18 is
    // the angle message; message +0x18 holds pitch/yaw/roll; +0x10 has bits.
    inline bool ReadHistoryAngles(uintptr_t history, Vector2& angles) {
        const auto message = history ? read_value<uintptr_t>(history + 0x18) : 0;
        return message && read_memory(message + 0x18, angles) && std::isfinite(angles.x) && std::isfinite(angles.y);
    }
    inline bool WriteHistoryAngles(uintptr_t history, const Vector2& angles) {
        if (!history || !std::isfinite(angles.x) || !std::isfinite(angles.y)) return false;
        const auto message = read_value<uintptr_t>(history + 0x18);
        uint32_t entry_bits = 0, angle_bits = 0;
        if (!message || !read_memory(history + 0x10, entry_bits) ||
            !read_memory(message + 0x10, angle_bits)) return false;
        const Vector3 value{clamp_pitch(angles.x), normalize_yaw(angles.y), 0};
        if (!write_memory(message + 0x18, value) ||
            !write_memory(message + 0x10, angle_bits | 7u) ||
            !write_memory(history + 0x10, entry_bits | 1u)) return false;
        Vector3 check{};
        return read_memory(message + 0x18, check) && check.x == value.x && check.y == value.y && check.z == 0 &&
            (read_value<uint32_t>(message + 0x10) & 7u) == 7u &&
            (read_value<uint32_t>(history + 0x10) & 1u) != 0;
    }
    enum ECommandButtons : int {
        IN_ATTACK = (1 << 0),
        IN_JUMP = (1 << 1),
        IN_DUCK = (1 << 2),
        IN_FORWARD = (1 << 3),
        IN_BACK = (1 << 4),
        IN_USE = (1 << 5),
        IN_CANCEL = (1 << 6),
        IN_LEFT = (1 << 7),
        IN_RIGHT = (1 << 8),
        IN_MOVELEFT = (1 << 9),
        IN_MOVERIGHT = (1 << 10),
        IN_SECOND_ATTACK = (1 << 11),
        IN_RUN = (1 << 12),
        IN_RELOAD = (1 << 13),
        IN_LEFT_ALT = (1 << 14),
        IN_RIGHT_ALT = (1 << 15),
        IN_SCORE = (1 << 16),
        IN_SPEED = (1 << 17),
        IN_WALK = (1 << 18),
        IN_ZOOM = (1 << 19),
        IN_FIRST_WEAPON = (1 << 20),
        IN_SECOND_WEAPON = (1 << 21),
        IN_BULLRUSH = (1 << 22),
        IN_FIRST_GRENADE = (1 << 23),
        IN_SECOND_GRENADE = (1 << 24),
        IN_MIDDLE_ATTACK = (1 << 25),
        IN_USE_OR_RELOAD = (1 << 26)
    };

    class CBasePB {
    public:
        uint8_t pad01[0x8];
        uint32_t nHasBits;
        uint64_t nCachedBits;
    };

    class CMsgQAngle : public CBasePB {
    public:
        QAngle angValue;
    };

    class CMsgVector : public CBasePB {
    public:
        void* vecValue;
    };

    class CCSGOInterpolationInfoPB : public CBasePB {
    public:
        float flFraction;
    };

    class CCSGOInputHistoryEntryPB : public CBasePB {
    public:
        CMsgQAngle* pViewAngles;
        CMsgVector* pShootPosition;
        CMsgVector* pTargetHeadPositionCheck;
        CMsgVector* pTargetAbsPositionCheck;
        CMsgQAngle* pTargetAngPositionCheck;
        CCSGOInterpolationInfoPB* cl_interp;
        CCSGOInterpolationInfoPB* sv_interp0;
        CCSGOInterpolationInfoPB* sv_interp1;
        CCSGOInterpolationInfoPB* player_interp;
        int32_t nRenderTickCount;
        float flRenderTickFraction;
        int32_t nPlayerTickCount;
        float flPlayerTickFraction;
        int32_t nFrameNumber;
        int32_t nTargetEntIndex;
    };

    struct CInButtonStatePB : CBasePB {
        uint64_t nValue;
        uint64_t nValueChanged;
        uint64_t nValueScroll;
    };

    struct CSubtickMoveStep : CBasePB {
        uint64_t nButton;
        bool bPressed;
        float flWhen;
        float flAnalogForwardDelta;
        float flAnalogLeftDelta;
    };

    template <typename T>
    struct RepeatedPtrField_t {
        struct Rep_t {
            int32_t nAllocatedSize;
            T* tElements[1];
        };
        void* pArena;
        int32_t nCurrentSize;
        int32_t nTotalSize;
        Rep_t* pRep;
    };

    class CBaseUserCmdPB : public CBasePB {
    public:
        RepeatedPtrField_t<CSubtickMoveStep> subtickMovesField;
        std::string* strMoveCrc;
        CInButtonStatePB* pInButtonState;
        CMsgQAngle* pViewAngles;
        int32_t nLegacyCommandNumber;
        int32_t nClientTick;
        float flForwardMove;
        float flSideMove;
        float flUpMove;
        int32_t nImpulse;
        int32_t nWeaponSelect;
        int32_t nRandomSeed;
        int32_t nMousedX;
        int32_t nMousedY;
        uint32_t nConsumedServerAngleChanges;
        int32_t nCmdFlags;
        uint32_t nPawnEntityHandle;
    };

    class CCSGOUserCmdPB {
    public:
        uint32_t nHasBits;
        uint64_t nCachedSize;
        RepeatedPtrField_t<CCSGOInputHistoryEntryPB> inputHistoryField;
        CBaseUserCmdPB* pBaseCmd;
        bool bLeftHandDesired;
        bool bIsPredictingBodyShotFX;
        bool bIsPredictingHeadShotFX;
        bool bIsPredictingKillRagdolls;
        int32_t nAttack3StartHistoryIndex;
        int32_t nAttack1StartHistoryIndex;
        int32_t nAttack2StartHistoryIndex;

        void CheckAndSetBits(uint32_t nBits) {
            if (!(nHasBits & nBits))
                nHasBits |= nBits;
        }
    };

    struct CInButtonState {
        uint8_t pad01[0x8];
        uint64_t nValue;
        uint64_t nValueChanged;
        uint64_t nValueScroll;
    };

    class CUserCmd {
    public:
        uint8_t pad01[0x8];
        uint8_t pad02[0x10];
        CCSGOUserCmdPB csgoUserCmd;
        CInButtonState nButtons;
        uint8_t pad03[0x20];

        CCSGOInputHistoryEntryPB* GetInputHistoryEntry(int nIndex) {
            if (nIndex < 0 || !csgoUserCmd.inputHistoryField.pRep ||
                nIndex >= csgoUserCmd.inputHistoryField.pRep->nAllocatedSize ||
                nIndex >= csgoUserCmd.inputHistoryField.nCurrentSize)
                return nullptr;
            return csgoUserCmd.inputHistoryField.pRep->tElements[nIndex];
        }

        void SetSubTickAngle(const QAngle& angView) {
            if (!csgoUserCmd.inputHistoryField.pRep || !sdk::finite(angView)) return;
            const int count = (std::min)(csgoUserCmd.inputHistoryField.nCurrentSize,
                csgoUserCmd.inputHistoryField.pRep->nAllocatedSize);
            if (count < 0 || count > 256) return;
            for (int i = 0; i < count; i++) {
                CCSGOInputHistoryEntryPB* pEntry = GetInputHistoryEntry(i);
                if (!pEntry || !pEntry->pViewAngles)
                    continue;
                pEntry->pViewAngles->angValue = angView;
            }
        }

        bool IsButtonPressed(uint64_t button) const {
            if (!csgoUserCmd.pBaseCmd || !csgoUserCmd.pBaseCmd->pInButtonState)
                return false;
            return csgoUserCmd.pBaseCmd->pInButtonState->nValue & button;
        }
    };
}

