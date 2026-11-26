#pragma once
#include "../external/imgui/imgui.h"

// Shared by the live overlay and menu preview. Positions are in screen pixels.
namespace esp_style {
    inline ImU32 Color(const float* c, float alpha) {
        return ImGui::ColorConvertFloat4ToU32({c[0], c[1], c[2], c[3] * alpha});
    }
    inline void Box(ImDrawList* draw, ImVec2 lo, ImVec2 hi, ImU32 color, float alpha, float scale) {
        draw->AddRect(lo, hi, IM_COL32(5, 8, 12, static_cast<int>(210 * alpha)), 0, 0, 3 * scale);
        draw->AddRect(lo, hi, color, 0, 0, scale);
    }
    inline void Text(ImDrawList* draw, ImVec2 pos, const char* text, ImU32 color,
        float alpha, float scale, bool centered = false) {
        const float size = ImGui::GetFontSize() * scale;
        if (centered) pos.x -= ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0, text).x * .5f;
        draw->AddText(ImGui::GetFont(), size, {pos.x + scale, pos.y + scale},
            IM_COL32(0, 0, 0, static_cast<int>(220 * alpha)), text);
        draw->AddText(ImGui::GetFont(), size, pos, color, text);
    }
    inline void Health(ImDrawList* draw, ImVec2 lo, ImVec2 hi, float fraction, float alpha, float scale) {
        const float x = lo.x - 6 * scale;
        draw->AddRectFilled({x - scale, lo.y - scale}, {x + 3 * scale, hi.y + scale},
            IM_COL32(8, 12, 18, static_cast<int>(220 * alpha)));
        const auto color = ImGui::ColorConvertFloat4ToU32({
            .95f - .65f * fraction, .28f + .57f * fraction, .32f + .22f * fraction, alpha});
        draw->AddRectFilled({x, hi.y - (hi.y - lo.y) * fraction}, {x + 2 * scale, hi.y}, color);
    }
    inline void Armor(ImDrawList* draw, ImVec2 lo, ImVec2 hi, float fraction, float alpha, float scale) {
        const float y = hi.y + 4 * scale;
        draw->AddRectFilled({lo.x - scale, y - scale}, {hi.x + scale, y + 3 * scale},
            IM_COL32(8, 12, 18, static_cast<int>(220 * alpha)));
        draw->AddRectFilled({lo.x, y}, {lo.x + (hi.x - lo.x) * fraction, y + 2 * scale},
            IM_COL32(100, 174, 244, static_cast<int>(255 * alpha)));
    }
}
