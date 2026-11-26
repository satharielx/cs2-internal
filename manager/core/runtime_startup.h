#pragma once
#include <array>
#include <string>

namespace runtime_startup {
    // Loaded backend names establish dependencies, not the active renderer.
    inline constexpr std::array required_modules{
        "client.dll", "engine2.dll", "rendersystemdx11.dll", "gameoverlayrenderer64.dll"
    };
    template<class Loaded>
    std::string MissingModules(Loaded loaded) {
        std::string missing;
        for (const auto* module : required_modules) {
            if (loaded(module)) continue;
            if (!missing.empty()) missing += ", ";
            missing += module;
        }
        return missing;
    }
    template<class Loaded>
    std::string TimeoutReason(Loaded loaded) {
        auto reason = "Missing modules: " + MissingModules(loaded);
        if (!loaded("rendersystemdx11.dll") && loaded("rendersystemvulkan.dll"))
            reason += ". Only the Vulkan renderer module is available; this build requires D3D11";
        if (!loaded("gameoverlayrenderer64.dll"))
            reason += ". The current rendering integration requires the Steam overlay";
        return reason;
    }
}
