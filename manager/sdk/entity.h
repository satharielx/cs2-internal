#pragma once
#pragma once
#include "../sdk/mem.h"
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

    // CS2 Entity base class
    class C_BaseEntity {
    public:
        // Get entity position
        Vector3 GetOrigin() {
            return *reinterpret_cast<Vector3*>((uintptr_t)this + 0x1334);  // m_vecAbsOrigin - from cs2_dumper schema
        }

        // Get entity health
        int GetHealth() {
            return *reinterpret_cast<int*>((uintptr_t)this + 0x33C);  // m_iHealth - from cs2_dumper schema
        }

        // Get entity team
        int GetTeam() {
            return *reinterpret_cast<int*>((uintptr_t)this + 0x3D3);  // m_iTeamNum - from cs2_dumper schema
        }

        // Check if entity is alive
        bool IsAlive() {
            int health = GetHealth();
            return health > 0 && health <= 100;
        }

        // Get entity flags
        int GetFlags() {
            return *reinterpret_cast<int*>((uintptr_t)this + 0x3DC);  // m_fFlags - from cs2_dumper schema
        }

        // Check if on ground
        bool IsOnGround() {
            return (GetFlags() & 1);  // FL_ONGROUND
        }
    };

    // CS2 Player (extends BaseEntity)
    class C_CSPlayerPawn : public C_BaseEntity {
    public:
        // Get bone position
        Vector3 GetBonePosition(int bone_index) {
            uintptr_t game_scene_node = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x328);  // m_pGameSceneNode - from cs2_dumper schema
            if (!game_scene_node) return Vector3();

            uintptr_t bone_array = *reinterpret_cast<uintptr_t*>(game_scene_node + 0x1F8);  // m_modelState + boneArray - from cs2_dumper schema
            if (!bone_array) return Vector3();

            // Each bone is 32 bytes (4x4 matrix, but we only need position)
            return *reinterpret_cast<Vector3*>(bone_array + bone_index * 32);
        }

        // Get eye position
        Vector3 GetEyePosition() {
            Vector3 origin = GetOrigin();
            Vector3 view_offset = *reinterpret_cast<Vector3*>((uintptr_t)this + 0xC78);  // m_vecViewOffset - from cs2_dumper schema
            return origin + view_offset;
        }

        // Check if dormant (not updated by server)
        bool IsDormant() {
            return *reinterpret_cast<bool*>((uintptr_t)this + 0xEF);  // m_bDormant - from cs2_dumper schema
        }

        // Get armor value
        int GetArmor() {
            return *reinterpret_cast<int*>((uintptr_t)this + 0x1524);  // m_ArmorValue - from cs2_dumper schema
        }

        // Check if spotted
        bool IsSpotted() {
            return *reinterpret_cast<bool*>((uintptr_t)this + 0x15B4);  // m_bSpotted - from cs2_dumper schema
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