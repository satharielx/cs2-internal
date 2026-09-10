#include "menu_advanced.h"
#include "config.h"
#include "skins.h"
#include "features.h"
#include "interfaces.h"
#include "game_state.h"
#include "debug_console.h"
#include "../sdk/entity.h"
#include <algorithm>
#include <set>
#include <d3d11.h>
#include <wincodec.h>
#include <wrl/client.h>
#include <Shlwapi.h>
#include <chrono>
#include <cmath>

#define IMGUI_DEFINE_MATH_OPERATORS

#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "Shlwapi.lib")

namespace menu_advanced {

    // ==================== ANIMATION STATE ====================
    MenuAnimationState g_anim;

    // ==================== UTILITY: GET DLL DIRECTORY ====================
    static std::string GetDllDirectory() {
        char dll_path[MAX_PATH];
        HMODULE hm = nullptr;
        if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCSTR)&GetDllDirectory, &hm))
            return "";
        if (!GetModuleFileNameA(hm, dll_path, MAX_PATH))
            return "";
        PathRemoveFileSpecA(dll_path);
        return std::string(dll_path) + "\\";
    }

    // ==================== ICON FONT LOADING ====================
    static ImFont* g_icon_font = nullptr;
    static ImFont* g_icon_font_large = nullptr;
    static bool g_fonts_loaded = false;
    static ImFont* g_dashboard_body = nullptr;
    static ImFont* g_dashboard_title = nullptr;

    bool LoadIconFont() {
        if (g_icon_font) return true;

        ImGuiIO& io = ImGui::GetIO();
        if (!g_dashboard_body) {
            char windows[MAX_PATH]{};
            if (GetWindowsDirectoryA(windows, MAX_PATH)) {
                const std::string face = std::string(windows) + "\\Fonts\\segoeui.ttf";
                if (GetFileAttributesA(face.c_str()) != INVALID_FILE_ATTRIBUTES) {
                    g_dashboard_title = io.Fonts->AddFontFromFileTTF(face.c_str(), 24);
                    g_dashboard_body = io.Fonts->AddFontFromFileTTF(face.c_str(), 16);
                }
            }
        }
        std::string dll_dir = GetDllDirectory();
        debug_console::Console::Get().Info("[ICONS] Searching for fa-solid-900.ttf...");

        std::vector<std::string> search_dirs = {
            dll_dir + "fonts\\",
            dll_dir + "..\\fonts\\",
            dll_dir + "core\\fonts\\",
            "C:\\Windows\\manager\\manager\\core\\fonts\\",
            ".\\fonts\\"
        };

        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        const char* icon_file = "fa-solid-900.ttf";

        for (const auto& dir : search_dirs) {
            std::string full_path = dir + icon_file;
            if (GetFileAttributesA(full_path.c_str()) == INVALID_FILE_ATTRIBUTES)
                continue;

            ImFontConfig cfg;
            cfg.MergeMode = true;
            cfg.PixelSnapH = true;
            cfg.GlyphMinAdvanceX = 13.0f;
            g_icon_font = io.Fonts->AddFontFromFileTTF(full_path.c_str(), 14.0f, &cfg, icons_ranges);

            cfg.MergeMode = false;
            g_icon_font_large = io.Fonts->AddFontFromFileTTF(full_path.c_str(), 22.0f, &cfg, icons_ranges);

            if (g_icon_font) {
                debug_console::Console::Get().Success("[ICONS] Loaded from: %s", full_path.c_str());
                return true;
            }
        }

        debug_console::Console::Get().Warning("[ICONS] FontAwesome not found – icons disabled.");
        g_icon_font = nullptr;
        g_icon_font_large = nullptr;
        return false;
    }

    void RenderIcon(ImFont* icon_font, const char* icon, const ImVec2& size, ImU32 color) {
        if (!icon_font) return;
        ImGui::PushFont(icon_font);
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::Text("%s", icon);
        ImGui::PopStyleColor();
        ImGui::PopFont();
    }

    // ==================== LOGO TEXTURE ====================
    static ID3D11ShaderResourceView* g_logo_srv = nullptr;
    static int g_logo_width = 0;
    static int g_logo_height = 0;
    static bool g_logo_load_attempted = false;

    void ResetRendererResources() {
        // The font atlas owns these fonts; clearing the pointers avoids reuse
        // after the ImGui context is destroyed and a new atlas is created.
        g_icon_font = nullptr;
        g_icon_font_large = nullptr;
        g_dashboard_body = g_dashboard_title = nullptr;
        g_fonts_loaded = false;

        if (g_logo_srv) {
            g_logo_srv->Release();
            g_logo_srv = nullptr;
        }
        g_logo_width = 0;
        g_logo_height = 0;
        g_logo_load_attempted = false;
    }

    static bool LoadLogoTexture() {
        if (g_logo_load_attempted) return g_logo_srv != nullptr;
        g_logo_load_attempted = true;

        if (!interfaces::d3d11_device) return false;

        std::string dll_dir = GetDllDirectory();
        std::string path = dll_dir + "logo.png";

        DWORD attr = GetFileAttributesA(path.c_str());
        if (attr == INVALID_FILE_ATTRIBUTES) {
            path = "C:\\Windows\\manager\\manager\\core\\logo.png";
            attr = GetFileAttributesA(path.c_str());
            if (attr == INVALID_FILE_ATTRIBUTES) return false;
        }

        int wlen = MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, nullptr, 0);
        std::wstring wpath(wlen, L'\0');
        MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, &wpath[0], wlen);

        Microsoft::WRL::ComPtr<IWICImagingFactory> wic_factory;
        HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&wic_factory));
        if (FAILED(hr)) {
            CoInitializeEx(nullptr, COINIT_MULTITHREADED);
            hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&wic_factory));
            if (FAILED(hr)) return false;
        }

        Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
        hr = wic_factory->CreateDecoderFromFilename(wpath.c_str(), nullptr, GENERIC_READ,
            WICDecodeMetadataCacheOnLoad, &decoder);
        if (FAILED(hr)) return false;

        Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
        hr = decoder->GetFrame(0, &frame);
        if (FAILED(hr)) return false;

        Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
        hr = wic_factory->CreateFormatConverter(&converter);
        if (FAILED(hr)) return false;

        hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA,
            WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);
        if (FAILED(hr)) return false;

        UINT width, height;
        converter->GetSize(&width, &height);

        std::vector<BYTE> pixels(width * height * 4);
        hr = converter->CopyPixels(nullptr, width * 4, (UINT)pixels.size(), pixels.data());
        if (FAILED(hr)) return false;

        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA init_data = {};
        init_data.pSysMem = pixels.data();
        init_data.SysMemPitch = width * 4;

        ID3D11Texture2D* texture = nullptr;
        hr = interfaces::d3d11_device->CreateTexture2D(&desc, &init_data, &texture);
        if (FAILED(hr)) return false;

        D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
        srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srv_desc.Texture2D.MipLevels = 1;

        hr = interfaces::d3d11_device->CreateShaderResourceView(texture, &srv_desc, &g_logo_srv);
        texture->Release();
        if (FAILED(hr)) return false;

        g_logo_width = (int)width;
        g_logo_height = (int)height;

        debug_console::Console::Get().Success("[MENU] Logo loaded: %dx%d", width, height);
        return true;
    }

    // ==================== ANIMATED CARD HELPERS ====================
    static float g_card_hover_time[10] = { 0 };
    static int   g_card_hover_index = 0;

    static void BeginAnimatedCard(const char* str_id, ImVec4 base_color, ImVec4 hover_color, float speed = 4.0f) {
        ImGui::PushID(str_id);
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 size = ImGui::GetContentRegionAvail();
        bool hovered = ImGui::IsMouseHoveringRect(pos, pos + size);

        int idx = g_card_hover_index++ % 10;
        float& t = g_card_hover_time[idx];
        t = ImLerp(t, hovered ? 1.0f : 0.0f, ImGui::GetIO().DeltaTime * speed);

        ImVec4 col = ImLerp(base_color, hover_color, t);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, col);
        ImGui::BeginChild(str_id, ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);
    }

    static void EndAnimatedCard() {
        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopID();
    }

    // ==================== COMMON UI COMPONENTS ====================
    static void SectionHeader(const char* title, const char* icon = nullptr) {
        ImGui::Dummy(ImVec2(0, 5));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 1.0f, 0.8f, 1.0f));
        if (icon && g_icon_font) {
            ImGui::PushFont(g_icon_font);
            ImGui::Text("%s %s", icon, title);
            ImGui::PopFont();
        }
        else {
            ImGui::Text("%s", title);
        }
        ImGui::PopStyleColor();
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 5));
    }

    // ==================== DATABASE STATE ====================
    static bool db_loaded = false;
    static bool db_loading = false;

    // ==================== RENDER ESP TAB (Players) ====================
    void RenderESPTab() {
        SectionHeader("ESP Settings", ICON_FA_EYE);

        // Master toggle
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.12f, 0.18f, 0.9f));
        ImGui::BeginChild("##ESPMaster", ImVec2(0, 60), true);
        ImGui::Dummy(ImVec2(0, 8));
        ImGui::Indent(15);
        ImGui::Checkbox("ENABLE ESP SYSTEM", &config::esp::enabled);
        ImGui::SameLine(300);
        ImGui::TextColored(config::esp::enabled ? ImVec4(0.3f, 1.0f, 0.3f, 1.0f) : ImVec4(0.7f, 0.3f, 0.3f, 1.0f),
            "Status: %s", config::esp::enabled ? "ACTIVE" : "DISABLED");
        ImGui::Unindent(15);
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::Dummy(ImVec2(0, 10));
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.5f);

        // Left column - Features
        BeginAnimatedCard("##FeaturesCard", ImVec4(0.12f, 0.12f, 0.15f, 0.8f), ImVec4(0.18f, 0.18f, 0.22f, 0.9f));
        ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.8f, 1.0f), "RENDERING FEATURES");
        ImGui::Separator();
        ImGui::Indent(10);
        ImGui::Checkbox("Box ESP", &config::esp::box);
        ImGui::SameLine(200); ImGui::TextDisabled("Player bounding boxes");
        ImGui::Checkbox("Skeleton", &config::esp::skeleton);
        ImGui::SameLine(200); ImGui::TextDisabled("Bone structure");
        ImGui::Checkbox("Health Bar", &config::esp::health_bar);
        ImGui::SameLine(200); ImGui::TextDisabled("HP indicator");
        ImGui::Checkbox("Player Name", &config::esp::name);
        ImGui::SameLine(200); ImGui::TextDisabled("Display names");
        ImGui::Checkbox("Distance", &config::esp::distance);
        ImGui::SameLine(200); ImGui::TextDisabled("Distance in meters");
        ImGui::Checkbox("Snaplines", &config::esp::snaplines);
        ImGui::SameLine(200); ImGui::TextDisabled("Lines to players");
        ImGui::Unindent(10);
        EndAnimatedCard();

        ImGui::Dummy(ImVec2(0, 10));

        BeginAnimatedCard("##FiltersCard", ImVec4(0.12f, 0.12f, 0.15f, 0.8f), ImVec4(0.18f, 0.18f, 0.22f, 0.9f));
        ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.8f, 1.0f), "FILTERS & OPTIONS");
        ImGui::Separator();
        ImGui::Indent(10);
        ImGui::Checkbox("Team Check", &config::esp::team_check);
        ImGui::SameLine(200); ImGui::TextDisabled("Ignore teammates");
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::Text("Max Distance:");
        ImGui::PushItemWidth(280);
        ImGui::SliderFloat("##MaxDistESP", &config::esp::max_distance, 50.0f, 500.0f, "%.0f m");
        ImGui::PopItemWidth();
        ImGui::Unindent(10);
        EndAnimatedCard();

        ImGui::NextColumn();

        // Right column - Colors
        BeginAnimatedCard("##ColorsCard", ImVec4(0.12f, 0.12f, 0.15f, 0.8f), ImVec4(0.18f, 0.18f, 0.22f, 0.9f));
        ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.8f, 1.0f), "COLOR CONFIGURATION");
        ImGui::Separator();
        ImGui::Indent(10);
        ImGui::Text("Enemy Color:");
        ImGui::ColorEdit4("##EnemyColor", config::esp::box_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
        ImGui::Dummy(ImVec2(0, 5));
        ImGui::Text("Teammate Color:");
        ImGui::ColorEdit4("##TeamColor", config::esp::team_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
        ImGui::Dummy(ImVec2(0, 5));
        ImGui::Text("Skeleton Color:");
        ImGui::ColorEdit4("##SkeletonColor", config::esp::skeleton_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
        ImGui::Unindent(10);
        EndAnimatedCard();

        ImGui::Columns(1);
    }

    // ==================== RENDER AIMBOT TAB ====================
    void RenderAimbotTab() {
        SectionHeader("Aimbot Configuration", ICON_FA_CROSSHAIRS);

        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.56f);

        BeginAnimatedCard("##AimbotMain", ImVec4(0.12f, 0.12f, 0.15f, 0.8f), ImVec4(0.18f, 0.18f, 0.22f, 0.9f));
        ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.8f, 1.0f), "AIMBOT");
        ImGui::Separator();
        ImGui::Indent(10);
        ImGui::Checkbox("Enable Aimbot", &config::aimbot::enabled);
        ImGui::Checkbox("Auto Shoot", &config::aimbot::auto_shoot);
        ImGui::Checkbox("Team Check", &config::aimbot::team_check);
        ImGui::Checkbox("Spotted Check", &config::aimbot::visible_check);
        ImGui::Checkbox("Silent Aim", &config::aimbot::silent_aim);
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::Text("FOV (degrees):");
        ImGui::SliderFloat("##FOV", &config::aimbot::fov, 1.0f, 30.0f, "%.1f");
        ImGui::Text("Smoothing:");
        ImGui::SliderFloat("##Smooth", &config::aimbot::smoothing, 0.0f, 1.0f, "%.2f");
        ImGui::Text("Max Distance:");
        ImGui::SliderFloat("##MaxDist", &config::aimbot::max_distance, 100.0f, 10000.0f, "%.0f");
        ImGui::Unindent(10);
        EndAnimatedCard();

        ImGui::NextColumn();

        BeginAnimatedCard("##RCSCard", ImVec4(0.12f, 0.12f, 0.15f, 0.8f), ImVec4(0.18f, 0.18f, 0.22f, 0.9f));
        ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.8f, 1.0f), "RECOIL CONTROL (RCS)");
        ImGui::Separator();
        ImGui::Indent(10);
        ImGui::Checkbox("Enable RCS", &config::rcs::enabled);
        ImGui::Text("Strength:");
        ImGui::SliderFloat("##RCSStrength", &config::rcs::strength, 0.0f, 1.0f, "%.2f");
        ImGui::Dummy(ImVec2(0, 20));
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.3f, 1.0f), "Pause Key: Mouse5 (hold)");
        ImGui::Unindent(10);
        EndAnimatedCard();

        ImGui::Dummy(ImVec2(0, 8));
        BeginAnimatedCard("##AimDiagnostics", ImVec4(.07f,.10f,.14f,1), ImVec4(.09f,.14f,.19f,1));
        ImGui::TextUnformatted("Last aim evaluation");
        ImGui::TextWrapped("Normal: %s", features::normal_aim_status.load());
        ImGui::TextWrapped("Silent: %s", features::silent_aim_status.load());
        ImGui::TextWrapped("Writes: normal %u, history %u | Engine calls (total): %u",
            features::normal_aim_writes.load(), features::silent_history_writes.load(), features::silent_callbacks.load());
        ImGui::TextWrapped("Target scans: %u | Cached evaluations: %u",
            features::silent_target_scans.load(), features::silent_cache_hits.load());
        if (ImGui::CollapsingHeader("Subtick angle debug")) {
            const auto debug = features::GetSilentAimDebug();
            ImGui::TextDisabled("Last sample, 4 Hz; angles are pitch/yaw/roll.");
            ImGui::Text("a1: %p", reinterpret_cast<void*>(debug.source));
            ImGui::TextWrapped("%s", debug.status);
            ImGui::TextWrapped("Passing filters: alive %u / team %u / spotted %u / bones %u / range %u / FOV %u",
                debug.alive, debug.enemies, debug.spotted, debug.bones, debug.in_range, debug.in_fov);
            ImGui::Text("DWORDs: %08X %08X %08X", debug.raw_angles[0], debug.raw_angles[1], debug.raw_angles[2]);
            if (debug.input_valid) ImGui::Text("Input: %.3f / %.3f / %.3f", debug.input.x, debug.input.y, debug.input.z);
            if (debug.eye_valid) ImGui::Text("Eye XYZ: %.1f / %.1f / %.1f", debug.eye.x, debug.eye.y, debug.eye.z);
            if (debug.target_valid) {
                ImGui::TextWrapped("Selected enemy: %s", debug.target_name[0] ? debug.target_name : "Name unavailable");
                const char* team = debug.target_team == 2 ? "T" : debug.target_team == 3 ? "CT" : "Other";
                ImGui::Text("Health: %d | Team: %s (%d)", debug.target_health, team, debug.target_team);
                ImGui::Text("Target XYZ: %.1f / %.1f / %.1f", debug.target.x, debug.target.y, debug.target.z);
                ImGui::Text("Output: %.3f / %.3f / %.3f", debug.output.x, debug.output.y, debug.output.z);
                ImGui::Text("Source write: %s", debug.written ? "Succeeded" : "Failed");
            } else ImGui::TextDisabled("No selected enemy in this sample.");
        }
        EndAnimatedCard();

        ImGui::Columns(1);
    }

    // ==================== RENDER TRIGGERBOT TAB ====================
    void RenderTriggerBotTab() {
        SectionHeader("Triggerbot Settings", ICON_FA_BOLT);

        BeginAnimatedCard("##TriggerCard", ImVec4(0.12f, 0.12f, 0.15f, 0.8f), ImVec4(0.18f, 0.18f, 0.22f, 0.9f));
        ImGui::Indent(10);
        ImGui::Checkbox("Enable Triggerbot", &config::misc::trigger_bot);
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::Text("Trigger Key: Left Shift");
        ImGui::Text("Trigger Delay:");
        ImGui::SliderFloat("##TriggerDelay", &config::misc::trigger_delay, 0.0f, 0.5f, "%.2f seconds");
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.3f, 1.0f), "Automatically fires when crosshair is over enemy.");
        ImGui::Unindent(10);
        EndAnimatedCard();
    }

    // ==================== RENDER ITEMS TAB (Weapon/Grenade ESP) ====================
    void RenderItemsTab() {
        SectionHeader("Item ESP & World", ICON_FA_CUBE);
        BeginAnimatedCard("##ItemsCard", ImVec4(0.12f, 0.12f, 0.15f, 0.8f), ImVec4(0.18f, 0.18f, 0.22f, 0.9f));
        ImGui::Indent(10);
        ImGui::Text("Weapon/Grenade ESP is not yet implemented.");
        ImGui::Text("Future: display dropped weapons, grenades, bomb.");
        ImGui::Unindent(10);
        EndAnimatedCard();
    }

    // ==================== RENDER VIEW TAB (No Flash, etc.) ====================
    void RenderViewTab() {
        SectionHeader("View & Visuals", ICON_FA_EYE);
        BeginAnimatedCard("##ViewCard", ImVec4(0.12f, 0.12f, 0.15f, 0.8f), ImVec4(0.18f, 0.18f, 0.22f, 0.9f));
        ImGui::Indent(10);
        ImGui::Checkbox("No Flash", &config::misc::no_flash);
        ImGui::SameLine(200); ImGui::TextDisabled("Remove flashbang effect");
        ImGui::Checkbox("Radar Hack", &config::misc::radar_hack);
        ImGui::SameLine(200); ImGui::TextDisabled("Show enemies on radar");
        ImGui::Unindent(10);
        EndAnimatedCard();
    }

    // ==================== RENDER HUD TAB ====================
    void RenderHudTab() {
        SectionHeader("HUD Customization", ICON_FA_SLIDERS_H);
        BeginAnimatedCard("##HudCard", ImVec4(0.12f, 0.12f, 0.15f, 0.8f), ImVec4(0.18f, 0.18f, 0.22f, 0.9f));
        ImGui::Indent(10);
        ImGui::Text("HUD customization options (spectator list, watermark) coming soon.");
        ImGui::Unindent(10);
        EndAnimatedCard();
    }

    // ==================== RENDER MAIN TAB (Misc) ====================
    void RenderMainTab() {
        SectionHeader("Main Misc Settings", ICON_FA_HOME);
        BeginAnimatedCard("##MiscCard", ImVec4(0.12f, 0.12f, 0.15f, 0.8f), ImVec4(0.18f, 0.18f, 0.22f, 0.9f));
        ImGui::Indent(10);
        ImGui::Checkbox("Bunny Hop", &config::misc::bunny_hop);
        ImGui::SameLine(200); ImGui::TextDisabled("Auto-jump");
        ImGui::Checkbox("No Flash", &config::misc::no_flash);
        ImGui::Checkbox("Radar Hack", &config::misc::radar_hack);
        ImGui::Unindent(10);
        EndAnimatedCard();
    }

    // ==================== RENDER MOVEMENT TAB ====================
    void RenderMovementTab() {
        SectionHeader("Movement Enhancements", ICON_FA_RUNNING);
        BeginAnimatedCard("##MoveCard", ImVec4(0.12f, 0.12f, 0.15f, 0.8f), ImVec4(0.18f, 0.18f, 0.22f, 0.9f));
        ImGui::Indent(10);
        ImGui::Checkbox("Bunny Hop", &config::misc::bunny_hop);
        ImGui::Text("More movement features coming soon.");
        ImGui::Unindent(10);
        EndAnimatedCard();
    }

    // ==================== RENDER GRENADES TAB ====================
    void RenderGrenadesTab() {
        SectionHeader("Grenade Helper", ICON_FA_BOMB);
        BeginAnimatedCard("##GrenadeCard", ImVec4(0.12f, 0.12f, 0.15f, 0.8f), ImVec4(0.18f, 0.18f, 0.22f, 0.9f));
        ImGui::Indent(10);
        ImGui::Text("Grenade trajectory prediction and lineups coming soon.");
        ImGui::Unindent(10);
        EndAnimatedCard();
    }

    // ==================== RENDER CONFIGS TAB ====================
    void RenderConfigsTab() {
        SectionHeader("Configuration Management", ICON_FA_SAVE);
        BeginAnimatedCard("##ConfigCard", ImVec4(0.12f, 0.12f, 0.15f, 0.8f), ImVec4(0.18f, 0.18f, 0.22f, 0.9f));
        ImGui::Indent(10);
        if (ImGui::Button("Save Config", ImVec2(150, 30))) {
            // Implement save
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Config", ImVec2(150, 30))) {
            // Implement load
        }
        ImGui::Dummy(ImVec2(0, 10));
        if (ImGui::Button("Reset to Defaults", ImVec2(150, 30))) {
            // Reset
        }
        ImGui::Unindent(10);
        EndAnimatedCard();
    }

    // ==================== SKIN CHANGER STATE ====================
    static int selected_knife = skins::selected_knife_id;

    struct KnifeInfo { int id; const char* name; int price; };
    const KnifeInfo knives[] = {
        { 500, "Bayonet", 2 }, { 503, "Classic Knife", 1 }, { 505, "Flip Knife", 1 },
        { 506, "Gut Knife", 1 }, { 507, "Karambit", 3 }, { 508, "M9 Bayonet", 2 },
        { 509, "Tactical Knife", 1 }, { 512, "Falchion", 1 }, { 514, "Survival Bowie", 2 },
        { 515, "Butterfly", 2 }, { 516, "Shadow Daggers", 2 }, { 517, "Paracord Knife", 1 },
        { 518, "Survival Knife", 1 }, { 519, "Ursus Knife", 2 }, { 520, "Navaja Knife", 1 },
        { 521, "Nomad Knife", 2 }, { 522, "Stiletto", 2 }, { 523, "Talon Knife", 3 },
        { 525, "Skeleton Knife", 3 }, { 526, "Kukri Knife", 2 }
    };

    const char* GetKnifeName(int id) {
        for (const auto& k : knives) if (k.id == id) return k.name;
        return "Unknown";
    }

    // Weapon categories
    static const int rifles[] = {
        skins::WEAPON_AK47, skins::WEAPON_M4A1, skins::WEAPON_M4A1_SILENCER,
        skins::WEAPON_AWP, skins::WEAPON_FAMAS, skins::WEAPON_GALILAR,
        skins::WEAPON_AUG, skins::WEAPON_SG556, skins::WEAPON_SSG08,
        skins::WEAPON_SCAR20, skins::WEAPON_G3SG1
    };
    static const int pistols[] = {
        skins::WEAPON_DEAGLE, skins::WEAPON_GLOCK, skins::WEAPON_USP_SILENCER,
        skins::WEAPON_HKP2000, skins::WEAPON_P250, skins::WEAPON_FIVESEVEN,
        skins::WEAPON_TEC9, skins::WEAPON_CZ75A, skins::WEAPON_REVOLVER,
        skins::WEAPON_ELITE
    };
    static const int smg_heavy[] = {
        skins::WEAPON_MAC10, skins::WEAPON_MP9, skins::WEAPON_MP7,
        skins::WEAPON_MP5SD, skins::WEAPON_UMP45, skins::WEAPON_P90,
        skins::WEAPON_BIZON, skins::WEAPON_NOVA, skins::WEAPON_XM1014,
        skins::WEAPON_SAWEDOFF, skins::WEAPON_MAG7, skins::WEAPON_M249,
        skins::WEAPON_NEGEV
    };

    struct CategoryState {
        int selected_weapon_idx = 0;
        int selected_skin_idx = -1;
        char search[64] = {};
    };
    static CategoryState rifle_state, pistol_state, smg_state;

    static void RenderWeaponCategory(const char* id, const int* weapon_ids, int count, CategoryState& state) {
        if (!skins::IsDatabaseLoaded()) {
            ImGui::TextDisabled("Skin database not loaded");
            if (ImGui::Button("Load Skins")) {
                db_loading = true;
                skins::LoadSkinsFromAPI();
                db_loaded = skins::IsDatabaseLoaded();
                db_loading = false;
            }
            return;
        }

        ImGui::Columns(3, id, true);
        ImGui::SetColumnWidth(0, 140);
        ImGui::SetColumnWidth(1, 220);

        // Weapon list
        ImGui::BeginChild((std::string("WeaponList##") + id).c_str(), ImVec2(0, 0), true);
        for (int i = 0; i < count; i++) {
            const char* name = skins::GetWeaponName(weapon_ids[i]);
            bool has_skin = skins::user_skins.find(weapon_ids[i]) != skins::user_skins.end();
            if (has_skin) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 1.0f, 0.3f, 1.0f));
            if (ImGui::Selectable(name, state.selected_weapon_idx == i)) {
                state.selected_weapon_idx = i;
                state.selected_skin_idx = -1;
                state.search[0] = '\0';
            }
            if (has_skin) ImGui::PopStyleColor();
        }
        ImGui::EndChild();

        ImGui::NextColumn();

        int sel_wep = weapon_ids[state.selected_weapon_idx];
        ImGui::BeginChild((std::string("SkinList##") + id).c_str(), ImVec2(0, 0), true);
        ImGui::Text("Skins for %s", skins::GetWeaponName(sel_wep));
        ImGui::Separator();
        ImGui::InputText((std::string("Search##") + id).c_str(), state.search, sizeof(state.search));
        ImGui::Separator();

        std::vector<skins::SkinInfo> available;
        std::set<int> seen;
        for (const auto& s : skins::skin_database) {
            if (s.weapon_id != sel_wep) continue;
            if (seen.count(s.paint_kit)) continue;
            seen.insert(s.paint_kit);
            if (s.name.empty()) continue;
            if (state.search[0]) {
                std::string nl = s.name, sl = state.search;
                std::transform(nl.begin(), nl.end(), nl.begin(), ::tolower);
                std::transform(sl.begin(), sl.end(), sl.begin(), ::tolower);
                if (nl.find(sl) == std::string::npos) continue;
            }
            available.push_back(s);
        }

        for (size_t i = 0; i < available.size(); i++) {
            const auto& s = available[i];
            const float* col = skins::GetRarityColor(s.rarity);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(col[0], col[1], col[2], col[3]));
            char lbl[256];
            sprintf_s(lbl, "%s##%s_%d", s.name.c_str(), id, s.paint_kit);
            if (ImGui::Selectable(lbl, state.selected_skin_idx == (int)i)) {
                state.selected_skin_idx = (int)i;
                auto it = skins::user_skins.find(sel_wep);
                if (it == skins::user_skins.end()) {
                    skins::PlayerSkinConfig cfg;
                    cfg.weapon_id = sel_wep;
                    cfg.paint_kit = s.paint_kit;
                    cfg.wear = 0.01f;
                    skins::user_skins[sel_wep] = cfg;
                }
                else {
                    it->second.paint_kit = s.paint_kit;
                }
            }
            ImGui::PopStyleColor();
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("%s", skins::GetRarityName(s.rarity));
        }
        if (available.empty()) ImGui::TextDisabled("No skins found");
        ImGui::EndChild();

        ImGui::NextColumn();

        ImGui::BeginChild((std::string("Config##") + id).c_str(), ImVec2(0, 0), true);
        auto cfg_it = skins::user_skins.find(sel_wep);
        if (cfg_it != skins::user_skins.end()) {
            skins::PlayerSkinConfig& cfg = cfg_it->second;
            ImGui::Text("Configuration");
            ImGui::Separator();
            for (const auto& s : skins::skin_database) {
                if (s.weapon_id == sel_wep && s.paint_kit == cfg.paint_kit) {
                    const float* c = skins::GetRarityColor(s.rarity);
                    ImGui::TextColored(ImVec4(c[0], c[1], c[2], c[3]), "%s", s.name.c_str());
                    break;
                }
            }
            ImGui::SliderInt("Seed", &cfg.seed, 0, 1000);
            ImGui::SliderFloat("Wear", &cfg.wear, 0.0f, 1.0f, "%.4f");
            if (ImGui::SmallButton("FN")) cfg.wear = 0.01f;
            ImGui::SameLine(); if (ImGui::SmallButton("MW")) cfg.wear = 0.08f;
            ImGui::SameLine(); if (ImGui::SmallButton("FT")) cfg.wear = 0.38f;
            ImGui::Checkbox("StatTrak", &cfg.stattrak);
            if (cfg.stattrak) ImGui::SliderInt("Kills", &cfg.stattrak_count, 0, 99999);
            if (ImGui::Button("Apply Now", ImVec2(-1, 30)))
                config::skin_changer::enabled = true;
            if (ImGui::Button("Remove Skin", ImVec2(-1, 0)))
                skins::user_skins.erase(sel_wep);
        }
        else {
            ImGui::TextDisabled("Select a skin");
        }
        ImGui::EndChild();
        ImGui::Columns(1);
    }

    void RenderSkinTab() {
        SectionHeader("Inventory Changer", ICON_FA_PAINT_BRUSH);

        ImGui::Checkbox("Enable Skin Changer", &config::skin_changer::enabled);
        ImGui::SameLine();
        if (!db_loaded && !db_loading) {
            if (ImGui::Button("Load Database")) {
                db_loading = true;
                skins::LoadSkinsFromAPI();
                db_loaded = skins::IsDatabaseLoaded();
                db_loading = false;
            }
        }
        else if (db_loading) {
            ImGui::Dummy(ImVec2(0, 5));
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 50) * 0.5f);
            static float angle = 0.0f;
            angle += ImGui::GetIO().DeltaTime * 360.0f;
            if (angle > 360.0f) angle -= 360.0f;
            ImDrawList* dl = ImGui::GetWindowDrawList();
            ImVec2 center = ImGui::GetCursorScreenPos() + ImVec2(25, 25);
            dl->AddCircle(center, 20.0f, IM_COL32(200, 200, 200, 255), 12, 2.0f);
            dl->PathArcTo(center, 18.0f, angle * (IM_PI / 180.0f), (angle + 300.0f) * (IM_PI / 180.0f), 12);
            dl->PathStroke(IM_COL32(100, 200, 255, 255), false, 3.0f);
            ImGui::Dummy(ImVec2(50, 50));
        }
        else {
            ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), " (%zu skins)", skins::skin_database.size());
        }
        ImGui::Separator();

        if (ImGui::BeginTabBar("SkinTabs")) {
            if (ImGui::BeginTabItem("Rifles")) {
                RenderWeaponCategory("rifles", rifles, IM_ARRAYSIZE(rifles), rifle_state);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Pistols")) {
                RenderWeaponCategory("pistols", pistols, IM_ARRAYSIZE(pistols), pistol_state);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("SMG/Heavy")) {
                RenderWeaponCategory("smg", smg_heavy, IM_ARRAYSIZE(smg_heavy), smg_state);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Knives")) {
                ImGui::Columns(2, "KnifeCols", true);
                ImGui::BeginChild("KnifeList", ImVec2(0, 250), true);
                for (const auto& k : knives) {
                    bool sel = (selected_knife == k.id);
                    if (ImGui::Selectable(k.name, sel)) {
                        selected_knife = k.id;
                        skins::selected_knife_id = k.id;
                    }
                }
                ImGui::EndChild();
                ImGui::NextColumn();
                ImGui::BeginChild("KnifeCfg", ImVec2(0, 250), true);
                ImGui::Text("Selected: %s", GetKnifeName(selected_knife));
                auto it = skins::user_skins.find(selected_knife);
                if (it == skins::user_skins.end()) {
                    skins::PlayerSkinConfig cfg;
                    cfg.weapon_id = selected_knife;
                    cfg.wear = 0.01f;
                    skins::user_skins[selected_knife] = cfg;
                    it = skins::user_skins.find(selected_knife);
                }
                auto& cfg = it->second;
                ImGui::SliderInt("Seed", &cfg.seed, 0, 1000);
                ImGui::SliderFloat("Wear", &cfg.wear, 0.0f, 1.0f);
                if (ImGui::Button("Apply Knife", ImVec2(-1, 0)))
                    ApplySelectedKnife(selected_knife);
                ImGui::EndChild();
                ImGui::Columns(1);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Gloves")) {
                ImGui::BeginChild("GloveList", ImVec2(0, 200), true);
                if (skins::glove_database.empty()) ImGui::TextDisabled("Load the skin database to choose gloves.");
                for (const auto& glove : skins::glove_database) {
                    if (ImGui::Selectable(glove.name.c_str(), skins::selected_glove_kit == glove.paint_kit))
                        skins::selected_glove_kit = glove.paint_kit;
                }
                ImGui::EndChild();
                if (ImGui::Button("Apply Gloves", ImVec2(150, 30)))
                    config::skin_changer::enabled = true;
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Debug")) {
                if (game_state::IsInGame()) {
                    const auto reads = skins::GetWeaponReadDiagnostics();
                    ImGui::Text("Material refresh: %s", reads.material_refresh_available ? "Resolved" : "Incomplete/unavailable");
                    ImGui::TextWrapped("%s", reads.status);
                    ImGui::Text("Pawn: %p | Weapon services: %p", reinterpret_cast<void*>(reads.pawn), reinterpret_cast<void*>(reads.services));
                    ImGui::Text("Handles: %d | Data: %p", reads.count, reinterpret_cast<void*>(reads.data));
                    ImGui::Text("Active handle: 0x%08X | Weapon: %p", reads.active_handle, reinterpret_cast<void*>(reads.active));
                    auto weapons = skins::GetCurrentWeaponsDebugInfo();
                    ImGui::Text("Weapons: %zu", weapons.size());
                    if (ImGui::BeginTable("DebugTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                        ImGui::TableSetupColumn("Weapon");
                        ImGui::TableSetupColumn("DefIdx");
                        ImGui::TableSetupColumn("Paint");
                        ImGui::TableSetupColumn("Wear");
                        ImGui::TableSetupColumn("Active");
                        ImGui::TableHeadersRow();
                        for (const auto& w : weapons) {
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn(); ImGui::Text("%s", skins::GetWeaponName(w.def_index));
                            ImGui::TableNextColumn(); ImGui::Text("%d", w.def_index);
                            ImGui::TableNextColumn(); ImGui::Text("%d", w.fallback_paint_kit);
                            ImGui::TableNextColumn(); ImGui::Text("%.4f", w.fallback_wear);
                            ImGui::TableNextColumn(); ImGui::Text("%s", w.is_active ? "Yes" : "No");
                        }
                        ImGui::EndTable();
                    }
                }
                else {
                    const auto state = game_state::GetSnapshot();
                    ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "Gameplay data unavailable");
                    ImGui::TextWrapped("%s", state.status);
                    ImGui::Text("Entity system: %p | Controller: %p | Pawn: %p", reinterpret_cast<void*>(state.entity_list), reinterpret_cast<void*>(state.controller), reinterpret_cast<void*>(state.pawn));
                }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }

    void ApplySelectedKnife(int knife_id) {
        config::skin_changer::enabled = true;
        selected_knife = knife_id;
        skins::selected_knife_id = knife_id;
        skins::ApplyKnife();
        debug_console::Console::Get().Success("[MENU] Applied knife: %s", GetKnifeName(knife_id));
    }

    // Dashboard icons are drawn directly, independent of external font files.
    static void DashboardIcon(ImDrawList* draw, ImVec2 p, int kind, ImU32 color) {
        const float x=p.x, y=p.y;
        if (kind==11) {
            for(int row=0;row<2;++row) for(int col=0;col<2;++col)
                draw->AddRect(ImVec2(x+col*10,y+row*10),ImVec2(x+7+col*10,y+7+row*10),color,2,0,1.5f);
        } else if(kind==0 || kind==1 || kind==9) {
            draw->AddCircle(ImVec2(x+9,y+9),6,color,20,1.5f);
            draw->AddLine(ImVec2(x+9,y-1),ImVec2(x+9,y+19),color,1.5f);
            draw->AddLine(ImVec2(x-1,y+9),ImVec2(x+19,y+9),color,1.5f);
        } else if(kind==2 || kind==4) {
            draw->AddCircle(ImVec2(x+9,y+5),4,color,16,1.5f);
            draw->AddBezierCubic(ImVec2(x,y+19),ImVec2(x,y+7),ImVec2(x+18,y+7),ImVec2(x+18,y+19),color,1.5f);
        } else if(kind==8 || kind==3) {
            draw->AddRect(ImVec2(x,y+5),ImVec2(x+18,y+19),color,3,0,1.5f);
            draw->AddRect(ImVec2(x+5,y),ImVec2(x+13,y+7),color,2,0,1.5f);
        } else {
            for(int row=0;row<3;++row) {
                draw->AddLine(ImVec2(x,y+3+row*6),ImVec2(x+18,y+3+row*6),color,1.5f);
                draw->AddCircleFilled(ImVec2(x+4+(row%2)*9,y+3+row*6),2.5f,color);
            }
        }
    }
    static int selected_tab = 11;
    static const char* page_names[] = {"Aimbot", "Triggerbot", "Players", "Items", "View", "HUD", "General", "Movement", "Inventory", "Grenades", "Profiles", "Overview"};

    static void DashboardStat(const char* id, const char* title, const char* value, const char* detail, float width) {
        ImGui::BeginChild(id, ImVec2(width,132), true);
        ImGui::TextDisabled("%s",title);
        ImGui::Dummy(ImVec2(0,3));
        ImGui::TextColored(ImVec4(.40f,.85f,.95f,1),"%s",value);
        ImGui::TextDisabled("%s",detail);
        ImGui::EndChild();
    }
    static void RenderDashboard() {
        const float width=(ImGui::GetContentRegionAvail().x-24)/3;
        const auto state=game_state::GetSnapshot();
        DashboardStat("session", "SESSION", state.in_game ? "Connected" : "Waiting", "Local player availability",width);
        ImGui::SameLine();
        DashboardStat("mode", "AIM MODE", !config::aimbot::enabled ? "Disabled" : config::aimbot::silent_aim ? "Silent" : "Normal", "Configure in Aimbot",width);
        ImGui::SameLine();
        DashboardStat("inventory", "INVENTORY", config::skin_changer::enabled ? "Enabled" : "Disabled", "Skins and weapon models",width);
        ImGui::Dummy(ImVec2(0,8));
        ImGui::BeginChild("quick_controls",ImVec2(0,172),true);
        ImGui::TextUnformatted("Quick controls");
        ImGui::TextDisabled("Your most-used settings, in one place.");
        ImGui::Separator();
        if(ImGui::BeginTable("toggles",3,ImGuiTableFlags_SizingStretchSame)) {
            ImGui::TableNextColumn(); ImGui::Checkbox("Aim assist",&config::aimbot::enabled);
            ImGui::TableNextColumn(); ImGui::Checkbox("Player visuals",&config::esp::enabled);
            ImGui::TableNextColumn(); ImGui::Checkbox("Skin changer",&config::skin_changer::enabled);
            ImGui::TableNextColumn(); ImGui::Checkbox("Triggerbot",&config::misc::trigger_bot);
            ImGui::TableNextColumn(); ImGui::Checkbox("Radar",&config::misc::radar_hack);
            ImGui::TableNextColumn(); ImGui::Checkbox("Bunny hop",&config::misc::bunny_hop);
            ImGui::EndTable();
        }
        ImGui::EndChild();
        ImGui::Dummy(ImVec2(0,8));
        ImGui::BeginChild("activity",ImVec2(0,214),true);
        ImGui::TextUnformatted("Aim diagnostics");
        ImGui::TextDisabled("Last evaluation; aiming pauses while this menu is open.");
        ImGui::Separator();
        ImGui::Text("Controllers: %u    Resolved pawns: %u",features::aim_controllers.load(),features::aim_resolved_pawns.load());
        ImGui::TextWrapped("Normal: %s",features::normal_aim_status.load());
        ImGui::TextWrapped("Silent: %s",features::silent_aim_status.load());
        ImGui::Text("Angle writes: %u normal / %u history",features::normal_aim_writes.load(),features::silent_history_writes.load());
        ImGui::EndChild();
    }

    void RenderMainMenu() {
        g_card_hover_index=0;
        ImGui::PushFont(g_dashboard_body);
        ImGui::SetNextWindowSize(ImVec2(1120,760),ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSizeConstraints(ImVec2(1040,680),ImVec2(1800,1200));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,14);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding,10);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,6);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(18,18));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(10,7));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(12,10));
        ImGui::PushStyleColor(ImGuiCol_WindowBg,ImVec4(.045f,.06f,.085f,1));
        ImGui::PushStyleColor(ImGuiCol_ChildBg,ImVec4(.065f,.085f,.115f,1));
        ImGui::PushStyleColor(ImGuiCol_Border,ImVec4(.16f,.21f,.28f,.7f));
        ImGui::PushStyleColor(ImGuiCol_Text,ImVec4(.89f,.93f,.98f,1));
        ImGui::PushStyleColor(ImGuiCol_TextDisabled,ImVec4(.46f,.56f,.67f,1));
        ImGui::PushStyleColor(ImGuiCol_FrameBg,ImVec4(.10f,.14f,.19f,1));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,ImVec4(.14f,.21f,.28f,1));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive,ImVec4(.16f,.25f,.33f,1));
        ImGui::PushStyleColor(ImGuiCol_CheckMark,ImVec4(.35f,.83f,.95f,1));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab,ImVec4(.28f,.73f,.88f,1));
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive,ImVec4(.46f,.90f,1,1));
        ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(.10f,.19f,.26f,1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(.15f,.29f,.38f,1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4(.20f,.38f,.47f,1));
        ImGui::PushStyleColor(ImGuiCol_Header,ImVec4(.12f,.25f,.33f,1));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered,ImVec4(.16f,.31f,.39f,1));
        ImGui::PushStyleColor(ImGuiCol_Separator,ImVec4(.17f,.23f,.29f,1));
        if(ImGui::Begin("NEPHILIMGATE##Dashboard",nullptr,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoCollapse)) {
            ImGui::BeginChild("navigation",ImVec2(188,0),false);
            ImGui::Dummy(ImVec2(0,8));
            ImGui::TextColored(ImVec4(.40f,.85f,.95f,1),"NEPHILIMGATE");
            ImGui::TextDisabled("CONTROL CENTER");
            ImGui::Dummy(ImVec2(0,18));
            auto nav=[](int index) {
                ImGui::PushID(index);
                const ImVec2 p=ImGui::GetCursorScreenPos();
                const ImVec2 size(ImGui::GetContentRegionAvail().x,36);
                if(ImGui::InvisibleButton("nav",size)) selected_tab=index;
                static float blend[12]{};
                const bool active=selected_tab==index;
                const float goal=active ? 1.0f : ImGui::IsItemHovered() ? .45f : 0.0f;
                blend[index]+=(goal-blend[index])*(1.0f-std::exp(-14.0f*ImGui::GetIO().DeltaTime));
                auto* draw=ImGui::GetWindowDrawList();
                draw->AddRectFilled(p,ImVec2(p.x+size.x,p.y+size.y),ImGui::GetColorU32(ImVec4(.14f,.31f,.40f,blend[index])),7);
                const ImU32 color=ImGui::GetColorU32(active ? ImVec4(.5f,.9f,1,1) : ImVec4(.55f,.65f,.76f,1));
                DashboardIcon(draw,ImVec2(p.x+12,p.y+9),index,color);
                draw->AddText(ImVec2(p.x+44,p.y+(36-ImGui::GetFontSize())/2),color,page_names[index]);
                ImGui::PopID();
            };
            nav(11);
            ImGui::TextDisabled("COMBAT"); nav(0); nav(1);
            ImGui::TextDisabled("VISUALS"); nav(2); nav(3); nav(4); nav(5);
            ImGui::TextDisabled("PERSONALIZE"); nav(8); nav(7); nav(6); nav(9); nav(10);
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("main",ImVec2(0,0),false);
            ImGui::TextDisabled("WORKSPACE / %s",page_names[selected_tab]);
            ImGui::PushFont(g_dashboard_title ? g_dashboard_title : g_dashboard_body);
            ImGui::TextUnformatted(page_names[selected_tab]);
            ImGui::PopFont();
            ImGui::TextDisabled(selected_tab==11 ? "Your settings and session at a glance." : "Fine-tune your settings. Changes apply immediately.");
            ImGui::Dummy(ImVec2(0,8));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0,8));
            static int previous=-1;
            static float fade=1;
            if(previous!=selected_tab) { previous=selected_tab; fade=.35f; }
            fade+=(1-fade)*(1-std::exp(-16.0f*ImGui::GetIO().DeltaTime));
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha,ImGui::GetStyle().Alpha*fade);
            ImGui::BeginChild("page",ImVec2(0,0),false);
            switch(selected_tab) {
                case 0: RenderAimbotTab(); break; case 1: RenderTriggerBotTab(); break;
                case 2: RenderESPTab(); break; case 3: RenderItemsTab(); break;
                case 4: RenderViewTab(); break; case 5: RenderHudTab(); break;
                case 6: RenderMainTab(); break; case 7: RenderMovementTab(); break;
                case 8: RenderSkinTab(); break; case 9: RenderGrenadesTab(); break;
                case 10: RenderConfigsTab(); break; default: RenderDashboard(); break;
            }
            ImGui::EndChild();
            ImGui::PopStyleVar();
            ImGui::EndChild();
        }
        ImGui::End();
        ImGui::PopStyleColor(17);
        ImGui::PopStyleVar(6);
        ImGui::PopFont();
    }
}
