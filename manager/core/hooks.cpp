#include "hooks.h"
#include "features.h"
#include "game_state.h"
#include "config.h"
#include "menu_advanced.h"
#include "skins.h"
#include "debug_console.h"
#include "error_logger.h"
#include "Hook_FrameStageNotify.hpp"
#include "pattern_resolver.h"
#include "../external/minhook/MinHook.h"
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_win32.h"
#include "../external/imgui/imgui_impl_dx11.h"
#include "../sdk/usercmd.h"
#include "../external/minhook/MinHook.h"
#include <stdexcept>
#include <atomic>
#include <mutex>
#include "skins.h"
#include "menu_advanced.h"
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace globals {
    inline bool menu_open = false;
    inline bool console_open = true; // Enable debug console on launch
    inline bool imgui_initialized = false;
    inline sdk::C_CSPlayerPawn* g_local_player = nullptr;
}

namespace {
    std::recursive_mutex s_render_mutex;
    std::atomic<bool> s_stopping{false};
    std::atomic<unsigned> s_active_callbacks{0};
    ULONGLONG s_next_init_attempt = 0;
    HWND s_hooked_window = nullptr;
    unsigned s_resizes_in_progress = 0;
    unsigned s_creations_in_progress = 0;
    ImGuiContext* s_imgui_context = nullptr;

    struct ImGuiContextScope {
        ImGuiContext* previous = ImGui::GetCurrentContext();
        ImGuiContext* owned_on_entry = s_imgui_context;
        ImGuiContextScope() {
            if (s_imgui_context) ImGui::SetCurrentContext(s_imgui_context);
        }
        ~ImGuiContextScope() {
            // A replacement can destroy the owned context while this scope runs.
            ImGui::SetCurrentContext(previous && previous == owned_on_entry ? s_imgui_context : previous);
        }
    };

    struct CallbackScope {
        CallbackScope() { ++s_active_callbacks; }
        ~CallbackScope() { --s_active_callbacks; }
    };

    // The bundled ImGui backend restores pipeline state but not OM bindings.
    struct RenderTargetScope {
        ID3D11DeviceContext* context;
        ID3D11RenderTargetView* targets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT]{};
        ID3D11DepthStencilView* depth = nullptr;
        explicit RenderTargetScope(ID3D11DeviceContext* ctx) : context(ctx) {
            context->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, targets, &depth);
            context->OMSetRenderTargets(1, &interfaces::d3d11_render_target_view, nullptr);
        }
        ~RenderTargetScope() {
            context->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, targets, depth);
            for (auto* target : targets) if (target) target->Release();
            if (depth) depth->Release();
        }
    };

    void shutdown_renderer();
    bool initialize_renderer(IDXGISwapChain* chain);
}
using CreateMoveFn = bool(__fastcall*)(void*, uint32_t, char);
inline CreateMoveFn oCreateMove = nullptr;

using fnSubTickAngle = __int64(__fastcall*)(DWORD* a1, void* a2, char a3, float a4, float a5, sdk::C_CSPlayerPawn* localPawn);
inline fnSubTickAngle oSubTickAngle = nullptr;

bool __fastcall hkCreateMove(void* input, uint32_t split_screen_index, char active) {
    CallbackScope callback;
    return oCreateMove(input, split_screen_index, active);
}
__int64 __fastcall hkSubTickAngle(DWORD* a1, void* a2, char a3, float a4, float a5, sdk::C_CSPlayerPawn* localPawn) {
    CallbackScope callback;
    ++features::silent_callbacks;
    struct RestoreAngles {
        DWORD* input;
        sdk::Vector2 angles{};
        bool readable;
        bool applied = false;
        explicit RestoreAngles(DWORD* p) : input(p), readable(p && sdk::read_memory(reinterpret_cast<uintptr_t>(p + 4), angles)) {}
        ~RestoreAngles() { if (applied) sdk::write_memory(reinterpret_cast<uintptr_t>(input + 4), angles); }
    } restore(a1);
    {
        std::lock_guard<std::recursive_mutex> settings_lock(config::mutex);
        if (!s_stopping && !globals::menu_open && !globals::console_open &&
            config::aimbot::enabled && config::aimbot::silent_aim && restore.readable) {
            const auto writes = features::silent_aim_writes.load();
            features::RunSilentAimSubTick(a1, game_state::GetLocalPawn());
            restore.applied = features::silent_aim_writes.load() != writes;
        }
        else features::silent_aim_status = "Disabled, menu/console open, or input unavailable";
    }
    return oSubTickAngle(a1, a2, a3, a4, a5, localPawn);
}

