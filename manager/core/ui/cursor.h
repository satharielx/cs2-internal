#pragma once
#include "../../external/imgui/imgui.h"

namespace menu_advanced {
    // Let the Win32 backend hide the OS cursor; this only paints the overlay.
    inline void DrawWorkspaceCursor(bool visible) {
        auto& io = ImGui::GetIO();
        io.MouseDrawCursor = visible;
        if (!visible || !ImGui::IsMousePosValid()) return;
        const auto kind = ImGui::GetMouseCursor();
        ImGui::SetMouseCursor(ImGuiMouseCursor_None);
        auto* draw = ImGui::GetForegroundDrawList();
        const auto p = io.MousePos;
        const ImU32 edge = IM_COL32(16, 20, 28, 255);
        const ImU32 fill = IM_COL32(222, 236, 250, 255);
        if (kind == ImGuiMouseCursor_TextInput) {
            draw->AddLine({p.x,p.y-8},{p.x,p.y+8},edge,4);
            draw->AddLine({p.x,p.y-8},{p.x,p.y+8},fill,2);
            draw->AddLine({p.x-4,p.y-8},{p.x+4,p.y-8},fill,2);
            draw->AddLine({p.x-4,p.y+8},{p.x+4,p.y+8},fill,2);
        } else if (kind == ImGuiMouseCursor_Arrow || kind == ImGuiMouseCursor_Hand) {
            const ImVec2 a{p.x,p.y}, b{p.x+5,p.y+20}, c{p.x+9,p.y+12}, d{p.x+18,p.y+10};
            draw->AddQuadFilled(a,b,c,d,fill);
            const ImVec2 outline[]{a,b,c,d};
            draw->AddPolyline(outline,4,edge,ImDrawFlags_Closed,1.5f);
        } else {
            // Keep directional resize and blocked indicators familiar.
            io.MouseDrawCursor = true;
            ImGui::SetMouseCursor(kind);
        }
    }
}
