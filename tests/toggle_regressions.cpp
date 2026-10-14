#define main FeatureRegressionMain
#include "feature_regressions.cpp"
#undef main

int main() {
    auto* context = ImGui::CreateContext();
    menu_advanced::LoadIconFont();
    auto& io = ImGui::GetIO();
    io.IniFilename = nullptr; io.DisplaySize = {1280, 900}; io.DeltaTime = 1.f / 60;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    unsigned char* pixels; int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    // Discover the actual setting pointers used by all production pages.
    for (int page = 0; page < 12; ++page) {
        menu_advanced::selected_tab = page;
        for (int frame = 0; frame < 3; ++frame) {
            ImGui::NewFrame(); menu_advanced::RenderMainMenu(); ImGui::Render();
        }
    }
    std::vector<bool*> settings;
    for (const auto& [setting, location] : checkboxLocations) settings.push_back(setting);
    assert(settings.size() > 40);
    for (auto* setting : settings) {
        const bool original = *setting;
        bool disabled = false;
        ImVec2 center{};
        auto render = [&] {
            ImGui::NewFrame();
            ImGui::SetNextWindowPos({50, 50}); ImGui::SetNextWindowSize({600, 150});
            ImGui::Begin("Toggle test", nullptr, ImGuiWindowFlags_NoSavedSettings);
            ImGui::BeginDisabled(disabled);
            menu_advanced::ModernToggle("Setting###StableSetting", setting);
            center = (ImGui::GetItemRectMin() + ImGui::GetItemRectMax()) * .5f;
            ImGui::EndDisabled(); ImGui::End(); ImGui::Render();
        };
        render(); render();
        for (int repeat = 0; repeat < 2; ++repeat) {
            const bool before = *setting;
            io.AddMousePosEvent(center.x, center.y); render();
            io.AddMouseButtonEvent(0, true); render();
            for (int held = 0; held < 3; ++held) render();
            io.AddMouseButtonEvent(0, false); render(); render();
            assert(*setting == !before);
        }
        assert(*setting == original);
        disabled = true; render();
        io.AddMouseButtonEvent(0, true); render();
        io.AddMouseButtonEvent(0, false); render();
        assert(*setting == original);
        disabled = false; render();
        // Native keyboard activation of the focused setting must toggle once.
        auto* window = ImGui::FindWindowByName("Toggle test");
        ImGui::FocusWindow(window);
        ImGui::SetNavID(checkboxLocations.at(setting).id, ImGuiNavLayer_Main, 0,
            ImRect({0, 0}, {100, 20}));
        ImGui::GetCurrentContext()->NavCursorVisible = true;
        io.AddKeyEvent(ImGuiKey_Space, true); render();
        io.AddKeyEvent(ImGuiKey_Space, false); render();
        assert(*setting != original);
        *setting = original;
    }
    ImGui::DestroyContext(context);
    std::cout << "Toggle regressions passed for " << settings.size() << " settings\n";
}