static void __fastcall hkFrameStageNotify(CSource2Client* client, int stage) {
    CallbackScope callback;
    if (s_stopping)
        hooks::FrameStageNotify_o(client, stage);
    else
        hooks::Hook_FrameStageNotify(client, stage);
}

void HookCreateMove() {
    uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
    if (!client) {
        error_logger::ErrorLogger::Get().Log("CreateMove", "client.dll not loaded", 1);
        return;
    }

    auto addr = sdk::find_pattern("client.dll",
        "85 D2 0F 85 ? ? ? ? 48 8B C4 44 88 40 18");
    if (!addr) {
        error_logger::ErrorLogger::Get().Log("CreateMove", "Pattern not found", 1);
        debug_console::Console::Get().Error("[HOOK] CreateMove pattern not found");
        return;
    }

    MH_STATUS status = MH_CreateHook((void*)addr, &hkCreateMove, (void**)&oCreateMove);
    if (status == MH_OK) {
        debug_console::Console::Get().Success("[HOOK] CreateMove prepared at 0x%llX", (uintptr_t)addr);
    }
    else {
        error_logger::ErrorLogger::Get().Log("MinHook", "Failed to create CreateMove hook", (int)status);
    }
}

static void render_overlay(IDXGISwapChain* swap_chain) {
    if (!swap_chain) {
        error_logger::ErrorLogger::Get().Log("hkPresent", "Invalid swap_chain parameter", 1);
        return;
    }

    try {
        ImGuiContextScope context;
        if (!initialize_renderer(swap_chain)) {
            return;
        }

        if (!interfaces::d3d11_render_target_view) {
            interfaces::create_render_target();
        }

        if (!interfaces::d3d11_device || !interfaces::d3d11_device_context) {
            error_logger::ErrorLogger::Get().Log("hkPresent", "D3D11 device or context is null", 1);
            return;
        }

        std::lock_guard<std::recursive_mutex> settings_lock(config::mutex);
        features::SetInputBlocked(globals::menu_open || globals::console_open);
        RenderTargetScope render_targets(interfaces::d3d11_device_context);

        try {
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            // Run features every frame
            if (config::esp::enabled) {
                try {
                    features::RenderESP();
                }
                catch (...) {}
            }

            // Aimbot runs on its own thread ? see features::StartAimbotThread()

            // Misc features ? run every frame
            try { features::TriggerBot(); } catch (...) {}
            try { features::BunnyHop(); } catch (...) {}
            try { features::NoFlash(); } catch (...) {}
            try { features::RadarHack(); } catch (...) {}

            // Render debug console (always render if open, above menu for input priority)
            if (globals::console_open) {
                try {
                    debug_console::Console::Get().Render();
                }
                catch (const std::exception& ex) {
                    error_logger::ErrorLogger::Get().LogException("Debug Console Render", ex);
                }
                catch (...) {
                    error_logger::ErrorLogger::Get().Log("Debug Console", "Unknown error during render", 1);
                }
            }

            // Render advanced menu
            if (globals::menu_open) {
                try {
                    menu_advanced::RenderMainMenu();
                }
                catch (const std::exception& ex) {
                    error_logger::ErrorLogger::Get().LogException("Menu Render", ex);
                }
                catch (...) {
                    error_logger::ErrorLogger::Get().Log("Menu", "Unknown error during render", 1);
                }
            }

            ImGui::EndFrame();
            ImGui::Render();
            if (!globals::menu_open && !globals::console_open) {
                ImGui::GetIO().MouseDrawCursor = false;
                ImGui::GetIO().WantCaptureMouse = false;
                ImGui::GetIO().WantCaptureKeyboard = false;
            }
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }
        catch (const std::exception& ex) {
            error_logger::ErrorLogger::Get().LogException("hkPresent Rendering", ex);
        }
        catch (...) {
            error_logger::ErrorLogger::Get().Log("hkPresent", "Unknown exception during rendering", 1);
        }
    }
    catch (const std::exception& ex) {
        error_logger::ErrorLogger::Get().LogException("hkPresent", ex);
    }
    catch (...) {
        error_logger::ErrorLogger::Get().Log("hkPresent", "Critical unknown exception", 1);
    }

    return;
}

