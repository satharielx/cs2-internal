#pragma once
#include "config.h"
#include "../sdk/entity.h"
#include <algorithm>

namespace viewmodel {
    // Render-start in the 0..11 frame-stage layout used by this client.
    inline constexpr int render_start = 1;

    // Keep overrides inside the render callback, including exception unwinding.
    // The caller holds config::mutex so recoil readers cannot see temporary zeros.
    class ScopedOverride {
        template<class T> struct Field {
            uintptr_t address = 0;
            T original{};
            void Apply(uintptr_t target, const T& value) {
                if (sdk::read_memory(target, original) && sdk::write_memory(target, value))
                    address = target;
            }
            ~Field() { if (address) sdk::write_memory(address, original); }
        };
        Field<float> fov_;
        Field<float> x_, y_, z_;
        Field<uint32_t> camera_fov_;
        Field<sdk::Vector3> punch_, velocity_, unpredictable_, camera_;
    public:
        explicit ScopedOverride(uintptr_t pawn) {
            if (!pawn || sdk::read_value<int>(pawn + sdk::off::C_BaseEntity::m_iHealth) <= 0)
                return;
            if (config::viewmodel::fov_enabled && std::isfinite(config::viewmodel::fov))
                fov_.Apply(pawn + sdk::off::C_CSPlayerPawn::m_flViewmodelFOV,
                    std::clamp(config::viewmodel::fov, 40.0f, 120.0f));
            if (config::viewmodel::offsets_enabled) {
                if (std::isfinite(config::viewmodel::offset[0])) x_.Apply(pawn + sdk::off::C_CSPlayerPawn::m_flViewmodelOffsetX, std::clamp(config::viewmodel::offset[0], -20.0f, 20.0f));
                if (std::isfinite(config::viewmodel::offset[1])) y_.Apply(pawn + sdk::off::C_CSPlayerPawn::m_flViewmodelOffsetY, std::clamp(config::viewmodel::offset[1], -20.0f, 20.0f));
                if (std::isfinite(config::viewmodel::offset[2])) z_.Apply(pawn + sdk::off::C_CSPlayerPawn::m_flViewmodelOffsetZ, std::clamp(config::viewmodel::offset[2], -20.0f, 20.0f));
            }
            if (config::viewmodel::camera_fov_enabled && std::isfinite(config::viewmodel::camera_fov) &&
                !sdk::read_value<bool>(pawn + sdk::off::C_CSPlayerPawn::m_bIsScoped)) {
                const auto camera = sdk::read_value<uintptr_t>(pawn + sdk::off::C_BasePlayerPawn::m_pCameraServices);
                if (camera) camera_fov_.Apply(camera + sdk::off::CCSPlayerBase_CameraServices::m_iFOV,
                    static_cast<uint32_t>(std::clamp(config::viewmodel::camera_fov, 40.0f, 140.0f)));
            }
            if (!config::viewmodel::no_aim_punch) return;
            const auto punch = sdk::read_value<uintptr_t>(pawn + sdk::off::C_CSPlayerPawn::m_pAimPunchServices);
            if (punch) {
                punch_.Apply(punch + sdk::off::CCSPlayer_AimPunchServices::m_predictableBaseAngle, {});
                velocity_.Apply(punch + sdk::off::CCSPlayer_AimPunchServices::m_predictableBaseAngleVel, {});
                unpredictable_.Apply(punch + sdk::off::CCSPlayer_AimPunchServices::m_unpredictableBaseAngle, {});
            }
            const auto camera = sdk::read_value<uintptr_t>(pawn + sdk::off::C_BasePlayerPawn::m_pCameraServices);
            if (camera) camera_.Apply(camera + sdk::off::CPlayer_CameraServices::m_vecCsViewPunchAngle, {});
        }
        ScopedOverride(const ScopedOverride&) = delete;
        ScopedOverride& operator=(const ScopedOverride&) = delete;
    };
}
