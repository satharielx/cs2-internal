#pragma once
#include "../external/imgui/imgui.h"
#include <string>
#include <vector>

namespace menu_advanced {
    // Advanced menu rendering
    void RenderMainMenu();
    void RenderESPTab();
    void RenderAimbotTab();
    void RenderTriggerBotTab();
    void RenderSkinTab();
    void RenderKnifeTab();
    void RenderUtilitiesTab();
    void RenderSettingsTab();
    void RenderItemsTab();
    void RenderViewTab();
    void RenderHudTab();
    void RenderMainTab();
    void RenderMovementTab();
    void RenderGrenadesTab();
    void RenderConfigsTab();

    // Knife changer dedicated
    void RenderKnifeSelector();
    void ApplySelectedKnife(int knife_id);
}

