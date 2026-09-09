#pragma once
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_internal.h"
#include <string>
#include <vector>

// FontAwesome 6 Free Solid codepoints (subset used)
#define ICON_MIN_FA 0xe005
#define ICON_MAX_FA 0xf8ff
#define ICON_FA_CROSSHAIRS    "\xef\x81\x9b"  // fa-crosshairs
#define ICON_FA_EYE           "\xef\x81\xae"  // fa-eye
#define ICON_FA_USER          "\xef\x80\x87"  // fa-user
#define ICON_FA_KNIFE         "\xef\x95\xa5"  // fa-utensils (knife substitute)
#define ICON_FA_COG           "\xef\x80\x93"  // fa-cog
#define ICON_FA_HOME          "\xef\x80\x95"  // fa-home
#define ICON_FA_BOLT          "\xef\x83\xa7"  // fa-bolt
#define ICON_FA_SHIELD        "\xef\x84\xa2"  // fa-shield
#define ICON_FA_PAINT_BRUSH   "\xef\x87\xbc"  // fa-paint-brush
#define ICON_FA_TACHOMETER    "\xef\x8f\xbd"  // fa-tachometer-alt
#define ICON_FA_PLAY          "\xef\x81\x8b"  // fa-play
#define ICON_FA_STOP          "\xef\x81\x8d"  // fa-stop
#define ICON_FA_SYNC          "\xef\x80\xa1"  // fa-sync
#define ICON_FA_EXCLAMATION   "\xef\x81\xaa"  // fa-exclamation-triangle
#define ICON_FA_CHECK         "\xef\x80\x8c"  // fa-check
#define ICON_FA_TIMES         "\xef\x80\x8d"  // fa-times
#define ICON_FA_CUBE          "\xef\x86\xb2"  // fa-cube
#define ICON_FA_BOMB          "\xef\x87\xa2"  // fa-bomb
#define ICON_FA_MAGIC         "\xef\x83\x90"  // fa-magic
#define ICON_FA_SLIDERS_H     "\xef\x87\x9e"  // fa-sliders-h
#define ICON_FA_PALETTE       "\xef\x94\xbf"  // fa-palette
#define ICON_FA_RUNNING       "\xef\x9c\x8c"  // fa-running
#define ICON_FA_MAP           "\xef\x89\xb9"  // fa-map
#define ICON_FA_SAVE          "\xef\x83\x87"  // fa-save
#define ICON_FA_FOLDER_OPEN   "\xef\x81\xbc"  // fa-folder-open
#define ICON_FA_TRASH         "\xef\x87\xb8"  // fa-trash-alt
#define ICON_FA_DICE          "\xef\x94\xa2"  // fa-dice
#define ICON_FA_GLOBE         "\xef\x82\xac"  // fa-globe

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
