#include "widgets.h"
#include <algorithm>

namespace menu_advanced {
    void BeginCard(const char* id) {
        ImGui::PushID(id);
        ImGui::BeginChild(id, {0,0}, ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);
    }
    void EndCard() {
        ImGui::EndChild();
        ImGui::PopID();
    }

    // Modern switch/toggle widget.
    bool ModernToggle(const char* label, bool* value) {
        ImGui::PushID(label);
        const auto start = ImGui::GetCursorScreenPos();
        const float width = ImGui::GetContentRegionAvail().x;
        const float height = ImGui::GetFrameHeight();
        const char* end = ImGui::FindRenderedTextEnd(label);
        auto* draw = ImGui::GetWindowDrawList();
        draw->PushClipRect(start, {start.x + (std::max)(0.f, width - 38), start.y + height}, true);
        draw->AddText({start.x, start.y + ImGui::GetStyle().FramePadding.y},
            ImGui::GetColorU32(ImGuiCol_Text), label, end);
        draw->PopClipRect();
        ImGui::SetCursorScreenPos({start.x + (std::max)(0.f, width - 34), start.y});
        ImGui::PushStyleColor(ImGuiCol_FrameBg, {0,0,0,0});
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, {0,0,0,0});
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, {0,0,0,0});
        ImGui::PushStyleColor(ImGuiCol_CheckMark, {0,0,0,0});
        // Native checkbox retains keyboard navigation, disabled state and test coverage.
        const bool changed = ImGui::Checkbox("  ##switch", value);
        ImGui::PopStyleColor(4);
        const auto accent = ImGui::GetStyleColorVec4(ImGuiCol_CheckMark);
        const float y = start.y + height * .5f;
        draw->AddRectFilled({start.x + width - 30, y - 6}, {start.x + width - 4, y + 6},
            ImGui::GetColorU32(*value ? ImVec4(accent.x,accent.y,accent.z,.25f) : ImVec4(.22f,.28f,.32f,.65f)), 6);
        draw->AddCircleFilled({start.x + width - (*value ? 10 : 24), y}, 6,
            ImGui::GetColorU32(*value ? ImGuiCol_CheckMark : ImGuiCol_TextDisabled), 16);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("%.*s", static_cast<int>(end-label), label);
        ImGui::PopID();
        return changed;
    }

    // Section titles use the workspace accent.
    void SectionHeader(const char* title, const char* icon) {
        ImGui::Spacing();
        if (icon && g_icon_font) {
            ImGui::PushFont(g_icon_font);
            ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), "%s", icon);
            ImGui::PopFont();
            ImGui::SameLine();
        }
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), "%s", title);
        ImGui::Spacing();
    }
}
