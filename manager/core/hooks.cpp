#include "hooks.h"
#include "features.h"
#include "config.h"
#include "menu_advanced.h"
#include "skins.h"
#include "debug_console.h"
#include "error_logger.h"
#include "Hook_FrameStageNotify.hpp"
#include "../external/minhook/MinHook.h"
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_win32.h"
#include "../external/imgui/imgui_impl_dx11.h"
#include "../sdk/usercmd.h"
#include "../external/minhook/MinHook.h"
#include <stdexcept>
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

using CreateMoveFn = bool(__fastcall*)(void*, CUserCmd*);
inline CreateMoveFn oCreateMove = nullptr;

bool __fastcall hkCreateMove(void* ecx, CUserCmd* cmd) {
    bool ret = oCreateMove(ecx, cmd);
    if (!cmd) return ret;

    // Silent aim is handled here; normal aim runs in separate thread
    if (!config::aimbot::enabled && config::aimbot::silent_aim) {
        features::RunSilentAim(cmd);
    }

    return ret;
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
        MH_EnableHook((void*)addr);
        debug_console::Console::Get().Success("[HOOK] CreateMove hooked at 0x%llX", (uintptr_t)addr);
    }
    else {
        error_logger::ErrorLogger::Get().Log("MinHook", "Failed to create CreateMove hook", (int)status);
    }
}