HRESULT __stdcall hkPresent(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags) {
    CallbackScope callback;
    if (!s_stopping && swap_chain && !(flags & DXGI_PRESENT_TEST)) {
        std::lock_guard<std::recursive_mutex> lock(s_render_mutex);
        if (!s_stopping && s_resizes_in_progress == 0 && s_creations_in_progress == 0)
            render_overlay(swap_chain);
    }
    const HRESULT hr = hooks::oPresent(swap_chain, sync_interval, flags);
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
        std::lock_guard<std::recursive_mutex> lock(s_render_mutex);
        if (swap_chain == interfaces::swap_chain)
            shutdown_renderer();
    }
    return hr;
}
HRESULT __stdcall hkResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count,
    UINT width, UINT height, DXGI_FORMAT format, UINT flags) {
    CallbackScope callback;
    bool tracked_resize = false;
    {
        std::lock_guard<std::recursive_mutex> lock(s_render_mutex);
        if (!s_stopping && swap_chain && swap_chain == interfaces::swap_chain) {
            tracked_resize = true;
            ++s_resizes_in_progress;
            try {
                ImGuiContextScope context;
                interfaces::d3d11_device_context->OMSetRenderTargets(0, nullptr, nullptr);
                interfaces::destroy_render_target();
                if (globals::imgui_initialized && ImGui::GetCurrentContext())
                    ImGui_ImplDX11_InvalidateDeviceObjects();
            }
            catch (const std::exception& ex) {
                error_logger::ErrorLogger::Get().LogException("ResizeBuffers cleanup", ex);
            }
            catch (...) {
                error_logger::ErrorLogger::Get().Log("ResizeBuffers cleanup", "Unknown exception", 1);
            }
        }
    }

    // DXGI may synchronously send window messages; never hold the ImGui lock
    // across the original call. Present skips rendering while resize is active.
    const HRESULT hr = hooks::oResizeBuffers(swap_chain, buffer_count, width, height, format, flags);
    if (tracked_resize) {
        std::lock_guard<std::recursive_mutex> lock(s_render_mutex);
        --s_resizes_in_progress;
    }
    if (FAILED(hr))
        error_logger::ErrorLogger::Get().LogHResult("ResizeBuffers", hr);
    // The next Present reacquires buffer 0 even when ResizeBuffers failed and
    // DXGI retained the old buffer. No second call to the original on failure.
    return hr;
}

HRESULT __stdcall hkCreateSwapChain(IDXGIFactory* factory, IUnknown* device,
    DXGI_SWAP_CHAIN_DESC* desc, IDXGISwapChain** result) {
    CallbackScope callback;
    bool replacing_selected = false;
    {
        std::lock_guard<std::recursive_mutex> lock(s_render_mutex);
        if (!s_stopping && desc && interfaces::swap_chain && desc->OutputWindow == interfaces::hwnd) {
            replacing_selected = true;
            ++s_creations_in_progress;
            // DXGI flip-model replacement requires every old back-buffer and
            // swap-chain reference held by the overlay to be released first.
            interfaces::d3d11_device_context->OMSetRenderTargets(0, nullptr, nullptr);
            shutdown_renderer();
        }
    }
    const HRESULT hr = hooks::oCreateSwapChain(factory, device, desc, result);
    if (replacing_selected) {
        std::lock_guard<std::recursive_mutex> lock(s_render_mutex);
        --s_creations_in_progress;
        s_next_init_attempt = 0;
    }
    // Capture and initialize on Present, including recovery of the old chain
    // if creation failed. Do not inspect an unsuccessful output argument.
    return hr;
}

