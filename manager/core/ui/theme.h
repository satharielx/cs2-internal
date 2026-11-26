#pragma once
#include "../../external/imgui/imgui.h"
#include "../config.h"
#include <algorithm>
#include <cmath>
namespace menu_advanced {
    class WorkspaceTheme {
        ImGuiStyle previous_;
    public:
        WorkspaceTheme() : previous_(ImGui::GetStyle()) {
            auto& s = ImGui::GetStyle();
            s.WindowPadding = {12, 12}; s.FramePadding = {8, 4};
            s.ItemSpacing = {10, 7}; s.ItemInnerSpacing = {8, 6};
            s.WindowRounding = 3; s.ChildRounding = 4; s.FrameRounding = 3;
            s.PopupRounding = 6; s.ScrollbarSize = 10; s.GrabMinSize = 10;
            s.GrabRounding = 10;
            s.WindowBorderSize = 1; s.ChildBorderSize = 1; s.FrameBorderSize = 0;
            s.CellPadding = {8, 6};
            ImVec4 accent{.38f,.68f,.85f,1};
            if (std::isfinite(config::colors::menu_accent[0]) && std::isfinite(config::colors::menu_accent[1]) && std::isfinite(config::colors::menu_accent[2]))
                accent = {std::clamp(config::colors::menu_accent[0],0.f,1.f),std::clamp(config::colors::menu_accent[1],0.f,1.f),std::clamp(config::colors::menu_accent[2],0.f,1.f),1};
            auto& c=s.Colors;
            c[ImGuiCol_WindowBg]={.025f,.03f,.045f,1}; c[ImGuiCol_ChildBg]={.015f,.055f,.08f,1};
            c[ImGuiCol_TitleBg]={.055f,.065f,.08f,1};
            c[ImGuiCol_TitleBgActive]={.09f,.11f,.14f,1};
            c[ImGuiCol_TitleBgCollapsed]=c[ImGuiCol_TitleBg];
            c[ImGuiCol_PopupBg]={.02f,.065f,.095f,1}; c[ImGuiCol_Border]={.035f,.15f,.20f,.8f};
            c[ImGuiCol_Text]={.90f,.92f,.95f,1}; c[ImGuiCol_TextDisabled]={.52f,.64f,.70f,1};
            c[ImGuiCol_FrameBg]={.015f,.035f,.055f,1}; c[ImGuiCol_FrameBgHovered]={.17f,.20f,.24f,1};
            c[ImGuiCol_FrameBgActive]={.20f,.24f,.29f,1};
            c[ImGuiCol_Button]={.13f,.16f,.20f,1}; c[ImGuiCol_ButtonHovered]={.19f,.24f,.29f,1};
            c[ImGuiCol_ButtonActive]={.23f,.29f,.35f,1};
            c[ImGuiCol_Header]={accent.x,accent.y,accent.z,.18f};
            c[ImGuiCol_HeaderHovered]={accent.x,accent.y,accent.z,.25f};
            c[ImGuiCol_HeaderActive]={accent.x,accent.y,accent.z,.35f};
            c[ImGuiCol_CheckMark]=c[ImGuiCol_SliderGrab]=accent;
            c[ImGuiCol_SliderGrabActive]={.75f,.85f,.95f,1};
            c[ImGuiCol_Separator]=c[ImGuiCol_Border];
            c[ImGuiCol_Tab]={.10f,.12f,.15f,1}; c[ImGuiCol_TabHovered]=c[ImGuiCol_HeaderHovered];
            c[ImGuiCol_TabSelected]={.17f,.21f,.26f,1};
            if (config::colors::light_theme) {
                c[ImGuiCol_WindowBg]={1,1,1,1}; c[ImGuiCol_ChildBg]={.975f,.98f,.985f,1};
                c[ImGuiCol_PopupBg]={1,1,1,1}; c[ImGuiCol_Border]={.78f,.81f,.83f,1};
                c[ImGuiCol_Text]={.15f,.17f,.19f,1}; c[ImGuiCol_TextDisabled]={.40f,.45f,.48f,1};
                c[ImGuiCol_FrameBg]={.92f,.94f,.95f,1}; c[ImGuiCol_FrameBgHovered]={.85f,.90f,.93f,1};
                c[ImGuiCol_FrameBgActive]={.80f,.87f,.91f,1};
                c[ImGuiCol_Button]={.92f,.94f,.95f,1}; c[ImGuiCol_ButtonHovered]={.82f,.90f,.94f,1};
                c[ImGuiCol_ButtonActive]={.75f,.85f,.91f,1};
                c[ImGuiCol_Separator]=c[ImGuiCol_Border];
            }
        }
        ~WorkspaceTheme() { ImGui::GetStyle() = previous_; }
        WorkspaceTheme(const WorkspaceTheme&)=delete;
        WorkspaceTheme& operator=(const WorkspaceTheme&)=delete;
    };
}