HRESULT __stdcall hkPresent(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags) {
    if (!swap_chain) {
        error_logger::ErrorLogger::Get().Log("hkPresent", "Invalid swap_chain parameter", 1);
        return E_INVALIDARG;
    }

    try {
        if (!globals::imgui_initialized) {
            return hooks::oPresent(swap_chain, sync_interval, flags);
        }

        if (!interfaces::d3d11_render_target_view) {
            error_logger::ErrorLogger::Get().LogPointerError("hkPresent", reinterpret_cast<uintptr_t>(interfaces::d3d11_render_target_view));
            return hooks::oPresent(swap_chain, sync_interval, flags);
        }

        if (!interfaces::d3d11_device || !interfaces::d3d11_device_context) {
            error_logger::ErrorLogger::Get().Log("hkPresent", "D3D11 device or context is null", 1);
            return hooks::oPresent(swap_chain, sync_interval, flags);
        }

        try {
            interfaces::d3d11_device_context->OMSetRenderTargets(1, &interfaces::d3d11_render_target_view, nullptr);
        }
        catch (const std::exception& ex) {
            error_logger::ErrorLogger::Get().LogException("OMSetRenderTargets", ex);
            return hooks::oPresent(swap_chain, sync_interval, flags);
        }
        catch (...) {
            error_logger::ErrorLogger::Get().Log("OMSetRenderTargets", "Failed to set render targets", 1);
            return hooks::oPresent(swap_chain, sync_interval, flags);
        }

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

            // Aimbot runs on its own thread � see features::StartAimbotThread()

            // Misc features � run every frame
			try { features::StartAimbotThread(); } catch (...) {}
            try { features::TriggerBot(); } catch (...) {}
            try { features::BunnyHop(); } catch (...) {}
            try { features::NoFlash(); } catch (...) {}
            try { features::RadarHack(); } catch (...) {}

            if (config::skin_changer::enabled) {
                try {
                    skins::ApplyAllSkins();
                }
                catch (...) {}
            }

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

    return hooks::oPresent(swap_chain, sync_interval, flags);
}

HRESULT __stdcall hkResizeBuffers(IDXGISwapChain* swap_chain, UINT buffer_count,
    UINT width, UINT height, DXGI_FORMAT format, UINT flags) {
    try {
        interfaces::destroy_render_target();
        
        if (globals::imgui_initialized && ImGui::GetCurrentContext()) {
            try {
                ImGui_ImplDX11_InvalidateDeviceObjects();
            }
            catch (const std::exception& ex) {
                error_logger::ErrorLogger::Get().LogException("InvalidateDeviceObjects", ex);
            }
            catch (...) {
                error_logger::ErrorLogger::Get().Log("InvalidateDeviceObjects", "Unknown error", 1);
            }
        }
        
        HRESULT hr = hooks::oResizeBuffers(swap_chain, buffer_count, width, height, format, flags);
        if (SUCCEEDED(hr)) {
            try {
                interfaces::create_render_target();
                
                if (globals::imgui_initialized && ImGui::GetCurrentContext()) {
                    try {
                        ImGui_ImplDX11_CreateDeviceObjects();
                    }
                    catch (const std::exception& ex) {
                        error_logger::ErrorLogger::Get().LogException("CreateDeviceObjects", ex);
                    }
                    catch (...) {
                        error_logger::ErrorLogger::Get().Log("CreateDeviceObjects", "Unknown error", 1);
                    }
                }
            }
            catch (const std::exception& ex) {
                error_logger::ErrorLogger::Get().LogException("create_render_target", ex);
            }
            catch (...) {
                error_logger::ErrorLogger::Get().Log("create_render_target", "Unknown error", 1);
            }
        }
        else {
            error_logger::ErrorLogger::Get().LogHResult("ResizeBuffers", hr);
        }
        
        return hr;
    }
    catch (const std::exception& ex) {
        error_logger::ErrorLogger::Get().LogException("hkResizeBuffers", ex);
        return hooks::oResizeBuffers(swap_chain, buffer_count, width, height, format, flags);
    }
    catch (...) {
        error_logger::ErrorLogger::Get().Log("hkResizeBuffers", "Unknown exception", 1);
        return hooks::oResizeBuffers(swap_chain, buffer_count, width, height, format, flags);
    }
}

static LRESULT __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    // Check if ImGui wants to capture input
    if (globals::imgui_initialized && (globals::menu_open || globals::console_open)) {
        // Let ImGui handle the input first
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
            // ImGui has handled this input, don't pass it to the game
            return true;
        }
    }
    
    // Handle key toggle events even when menu is closed
    if (msg == WM_KEYDOWN) {
        if (wparam == VK_INSERT) {
            globals::menu_open = !globals::menu_open;
            return 0;
        }
        if (wparam == VK_F1) {
            globals::console_open = !globals::console_open;
            return 0;
        }
    }
    
    return CallWindowProcA(hooks::oWndProc, hwnd, msg, wparam, lparam);
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
    // - Knife changer registered to execute only at frame stage 7
    // - Stage 7 is POST_RENDER_END - skeleton valid, entity won't be deleted
    //
    // Knife changer workflow:
    // 1. FrameStageNotify(7) fires - game engine post-render phase
    // 2. Callback dispatcher invokes registered callbacks
    // 3. ApplyKnifeSkins executes: writes def_index/subclass_hash/paint_kit
    // 4. Calls UpdateSubclass → UpdateComposite → SetModel in sequence
    // 5. 3D model updates on next render (scene node refreshed)
    //
    // Performance: No spam (callbacks execute only at correct stage)
    // Safety: Stage 7 guaranteed safe for skeleton operations
    // Architecture: Event-driven matches Andromeda infrastructure pattern
    void create() {
        if (!interfaces::swap_chain_dx11 || !interfaces::swap_chain_dx11->swap_chain) {
            error_logger::ErrorLogger::Get().Log("hooks::create", "SwapChain not available", 1);
            throw std::runtime_error("SwapChain not available");
        }
        if (!interfaces::hwnd) {
            error_logger::ErrorLogger::Get().Log("hooks::create", "HWND not available", 1);
            throw std::runtime_error("HWND not available");
        }

        MH_STATUS status = MH_Initialize();
        if (status != MH_OK) {
            error_logger::ErrorLogger::Get().Log("MinHook", "Initialization failed", static_cast<int>(status));
            throw std::runtime_error("MinHook init failed");
        }
        
        error_logger::ErrorLogger::Get().Log("MinHook", "Initialized successfully", 0);

        try {
            // Create Present hook
            void* present_target = sdk::virtual_function_get<void*, 8>(interfaces::swap_chain_dx11->swap_chain);
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
            void* resize_target = sdk::virtual_function_get<void*, 13>(interfaces::swap_chain_dx11->swap_chain);
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

            // FrameStageNotify hook — offset-based address resolution from client.dll
            
            void* client_base = reinterpret_cast<void*>(GetModuleHandleA("client.dll"));
            if (client_base) {
                // FrameStageNotify offset from cs2-dumper (function at client.dll + offset)
                
                void* framestagnotify_addr = reinterpret_cast<void*>(
                    reinterpret_cast<uintptr_t>(client_base) + 0x0 // Offset placeholder — use pattern scan as fallback
                );

                // Fallback to pattern scan if offset method fails
                if (!framestagnotify_addr || framestagnotify_addr == client_base) {
                    if (interfaces::client) {
                        framestagnotify_addr = interfaces::client;
                        debug_console::Console::Get().Info("[KNIFE] Using pattern-scanned FrameStageNotify address");
                    }
                }

                if (framestagnotify_addr && framestagnotify_addr != client_base) {
                    status = MH_CreateHook(framestagnotify_addr, &Hook_FrameStageNotify, reinterpret_cast<LPVOID*>(&FrameStageNotify_o));
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
                        // 7: POST_RENDER_END (optimal for model changes - skeleton valid, entity safe)
                        // 11: FRAME_END (cleanup)
                        //
                        // Knife changer only executes at stage 7 (not stage 5 like legacy systems)
                        // Stage 7 guarantee: m_pSkeletonInstance is valid, entity won't be deleted
                        RegisterOnFrameStageNotify([](int stage) {
                            // [CRITICAL] Update thread-local frame stage for ApplyKnifeSkins state machine
                            // ApplyKnifeSkins checks this to ensure execution only at stage 7
                            // Prevents skeleton NULL corruption from SetModel at wrong stage
                            skins::SetCurrentFrameStage(stage);

                            // [CRITICAL] Only call knife changer at stage 7 (POST_RENDER_END)
                            // Config check: skin_changer::enabled (from user settings)
                            // Stage check: stage == 7 (safety mechanism)
                            // If either fails, callback silently exits (no log spam)
                            if (config::skin_changer::enabled && stage == 7) {
                                try {
                                    // Execute knife model update sequence
                                    // Function modifies entity memory: def_index, subclass_hash, item IDs, paint kit
                                    // Calls game engine vtable functions: UpdateSubclass, SetModel, UpdateComposite
                                    skins::ApplyKnifeSkins();
                                }
                                catch (const std::exception& e) {
                                    // Log exception message for debugging (only on crash)
                                    debug_console::Console::Get().Error("[!][KNIFE] ApplyKnifeSkins exception: %s", e.what());
                                }
                                catch (...) {
                                    // Unknown exception (memory corruption, nullptr, etc)
                                    debug_console::Console::Get().Error("[!][KNIFE] ApplyKnifeSkins unknown exception");
                                }
                            }
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
            // Create WndProc hook
            oWndProc = (WNDPROC)SetWindowLongPtrA(interfaces::hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
            if (!oWndProc) {
                error_logger::ErrorLogger::Get().LogWin32Error("SetWindowLongPtr", GetLastError());
                throw std::runtime_error("Hook WndProc failed");
            }
            error_logger::ErrorLogger::Get().Log("WndProc", "Hook created successfully", 0);

            // Init ImGui before enabling hooks so hkPresent never fires
            // before the backends are ready.
            if (!ImGui::GetCurrentContext()) {
                try {
                    ImGui::CreateContext();
                    error_logger::ErrorLogger::Get().Log("ImGui", "Context created", 0);
                }
                catch (const std::exception& ex) {
                    error_logger::ErrorLogger::Get().LogException("ImGui::CreateContext", ex);
                    throw;
                }
            }

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

            if (!ImGui_ImplWin32_Init(interfaces::hwnd)) {
                error_logger::ErrorLogger::Get().Log("ImGui", "Win32 backend initialization failed", 1);
                throw std::runtime_error("ImGui Win32 init failed");
            }
            error_logger::ErrorLogger::Get().Log("ImGui", "Win32 backend initialized", 0);
            
            if (!ImGui_ImplDX11_Init(interfaces::d3d11_device, interfaces::d3d11_device_context)) {
                error_logger::ErrorLogger::Get().Log("ImGui", "DX11 backend initialization failed", 1);
                throw std::runtime_error("ImGui DX11 init failed");
            }
            error_logger::ErrorLogger::Get().Log("ImGui", "DX11 backend initialized", 0);
            
            ImGui::StyleColorsDark();

            // Now that ImGui is fully initialized, enable hooks.
            status = MH_EnableHook(MH_ALL_HOOKS);
            if (status != MH_OK) {
                error_logger::ErrorLogger::Get().Log("MinHook", "Failed to enable hooks", static_cast<int>(status));
                throw std::runtime_error("EnableHook failed");
            }
            error_logger::ErrorLogger::Get().Log("MinHook", "All hooks enabled successfully", 0);

            globals::imgui_initialized = true;
            error_logger::ErrorLogger::Get().Log("hooks::create", "All hooks created and initialized successfully", 0);


        }
        catch (const std::exception& ex) {
            error_logger::ErrorLogger::Get().LogException("hooks::create", ex);
            MH_Uninitialize();
            throw;
        }
        catch (...) {
            error_logger::ErrorLogger::Get().Log("hooks::create", "Unknown exception during setup", 1);
            MH_Uninitialize();
            throw;
        }
    }

    void destroy() {
        try {
            if (ImGui::GetCurrentContext()) {
                try {
                    ImGui_ImplDX11_Shutdown();
                    ImGui_ImplWin32_Shutdown();
                    ImGui::DestroyContext();
                    error_logger::ErrorLogger::Get().Log("ImGui", "Shutdown completed", 0);
                }
                catch (const std::exception& ex) {
                    error_logger::ErrorLogger::Get().LogException("ImGui Shutdown", ex);
                }
                catch (...) {
                    error_logger::ErrorLogger::Get().Log("ImGui Shutdown", "Unknown error", 1);
                }
            }
        }
        catch (...) {
            error_logger::ErrorLogger::Get().Log("ImGui Cleanup", "Unexpected error", 1);
        }
        
        try {
            MH_DisableHook(MH_ALL_HOOKS);
            MH_RemoveHook(MH_ALL_HOOKS);
            MH_Uninitialize();
            error_logger::ErrorLogger::Get().Log("MinHook", "Uninitialized", 0);
        }
        catch (...) {
            error_logger::ErrorLogger::Get().Log("MinHook Cleanup", "Error during cleanup", 1);
        }

        if (oWndProc && interfaces::hwnd) {
            try {
                SetWindowLongPtrA(interfaces::hwnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);
                oWndProc = nullptr;
                error_logger::ErrorLogger::Get().Log("WndProc", "Unhooked", 0);
            }
            catch (...) {
                error_logger::ErrorLogger::Get().Log("WndProc Unhook", "Error", 1);
            }
        }
        
        error_logger::ErrorLogger::Get().Log("hooks::destroy", "Cleanup completed", 0);
    }
}