static LRESULT __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    CallbackScope callback;
    WNDPROC original = nullptr;
    {
        std::lock_guard<std::recursive_mutex> lock(s_render_mutex);
        original = hooks::oWndProc;
        if (!s_stopping) {
            ImGuiContextScope context;
            std::lock_guard<std::recursive_mutex> settings_lock(config::mutex);
            if (globals::imgui_initialized && (globals::menu_open || globals::console_open)) {
                if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
                    return true;
            }

            if (msg == WM_KEYUP) {
                if (wparam == VK_INSERT) {
                    globals::menu_open = !globals::menu_open;
                    features::SetInputBlocked(globals::menu_open || globals::console_open);
                    if (globals::menu_open) features::ReleaseInputs();
                    if (globals::imgui_initialized) {
                        ImGuiIO& io = ImGui::GetIO();
                        io.MouseDrawCursor = globals::menu_open;
                        if (!globals::menu_open) {
                            // Reset ImGui mouse state
                            io.MouseDown[0] = io.MouseDown[1] = io.MouseDown[2] = false;
                            io.MouseWheel = 0.0f;
                            // Release window capture
                            ReleaseCapture();
                            ClipCursor(nullptr);
                            // Force show system cursor
                            while (ShowCursor(TRUE) < 0);
                        }
                    }
                    return 0;
                }
                if (wparam == VK_F1) {
                    globals::console_open = !globals::console_open;
                    features::SetInputBlocked(globals::menu_open || globals::console_open);
                    if (globals::console_open) features::ReleaseInputs();
                    if (globals::imgui_initialized) {
                        ImGuiIO& io = ImGui::GetIO();
                        io.MouseDrawCursor = (globals::menu_open || globals::console_open);
                        if (!globals::console_open && !globals::menu_open) {
                            io.MouseDown[0] = io.MouseDown[1] = io.MouseDown[2] = false;
                            io.MouseWheel = 0.0f;
                            ReleaseCapture();
                            ClipCursor(nullptr);
                            while (ShowCursor(TRUE) < 0);
                        }
                    }
                    return 0;
                }
            }

            // Also handle WM_DESTROY or WM_CLOSE to clean up
            if (msg == WM_DESTROY || msg == WM_CLOSE) {
                if (globals::imgui_initialized) {
                    ImGui::GetIO().MouseDrawCursor = false;
                    ReleaseCapture();
                    ClipCursor(nullptr);
                }
            }

        }
    }
    return original ? CallWindowProcA(original, hwnd, msg, wparam, lparam)
                    : DefWindowProcA(hwnd, msg, wparam, lparam);
}
namespace {
    void shutdown_renderer() {
        ImGuiContextScope context;
        globals::imgui_initialized = false;
        if (hooks::oWndProc && s_hooked_window && IsWindow(s_hooked_window)) {
            SetWindowLongPtrA(s_hooked_window, GWLP_WNDPROC,
                reinterpret_cast<LONG_PTR>(hooks::oWndProc));
        }
        s_hooked_window = nullptr;
        // Keep oWndProc until callbacks already dispatched to us have drained.
        menu_advanced::ResetRendererResources();
        if (s_imgui_context) {
            if (ImGui::GetIO().BackendRendererUserData) ImGui_ImplDX11_Shutdown();
            if (ImGui::GetIO().BackendPlatformUserData) ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext(s_imgui_context);
            s_imgui_context = nullptr;
        }
        interfaces::destroy_d3d11_resources();
    }

