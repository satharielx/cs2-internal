#include "widgets.h"

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
        // The native checkbox handles mouse, keyboard and disabled input.
        return ImGui::Checkbox(label, value);
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
