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

#pragma comment(lib, "windowscodecs.lib")

namespace menu_advanced {

    // ==================== LOGO TEXTURE ====================
    static ID3D11ShaderResourceView* g_logo_srv = nullptr;
    static int g_logo_width = 0;
    static int g_logo_height = 0;
    static bool g_logo_load_attempted = false;

    static bool LoadLogoTexture() {
        if (g_logo_load_attempted) return g_logo_srv != nullptr;
        g_logo_load_attempted = true;

        if (!interfaces::d3d11_device) return false;

        // Build path relative to DLL
        char dll_path[MAX_PATH];
        HMODULE hm = nullptr;
        GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCSTR)&LoadLogoTexture, &hm);
        GetModuleFileNameA(hm, dll_path, MAX_PATH);
        std::string path(dll_path);
        size_t last_sep = path.find_last_of("\\/");
        if (last_sep != std::string::npos) path = path.substr(0, last_sep + 1);
        path += "logo.png";

        // Check if file exists, if not try hardcoded path
        DWORD attr = GetFileAttributesA(path.c_str());
        if (attr == INVALID_FILE_ATTRIBUTES) {
            // Try relative to known location
            path = "C:\\Windows\\manager\\manager\\core\\logo.png";
            attr = GetFileAttributesA(path.c_str());
            if (attr == INVALID_FILE_ATTRIBUTES) return false;
        }

        // Convert to wide string
        int wlen = MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, nullptr, 0);
        std::wstring wpath(wlen, L'\0');
        MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, &wpath[0], wlen);

        // Use WIC to decode PNG
        Microsoft::WRL::ComPtr<IWICImagingFactory> wic_factory;
        HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&wic_factory));
        if (FAILED(hr)) {
            // Try CoInitialize first
            CoInitializeEx(nullptr, COINIT_MULTITHREADED);
            hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&wic_factory));
            if (FAILED(hr)) return false;
        }

        Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
        hr = wic_factory->CreateDecoderFromFilename(wpath.c_str(), nullptr,
            GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
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

        // Create D3D11 texture
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
    // Knife selector state
    static int selected_knife = 507; // Karambit by default
    static bool show_knife_preview = true;
    static int knife_filter = 0; // 0 = all, 1 = expensive, 2 = cheap
    static bool show_knife_stats = true;
    static int last_applied_knife = -1;

    // Weapon selector state
    static int selected_weapon_tab = 0;
    static int selected_weapon_skin = -1;
    static std::vector<skins::SkinInfo> current_weapon_skins;

    // Glove state
    static int selected_glove = 0;
    static const int glove_ids[] = { 10006, 10007, 10015, 10016, 10018, 10024 };
    static const char* glove_names[] = { "Superconductor", "Arid", "Pandora's Box", "Foundation", "Vice", "Emerald Web" };

    // Database load state
    static bool db_loaded = false;
    static bool db_loading = false;

    // Weapon category definitions
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

    // Per-category UI state
    struct CategoryState {
        int selected_weapon_idx = 0;
        int selected_skin_idx = -1;
        char search[64] = {};
    };
    static CategoryState rifle_state;
    static CategoryState pistol_state;
    static CategoryState smg_state;

    struct KnifeInfo {
        int id;
        const char* name;
        int price; // Relative price: 1 = cheap, 2 = medium, 3 = expensive
    };

    const KnifeInfo knives[] = {
        { 500, "Bayonet", 2 },
        { 503, "Classic Knife", 1 },
        { 505, "Flip Knife", 1 },
        { 506, "Gut Knife", 1 },
        { 507, "Karambit", 3 },
        { 508, "M9 Bayonet", 2 },
        { 509, "Tactical Knife", 1 },
        { 512, "Falchion", 1 },
        { 514, "Survival Bowie", 2 },
        { 515, "Butterfly", 2 },
        { 516, "Shadow Daggers", 2 },
        { 517, "Paracord Knife", 1 },
        { 518, "Survival Knife", 1 },
        { 519, "Ursus Knife", 2 },
        { 520, "Navaja Knife", 1 },
        { 521, "Nomad Knife", 2 },
        { 522, "Stiletto", 2 },
        { 523, "Talon Knife", 3 },
        { 525, "Skeleton Knife", 3 },
        { 526, "Kukri Knife", 2 }
    };

    const char* GetKnifeName(int knife_id) {
        for (const auto& k : knives) {
            if (k.id == knife_id) {
                return k.name;
            }
        }
        return "Unknown";
    }

    const char* GetKnifePrice(int knife_id) {
        for (const auto& k : knives) {
            if (k.id == knife_id) {
                switch (k.price) {
                    case 1: return "Budget";
                    case 2: return "Mid-Tier";
                    case 3: return "Premium";
                    default: return "Unknown";
                }
            }
        }
        return "Unknown";
    }

    void RenderKnifeTab() {
        ImGui::Columns(2, "KnifeColumns", true);

        // Left side - Knife list
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Available Knives");
        ImGui::Separator();

        // Filter options
        ImGui::Text("Filter:");
        ImGui::RadioButton("All Knives##filter", &knife_filter, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Budget##filter", &knife_filter, 1);
        ImGui::SameLine();
        ImGui::RadioButton("Premium##filter", &knife_filter, 2);

        ImGui::Separator();
        ImGui::Text("Knife Selection:");
        ImGui::BeginChild("KnifeList", ImVec2(0, 300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

        for (const auto& knife : knives) {
            int price = knife.price;
            
            // Apply filter
            if (knife_filter == 1 && price != 1) continue; // Budget
            if (knife_filter == 2 && price != 3) continue; // Premium

            bool is_selected = (selected_knife == knife.id);
            ImVec4 color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
            
            if (price == 3) color = ImVec4(1.0f, 0.8f, 0.0f, 1.0f); // Premium - Gold
            else if (price == 2) color = ImVec4(0.7f, 0.9f, 1.0f, 1.0f); // Mid - Blue
            else color = ImVec4(0.7f, 1.0f, 0.7f, 1.0f); // Budget - Green

            ImGui::PushStyleColor(ImGuiCol_Text, color);
            
            std::string label = std::string(knife.name) + " (" + GetKnifePrice(knife.id) + ")";
            
            if (ImGui::Selectable(label.c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(0, 25))) {
                selected_knife = knife.id;
                skins::selected_knife_id = knife.id;
                if (ImGui::IsMouseDoubleClicked(0)) {
                    ApplySelectedKnife(knife.id);
                }
            }
            
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();

        ImGui::NextColumn();

        // Right side - Knife config
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.8f, 1.0f), "Knife Details");
        ImGui::Separator();

        ImGui::BeginChild("KnifePreview", ImVec2(0, 300), true);

        ImGui::Text("Selected: %s", GetKnifeName(selected_knife));
        ImGui::Text("Category: %s", GetKnifePrice(selected_knife));
        ImGui::Separator();

        // Editable knife skin config — synced with skins::user_skins
        {
            auto it = skins::user_skins.find(selected_knife);
            if (it == skins::user_skins.end()) {
                skins::PlayerSkinConfig cfg;
                cfg.weapon_id = selected_knife;
                cfg.wear = 0.01f;
                skins::user_skins[selected_knife] = cfg;
                it = skins::user_skins.find(selected_knife);
            }
            skins::PlayerSkinConfig& cfg = it->second;

            // Show available knife skins from database
            if (skins::IsDatabaseLoaded()) {
                std::vector<skins::SkinInfo> knife_skins;
                std::set<int> seen;
                for (const auto& s : skins::skin_database) {
                    if (s.weapon_id == selected_knife && !s.name.empty() && !seen.count(s.paint_kit)) {
                        seen.insert(s.paint_kit);
                        knife_skins.push_back(s);
                    }
                }
                if (!knife_skins.empty()) {
                    static int combo_idx = 0;
                    const char* preview = "Vanilla";
                    for (size_t i = 0; i < knife_skins.size(); i++) {
                        if (knife_skins[i].paint_kit == cfg.paint_kit) {
                            preview = knife_skins[i].name.c_str();
                            combo_idx = (int)i;
                            break;
                        }
                    }
                    if (ImGui::BeginCombo("Skin##knife_skin", preview)) {
                        if (ImGui::Selectable("Vanilla", cfg.paint_kit == 0)) {
                            cfg.paint_kit = 0;
                        }
                        for (size_t i = 0; i < knife_skins.size(); i++) {
                            if (ImGui::Selectable(knife_skins[i].name.c_str(), cfg.paint_kit == knife_skins[i].paint_kit)) {
                                cfg.paint_kit = knife_skins[i].paint_kit;
                            }
                        }
                        ImGui::EndCombo();
                    }
                }
            }

            ImGui::SliderInt("Seed##kn", &cfg.seed, 0, 1000);
            ImGui::SliderFloat("Wear##kn", &cfg.wear, 0.0f, 1.0f, "%.4f");
            if (ImGui::SmallButton("FN##kn")) cfg.wear = 0.01f;
            ImGui::SameLine();
            if (ImGui::SmallButton("MW##kn")) cfg.wear = 0.08f;
            ImGui::SameLine();
            if (ImGui::SmallButton("FT##kn")) cfg.wear = 0.38f;

            ImGui::Checkbox("StatTrak##kn", &cfg.stattrak);
            if (cfg.stattrak)
                ImGui::SliderInt("Kills##kn", &cfg.stattrak_count, 0, 99999);
        }

        ImGui::EndChild();

        ImGui::Columns(1);
        ImGui::Separator();

        // Action buttons
        if (ImGui::Button("Apply Selected Knife", ImVec2(200, 35))) {
            ApplySelectedKnife(selected_knife);
        }
        ImGui::SameLine();
        if (ImGui::Button("Random Knife", ImVec2(150, 35))) {
            int random_idx = rand() % 15;
            selected_knife = knives[random_idx].id;
            ApplySelectedKnife(selected_knife);
        }

        ImGui::Checkbox("Show Stats##knife", &show_knife_stats);
        ImGui::SameLine();
        ImGui::Checkbox("Auto Preview##knife", &show_knife_preview);

        if (last_applied_knife != -1) {
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Last Applied: %s", GetKnifeName(last_applied_knife));
        }
    }

    void RenderESPTab() {
        auto RenderCard = [](const char* title, auto content_func) {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.15f, 0.8f));
            ImGui::BeginChild(title, ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 1.0f, 0.8f, 1.0f));
            ImGui::Text("%s", title);
            ImGui::PopStyleColor();
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 5));
            
            content_func();
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        };

        // Main ESP Toggle
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.12f, 0.18f, 0.9f));
        ImGui::BeginChild("##ESPMaster", ImVec2(0, 80), true);
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::Indent(15);
        
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
        ImGui::Checkbox("ENABLE ESP SYSTEM", &config::esp::enabled);
        ImGui::PopStyleVar();
        
        ImGui::SameLine(300);
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), 
            "Status: %s", config::esp::enabled ? "ACTIVE" : "DISABLED");
        
        ImGui::Unindent(15);
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::Dummy(ImVec2(0, 10));

        // Two-column layout for cards
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, 420);

        // Left Column - Rendering Features
        RenderCard("RENDERING FEATURES", []() {
            ImGui::Indent(10);
            
            ImGui::Checkbox("Box ESP", &config::esp::box);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Player bounding boxes");
            
            ImGui::Checkbox("Skeleton", &config::esp::skeleton);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Bone structure");
            
            ImGui::Checkbox("Health Bar", &config::esp::health_bar);
            ImGui::SameLine(200);
            ImGui::TextDisabled("HP indicator");
            
            ImGui::Checkbox("Distance", &config::esp::distance);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Distance in meters");
            
            ImGui::Checkbox("Snaplines", &config::esp::snaplines);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Lines to players");
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        });

        ImGui::Dummy(ImVec2(0, 10));

        RenderCard("FILTERS & OPTIONS", []() {
            ImGui::Indent(10);
            
            ImGui::Checkbox("Team Check", &config::esp::team_check);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Ignore teammates");
            
            ImGui::Dummy(ImVec2(0, 10));
            ImGui::Text("Max Distance:");
            ImGui::PushItemWidth(280);
            ImGui::SliderFloat("##MaxDistESP", &config::esp::max_distance, 50.0f, 500.0f, "%.0fm");
            ImGui::PopItemWidth();
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        });

        ImGui::NextColumn();

        // Right Column - Colors
        RenderCard("COLOR CONFIGURATION", []() {
            ImGui::Indent(10);
            
            ImGui::Text("Enemy Color:");
            ImGui::ColorEdit4("##EnemyColor", config::esp::box_color, 
                ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
            
            ImGui::Dummy(ImVec2(0, 5));
            ImGui::Text("Teammate Color:");
            ImGui::ColorEdit4("##TeamColor", config::esp::team_color, 
                ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
            
            ImGui::Dummy(ImVec2(0, 5));
            ImGui::Text("Skeleton Color:");
            ImGui::ColorEdit4("##SkeletonColor", config::esp::skeleton_color, 
                ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        });

        ImGui::Columns(1);
    }

    void RenderAimbotTab() {
        // Two-column layout similar to screenshot
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, 300);

        // LEFT COLUMN - Aimbot Settings
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        ImGui::BeginChild("##AimbotLeft", ImVec2(0, 400), true);
        
        ImGui::Indent(10);
        ImGui::Dummy(ImVec2(0, 5));
        
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Aimbot");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 8));

        ImGui::Checkbox("Enable", &config::aimbot::enabled);
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("Auto-fire");
        ImGui::Checkbox("##AutoFire", &config::aimbot::auto_shoot);
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("Auto-stop");
        ImGui::Checkbox("##AutoStop", &config::aimbot::team_check);
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("Auto-scope");
        bool auto_scope = false;
        ImGui::Checkbox("##AutoScope", &auto_scope);
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Checkbox("Disable aimbot while flashed", &config::aimbot::visible_check);
        ImGui::Checkbox("Disable aimbot through smoke", &config::aimbot::team_check);
        ImGui::Dummy(ImVec2(0, 10));

        ImGui::Text("Switch target delay");
        ImGui::PushItemWidth(230);
        float switch_delay = 0.100f;
        ImGui::SliderFloat("##SwitchDelay", &switch_delay, 0.0f, 1.0f, "%.3f");
        ImGui::PopItemWidth();
        ImGui::Dummy(ImVec2(0, 10));

        ImGui::Text("Multipoint");
        ImGui::Dummy(ImVec2(0, 5));
        const char* multipoint_items[] = { "Medium" };
        static int multipoint_current = 0;
        ImGui::PushItemWidth(230);
        ImGui::Combo("##Multipoint", &multipoint_current, multipoint_items, IM_ARRAYSIZE(multipoint_items));
        ImGui::PopItemWidth();
        ImGui::Dummy(ImVec2(0, 10));

        ImGui::Text("Mouse lock percentage x");
        ImGui::PushItemWidth(230);
        float mouse_lock_x = 1.000f;
        ImGui::SliderFloat("##MouseLockX", &mouse_lock_x, 0.0f, 2.0f, "%.3f");
        ImGui::PopItemWidth();
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("Mouse lock percentage y");
        ImGui::PushItemWidth(230);
        float mouse_lock_y = 1.000f;
        ImGui::SliderFloat("##MouseLockY", &mouse_lock_y, 0.0f, 2.0f, "%.3f");
        ImGui::PopItemWidth();

        ImGui::Unindent(10);
        ImGui::Dummy(ImVec2(0, 5));
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::NextColumn();

        // RIGHT COLUMN - Recoil/Triggerbot
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        ImGui::BeginChild("##TriggerRight", ImVec2(0, 200), true);
        
        ImGui::Indent(10);
        ImGui::Dummy(ImVec2(0, 5));
        
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Triggerbot");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 8));

        ImGui::Checkbox("Enable##Trigger", &config::misc::trigger_bot);
        ImGui::Dummy(ImVec2(0, 10));

        ImGui::Checkbox("Disable triggerbot while flashed", &config::misc::no_flash);
        ImGui::Checkbox("Disable triggerbot through smoke", &config::misc::radar_hack);
        ImGui::Dummy(ImVec2(0, 10));

        ImGui::Unindent(10);
        ImGui::Dummy(ImVec2(0, 5));
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::Dummy(ImVec2(0, 10));

        // Recoil section
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        ImGui::BeginChild("##RecoilRight", ImVec2(0, 190), true);
        
        ImGui::Indent(10);
        ImGui::Dummy(ImVec2(0, 5));
        
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Recoil");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 8));

        ImGui::Checkbox("Enable##Recoil", &config::rcs::enabled);
        ImGui::Dummy(ImVec2(0, 10));

        ImGui::Text("Return crosshair");
        bool return_crosshair = false;
        ImGui::Checkbox("##ReturnCrosshair", &return_crosshair);
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("Start bullet");
        int start_bullet = 1;
        ImGui::PushItemWidth(230);
        ImGui::SliderInt("##StartBullet", &start_bullet, 1, 30);
        ImGui::PopItemWidth();
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("X Axis");
        ImGui::PushItemWidth(230);
        float x_axis = 0.60f;
        ImGui::SliderFloat("##XAxis", &x_axis, 0.0f, 1.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("Y Axis");
        ImGui::PushItemWidth(230);
        float y_axis = 0.60f;
        ImGui::SliderFloat("##YAxis", &y_axis, 0.0f, 1.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::Text("Smooth");
        ImGui::PushItemWidth(230);
        ImGui::SliderFloat("##Smooth", &config::rcs::strength, 0.0f, 2.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::Unindent(10);
        ImGui::Dummy(ImVec2(0, 5));
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::Columns(1);
    }

    // Reusable weapon category skin selector
    static void RenderWeaponCategory(const char* id, const int* weapon_ids, int weapon_count, CategoryState& state) {
        if (!skins::IsDatabaseLoaded()) {
            ImGui::TextDisabled("Skin database not loaded");
            if (ImGui::Button("Load Skins##load_db")) {
                if (!db_loading) {
                    db_loading = true;
                    skins::LoadSkinsFromAPI();
                    db_loaded = skins::IsDatabaseLoaded();
                    db_loading = false;
                }
            }
            return;
        }

        ImGui::Columns(3, id, true);
        ImGui::SetColumnWidth(0, 140);
        ImGui::SetColumnWidth(1, 220);

        // Column 1 — Weapon list
        ImGui::BeginChild((std::string("WeaponList##") + id).c_str(), ImVec2(0, 0), true);
        for (int i = 0; i < weapon_count; i++) {
            const char* name = skins::GetWeaponName(weapon_ids[i]);
            bool has_skin = skins::user_skins.find(weapon_ids[i]) != skins::user_skins.end();

            if (has_skin)
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 1.0f, 0.3f, 1.0f));

            if (ImGui::Selectable(name, state.selected_weapon_idx == i)) {
                state.selected_weapon_idx = i;
                state.selected_skin_idx = -1;
                state.search[0] = '\0';
            }

            if (has_skin)
                ImGui::PopStyleColor();
        }
        ImGui::EndChild();

        ImGui::NextColumn();

        // Column 2 — Skin list for selected weapon
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
            if (state.search[0] != '\0') {
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

                // Write initial config
                auto it = skins::user_skins.find(sel_wep);
                if (it == skins::user_skins.end()) {
                    skins::PlayerSkinConfig cfg;
                    cfg.weapon_id = sel_wep;
                    cfg.paint_kit = s.paint_kit;
                    cfg.wear = 0.01f;
                    skins::user_skins[sel_wep] = cfg;
                } else {
                    it->second.paint_kit = s.paint_kit;
                }
            }
            ImGui::PopStyleColor();

            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("%s", skins::GetRarityName(s.rarity));
        }

        if (available.empty())
            ImGui::TextDisabled("No skins found");

        ImGui::EndChild();

        ImGui::NextColumn();

        // Column 3 — Config panel
        ImGui::BeginChild((std::string("Config##") + id).c_str(), ImVec2(0, 0), true);
        auto cfg_it = skins::user_skins.find(sel_wep);
        if (cfg_it != skins::user_skins.end()) {
            skins::PlayerSkinConfig& cfg = cfg_it->second;
            ImGui::Text("Configuration");
            ImGui::Separator();

            // Find skin name
            for (const auto& s : skins::skin_database) {
                if (s.weapon_id == sel_wep && s.paint_kit == cfg.paint_kit) {
                    const float* c = skins::GetRarityColor(s.rarity);
                    ImGui::TextColored(ImVec4(c[0], c[1], c[2], c[3]), "%s", s.name.c_str());
                    ImGui::Text("Kit: %d | %s", s.paint_kit, skins::GetRarityName(s.rarity));
                    break;
                }
            }
            ImGui::Separator();

            ImGui::SliderInt("Seed", &cfg.seed, 0, 1000);
            ImGui::SameLine();
            if (ImGui::SmallButton("Rnd")) cfg.seed = rand() % 1000;

            ImGui::SliderFloat("Wear", &cfg.wear, 0.0f, 1.0f, "%.4f");
            if (ImGui::SmallButton("FN")) cfg.wear = 0.01f;
            ImGui::SameLine();
            if (ImGui::SmallButton("MW")) cfg.wear = 0.08f;
            ImGui::SameLine();
            if (ImGui::SmallButton("FT")) cfg.wear = 0.38f;
            ImGui::SameLine();
            if (ImGui::SmallButton("WW")) cfg.wear = 0.45f;
            ImGui::SameLine();
            if (ImGui::SmallButton("BS")) cfg.wear = 0.80f;

            ImGui::Separator();
            ImGui::Checkbox("StatTrak", &cfg.stattrak);
            if (cfg.stattrak)
                ImGui::SliderInt("Kills", &cfg.stattrak_count, 0, 99999);

            ImGui::Separator();
            char nametag[32];
            strncpy_s(nametag, cfg.name_tag.c_str(), sizeof(nametag) - 1);
            if (ImGui::InputText("Name Tag", nametag, sizeof(nametag)))
                cfg.name_tag = nametag;

            ImGui::Separator();
            if (ImGui::Button("Apply Now", ImVec2(-1, 30))) {
                skins::ApplyAllSkins();
            }
            if (ImGui::Button("Remove Skin", ImVec2(-1, 0))) {
                skins::user_skins.erase(sel_wep);
                state.selected_skin_idx = -1;
            }
        } else {
            ImGui::TextDisabled("Select a skin from the list");
        }
        ImGui::EndChild();

        ImGui::Columns(1);
    }

    void RenderSkinTab() {
        ImGui::BeginChild("Skin Tab", ImVec2(0, 0), false);

        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Weapon Skins & Knife");
        ImGui::Separator();

        ImGui::Checkbox("Enable Skin Changer##main", &config::skin_changer::enabled);
        ImGui::SameLine();
        if (!db_loaded && !db_loading) {
            if (ImGui::Button("Load Skin Database")) {
                db_loading = true;
                skins::LoadSkinsFromAPI();
                db_loaded = skins::IsDatabaseLoaded();
                db_loading = false;
                debug_console::Console::Get().Success("[MENU] Skin database loaded: %zu skins", skins::skin_database.size());
            }
        } else if (db_loading) {
            ImGui::TextDisabled("Loading...");
        } else {
            ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "(%zu skins loaded)", skins::skin_database.size());
        }
        ImGui::Separator();

        if (ImGui::BeginTabBar("SkinCategories")) {
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
                RenderKnifeTab();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Gloves")) {
                ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Glove Selection");
                ImGui::Separator();

                ImGui::BeginChild("GloveList", ImVec2(0, 250), true);
                for (int i = 0; i < 6; i++) {
                    bool is_selected = (selected_glove == i);
                    std::string glove_label = std::string(glove_names[i]) + " (ID: " + std::to_string(glove_ids[i]) + ")";

                    if (ImGui::Selectable(glove_label.c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick)) {
                        selected_glove = i;
                        skins::selected_glove_kit = glove_ids[i];
                        if (ImGui::IsMouseDoubleClicked(0)) {
                            skins::ApplyGloves();
                        }
                    }
                }
                ImGui::EndChild();

                ImGui::Separator();
                ImGui::Text("Selected: %s (Kit: %d)", glove_names[selected_glove], glove_ids[selected_glove]);

                static float glove_wear = 0.01f;
                ImGui::SliderFloat("Wear##glove", &glove_wear, 0.0f, 1.0f, "%.4f");

                if (ImGui::Button("Apply Gloves", ImVec2(150, 35))) {
                    skins::selected_glove_kit = glove_ids[selected_glove];
                    skins::ApplyGloves();
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Debug")) {
                if (!game_state::IsInGame()) {
                    ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Not in game - no weapon data available");
                    ImGui::EndTabItem();
                } else {
                    auto weapons = skins::GetCurrentWeaponsDebugInfo();

                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Live Weapon Inventory (%zu weapons)", weapons.size());
                    ImGui::Text("Configured skins: %zu | Knife: %s | Glove kit: %d",
                        skins::user_skins.size(),
                        skins::GetWeaponName(skins::selected_knife_id),
                        skins::selected_glove_kit);
                    ImGui::TextColored(skins::IsSetModelAvailable()
                        ? ImVec4(0.3f, 1.0f, 0.3f, 1.0f)
                        : ImVec4(1.0f, 0.4f, 0.4f, 1.0f),
                        "SetModel: %s", skins::IsSetModelAvailable() ? "FOUND" : "NOT FOUND");
                    int loadout_count = skins::GetLoadoutItemCount();
                    ImGui::TextColored(loadout_count > 0
                        ? ImVec4(0.3f, 1.0f, 0.3f, 1.0f)
                        : ImVec4(1.0f, 0.7f, 0.2f, 1.0f),
                        "Inventory Loadout: %d items%s", loadout_count,
                        loadout_count > 0 ? " (active)" : " (using fallback)");
                    ImGui::Separator();

                    if (weapons.empty()) {
                        ImGui::TextDisabled("No weapons detected");
                    } else {
                        if (ImGui::BeginTable("##WeaponDebug", 7,
                            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                            ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp,
                            ImVec2(0, 200))) {

                            ImGui::TableSetupScrollFreeze(0, 1);
                            ImGui::TableSetupColumn("Weapon", ImGuiTableColumnFlags_None, 120.0f);
                            ImGui::TableSetupColumn("DefIdx", ImGuiTableColumnFlags_None, 45.0f);
                            ImGui::TableSetupColumn("Paint Kit", ImGuiTableColumnFlags_None, 60.0f);
                            ImGui::TableSetupColumn("Seed", ImGuiTableColumnFlags_None, 40.0f);
                            ImGui::TableSetupColumn("Wear", ImGuiTableColumnFlags_None, 65.0f);
                            ImGui::TableSetupColumn("StatTrak", ImGuiTableColumnFlags_None, 55.0f);
                            ImGui::TableSetupColumn("Flags", ImGuiTableColumnFlags_None, 110.0f);
                            ImGui::TableHeadersRow();

                            for (const auto& w : weapons) {
                                ImGui::TableNextRow();

                                ImGui::TableNextColumn();
                                const char* name = skins::GetWeaponName(w.def_index);
                                if (w.is_active)
                                    ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "%s *", name);
                                else
                                    ImGui::Text("%s", name);

                                ImGui::TableNextColumn();
                                ImGui::Text("%d", w.def_index);

                                ImGui::TableNextColumn();
                                if (w.fallback_paint_kit > 0)
                                    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "%d", w.fallback_paint_kit);
                                else
                                    ImGui::TextDisabled("0");

                                ImGui::TableNextColumn();
                                ImGui::Text("%d", w.fallback_seed);

                                ImGui::TableNextColumn();
                                if (w.fallback_wear <= 0.07f)
                                    ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.2f, 1.0f), "%.4f", w.fallback_wear);
                                else if (w.fallback_wear <= 0.15f)
                                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 0.4f, 1.0f), "%.4f", w.fallback_wear);
                                else if (w.fallback_wear <= 0.38f)
                                    ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.3f, 1.0f), "%.4f", w.fallback_wear);
                                else if (w.fallback_wear <= 0.45f)
                                    ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.2f, 1.0f), "%.4f", w.fallback_wear);
                                else
                                    ImGui::TextColored(ImVec4(0.9f, 0.3f, 0.3f, 1.0f), "%.4f", w.fallback_wear);

                                ImGui::TableNextColumn();
                                if (w.fallback_stattrak >= 0)
                                    ImGui::TextColored(ImVec4(0.9f, 0.5f, 0.1f, 1.0f), "%d", w.fallback_stattrak);
                                else
                                    ImGui::TextDisabled("OFF");

                                ImGui::TableNextColumn();
                                ImGui::Text("SOC:%s Mat:%s Q:%d",
                                    w.disallow_soc ? "Y" : "N",
                                    w.restore_material ? "Y" : "N",
                                    w.entity_quality);
                            }
                            ImGui::EndTable();
                        }

                        ImGui::Separator();

                        // Detailed per-weapon expandable view
                        for (size_t i = 0; i < weapons.size(); i++) {
                            const auto& w = weapons[i];
                            const char* wname = skins::GetWeaponName(w.def_index);

                            char header[128];
                            sprintf_s(header, "%s%s [%d]##adv_detail_%zu", wname,
                                w.is_active ? " (ACTIVE)" : "", w.def_index, i);

                            if (ImGui::TreeNode(header)) {
                                ImGui::Text("Address:        0x%p", (void*)w.address);
                                ImGui::Text("Entity Quality: %d (%s)",
                                    w.entity_quality,
                                    w.entity_quality == 3 ? "Knife" :
                                    w.entity_quality == 9 ? "StatTrak" :
                                    w.entity_quality == 4 ? "Unique" : "Other");
                                ImGui::Text("Item ID High:   %d%s", w.item_id_high,
                                    w.item_id_high == -1 ? " (FALLBACK)" : "");
                                ImGui::Text("Item ID (full): %llu", w.item_id);
                                ImGui::Text("Account ID:     %u", w.account_id);
                                ImGui::Text("Owner XUID Low: %u", w.owner_xuid_low);
                                ImGui::TextColored(w.loadout_matched
                                    ? ImVec4(0.3f, 1.0f, 0.3f, 1.0f)
                                    : ImVec4(1.0f, 0.7f, 0.2f, 1.0f),
                                    "Loadout:        %s", w.loadout_matched ? "INVENTORY" : "FALLBACK");
                                ImGui::Text("DisallowSOC:    %s", w.disallow_soc ? "TRUE" : "FALSE");
                                ImGui::Text("RestoreMat:     %s", w.restore_material ? "TRUE" : "FALSE");
                                ImGui::Text("SubclassID:     %u (0x%08X)", w.subclass_id, w.subclass_id);
                                if (w.custom_name[0] != '\0')
                                    ImGui::Text("Name Tag:       \"%s\"", w.custom_name);

                                auto cfg_it = skins::user_skins.find(w.def_index);
                                if (cfg_it != skins::user_skins.end()) {
                                    ImGui::Separator();
                                    ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "Configured:");
                                    ImGui::Text("  Kit: %d %s", cfg_it->second.paint_kit,
                                        cfg_it->second.paint_kit == w.fallback_paint_kit ? "(MATCH)" : "(MISMATCH!)");
                                    ImGui::Text("  Seed: %d %s", cfg_it->second.seed,
                                        cfg_it->second.seed == w.fallback_seed ? "(MATCH)" : "(MISMATCH!)");
                                    ImGui::Text("  Wear: %.4f %s", cfg_it->second.wear,
                                        (cfg_it->second.wear == w.fallback_wear) ? "(MATCH)" : "(MISMATCH!)");
                                } else {
                                    ImGui::Separator();
                                    ImGui::TextDisabled("No skin configured");
                                }
                                ImGui::TreePop();
                            }
                        }
                    }
                    ImGui::EndTabItem();
                }
            }

            ImGui::EndTabBar();
        }

        ImGui::EndChild();
    }

    void RenderUtilitiesTab() {
        auto RenderCard = [](const char* title, auto content_func, ImVec4 title_color = ImVec4(0.5f, 1.0f, 0.5f, 1.0f)) {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.15f, 0.8f));
            ImGui::BeginChild(title, ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
            
            ImGui::PushStyleColor(ImGuiCol_Text, title_color);
            ImGui::Text("%s", title);
            ImGui::PopStyleColor();
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 5));
            
            content_func();
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        };

        // Two-column layout
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, 420);

        // Left Column - Movement & Vision
        RenderCard("MOVEMENT & VISION", []() {
            ImGui::Indent(10);
            
            ImGui::Checkbox("Bunny Hop", &config::misc::bunny_hop);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Auto-jump");
            
            ImGui::Checkbox("No Flash", &config::misc::no_flash);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Remove flashbang");
            
            ImGui::Checkbox("Radar Hack", &config::misc::radar_hack);
            ImGui::SameLine(200);
            ImGui::TextDisabled("Show all on radar");
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        });

        ImGui::NextColumn();

        // Right Column - Combat
        RenderCard("COMBAT ASSIST", []() {
            ImGui::Indent(10);
            
            ImGui::Checkbox("Trigger Bot", &config::misc::trigger_bot);
            
            if (config::misc::trigger_bot) {
                ImGui::Dummy(ImVec2(0, 5));
                ImGui::Indent(20);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
                ImGui::Text("Trigger Key: Left Shift");
                ImGui::PopStyleColor();
                
                ImGui::Dummy(ImVec2(0, 5));
                ImGui::Text("Delay:");
                ImGui::PushItemWidth(230);
                ImGui::SliderFloat("##TriggerDelay", &config::misc::trigger_delay, 0.0f, 0.5f, "%.2fs");
                ImGui::PopItemWidth();
                ImGui::Unindent(20);
            }
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        }, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));

        ImGui::Columns(1);
    }

    void RenderSettingsTab() {
        auto RenderCard = [](const char* title, auto content_func, ImVec4 title_color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f)) {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.15f, 0.8f));
            ImGui::BeginChild(title, ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
            
            ImGui::PushStyleColor(ImGuiCol_Text, title_color);
            ImGui::Text("%s", title);
            ImGui::PopStyleColor();
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 5));
            
            content_func();
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        };

        // Info Banner — monochrome silver
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.13f, 0.95f));
        ImGui::BeginChild("##InfoBanner", ImVec2(0, 90), true);
        ImGui::Dummy(ImVec2(0, 15));
        ImGui::Indent(20);

        {
            ImGuiIO& io = ImGui::GetIO();
            bool has_bold = io.Fonts->Fonts.Size > 1 && io.Fonts->Fonts[1];
            if (has_bold) ImGui::PushFont(io.Fonts->Fonts[1]);
            ImGui::TextColored(ImVec4(0.85f, 0.85f, 0.88f, 1.0f), "NEPHILIMGATE");
            if (has_bold) ImGui::PopFont();
        }

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.45f, 0.45f, 0.48f, 1.0f));
        ImGui::Text("v2.0 | Settings & Configuration");
        ImGui::PopStyleColor();

        ImGui::Unindent(20);
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::Dummy(ImVec2(0, 10));

        // Two-column layout
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, 420);

        // Left Column
        RenderCard("CONTROLS", []() {
            ImGui::Indent(10);
            
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.8f, 1.0f), "[INSERT]");
            ImGui::SameLine(120);
            ImGui::Text("Toggle Menu");
            
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "[END]");
            ImGui::SameLine(120);
            ImGui::Text("Unload Cheat");
            
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "[MOUSE5]");
            ImGui::SameLine(120);
            ImGui::Text("Pause Aimbot (hold)");
            
            ImGui::TextColored(ImVec4(0.8f, 0.6f, 1.0f, 1.0f), "[L-SHIFT]");
            ImGui::SameLine(120);
            ImGui::Text("Trigger Bot");
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        }, ImVec4(0.6f, 0.8f, 1.0f, 1.0f));

        ImGui::Dummy(ImVec2(0, 10));

        RenderCard("SYSTEM STATUS", []() {
            ImGui::Indent(10);
            
            ImGui::Text("Status:");
            ImGui::SameLine(120);
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "RUNNING");
            
            ImGui::Text("FPS:");
            ImGui::SameLine(120);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), "%.0f", ImGui::GetIO().Framerate);
            
            ImGui::Text("Render:");
            ImGui::SameLine(120);
            ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "DX11");
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        }, ImVec4(0.4f, 1.0f, 0.4f, 1.0f));

        ImGui::NextColumn();

        // Right Column
        RenderCard("QUICK ACTIONS", []() {
            ImGui::Indent(10);
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 0.8f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
            
            if (ImGui::Button("Reset All Settings", ImVec2(340, 40))) {
                config::esp::enabled = false;
                config::aimbot::enabled = false;
                config::misc::bunny_hop = false;
                config::skin_changer::enabled = false;
            }
            
            ImGui::PopStyleColor(3);
            
            ImGui::Dummy(ImVec2(0, 10));
            
            if (ImGui::Button("Disable All Features", ImVec2(165, 35))) {
                config::esp::enabled = false;
                config::aimbot::enabled = false;
                config::rcs::enabled = false;
                config::misc::bunny_hop = false;
                config::misc::no_flash = false;
                config::misc::radar_hack = false;
                config::misc::trigger_bot = false;
            }
            
            ImGui::SameLine();
            
            if (ImGui::Button("Enable Safe Mode", ImVec2(165, 35))) {
                config::esp::enabled = true;
                config::aimbot::enabled = false;
                config::misc::bunny_hop = true;
                config::misc::no_flash = true;
            }
            
            ImGui::Unindent(10);
            ImGui::Dummy(ImVec2(0, 5));
        }, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));

        ImGui::Columns(1);
    }

    // Current selected tab
    static int selected_tab = 0;

    void RenderMainMenu() {
        static int weapon_tab = 0; // 0 = Globals, 1 = Weapons

        // Load logo on first call
        LoadLogoTexture();

        ImGui::SetNextWindowSize(ImVec2(850, 600), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(100, 50), ImGuiCond_FirstUseEver);

        // Monochrome silver/dark theme matching logo
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.08f, 0.09f, 0.98f));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.10f, 0.10f, 0.11f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.22f, 0.22f, 0.24f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.08f, 0.08f, 0.09f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.35f, 0.35f, 0.38f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.50f, 0.50f, 0.53f, 0.9f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.14f, 0.14f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.20f, 0.20f, 0.22f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.85f, 0.85f, 0.88f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.55f, 0.55f, 0.58f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.18f, 0.18f, 0.20f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.25f, 0.25f, 0.28f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(0.12f, 0.12f, 0.13f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TabHovered, ImVec4(0.22f, 0.22f, 0.24f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TabActive, ImVec4(0.18f, 0.18f, 0.20f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.25f, 0.25f, 0.28f, 0.6f));

        if (ImGui::Begin("NEPHILIMGATE", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
            // ===================== LEFT SIDEBAR =====================
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.07f, 0.07f, 0.08f, 1.0f));
            ImGui::BeginChild("##Sidebar", ImVec2(200, 0), false, ImGuiWindowFlags_NoScrollbar);
            ImGui::PopStyleColor();

            // Logo
            if (g_logo_srv) {
                float logo_display_w = 64.0f;
                float logo_display_h = logo_display_w * ((float)g_logo_height / (float)g_logo_width);
                float sidebar_w = 200.0f;
                ImGui::Dummy(ImVec2(0, 10));
                ImGui::SetCursorPosX((sidebar_w - logo_display_w) * 0.5f);
                ImGui::Image((ImTextureID)g_logo_srv, ImVec2(logo_display_w, logo_display_h));
            } else {
                ImGui::Dummy(ImVec2(0, 10));
            }

            // Brand name — use Bold font (Fonts[1]) if available
            {
                const char* brand = "NEPHILIMGATE";
                ImGuiIO& io = ImGui::GetIO();
                bool has_bold = io.Fonts->Fonts.Size > 1 && io.Fonts->Fonts[1];
                if (has_bold) ImGui::PushFont(io.Fonts->Fonts[1]);
                float text_w = ImGui::CalcTextSize(brand).x;
                ImGui::SetCursorPosX((200.0f - text_w) * 0.5f);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.82f, 0.82f, 0.85f, 1.0f));
                ImGui::Text("%s", brand);
                ImGui::PopStyleColor();
                if (has_bold) ImGui::PopFont();
            }

            // Thin separator line
            ImGui::Dummy(ImVec2(0, 5));
            ImGui::Indent(15);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.30f, 0.30f, 0.33f, 1.0f));
            ImGui::Separator();
            ImGui::PopStyleColor();
            ImGui::Unindent(15);
            ImGui::Dummy(ImVec2(0, 8));

            // Category sections
            auto SectionLabel = [](const char* label) {
                ImGuiIO& io = ImGui::GetIO();
                bool has_medium = io.Fonts->Fonts.Size > 2 && io.Fonts->Fonts[2];
                if (has_medium) ImGui::PushFont(io.Fonts->Fonts[2]);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.40f, 0.40f, 0.43f, 1.0f));
                ImGui::Indent(20);
                ImGui::Text("%s", label);
                ImGui::Unindent(20);
                ImGui::PopStyleColor();
                if (has_medium) ImGui::PopFont();
                ImGui::Dummy(ImVec2(0, 3));
            };

            // Tab button helper — monochrome silver style
            auto TabButton = [](const char* icon, const char* label, int index, int* current_tab) -> bool {
                bool is_selected = (*current_tab == index);

                ImGui::Indent(10);

                if (is_selected) {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.16f, 0.18f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.19f, 0.19f, 0.21f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.14f, 0.14f, 0.16f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.92f, 0.92f, 0.95f, 1.0f));
                } else {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.13f, 0.13f, 0.15f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.10f, 0.10f, 0.12f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.50f, 0.53f, 1.0f));
                }

                bool clicked = ImGui::Button((std::string(icon) + " " + label).c_str(), ImVec2(180, 28));
                ImGui::PopStyleColor(4);
                ImGui::Unindent(10);

                if (clicked) *current_tab = index;
                return clicked;
            };

            SectionLabel("Combat");
            TabButton("", "Aimbot", 0, &selected_tab);
            TabButton("", "Triggerbot", 1, &selected_tab);

            ImGui::Dummy(ImVec2(0, 10));
            SectionLabel("Visuals");
            TabButton("", "Players", 2, &selected_tab);
            TabButton("", "Items", 3, &selected_tab);
            TabButton("", "View", 4, &selected_tab);
            TabButton("", "Hud", 5, &selected_tab);

            ImGui::Dummy(ImVec2(0, 10));
            SectionLabel("Misc");
            TabButton("", "Main", 6, &selected_tab);
            TabButton("", "Movement", 7, &selected_tab);

            ImGui::Dummy(ImVec2(0, 10));
            SectionLabel("Cheat");
            TabButton("", "Inventory", 8, &selected_tab);
            TabButton("", "Grenades", 9, &selected_tab);
            TabButton("", "Configs", 10, &selected_tab);

            ImGui::EndChild();

            // ===================== MAIN CONTENT AREA =====================
            ImGui::SameLine();
            ImGui::BeginChild("##MainContent", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);

            // Top bar
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.06f, 0.06f, 0.07f, 1.0f));
            ImGui::BeginChild("##TopBar", ImVec2(0, 50), false, ImGuiWindowFlags_NoScrollbar);

            ImGui::Dummy(ImVec2(0, 8));
            ImGui::Indent(15);

            // GLOBALS and WEAPONS tabs — silver style
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 8));

            auto TopTab = [](const char* label, int idx, int* tab) {
                bool active = (*tab == idx);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, active
                    ? ImVec4(0.90f, 0.90f, 0.93f, 1.0f)
                    : ImVec4(0.40f, 0.40f, 0.43f, 1.0f));
                if (ImGui::Button(label)) *tab = idx;
                ImGui::PopStyleColor(2);
            };

            TopTab("GLOBALS", 0, &weapon_tab);
            ImGui::SameLine();
            TopTab("WEAPONS", 1, &weapon_tab);

            ImGui::PopStyleVar();

            ImGui::Unindent(15);
            ImGui::EndChild();
            ImGui::PopStyleColor();

            // Warning banner — darker silver/grey instead of red
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.18f, 0.18f, 0.20f, 0.9f));
            ImGui::BeginChild("##VACWarning", ImVec2(0, 28), false, ImGuiWindowFlags_NoScrollbar);
            ImGui::Dummy(ImVec2(0, 4));
            ImGui::Indent(15);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.70f, 0.70f, 0.73f, 1.0f));
            ImGui::Text("VAC Live is active. Use features at your own risk.");
            ImGui::PopStyleColor();
            ImGui::Unindent(15);
            ImGui::EndChild();
            ImGui::PopStyleColor();

            ImGui::Dummy(ImVec2(0, 5));

            // Main scrollable content area
            ImGui::BeginChild("##ContentScroll", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
            
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 15));

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
            }

            ImGui::PopStyleVar();
            ImGui::EndChild();

            ImGui::EndChild();

            ImGui::End();
        }

        ImGui::PopStyleColor(16);
        ImGui::PopStyleVar(5);
    }

    void RenderTriggerBotTab() {
        ImGui::Text("Triggerbot configuration");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 10));

        ImGui::Checkbox("Enable Triggerbot", &config::misc::trigger_bot);
        ImGui::Text("Trigger delay:");
        ImGui::SliderFloat("##TriggerDelay", &config::misc::trigger_delay, 0.0f, 0.5f, "%.2fs");
    }

    void RenderItemsTab() {
        ImGui::Text("Item ESP and visualization");
        ImGui::Separator();
        ImGui::Text("Weapon drops, grenades, and other items");
    }

    void RenderViewTab() {
        ImGui::Text("View settings");
        ImGui::Separator();
        ImGui::Checkbox("No Flash", &config::misc::no_flash);
        ImGui::Checkbox("Third Person (experimental)", &config::misc::radar_hack);
    }

    void RenderHudTab() {
        ImGui::Text("HUD customization");
        ImGui::Separator();
        ImGui::Text("Customize your in-game HUD elements");
    }

    void RenderMainTab() {
        ImGui::Text("Main miscellaneous settings");
        ImGui::Separator();
        ImGui::Checkbox("Bunny Hop", &config::misc::bunny_hop);
        ImGui::Checkbox("Radar Hack", &config::misc::radar_hack);
    }

    void RenderMovementTab() {
        ImGui::Text("Movement enhancements");
        ImGui::Separator();
        ImGui::Checkbox("Auto Strafe", &config::misc::bunny_hop);
    }

    void RenderGrenadesTab() {
        ImGui::Text("Grenade helper");
        ImGui::Separator();
        ImGui::Text("Grenade trajectories and lineups");
    }

    void RenderConfigsTab() {
        ImGui::Text("Configuration management");
        ImGui::Separator();
        
        if (ImGui::Button("Save Config", ImVec2(200, 30))) {
            // Save config logic
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Config", ImVec2(200, 30))) {
            // Load config logic
        }
    }

    void ApplySelectedKnife(int knife_id) {
        try {
            config::skin_changer::enabled = true;

            selected_knife = knife_id;
            last_applied_knife = knife_id;

            // Sync with skins.cpp — this is the variable KnifeChanger() reads
            skins::selected_knife_id = knife_id;

            skins::ApplyKnife();

            debug_console::Console::Get().Success("[MENU] Applied knife: %s (ID: %d)",
                skins::GetWeaponName(knife_id), knife_id);
        }
        catch (...) {
            debug_console::Console::Get().Error("[MENU] Failed to apply knife ID %d", knife_id);
        }
    }
}
