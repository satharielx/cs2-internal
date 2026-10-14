#pragma once
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_internal.h"
namespace menu_advanced {
    extern ImFont* g_icon_font;
    void BeginCard(const char* id);
    void EndCard();
    bool ModernToggle(const char* label, bool* value);
    void SectionHeader(const char* title, const char* icon = nullptr);
}
