#include "../manager/core/viewmodel.h"
#include <array>
#include <cassert>
#include <limits>
#include <iostream>

int main() {
    std::array<unsigned char, 0x3000> pawn{};
    std::array<unsigned char, 0x100> punch{}, camera{};
    const auto p = reinterpret_cast<uintptr_t>(pawn.data());
    const auto a = reinterpret_cast<uintptr_t>(punch.data());
    const auto c = reinterpret_cast<uintptr_t>(camera.data());
    const auto fov = p + sdk::off::C_CSPlayerPawn::m_flViewmodelFOV;
    const auto angle = a + sdk::off::CCSPlayer_AimPunchServices::m_predictableBaseAngle;
    const auto velocity = a + sdk::off::CCSPlayer_AimPunchServices::m_predictableBaseAngleVel;
    const auto unpredictable = a + sdk::off::CCSPlayer_AimPunchServices::m_unpredictableBaseAngle;
    const auto view = c + sdk::off::CPlayer_CameraServices::m_vecCsViewPunchAngle;
    sdk::write_memory(p + sdk::off::C_BaseEntity::m_iHealth, 100);
    sdk::write_memory(p + sdk::off::C_CSPlayerPawn::m_pAimPunchServices, a);
    sdk::write_memory(p + sdk::off::C_BasePlayerPawn::m_pCameraServices, c);
    sdk::write_memory(fov, 68.0f);
    for (auto field : {angle, velocity, unpredictable, view})
        sdk::write_memory(field, sdk::Vector3(2, 3, 4));
    { viewmodel::ScopedOverride disabled(p); assert(sdk::read_value<float>(fov) == 68); }
    config::viewmodel::fov_enabled = config::viewmodel::no_aim_punch = true;
    config::viewmodel::fov = 95;
    try {
        viewmodel::ScopedOverride active(p);
        assert(sdk::read_value<float>(fov) == 95);
        for (auto field : {angle, velocity, unpredictable, view})
            assert(sdk::read_value<sdk::Vector3>(field).Length() == 0);
        throw 1;
    } catch (int) {}
    assert(sdk::read_value<float>(fov) == 68);
    for (auto field : {angle, velocity, unpredictable, view}) {
        const auto restored = sdk::read_value<sdk::Vector3>(field);
        assert(restored.x == 2 && restored.y == 3 && restored.z == 4);
    }
    sdk::write_memory(p + sdk::off::C_CSPlayerPawn::m_flViewmodelOffsetX, 1.0f);
    config::viewmodel::offsets_enabled = config::viewmodel::camera_fov_enabled = true;
    config::viewmodel::offset[0] = 100; config::viewmodel::camera_fov = 110;
    std::array<unsigned char, 0x400> largeCamera{};
    const auto lc = reinterpret_cast<uintptr_t>(largeCamera.data());
    sdk::write_memory(p + sdk::off::C_BasePlayerPawn::m_pCameraServices, lc);
    sdk::write_memory(lc + sdk::off::CCSPlayerBase_CameraServices::m_iFOV, uint32_t{90});
    {
        viewmodel::ScopedOverride expanded(p);
        assert(sdk::read_value<float>(p + sdk::off::C_CSPlayerPawn::m_flViewmodelOffsetX) == 20);
        assert(sdk::read_value<uint32_t>(lc + sdk::off::CCSPlayerBase_CameraServices::m_iFOV) == 110);
    }
    assert(sdk::read_value<float>(p + sdk::off::C_CSPlayerPawn::m_flViewmodelOffsetX) == 1);
    assert(sdk::read_value<uint32_t>(lc + sdk::off::CCSPlayerBase_CameraServices::m_iFOV) == 90);
    sdk::write_memory(p + sdk::off::C_CSPlayerPawn::m_bIsScoped, true);
    { viewmodel::ScopedOverride scoped(p); assert(sdk::read_value<uint32_t>(lc + sdk::off::CCSPlayerBase_CameraServices::m_iFOV) == 90); }
    sdk::write_memory(p + sdk::off::C_CSPlayerPawn::m_bIsScoped, false);
    sdk::write_memory(p + sdk::off::C_BasePlayerPawn::m_pCameraServices, c);
    config::viewmodel::offsets_enabled = config::viewmodel::camera_fov_enabled = false;
    config::viewmodel::fov = 200;
    { viewmodel::ScopedOverride clamped(p); assert(sdk::read_value<float>(fov) == 120); }
    config::viewmodel::fov = std::numeric_limits<float>::quiet_NaN();
    { viewmodel::ScopedOverride invalid(p); assert(sdk::read_value<float>(fov) == 68); }
    config::viewmodel::fov = 95;
    sdk::write_memory(p + sdk::off::C_BaseEntity::m_iHealth, 0);
    { viewmodel::ScopedOverride dead(p); assert(sdk::read_value<float>(fov) == 68); }
    { viewmodel::ScopedOverride missing(0); }
    std::cout << "Viewmodel regressions passed\n";
}
