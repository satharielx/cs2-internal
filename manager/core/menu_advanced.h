#pragma once
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_internal.h"
#include <string>
#include <vector>

#include "ui/icons.h"

namespace menu_advanced {
    // Animation state for tab transitions
    struct MenuAnimationState {
        float tab_fade_alpha = 1.0f;
        int   current_tab = 0;
        float tab_transition_timer = 0.0f;
        bool  transitioning = false;
    };
    extern MenuAnimationState g_anim;

    // Icon font management
    bool LoadIconFont();
    // Release device resources and clear atlas pointers before renderer shutdown.
    void ResetRendererResources();
    void RenderIcon(ImFont* icon_font, const char* icon, const ImVec2& size = ImVec2(0, 0), ImU32 color = IM_COL32_WHITE);

    // Advanced menu rendering
    void RenderMainMenu();
    void RenderAimbotTab();
    void RenderTriggerBotTab();
    void RenderESPTab();
    void RenderItemsTab();
    void RenderViewTab();
    void RenderHudTab();
    void RenderMainTab();
    void RenderMovementTab();
    void RenderSkinTab();
    void RenderGrenadesTab();
    void RenderConfigsTab();

    // Knife changer
    void ApplySelectedKnife(int knife_id);
}
