#pragma once
#include "../sdk/mem.h"
#include "../sdk/source2sdk_offsets.h"
#include <cstdint>
#include <cmath>

namespace sdk {
    // CS2 Vector3
    struct Vector3 {
        float x, y, z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
        Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
        Vector3 operator*(float f) const { return Vector3(x * f, y * f, z * f); }
        Vector3 operator/(float f) const { return Vector3(x / f, y / f, z / f); }

        float Length() const {
            return sqrtf(x * x + y * y + z * z);
        }

        float Distance(const Vector3& v) const {
            return (*this - v).Length();
        }

        Vector3 Normalize() const {
            float len = Length();
            if (len > 0) return *this / len;
            return Vector3();
        }
    };

    // CS2 Vector2
    struct Vector2 {
        float x, y;

        Vector2() : x(0), y(0) {}
        Vector2(float x, float y) : x(x), y(y) {}

        Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }

        float Length() const {
            return sqrtf(x * x + y * y);
        }
    };

    // View matrix for world-to-screen
    struct ViewMatrix {
        float matrix[4][4];
    };

    // Offset aliases from source2sdk (used below)
    namespace off {
        using namespace cs2_dumper::schemas::client_dll;
    }

    // CS2 Entity base class
    class C_BaseEntity {
    public:
        Vector3 GetOrigin() {
            uintptr_t scene_node = *reinterpret_cast<uintptr_t*>((uintptr_t)this + off::C_BaseEntity::m_pGameSceneNode);
            if (!scene_node || !is_valid_ptr(scene_node)) return Vector3();
            return *reinterpret_cast<Vector3*>(scene_node + off::CGameSceneNode::m_vecAbsOrigin);
        }

        int GetHealth() {
            return *reinterpret_cast<int*>((uintptr_t)this + off::C_BaseEntity::m_iHealth);
        }

        int GetTeam() {
            return *reinterpret_cast<int*>((uintptr_t)this + off::C_BaseEntity::m_iTeamNum);
        }

        bool IsAlive() {
            int health = GetHealth();
            return health > 0 && health <= 100;
        }

        int GetFlags() {
            return *reinterpret_cast<int*>((uintptr_t)this + off::C_BaseEntity::m_fFlags);
        }

        bool IsOnGround() {
            return (GetFlags() & 1);  // FL_ONGROUND
        }
    };

    // CS2 Player (extends BaseEntity)
    class C_CSPlayerPawn : public C_BaseEntity {
    public:
        Vector3 GetBonePosition(int bone_index) {
            uintptr_t game_scene_node = *reinterpret_cast<uintptr_t*>((uintptr_t)this + off::C_BaseEntity::m_pGameSceneNode);
            if (!game_scene_node || !is_valid_ptr(game_scene_node)) return Vector3();

            // CSkeletonInstance::m_modelState + 0x80 (bone array pointer inside CModelState)
            uintptr_t bone_array = *reinterpret_cast<uintptr_t*>(game_scene_node + off::CSkeletonInstance::m_modelState + 0x80);
            if (!bone_array || !is_valid_ptr(bone_array)) return Vector3();

            return *reinterpret_cast<Vector3*>(bone_array + bone_index * 32);
        }

        Vector3 GetEyePosition() {
            Vector3 origin = GetOrigin();
            Vector3 view_offset = *reinterpret_cast<Vector3*>((uintptr_t)this + off::C_BaseModelEntity::m_vecViewOffset);
            return origin + view_offset;
        }

        bool IsDormant() {
            uintptr_t scene_node = *reinterpret_cast<uintptr_t*>((uintptr_t)this + off::C_BaseEntity::m_pGameSceneNode);
            if (!scene_node || !is_valid_ptr(scene_node)) return true;
            return *reinterpret_cast<bool*>(scene_node + off::CGameSceneNode::m_bDormant);
        }

        int GetArmor() {
            return *reinterpret_cast<int*>((uintptr_t)this + off::C_CSPlayerPawn::m_ArmorValue);
        }

        bool IsSpotted() {
            return *reinterpret_cast<bool*>((uintptr_t)this + off::C_CSPlayerPawn::m_entitySpottedState + off::EntitySpottedState_t::m_bSpotted);
        }
    };

    // Bone IDs for skeleton
    enum BoneIndex {
        BONE_HEAD = 8,
        BONE_NECK = 7,
        BONE_CHEST = 6,
        BONE_PELVIS = 0,
        BONE_LEFT_SHOULDER = 13,
        BONE_LEFT_ELBOW = 14,
        BONE_LEFT_HAND = 15,
        BONE_RIGHT_SHOULDER = 9,
        BONE_RIGHT_ELBOW = 10,
        BONE_RIGHT_HAND = 11,
        BONE_LEFT_HIP = 25,
        BONE_LEFT_KNEE = 26,
        BONE_LEFT_FOOT = 27,
        BONE_RIGHT_HIP = 22,
        BONE_RIGHT_KNEE = 23,
        BONE_RIGHT_FOOT = 24
    };
}