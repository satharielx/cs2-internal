#include "hooks.h"
#include "features.h"
#include "config.h"
#include "menu_advanced.h"
#include "skins.h"
#include "debug_console.h"
#include "error_logger.h"
#include "../external/minhook/MinHook.h"
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_win32.h"
#include "../external/imgui/imgui_impl_dx11.h"
#include <stdexcept>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace globals {
    inline bool menu_open = false;
    inline bool console_open = true; // Enable debug console on launch
    inline bool imgui_initialized = false;
    inline sdk::C_CSPlayerPawn* g_local_player = nullptr;
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

            // Aimbot runs on its own thread — see features::StartAimbotThread()

            // Misc features — run every frame
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