    bool initialize_renderer(IDXGISwapChain* chain) {
        if (globals::imgui_initialized && chain == interfaces::swap_chain)
            return true;
        if (GetTickCount64() < s_next_init_attempt)
            return false;

        // Other windows may also present through Steam. Allow replacement of
        // the selected window's chain, or a new window after the old one closes.
        DXGI_SWAP_CHAIN_DESC desc{};
        if (FAILED(chain->GetDesc(&desc))) return false;
        if (interfaces::swap_chain && chain != interfaces::swap_chain &&
            IsWindow(interfaces::hwnd) && desc.OutputWindow != interfaces::hwnd) {
            return false;
        }

        try {
            shutdown_renderer();
            interfaces::create_d3d11_resources(chain);
            s_imgui_context = ImGui::CreateContext();
            if (!s_imgui_context)
                throw std::runtime_error("ImGui context creation failed");
            ImGui::SetCurrentContext(s_imgui_context);
            {
                ImGuiIO& io = ImGui::GetIO();

                // Helper to get the directory containing the DLL
                auto GetDllDir = []() -> std::string {
                    char dll_path[MAX_PATH];
                    HMODULE hm = nullptr;
                    // Get handle to this module (the cheat DLL)
                    if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                        (LPCSTR)&hkPresent, &hm))
                        return "";
                    if (!GetModuleFileNameA(hm, dll_path, MAX_PATH))
                        return "";
                    // Remove filename to get directory
                    char* last_slash = strrchr(dll_path, '\\');
                    if (last_slash) *last_slash = '\0';
                    return std::string(dll_path) + "\\";
                    };

                std::string dll_dir = GetDllDir();
                debug_console::Console::Get().Info("[FONTS] DLL directory: %s", dll_dir.c_str());

                // Search paths for fonts (in priority order)
                std::vector<std::string> search_dirs = {
                    dll_dir + "fonts\\",
                    dll_dir + "..\\fonts\\",
                    dll_dir + "core\\fonts\\",
                    "C:\\Windows\\manager\\manager\\core\\fonts\\",
                    ".\\fonts\\"
                };

                // Orbitron font file names (relative to search_dirs)
                const char* regular_file = "main_menu\\variants\\Orbitron-Regular.ttf";
                const char* bold_file = "main_menu\\variants\\Orbitron-Bold.ttf";
                const char* medium_file = "main_menu\\variants\\Orbitron-Medium.ttf";

                ImFont* f0 = nullptr; // Regular (14px)
                ImFont* f1 = nullptr; // Bold (22px)
                ImFont* f2 = nullptr; // Medium (11px)

                // Attempt to load each font from the search paths
                for (const auto& dir : search_dirs) {
                    if (f0 && f1 && f2) break; // All loaded

                    std::string reg_path = dir + regular_file;
                    std::string bold_path = dir + bold_file;
                    std::string med_path = dir + medium_file;

                    if (!f0 && GetFileAttributesA(reg_path.c_str()) != INVALID_FILE_ATTRIBUTES) {
                        f0 = io.Fonts->AddFontFromFileTTF(reg_path.c_str(), 14.0f);
                        if (f0) debug_console::Console::Get().Success("[FONTS] Orbitron Regular loaded from: %s", reg_path.c_str());
                    }
                    if (!f1 && GetFileAttributesA(bold_path.c_str()) != INVALID_FILE_ATTRIBUTES) {
                        f1 = io.Fonts->AddFontFromFileTTF(bold_path.c_str(), 22.0f);
                        if (f1) debug_console::Console::Get().Success("[FONTS] Orbitron Bold loaded from: %s", bold_path.c_str());
                    }
                    if (!f2 && GetFileAttributesA(med_path.c_str()) != INVALID_FILE_ATTRIBUTES) {
                        f2 = io.Fonts->AddFontFromFileTTF(med_path.c_str(), 11.0f);
                        if (f2) debug_console::Console::Get().Success("[FONTS] Orbitron Medium loaded from: %s", med_path.c_str());
                    }
                }

                // Fallbacks: if any font failed to load, use default ImGui font
                if (!f0) {
                    f0 = io.Fonts->AddFontDefault();
                    debug_console::Console::Get().Warning("[FONTS] Orbitron Regular not found, using default font.");
                }
                if (!f1) {
                    f1 = f0; // Use regular as fallback for bold
                    debug_console::Console::Get().Warning("[FONTS] Orbitron Bold not found, using regular.");
                }
                if (!f2) {
                    f2 = f0; // Use regular as fallback for medium
                    debug_console::Console::Get().Warning("[FONTS] Orbitron Medium not found, using regular.");
                }

                // Store the fonts in the ImGui font atlas order:
                // Fonts[0] = Regular (14px) - default body
                // Fonts[1] = Bold (22px) - headings
                // Fonts[2] = Medium (11px) - section labels
                // The order matches what the menu expects (index 0,1,2)

                error_logger::ErrorLogger::Get().Log("ImGui", "Fonts initialized (with fallbacks if needed)", 0);

                // Now load the icon font (FontAwesome) - this will be merged or standalone
                menu_advanced::LoadIconFont();
            }


