#include "engine_trace.h"
#include "pattern_resolver.h"
#include "entity_events.h"
#include "aim_profiler.h"
#include "menu_advanced.h"
#include "config.h"
#include "ui/widgets.h"
#include "ui/catalog.h"
#include "ui/theme.h"
#include "config_store.h"
#include "skybox.h"
#include "lighting.h"
#include "skins.h"
#include "loadout_web.h"
#include "features.h"
#include "camera_view.h"
#include "skin_hud.h"
#include "engine_trace.h"
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
#include <sstream>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "skin_images.h"

#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "Shlwapi.lib")

namespace menu_advanced {

    // Animation state.
    MenuAnimationState g_anim;

    // Utility: get dll directory.
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

    // Icon font loading.
    ImFont* g_icon_font = nullptr;
    ImFont* g_icon_font_large = nullptr;
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
                    g_dashboard_title = io.Fonts->AddFontFromFileTTF(face.c_str(), 22);
                    g_dashboard_body = io.Fonts->AddFontFromFileTTF(face.c_str(), 15);
                }
            }
        }
        std::string dll_dir = GetDllDirectory();
        debug_console::Console::Get().Info("[ICONS] Searching for fa-solid-900.ttf...");

        std::vector<std::string> search_dirs = {
            dll_dir + "fonts\\",
            dll_dir + "..\\fonts\\",
            dll_dir + "core\\fonts\\",
            dll_dir + "..\\..\\..\\manager\\core\\fonts\\",
            ".\\fonts\\"
        };

        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        const char* icon_file = "fa-solid-900.ttf";

        for (const auto& dir : search_dirs) {
            std::string full_path = dir + icon_file;
            if (GetFileAttributesA(full_path.c_str()) == INVALID_FILE_ATTRIBUTES)
                continue;

            ImFontConfig cfg;
            cfg.MergeMode = false;
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

        debug_console::Console::Get().Warning("[ICONS] FontAwesome not found - icons disabled.");
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

    // Logo texture.
    static ID3D11ShaderResourceView* g_logo_srv = nullptr;
    static int g_logo_width = 0;
    static int g_logo_height = 0;
    static bool g_logo_load_attempted = false;

    void ResetRendererResources() {
        skin_images::Reset();
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

    // Database & esp preview.
    static bool db_loaded = false;
    static bool db_loading = false;

    static void RenderEspPreview() {
        if (!config::esp::preview) return;
        const ImVec2 origin = ImGui::GetCursorScreenPos();
        const float areaWidth = (std::max)(180.0f, ImGui::GetContentRegionAvail().x);
        ImGui::Dummy({ areaWidth, 260 });
        const auto draw = ImGui::GetWindowDrawList();
        const float alpha = std::isfinite(config::esp::alpha) ? std::clamp(config::esp::alpha, 0.0f, 1.0f) : 1.0f;
        const float scale = std::isfinite(config::esp::scale) ? std::clamp(config::esp::scale, .5f, 2.0f) : 1.0f;
        const auto c = config::esp::box_color;
        const auto color = ImGui::ColorConvertFloat4ToU32({ c[0], c[1], c[2], c[3] * alpha });
        const ImVec2 top(origin.x + areaWidth * .5f - 45, origin.y + 42), bottom(top.x + 90, top.y + 160);

        draw->AddRectFilled(origin, { origin.x + areaWidth, origin.y + 260 }, IM_COL32(11, 14, 21, 255), 10);
        draw->AddRect(origin, { origin.x + areaWidth, origin.y + 260 }, IM_COL32(26, 34, 48, 255), 10, 0, 1.5f);

        if (config::esp::box) {
            draw->AddRect(top, bottom, color, 4.0f, 0, 1.8f);
            draw->AddRect({ top.x - 1, top.y - 1 }, { bottom.x + 1, bottom.y + 1 }, IM_COL32(0, 0, 0, 180), 4.0f);
        }
        if (config::esp::health_bar) {
            draw->AddRectFilled({ top.x - 9, top.y }, { top.x - 5, bottom.y }, IM_COL32(20, 25, 35, static_cast<int>(alpha * 255)), 2);
            draw->AddRectFilled({ top.x - 9, top.y + 35 }, { top.x - 5, bottom.y }, IM_COL32(50, 220, 90, static_cast<int>(alpha * 255)), 2);
        }
        if (config::esp::armor_bar)
            draw->AddRectFilled({ top.x, bottom.y + 4 }, { bottom.x, bottom.y + 7 }, IM_COL32(50, 160, 255, static_cast<int>(alpha * 255)), 2);

        auto text = [&](ImVec2 position, const char* label) { draw->AddText(ImGui::GetFont(), ImGui::GetFontSize() * scale, position, color, label); };
        if (config::esp::name) text({ top.x, top.y - 24 }, "Enemy Player");
        if (config::esp::distance) text({ top.x, bottom.y + 14 }, "24.5m");
        float y = top.y;
        auto flag = [&](bool enabled, const char* label) { if (enabled) { text({ bottom.x + 6, y }, label); y += 15 * scale; } };
        flag(config::esp::weapon_text, "AK-47"); flag(config::esp::bomb, "BOMB"); flag(config::esp::defuse_kit, "KIT");
        flag(config::esp::flashed, "FLASHED"); flag(config::esp::scoped, "SCOPED"); flag(config::esp::planting, "PLANTING");
        flag(config::esp::defusing, "DEFUSING"); flag(config::esp::hostage, "HOSTAGE"); flag(config::misc::show_money, "$4200");
        if (config::esp::snaplines) draw->AddLine({ origin.x + areaWidth * .5f, origin.y + 252 }, { top.x + 45, bottom.y }, color, 1.5f);
        if (config::esp::skeleton) {
            const auto sc = config::esp::skeleton_color;
            const auto skeleton = ImGui::ColorConvertFloat4ToU32({ sc[0], sc[1], sc[2], sc[3] * alpha });
            const float x = top.x + 45;
            draw->AddCircle({ x, top.y + 15 }, 9, skeleton, 16, 1.5f);
            draw->AddLine({ x, top.y + 24 }, { x, top.y + 95 }, skeleton, 1.5f);
            draw->AddLine({ x, top.y + 40 }, { x - 30, top.y + 80 }, skeleton, 1.5f); draw->AddLine({ x, top.y + 40 }, { x + 30, top.y + 80 }, skeleton, 1.5f);
            draw->AddLine({ x, top.y + 95 }, { x - 22, bottom.y - 5 }, skeleton, 1.5f); draw->AddLine({ x, top.y + 95 }, { x + 22, bottom.y - 5 }, skeleton, 1.5f);
        }
    }

    // Render esp tab (players).
    void RenderESPTab() {
        SectionHeader("Player ESP & Visuals", ICON_FA_EYE);
        if (config::esp::enabled) {
            const auto& d = features::esp_diagnostics;
            ImGui::TextWrapped("ESP Diagnostic: %s | Matrix: %s", d.status, pattern_resolver::view_matrix ? "Signature" : "Fallback");
            ImGui::TextDisabled("Entities: %u pawns / %u alive / %u team / %u drawn", d.pawns, d.alive, d.team_pass, d.drawn);
        }

        // Master Toggle Card
        BeginCard("##ESPMasterCard");
        ImGui::Indent(12);
        ImGui::Dummy(ImVec2(0, 4));
        ModernToggle("ENABLE PLAYER ESP SYSTEM", &config::esp::enabled);
        ImGui::SameLine(350);
        ImGui::TextColored(config::esp::enabled ? ImVec4(0.0f, 0.90f, 0.40f, 1.0f) : ImVec4(0.80f, 0.30f, 0.30f, 1.0f),
            "Status: %s", config::esp::enabled ? "ACTIVE" : "DISABLED");
        ImGui::Dummy(ImVec2(0, 4));
        ImGui::Unindent(12);
        EndCard();

        ImGui::Dummy(ImVec2(0, 10));
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.52f);

        // Left Column - Features
        BeginCard("##FeaturesCard");
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), "RENDERING FEATURES");
        ImGui::Separator();
        ImGui::Indent(10);
        ModernToggle("Box ESP", &config::esp::box);
        ModernToggle("Skeleton", &config::esp::skeleton);
        ModernToggle("Glow ESP", &config::esp::glow);
        ImGui::ColorEdit4("Glow Color", config::esp::glow_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
        ModernToggle("Health Bar", &config::esp::health_bar);
        ModernToggle("Armor Bar", &config::esp::armor_bar);
        ModernToggle("Player Name", &config::esp::name);
        ModernToggle("Distance", &config::esp::distance);
        ModernToggle("Weapon Text", &config::esp::weapon_text);
        ModernToggle("Snaplines", &config::esp::snaplines);
        ModernToggle("Bomb Carrier", &config::esp::bomb);
        ModernToggle("Defuse Kit", &config::esp::defuse_kit);
        ModernToggle("Flashed Indicator", &config::esp::flashed);
        ModernToggle("Scoped Indicator", &config::esp::scoped);
        ModernToggle("Planting Flag", &config::esp::planting);
        ModernToggle("Defusing Flag", &config::esp::defusing);
        ModernToggle("Out of FOV Arrows", &config::esp::out_of_fov_arrows);
        ModernToggle("Show Player Money", &config::misc::show_money);
        ImGui::Unindent(10);
        EndCard();

        ImGui::Dummy(ImVec2(0, 10));

        BeginCard("##FiltersCard");
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), "FILTERS & LIMITS");
        ImGui::Separator();
        ImGui::Indent(10);
        ModernToggle("Team Check (Ignore Teammates)", &config::esp::team_check);
        ImGui::Dummy(ImVec2(0, 6));
        ImGui::SliderFloat("ESP Scale", &config::esp::scale, .5f, 2.0f, "%.2fx");
        ImGui::SliderFloat("ESP Opacity", &config::esp::alpha, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Max Render Distance", &config::esp::max_distance, 50.0f, 500.0f, "%.0f meters");
        ImGui::Unindent(10);
        EndCard();

        ImGui::NextColumn();

        // Right Column - Preview & Colors
        ModernToggle("Live ESP Preview", &config::esp::preview);
        RenderEspPreview();

        ImGui::Dummy(ImVec2(0, 10));
        BeginCard("##ColorsCard");
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), "COLOR CONFIGURATION");
        ImGui::Separator();
        ImGui::Indent(10);
        ImGui::Text("Enemy Box Color:");
        ImGui::ColorEdit4("##EnemyColor", config::esp::box_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
        ImGui::Dummy(ImVec2(0, 6));
        ImGui::Text("Teammate Color:");
        ImGui::ColorEdit4("##TeamColor", config::esp::team_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
        ImGui::Dummy(ImVec2(0, 6));
        ImGui::Text("Skeleton Color:");
        ImGui::ColorEdit4("##SkeletonColor", config::esp::skeleton_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
        ImGui::Unindent(10);
        EndCard();

        ImGui::Columns(1);
    }

    // Render aimbot tab.
    void RenderAimbotTab() {
        SectionHeader("Aimbot & Recoil System", ICON_FA_CROSSHAIRS);

        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.54f);

        BeginCard("##AimbotMainCard");
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), "AIMBOT CORE");
        ImGui::Separator();
        ImGui::Indent(10);
        ModernToggle("Enable Aimbot", &config::aimbot::enabled);
        ModernToggle("Auto Shoot (Force Shoot)", &config::aimbot::auto_shoot);
        ModernToggle("Autoscope", &config::aimbot::autoscope);
        ModernToggle("Team Check", &config::aimbot::team_check);
        ModernToggle(config::aimbot::silent_aim ? "Wall Check (Ray Trace)###AimVisibility" : "Spotted Check###AimVisibility", &config::aimbot::visible_check);
        if (config::aimbot::silent_aim && config::aimbot::visible_check)
            ImGui::TextWrapped("Trace Engine: %s", engine_trace::status.load());
        ModernToggle("Silent Aim (Micro-flick)", &config::aimbot::silent_aim);
        ModernToggle("Lock Target", &config::aimbot::lock_target);
        ModernToggle("Draw FOV Circle", &config::aimbot::draw_fov);
        ImGui::ColorEdit4("FOV Circle Color", config::aimbot::fov_color, ImGuiColorEditFlags_NoInputs);

        ImGui::Dummy(ImVec2(0, 6));
        ImGui::TextUnformatted("Target Hitboxes");
        ImGui::Checkbox("Head", &config::aimbot::hitboxes[0]); ImGui::SameLine();
        ImGui::Checkbox("Neck", &config::aimbot::hitboxes[1]); ImGui::SameLine();
        ImGui::Checkbox("Chest", &config::aimbot::hitboxes[2]); ImGui::SameLine();
        ImGui::Checkbox("Pelvis", &config::aimbot::hitboxes[3]);
        ModernToggle("Hitscan (Nearest Enabled Bone)", &config::aimbot::hitscan);

        ImGui::Dummy(ImVec2(0, 6));
        ImGui::Text("Aimbot FOV (degrees):");
        ImGui::SliderFloat("##FOV", &config::aimbot::fov, 1.0f, 30.0f, "%.1f deg");
        ImGui::Text("Smoothing Factor:");
        ImGui::SliderFloat("##Smooth", &config::aimbot::smoothing, 0.0f, 1.0f, "%.2f");
        ImGui::Text("Max Target Distance:");
        ImGui::SliderFloat("##MaxDist", &config::aimbot::max_distance, 100.0f, 10000.0f, "%.0f units");
        ImGui::Unindent(10);
        EndCard();

        ImGui::NextColumn();

        BeginCard("##RCSCard");
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), "RECOIL CONTROL SYSTEM (RCS)");
        ImGui::Separator();
        ImGui::Indent(10);
        ModernToggle("Enable Standalone & Aim RCS", &config::rcs::enabled);
        ImGui::Text("RCS Strength (0%% - 100%%):");
        ImGui::SliderFloat("##RCSStrength", &config::rcs::strength, 0.0f, 1.0f, "%.2f");
        ImGui::Dummy(ImVec2(0, 6));
        ImGui::TextColored(ImVec4(0.9f, 0.8f, 0.2f, 1.0f), "Pause Key: Mouse5 (Hold to pause)");
        ImGui::Unindent(10);
        EndCard();

        ImGui::Dummy(ImVec2(0, 10));
        BeginCard("##AimDiagnosticsCard");
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), "AIM DIAGNOSTICS & PROFILER");
        ImGui::Separator();
        ImGui::Indent(10);
        ImGui::TextWrapped("Events: %s | Cache Hits: %llu", entity_events::enabled.load() ? "Active" : "Scan Fallback", entity_events::hits.load());
        ImGui::TextWrapped("Normal Aim Status: %s", features::normal_aim_status.load());
        ImGui::TextWrapped("Silent Aim Status: %s", features::silent_aim_status.load());
        if (ImGui::CollapsingHeader("Performance Profiler")) {
            if (ImGui::Button("Capture 10 Seconds Profile", ImVec2(220, 28))) {
                std::ostringstream settings;
                settings << "aim=" << config::aimbot::enabled << " silent=" << config::aimbot::silent_aim
                    << " FOV=" << config::aimbot::fov << " skins=" << config::skin_changer::enabled;
                aim_profiler::Start(settings.str());
            }
            if (auto capture = aim_profiler::current.load()) {
                const auto report = aim_profiler::Report(*capture);
                if (ImGui::Button("Copy Profile to Clipboard")) ImGui::SetClipboardText(report.c_str());
                ImGui::TextWrapped("%s", report.c_str());
            }
        }
        ImGui::Unindent(10);
        EndCard();

        ImGui::Columns(1);
    }

    // Skin changer state.
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

    using namespace ui_constants;

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
        }

        const float browserWidth=ImGui::GetContentRegionAvail().x;
        ImGui::Columns(3, id, false);
        ImGui::SetColumnWidth(0, 135);
        ImGui::SetColumnWidth(1, (std::max)(180.f, browserWidth - 395.f));

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
                if(skins::IsKnife(weapon_ids[i])) skins::selected_knife_id=weapon_ids[i];
            }
            if (has_skin) ImGui::PopStyleColor();
        }
        ImGui::EndChild();

        ImGui::NextColumn();

        int sel_wep = weapon_ids[state.selected_weapon_idx];
        ImGui::BeginChild((std::string("SkinList##") + id).c_str(), ImVec2(0, 0), true);
        ImGui::Text("Skins for %s", skins::GetWeaponName(sel_wep));
        ImGui::Separator();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputTextWithHint((std::string("##Search") + id).c_str(), "Search finishes...", state.search, sizeof(state.search));
        ImGui::Separator();

        if(ImGui::Selectable("Default finish", skins::user_skins.count(sel_wep) && skins::user_skins.at(sel_wep).paint_kit==0)) {
            auto& cfg=skins::user_skins[sel_wep]; cfg.weapon_id=sel_wep; cfg.paint_kit=0;
            if(skins::IsKnife(sel_wep)) skins::selected_knife_id=sel_wep;
        }
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

        const float gap = 10.f;
        const int columns = (std::max)(1, int((ImGui::GetContentRegionAvail().x + gap) / 160.f));
        const float cardWidth = (ImGui::GetContentRegionAvail().x - gap * (columns - 1)) / columns;
        ImGuiListClipper clipper;
        clipper.Begin((int(available.size()) + columns - 1) / columns, 140.f + ImGui::GetStyle().ItemSpacing.y);
        while (clipper.Step()) for (int row=clipper.DisplayStart; row<clipper.DisplayEnd; ++row) {
            for (int column=0; column<columns; ++column) {
                const int index=row*columns+column;
                if (index>=int(available.size())) break;
                if(column) ImGui::SameLine(0,gap);
                const auto& skin=available[index];
                ImGui::PushID(skin.paint_kit);
                const auto configured=skins::user_skins.find(sel_wep);
                const bool selected=configured!=skins::user_skins.end() && configured->second.paint_kit==skin.paint_kit;
                const ImVec2 pos=ImGui::GetCursorScreenPos(), size(cardWidth,140);
                if(ImGui::InvisibleButton("finish",size)) {
                    auto& cfg=skins::user_skins[sel_wep];
                    cfg.weapon_id=sel_wep; cfg.paint_kit=skin.paint_kit;
                    if(!skin.stattrak_available) cfg.stattrak=false;
                    if(skins::IsKnife(sel_wep)) skins::selected_knife_id=sel_wep;
                }
                const bool hovered=ImGui::IsItemHovered();
                auto* draw=ImGui::GetWindowDrawList();
                draw->AddRectFilled(pos,pos+size,selected?IM_COL32(30,62,75,255):hovered?IM_COL32(35,43,56,255):IM_COL32(24,30,41,255),8);
                draw->AddRect(pos,pos+size,selected?IM_COL32(82,214,204,255):IM_COL32(51,62,78,255),8);
                skin_images::Draw(sel_wep,skin.name,pos+ImVec2(8,5),ImVec2(cardWidth-16,100));
                draw->PushClipRect(pos+ImVec2(10,105),pos+size-ImVec2(8,5),true);
                draw->AddText(pos+ImVec2(10,109),IM_COL32(230,237,246,255),skin.name.c_str());
                draw->PopClipRect();
                if(hovered) ImGui::SetTooltip("%s | %s%s",skins::GetWeaponName(sel_wep),skin.name.c_str(),selected?" (selected)":"");
                ImGui::PopID();
            }
        }
        if (available.empty()) ImGui::TextDisabled("No skins found");
        ImGui::EndChild();

        ImGui::NextColumn();

        ImGui::BeginChild((std::string("Config##") + id).c_str(), ImVec2(0, 0), true);
        auto cfg_it = skins::user_skins.find(sel_wep);
        if (cfg_it != skins::user_skins.end()) {
            skins::PlayerSkinConfig& cfg = cfg_it->second;
            ImGui::Text("Selected finish");
            for(const auto& skin: skins::skin_database) if(skin.weapon_id==sel_wep && skin.paint_kit==cfg.paint_kit) {
                skin_images::Draw(sel_wep,skin.name,ImGui::GetCursorScreenPos(),ImVec2(ImGui::GetContentRegionAvail().x,120));
                ImGui::Dummy(ImVec2(0,120)); break;
            }
            ImGui::Separator();
            for (const auto& s : skins::skin_database) {
                if (s.weapon_id == sel_wep && s.paint_kit == cfg.paint_kit) {
                    const float* c = skins::GetRarityColor(s.rarity);
                    ImGui::TextColored(ImVec4(c[0], c[1], c[2], c[3]), "%s", s.name.c_str());
                    break;
                }
            }
            ImGui::PushItemWidth(-1);
            ImGui::TextDisabled("Paint kit ID");
            if(ImGui::InputInt("##paint", &cfg.paint_kit)) cfg.paint_kit=(std::max)(0,cfg.paint_kit);
            ImGui::TextDisabled("Pattern seed");
            ImGui::SliderInt("##seed", &cfg.seed, 0, 1000);
            ImGui::TextDisabled("Wear");
            ImGui::SliderFloat("##wear", &cfg.wear, 0.0f, 1.0f, "%.4f");
            ImGui::PopItemWidth();
            if (ImGui::SmallButton("FN")) cfg.wear = 0.01f;
            ImGui::SameLine(); if (ImGui::SmallButton("MW")) cfg.wear = 0.08f;
            ImGui::SameLine(); if (ImGui::SmallButton("FT")) cfg.wear = 0.38f;
            ImGui::Checkbox("StatTrak", &cfg.stattrak);
            if (cfg.stattrak) ImGui::SliderInt("Kills", &cfg.stattrak_count, 0, 99999);
            if (ImGui::Button("Apply Now", ImVec2(-1, 30))) {
                config::skin_changer::enabled = true;
                if(skins::IsKnife(sel_wep)) ApplySelectedKnife(sel_wep);
            }
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
        skin_images::Tick(GetDllDirectory());
        ImGui::TextColored(ImVec4(.32f,.84f,.80f,1), "FINISH LIBRARY");
        ImGui::SameLine(); ImGui::TextDisabled("Choose a finish, then tune your loadout.");
        ImGui::Checkbox("Enable", &config::skin_changer::enabled);
        ImGui::SameLine();
        if (!skins::IsDatabaseLoaded()) {
            if(ImGui::Button("Load Database")) {
                skins::LoadSkinsFromAPI(); db_loaded=skins::IsDatabaseLoaded();
            }
        } else ImGui::TextDisabled("%zu finishes",skins::skin_database.size());
        ImGui::SameLine();
        if(ImGui::Button("Loadout tools")) ImGui::OpenPopup("loadout_tools");
        if(ImGui::BeginPopup("loadout_tools")) {
            if(ImGui::MenuItem("Open web editor")) loadout_web::OpenEditor();
            if(ImGui::MenuItem("Export catalog")) loadout_web::ExportCatalog();
            if(ImGui::MenuItem("Import loadout")) loadout_web::ImportLoadout();
            ImGui::EndPopup();
        }
        if (!loadout_web::status.empty()) ImGui::TextWrapped("%s", loadout_web::status.c_str());
        if(!skin_images::catalogJob.valid() && skin_images::catalog.images.empty())
            ImGui::TextWrapped("Preview library unavailable. Place skins_images beside the DLL.");
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
                static CategoryState knife_state;
                static const int knife_ids[]={500,503,505,506,507,508,509,512,514,515,516,517,518,519,520,521,522,523,525,526};
                for(int i=0;i<IM_ARRAYSIZE(knife_ids);++i)
                    if(knife_ids[i]==skins::selected_knife_id) knife_state.selected_weapon_idx=i;
                RenderWeaponCategory("knives",knife_ids,IM_ARRAYSIZE(knife_ids),knife_state);
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
                ImGui::TextWrapped("HUD: %s", skin_hud::status.load());
                ImGui::Text("HUD ticks: %u | Requests: %u | Clear calls: %u", skin_hud::ticks.load(), skin_hud::revision.load(), skin_hud::clear_calls.load());
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

    // Render dashboard / overview.
    static void DashboardStat(const char* id, const char* title, const char* value, const char* detail, float width) {
        ImGui::BeginChild(id, ImVec2(width, 110), true);
        ImGui::TextDisabled("%s", title);
        ImGui::Dummy(ImVec2(0, 2));
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), "%s", value);
        ImGui::TextDisabled("%s", detail);
        ImGui::EndChild();
    }

    static void RenderDashboard() {
        const float width = (ImGui::GetContentRegionAvail().x - 24) / 3;
        const auto state = game_state::GetSnapshot();
        DashboardStat("session", "GAME CONNECTION", state.in_game ? "Connected" : "Waiting for Match", "Local Player Status", width);
        ImGui::SameLine();
        DashboardStat("mode", "AIMBOT STATUS", !config::aimbot::enabled ? "Disabled" : config::aimbot::silent_aim ? "Silent Micro-Flick" : "Smooth Lock", "Combat Control", width);
        ImGui::SameLine();
        DashboardStat("inventory", "SKIN CHANGER", config::skin_changer::enabled ? "Active" : "Disabled", "Custom Weapon Skins", width);

        ImGui::Dummy(ImVec2(0, 8));
        BeginCard("##QuickControlsCard");
        ImGui::Indent(10);
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), "Quick controls");
        ImGui::TextDisabled("Choose the tools you want active in this session.");
        ImGui::Separator();
        if (ImGui::BeginTable("toggles_table", 3, ImGuiTableFlags_SizingStretchSame)) {
            ImGui::TableNextColumn(); ModernToggle("Aimbot System", &config::aimbot::enabled);
            ImGui::TableNextColumn(); ModernToggle("Player ESP", &config::esp::enabled);
            ImGui::TableNextColumn(); ModernToggle("Skin Changer", &config::skin_changer::enabled);
            ImGui::TableNextColumn(); ModernToggle("Triggerbot", &config::misc::trigger_bot);
            ImGui::TableNextColumn(); ModernToggle("Radar Hack", &config::misc::radar_hack);
            ImGui::TableNextColumn(); ModernToggle("Auto BHop", &config::misc::bunny_hop);
            ImGui::EndTable();
        }
        ImGui::Unindent(10);
        EndCard();

        ImGui::Dummy(ImVec2(0, 8));
        BeginCard("##AimDiagCard");
        ImGui::Indent(10);
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), "Runtime status");
        ImGui::Separator();
        ImGui::Text("Controllers: %u | Resolved Pawns: %u", features::aim_controllers.load(), features::aim_resolved_pawns.load());
        ImGui::TextWrapped("Normal Aim Status: %s", features::normal_aim_status.load());
        ImGui::TextWrapped("Silent Aim Status: %s", features::silent_aim_status.load());
        ImGui::Text("Engine Trace Status: %s", engine_trace::status.load());
        ImGui::Unindent(10);
        EndCard();
    }

    // Selected tab & names.
    static int selected_tab = 11;


    // Master render main menu.
    void RenderMainMenu() {
        WorkspaceTheme theme;
        ImGui::PushFont(g_dashboard_body);
        const auto display = ImGui::GetIO().DisplaySize;
        ImGui::SetNextWindowSize({(std::min)(1120.f, display.x-32), (std::min)(760.f, display.y-32)}, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSizeConstraints({760, 520}, {1920, 1400});
        if (ImGui::Begin("NEPHILIMGATE_UI##Master", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse)) {
            ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_CheckMark), "N / NEPHILIMGATE");
            ImGui::SameLine(); ImGui::TextDisabled("WORKSPACE");
            const auto state = game_state::GetSnapshot();
            ImGui::SameLine((std::max)(350.f, ImGui::GetWindowWidth()-260.f));
            ImGui::TextDisabled("%s  |  %.0f FPS", state.in_game ? "In session" : "Not connected", ImGui::GetIO().Framerate);
            ImGui::Separator();
            const float bodyHeight = ImGui::GetContentRegionAvail().y - 28;
            ImGui::BeginChild("navigation", {204, bodyHeight}, ImGuiChildFlags_Borders);
            static ImGuiTextFilter filter;
            filter.Draw("##PageFilter", -1);
            if (!filter.IsActive() && !ImGui::IsItemActive()) {
                const auto p=ImGui::GetItemRectMin();
                ImGui::GetWindowDrawList()->AddText({p.x+10,p.y+6},ImGui::GetColorU32(ImGuiCol_TextDisabled),"Find a page...");
            }
            ImGui::Spacing();
            auto section = [&](const char* title, std::initializer_list<int> pages) {
                bool any=false; for(int page:pages) any |= filter.PassFilter(page_names[page]);
                if (!any) return;
                ImGui::TextDisabled("%s", title);
                for (int page : pages) if (filter.PassFilter(page_names[page])) {
                    ImGui::PushID(page);
                    static float highlight[12]{};
                    static constexpr const char* icons[] = { ICON_FA_CROSSHAIRS, ICON_FA_BOLT, ICON_FA_EYE, ICON_FA_CUBE,
                        ICON_FA_PALETTE, ICON_FA_MAP, ICON_FA_COG, ICON_FA_RUNNING, ICON_FA_KNIFE, ICON_FA_BOMB, ICON_FA_SAVE, ICON_FA_HOME };
                    const auto start = ImGui::GetCursorScreenPos();
                    const float width = ImGui::GetContentRegionAvail().x;
                    ImGui::PushStyleColor(ImGuiCol_Header, {0,0,0,0});
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, {0,0,0,0});
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, {0,0,0,0});
                    if (ImGui::Selectable("##page", selected_tab==page, 0, {0,32})) selected_tab=page;
                    ImGui::PopStyleColor(3);
                    const float target = selected_tab==page ? 1.f : (ImGui::IsItemHovered() ? .45f : 0.f);
                    highlight[page] += (target-highlight[page]) * (1.f-std::exp(-18.f*ImGui::GetIO().DeltaTime));
                    auto* draw = ImGui::GetWindowDrawList();
                    auto accent = ImGui::GetStyleColorVec4(ImGuiCol_CheckMark);
                    accent.w = highlight[page] * .14f;
                    draw->AddRectFilled(start, {start.x+width, start.y+32}, ImGui::GetColorU32(accent), 6);
                    if (selected_tab==page) draw->AddRectFilled({start.x,start.y+8},{start.x+3,start.y+24},ImGui::GetColorU32(ImGuiCol_CheckMark),2);
                    if (g_icon_font) draw->AddText(g_icon_font, 14, {start.x+12,start.y+9}, ImGui::GetColorU32(selected_tab==page ? ImGuiCol_CheckMark : ImGuiCol_TextDisabled), icons[page]);
                    draw->AddText({start.x+38,start.y+8},ImGui::GetColorU32(ImGuiCol_Text),page_names[page]);
                    ImGui::PopID();
                }
                ImGui::Spacing();
            };
            section("WORKSPACE", {11,10});
            section("ASSISTANCE", {0,1,7});
            section("VISUALS", {2,3,4,5,9});
            section("PERSONALIZATION", {8,6});
            if (filter.IsActive()) {
                bool any=false; for(const char* page:page_names) any |= filter.PassFilter(page);
                if (!any) ImGui::TextDisabled("No matching pages");
            }
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("workspace", {0,bodyHeight}, ImGuiChildFlags_Borders);
            ImGui::PushFont(g_dashboard_title ? g_dashboard_title : g_dashboard_body);
            ImGui::TextUnformatted(page_names[selected_tab]);
            ImGui::PopFont();
            static constexpr const char* descriptions[] = {
                "Target selection, aim response and recoil control.", "Activation and timing for the trigger assistant.",
                "Choose which player details to display.", "Labels for weapons, items and objectives.",
                "Camera, lighting and environment settings.", "On-screen feedback and information.",
                "General preferences and gameplay helpers.", "Movement assistance controls.",
                "Customize weapons, knives and gloves.", "Throw prediction, projectile trails and surface details.",
                "Save, load and reset your workspace settings.", "Your session and frequently used controls."
            };
            ImGui::TextDisabled("%s", descriptions[selected_tab]);
            ImGui::Separator();
            ImGui::BeginChild("page_view", {0,0}, ImGuiChildFlags_None);
            switch (selected_tab) {
                case 0: RenderAimbotTab(); break;
                case 1: RenderTriggerBotTab(); break;
                case 2: RenderESPTab(); break;
                case 3: RenderItemsTab(); break;
                case 4: RenderViewTab(); break;
                case 5: RenderHudTab(); break;
                case 6: RenderMainTab(); break;
                case 7: RenderMovementTab(); break;
                case 8: RenderSkinTab(); break;
                case 9: RenderGrenadesTab(); break;
                case 10: RenderConfigsTab(); break;
                default: RenderDashboard(); break;
            }
            ImGui::EndChild();
            ImGui::EndChild();
            ImGui::Separator();
            ImGui::TextDisabled("INSERT  Menu     F1  Console");
            ImGui::SameLine();
            if (!config_store::status.empty()) ImGui::TextDisabled("  |  %s", config_store::status.c_str());
        }
        ImGui::End();
        ImGui::PopFont();
    }
}
