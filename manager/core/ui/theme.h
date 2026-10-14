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
            s.WindowPadding = {16, 16}; s.FramePadding = {10, 6};
            s.ItemSpacing = {10, 9}; s.ItemInnerSpacing = {8, 6};
            s.WindowRounding = 10; s.ChildRounding = 8; s.FrameRounding = 5;
            s.PopupRounding = 6; s.ScrollbarSize = 10; s.GrabMinSize = 10;
            s.WindowBorderSize = 1; s.ChildBorderSize = 1; s.FrameBorderSize = 0;
            s.CellPadding = {12, 10};
            ImVec4 accent{.38f,.68f,.85f,1};
            if (std::isfinite(config::colors::menu_accent[0]) && std::isfinite(config::colors::menu_accent[1]) && std::isfinite(config::colors::menu_accent[2]))
                accent = {std::clamp(config::colors::menu_accent[0],0.f,1.f),std::clamp(config::colors::menu_accent[1],0.f,1.f),std::clamp(config::colors::menu_accent[2],0.f,1.f),1};
            auto& c=s.Colors;
            c[ImGuiCol_WindowBg]={.055f,.065f,.08f,1}; c[ImGuiCol_ChildBg]={.075f,.085f,.105f,1};
            c[ImGuiCol_TitleBg]={.055f,.065f,.08f,1};
            c[ImGuiCol_TitleBgActive]={.09f,.11f,.14f,1};
            c[ImGuiCol_TitleBgCollapsed]=c[ImGuiCol_TitleBg];
            c[ImGuiCol_PopupBg]={.085f,.095f,.12f,1}; c[ImGuiCol_Border]={.19f,.21f,.25f,.65f};
            c[ImGuiCol_Text]={.90f,.92f,.95f,1}; c[ImGuiCol_TextDisabled]={.48f,.53f,.61f,1};
            c[ImGuiCol_FrameBg]={.12f,.14f,.17f,1}; c[ImGuiCol_FrameBgHovered]={.17f,.20f,.24f,1};
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
        }
        ~WorkspaceTheme() { ImGui::GetStyle() = previous_; }
        WorkspaceTheme(const WorkspaceTheme&)=delete;
        WorkspaceTheme& operator=(const WorkspaceTheme&)=delete;
    };
}