            if (!ImGui_ImplWin32_Init(interfaces::hwnd))
                throw std::runtime_error("ImGui Win32 init failed");
            if (!ImGui_ImplDX11_Init(interfaces::d3d11_device, interfaces::d3d11_device_context))
                throw std::runtime_error("ImGui DX11 init failed");
            ImGui::StyleColorsDark();

            SetLastError(0);
            auto original = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(
                interfaces::hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProc)));
            if (!original)
                throw std::runtime_error("Hook WndProc failed");
            hooks::oWndProc = original;
            s_hooked_window = interfaces::hwnd;
            globals::imgui_initialized = true;
            features::SetInputBlocked(globals::menu_open || globals::console_open);
            s_next_init_attempt = 0;
            error_logger::ErrorLogger::Get().Log("D3D11", "Rendering initialized from Present", 0);
            return true;
        }
        catch (const std::exception& ex) {
            error_logger::ErrorLogger::Get().LogException("Present initialization", ex);
        }
        catch (...) {
            error_logger::ErrorLogger::Get().Log("Present initialization", "Unknown exception", 1);
        }
        shutdown_renderer();
        s_next_init_attempt = GetTickCount64() + 1000;
        return false;
    }
}
namespace hooks {
    // ==================== HOOK SYSTEM INITIALIZATION ====================
    // 
    // create() - Initialize all MinHook hooks and ImGui rendering pipeline
    //
    // Hooks created (in order):
    // 1. Present (DX11 render callback) - for ImGui rendering
    // 2. ResizeBuffers (DX11 buffer resize) - for window resize handling
    // 3. FrameStageNotify (game frame callback) - for knife changer via event dispatcher
    // 4. WndProc (window message callback) - for ImGui input handling
    //
    // FrameStageNotify is the critical hook for knife changer:
    // - Uses event-driven callback system (RegisterOnFrameStageNotify)
    // - Callback dispatcher (Hook_FrameStageNotify.cpp) invokes all registered callbacks
    //
    // Knife changer workflow:
    // 2. Callback dispatcher invokes registered callbacks
    // 3. ApplyKnifeSkins executes: writes def_index/subclass_hash/paint_kit
    // 4. Calls UpdateSubclass ? UpdateComposite ? SetModel in sequence
    // 5. 3D model updates on next render (scene node refreshed)
    //
    // Performance: No spam (callbacks execute only at correct stage)
    void create() {
        s_stopping = false;

        MH_STATUS status = MH_Initialize();
        if (status != MH_OK) {
            error_logger::ErrorLogger::Get().Log("MinHook", "Initialization failed", static_cast<int>(status));
            throw std::runtime_error("MinHook init failed");
        }

        try {
            if (!pattern_resolver::Initialize()) {
                error_logger::ErrorLogger::Get().Log("PatternResolver", "Failed to resolve critical addresses", 1);
                debug_console::Console::Get().Warning("[HOOK] Pattern resolver failed - falling back to offsets");
            }
            else {
                debug_console::Console::Get().Success("[HOOK] Pattern resolver initialized");
            }

            error_logger::ErrorLogger::Get().Log("MinHook", "Initialized successfully", 0);

            // Create Present hook
            void* present_target = sdk::find_pattern("gameoverlayrenderer64.dll",
                "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 41 8B F0");
            if (!present_target) {
                error_logger::ErrorLogger::Get().LogPointerError("Present target", reinterpret_cast<uintptr_t>(present_target));
                throw std::runtime_error("Present target is null");
            }

            status = MH_CreateHook(present_target, &hkPresent, reinterpret_cast<void**>(&oPresent));
            if (status != MH_OK) {
                error_logger::ErrorLogger::Get().Log("MinHook", "Failed to create Present hook", static_cast<int>(status));
                throw std::runtime_error("Hook Present failed");
            }
            error_logger::ErrorLogger::Get().Log("MinHook", "Present hook created successfully", 0);

            // Create ResizeBuffers hook
            void* resize_target = sdk::find_pattern("gameoverlayrenderer64.dll",
                "40 53 55 56 57 41 54 41 56 41 57 48 83 EC ? 44 8B E2");
            if (!resize_target) {
                error_logger::ErrorLogger::Get().LogPointerError("ResizeBuffers target", reinterpret_cast<uintptr_t>(resize_target));
                throw std::runtime_error("ResizeBuffers target is null");
            }

            status = MH_CreateHook(resize_target, &hkResizeBuffers, reinterpret_cast<void**>(&oResizeBuffers));
            if (status != MH_OK) {
                error_logger::ErrorLogger::Get().Log("MinHook", "Failed to create ResizeBuffers hook", static_cast<int>(status));
                throw std::runtime_error("Hook ResizeBuffers failed");
            }
            error_logger::ErrorLogger::Get().Log("MinHook", "ResizeBuffers hook created successfully", 0);

            void* create_target = sdk::find_pattern("gameoverlayrenderer64.dll",
                "40 53 55 56 57 48 83 EC ? 48 8B F9 49 8B F1 48 8D 0D ? ? ? ? 49 8B D8 48 8B EA E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B 05 ? ? ? ? 4C 8B CE 4C 8B C3 48 8B D5 48 8B CF FF D0 8B D8 85 C0 78 ? 48 85 F6 74 ? 48 83 3E ? 74 ? 48 8B D5 48 8B CE E8 ? ? ? ? 8B C3 48 83 C4 ? 5F 5E 5D 5B C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC");
            if (!create_target)
                throw std::runtime_error("Steam overlay CreateSwapChain pattern not found");
            status = MH_CreateHook(create_target, &hkCreateSwapChain, reinterpret_cast<void**>(&oCreateSwapChain));
            if (status != MH_OK)
                throw std::runtime_error("Hook CreateSwapChain failed");
            error_logger::ErrorLogger::Get().Log("MinHook", "CreateSwapChain hook created successfully", 0);

            // FrameStageNotify hook - offset-based address resolution from client.dll

            void* client_base = reinterpret_cast<void*>(GetModuleHandleA("client.dll"));
            if (client_base) {
                // FrameStageNotify offset from cs2-dumper (function at client.dll + offset)

                void* framestagnotify_addr = reinterpret_cast<void*>(
                    reinterpret_cast<uintptr_t>(client_base) + 0x0 // Offset placeholder - use pattern scan as fallback
                );

                // Fallback to pattern scan if offset method fails
                if (!framestagnotify_addr || framestagnotify_addr == client_base) {
                    if (interfaces::client) {
                        framestagnotify_addr = interfaces::client;
                        debug_console::Console::Get().Info("[KNIFE] Using pattern-scanned FrameStageNotify address");
                    }
                }

                if (framestagnotify_addr && framestagnotify_addr != client_base) {
                    status = MH_CreateHook(framestagnotify_addr, &hkFrameStageNotify, reinterpret_cast<LPVOID*>(&FrameStageNotify_o));
                    if (status == MH_OK) {
                        debug_console::Console::Get().Success("[KNIFE] FrameStageNotify hook created successfully");
                        error_logger::ErrorLogger::Get().Log("MinHook", "FrameStageNotify hook created", 0);

                        // ========== KNIFE CHANGER CALLBACK REGISTRATION ==========
                        // Subscribe to FrameStageNotify events via event-driven callback system
                        // Callback dispatcher (Hook_FrameStageNotify.cpp) invokes all registered callbacks
                        // This callback executes on game thread during frame stage transitions (0-11)
                        //
                        // Frame stage lifecycle:
                        // 0: FRAME_START (resource loading)
                        // 5: SIMULATE (animation update, bone cache build)
                        // 11: FRAME_END (cleanup)
                        //
                        RegisterOnFrameStageNotify([](int stage) {
                            std::lock_guard<std::recursive_mutex> settings_lock(config::mutex);
                            skins::SetCurrentFrameStage(stage);
                            try {
                                if (config::skin_changer::enabled && stage == 6)
                                    skins::ApplyAllSkins();
                            } catch (...) {
                                error_logger::ErrorLogger::Get().Log("Skins", "Frame-stage update failed", 1);
                            }
                            skins::SetCurrentFrameStage(-1);
                        });
                        debug_console::Console::Get().Success("[KNIFE] FrameStageNotify callback registered");
                    } else {
                        error_logger::ErrorLogger::Get().Log("MinHook", "Failed to create FrameStageNotify hook", static_cast<int>(status));
                        debug_console::Console::Get().Warning("[KNIFE] Failed to create FrameStageNotify hook");
                    }
                } else {
                    error_logger::ErrorLogger::Get().Log("FrameStageNotify", "Failed to resolve FrameStageNotify address (use pattern scan in interfaces.cpp)", 1);
                    debug_console::Console::Get().Warning("[KNIFE] Failed to resolve FrameStageNotify address");
                }
            } else {
                error_logger::ErrorLogger::Get().Log("FrameStageNotify", "Failed to get client.dll base address", 1);
                debug_console::Console::Get().Warning("[KNIFE] Failed to get client.dll base address");
            }
            HookCreateMove();

            {
                auto subtick_addr = sdk::find_pattern("client.dll", "48 89 5C 24 ? 55 57 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 8B 01 48 8B F9");
                if (subtick_addr) {
                    MH_STATUS status = MH_CreateHook(subtick_addr, &hkSubTickAngle,
                        reinterpret_cast<void**>(&oSubTickAngle));

                    if (status == MH_OK) {

                        debug_console::Console::Get().Success("[HOOK] SubTickAngle prepared at 0x%llX", (uintptr_t)subtick_addr);

                    }
                    else {
                        error_logger::ErrorLogger::Get().Log("MinHook", "Failed to create SubTickAngle hook", static_cast<int>(status));
                        debug_console::Console::Get().Error("[HOOK] SubTickAngle hook failed");
                    }
                }
                else {
                    error_logger::ErrorLogger::Get().Log("SubTickAngle", "Failed to find SubTickAngle address", 1);
                    debug_console::Console::Get().Warning("[HOOK] Failed to find SubTickAngle address");

                }
            }

            // Present will initialize D3D11, ImGui and WndProc from its COM argument.
            status = MH_EnableHook(MH_ALL_HOOKS);
            if (status != MH_OK) {
                error_logger::ErrorLogger::Get().Log("MinHook", "Failed to enable hooks", static_cast<int>(status));
                throw std::runtime_error("EnableHook failed");
            }
            error_logger::ErrorLogger::Get().Log("MinHook", "All hooks enabled successfully", 0);


            error_logger::ErrorLogger::Get().Log("hooks::create", "Hooks enabled; waiting for first Present to initialize rendering", 0);


        }
        catch (const std::exception& ex) {
            error_logger::ErrorLogger::Get().LogException("hooks::create", ex);
            destroy();
            throw;
        }
        catch (...) {
            error_logger::ErrorLogger::Get().Log("hooks::create", "Unknown exception during setup", 1);
            destroy();
            throw;
        }
    }

    void destroy() {
        s_stopping = true;
        features::SetInputBlocked(true);
        MH_DisableHook(MH_ALL_HOOKS);
        {
            std::lock_guard<std::recursive_mutex> lock(s_render_mutex);
            if (oWndProc && s_hooked_window && IsWindow(s_hooked_window)) {
                SetWindowLongPtrA(s_hooked_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(oWndProc));
            }
            s_hooked_window = nullptr;
        }
        // An in-flight callback may still be using an original trampoline.
        while (s_active_callbacks.load() != 0) Sleep(1);
        {
            std::lock_guard<std::recursive_mutex> lock(s_render_mutex);
            shutdown_renderer();
            oWndProc = nullptr;
            features::ReleaseInputs();
        }
        MH_RemoveHook(MH_ALL_HOOKS);
        MH_Uninitialize();
        error_logger::ErrorLogger::Get().Log("hooks::destroy", "Cleanup completed", 0);
    }
}
