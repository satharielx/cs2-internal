#include "interfaces.h"
#include "error_logger.h"
#include "debug_console.h"
#include "../sdk/mem.h"
#include <stdexcept>
#include <format>
#include <wrl/client.h>

namespace interfaces {
    static CGameEntitySystem* g_pGameEntitySystem = nullptr;

    CGameEntitySystem* GameEntitySystem() {
        if (!g_pGameEntitySystem) {
            // 48 8B 0D rel32 = mov rcx, [rip + rel32]
            // This is the ppCGameEntitySystem load immediately before
            // CGameEntitySystem::GetBaseEntity.
            auto* instruction = sdk::find_pattern(
                "client.dll",
                "48 8B 0D ? ? ? ? 8B D3 E8 ? ? ? ?");

            if (!instruction)
                return nullptr;

            auto* pointer_address = sdk::resolve_absolute_rip_address(
                instruction, 3, 7);

            if (!pointer_address ||
                !sdk::is_valid_ptr(reinterpret_cast<uintptr_t>(pointer_address))) {
                return nullptr;
            }

            g_pGameEntitySystem =
                *reinterpret_cast<CGameEntitySystem**>(pointer_address);
        }

        return g_pGameEntitySystem;
    }

    template <class T>
    T* capture_interface(const char* mod, const char* name) {
        HMODULE hMod = GetModuleHandleA(mod);
        if (!hMod) {
            error_logger::ErrorLogger::Get().LogInitError(mod, "Module not found");
            throw std::runtime_error(std::format("Module {} not found", mod));
        }
        auto create = reinterpret_cast<T * (*)(const char*, int*)>(GetProcAddress(hMod, "CreateInterface"));
        if (!create) {
            error_logger::ErrorLogger::Get().LogInitError(mod, "CreateInterface not found");
            throw std::runtime_error("CreateInterface not found");
        }
        T* ptr = create(name, nullptr);
        if (!ptr) {
            error_logger::ErrorLogger::Get().LogInitError(name, "Interface creation failed");
            throw std::runtime_error(std::format("Interface {} not found", name));
        }
        error_logger::ErrorLogger::Get().Log("Interface", std::format("Interface {} created successfully", name).c_str(), 0);
        return ptr;
    }

    void create_render_target() {
        if (d3d11_render_target_view) return;
        if (!swap_chain || !d3d11_device)
            throw std::runtime_error("D3D11 swap chain/device unavailable");

        Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
        HRESULT hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(back_buffer.GetAddressOf()));
        if (FAILED(hr)) {
            error_logger::ErrorLogger::Get().LogHResult("SwapChain::GetBuffer", hr);
            throw std::runtime_error("Failed to get swap-chain back buffer");
        }
        hr = d3d11_device->CreateRenderTargetView(back_buffer.Get(), nullptr, &d3d11_render_target_view);
        if (FAILED(hr)) {
            error_logger::ErrorLogger::Get().LogHResult("CreateRenderTargetView", hr);
            throw std::runtime_error("Failed to create render target");
        }
    }

    void destroy_render_target() {
        if (d3d11_render_target_view) {
            d3d11_render_target_view->Release();
            d3d11_render_target_view = nullptr;
        }
    }

    void destroy_d3d11_resources() {
        destroy_render_target();
        if (d3d11_device_context) {
            d3d11_device_context->Release();
            d3d11_device_context = nullptr;
        }
        if (d3d11_device) {
            d3d11_device->Release();
            d3d11_device = nullptr;
        }
        if (swap_chain) {
            swap_chain->Release();
            swap_chain = nullptr;
        }
        hwnd = nullptr;
    }

    void create_d3d11_resources(IDXGISwapChain* chain) {
        if (!chain)
            throw std::runtime_error("Present supplied a null swap chain");

        // These signatures locate callbacks, not a RIP-relative global or an
        // engine wrapper. Query the actual COM object received by Present.
        DXGI_SWAP_CHAIN_DESC desc{};
        HRESULT hr = chain->GetDesc(&desc);
        if (FAILED(hr)) {
            error_logger::ErrorLogger::Get().LogHResult("SwapChain::GetDesc", hr);
            throw std::runtime_error("Failed to get swap-chain description");
        }
        DWORD window_process = 0;
        if (!desc.OutputWindow || !IsWindow(desc.OutputWindow) ||
            !GetWindowThreadProcessId(desc.OutputWindow, &window_process) ||
            window_process != GetCurrentProcessId()) {
            throw std::runtime_error("Swap chain has no window in this process");
        }

        Microsoft::WRL::ComPtr<ID3D11Device> device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target;
        hr = chain->GetDevice(IID_PPV_ARGS(device.GetAddressOf()));
        if (FAILED(hr)) {
            error_logger::ErrorLogger::Get().LogHResult("SwapChain::GetDevice", hr);
            throw std::runtime_error("Swap chain is not backed by D3D11");
        }
        device->GetImmediateContext(context.GetAddressOf());
        if (!context)
            throw std::runtime_error("D3D11 immediate context is null");
        hr = chain->GetBuffer(0, IID_PPV_ARGS(back_buffer.GetAddressOf()));
        if (FAILED(hr)) {
            error_logger::ErrorLogger::Get().LogHResult("SwapChain::GetBuffer", hr);
            throw std::runtime_error("Failed to get swap-chain back buffer");
        }
        hr = device->CreateRenderTargetView(back_buffer.Get(), nullptr, render_target.GetAddressOf());
        if (FAILED(hr)) {
            error_logger::ErrorLogger::Get().LogHResult("CreateRenderTargetView", hr);
            throw std::runtime_error("Failed to create swap-chain render target");
        }

        // Commit only after every query succeeded. Locals release on failure.
        chain->AddRef();
        destroy_d3d11_resources();
        swap_chain = chain;
        d3d11_device = device.Detach();
        d3d11_device_context = context.Detach();
        d3d11_render_target_view = render_target.Detach();
        hwnd = desc.OutputWindow;
        debug_console::Console::Get().Success("[D3D11] Captured IDXGISwapChain from Present: 0x%llX",
            reinterpret_cast<uintptr_t>(chain));
    }

    void create() {
        error_logger::ErrorLogger::Get().Log("Interfaces", "Starting interface creation...", 0);

        try {
            error_logger::ErrorLogger::Get().Log("cs_io", "Searching for pattern...", 0);
            auto addr = sdk::find_pattern("client.dll", "48 8B 0D ? ? ? ? 4C 8D 8F ? ? ? ? 45 33 F6");
            if (!addr) {
                error_logger::ErrorLogger::Get().LogInitError("cs_io", "Pattern not found");
                throw std::runtime_error("cs_io pattern not found");
            }

            error_logger::ErrorLogger::Get().Log("cs_io", "Pattern found, resolving pointer...", 0);

            try {
                auto ptr = sdk::resolve_absolute_rip_address(addr, 3, 7);
                if (!ptr) {
                    error_logger::ErrorLogger::Get().LogInitError("cs_io", "Failed to resolve pointer address");
                    throw std::runtime_error("Failed to resolve cs_io pointer address");
                }
                if (!sdk::is_valid_ptr(reinterpret_cast<uintptr_t>(ptr))) {
                    error_logger::ErrorLogger::Get().LogPointerError("cs_io Resolved", reinterpret_cast<uintptr_t>(ptr));
                    throw std::runtime_error("Invalid cs_io pointer address");
                }

                auto pCs_ioPtr = reinterpret_cast<sdk::interface_cs_io**>(ptr);
                if (!pCs_ioPtr) {
                    error_logger::ErrorLogger::Get().LogInitError("cs_io", "Pointer array is null");
                    throw std::runtime_error("cs_io pointer array is null");
                }
                if (!sdk::is_valid_ptr(reinterpret_cast<uintptr_t>(pCs_ioPtr))) {
                    error_logger::ErrorLogger::Get().LogPointerError("cs_io Array", reinterpret_cast<uintptr_t>(pCs_ioPtr));
                    throw std::runtime_error("cs_io pointer array location invalid");
                }

                auto pCs_io = *pCs_ioPtr;
                if (!pCs_io) {
                    error_logger::ErrorLogger::Get().LogInitError("cs_io", "Dereferenced pointer is null");
                    throw std::runtime_error("cs_io pointer is null");
                }
                if (!sdk::is_valid_ptr(reinterpret_cast<uintptr_t>(pCs_io))) {
                    error_logger::ErrorLogger::Get().LogPointerError("cs_io Structure", reinterpret_cast<uintptr_t>(pCs_io));
                    throw std::runtime_error("cs_io structure invalid");
                }

                cs_io = pCs_io;
                error_logger::ErrorLogger::Get().Log("cs_io", "Resolved successfully", 0);
            }
            catch (const std::exception& e) {
                error_logger::ErrorLogger::Get().LogException("cs_io Resolution", e);
                throw std::runtime_error(std::string("Failed to resolve cs_io: ") + e.what());
            }
            catch (...) {
                error_logger::ErrorLogger::Get().Log("cs_io", "Unknown error during resolution", 1);
                throw;
            }

            if (!cs_io) {
                error_logger::ErrorLogger::Get().LogInitError("cs_io", "Final validation failed");
                throw std::runtime_error("cs_io null");
            }
            error_logger::ErrorLogger::Get().Log("cs_io", "Interface ready", 0);
        }
        catch (const std::exception& e) {
            error_logger::ErrorLogger::Get().LogException("cs_io Creation", e);
            throw std::runtime_error(std::string("cs_io interface creation failed: ") + e.what());
        }
        catch (...) {
            error_logger::ErrorLogger::Get().Log("cs_io", "Unknown exception during creation", 1);
            throw;
        }

        try {
            error_logger::ErrorLogger::Get().Log("InputSystem", "Creating interface...", 0);
            input_system = capture_interface<sdk::interface_input_system>("inputsystem.dll", "InputSystemVersion001");
            error_logger::ErrorLogger::Get().Log("InputSystem", "Created successfully", 0);
        }
        catch (const std::exception& e) {
            error_logger::ErrorLogger::Get().LogException("InputSystem Creation", e);
            throw std::runtime_error(std::string("input_system interface creation failed: ") + e.what());
        }
        catch (...) {
            error_logger::ErrorLogger::Get().Log("InputSystem", "Unknown exception", 1);
            throw;
        }

        // In CS2, client.dll no longer exports a CHLClient interface via CreateInterface.
        // Instead, scan for FrameStageNotify directly by pattern and store the function
        // pointer in interfaces::client so hooks.cpp can hook it without a vtable lookup.
        try {
            error_logger::ErrorLogger::Get().Log("ClientInterface", "Scanning for FrameStageNotify via pattern...", 0);
            // CS2 FrameStageNotify pattern — called with stage index, processes render frames
            std::uint8_t* fsn = sdk::find_pattern("client.dll",
                "48 89 5C 24 ? 48 89 6C 24 ? 57 48 83 EC ? 48 8B F9 33 ED");
            if (fsn) {
                client = reinterpret_cast<void*>(fsn);
                error_logger::ErrorLogger::Get().Log("ClientInterface", "FrameStageNotify found via pattern", 0);
            }
            else {
                error_logger::ErrorLogger::Get().Log("ClientInterface", "FrameStageNotify pattern not found — knife subclass queue will not drain", 1);
            }
        }
        catch (const std::exception& e) {
            error_logger::ErrorLogger::Get().LogException("ClientInterface", e);
            // Not fatal; knife subclass queue falls back to render-thread drain
        }
        catch (...) {
            error_logger::ErrorLogger::Get().Log("ClientInterface", "Unknown exception scanning FrameStageNotify", 1);
        }

        error_logger::ErrorLogger::Get().Log("D3D11", "Waiting for the first Present callback", 0);
    }

    void destroy() {
        try {
            destroy_d3d11_resources();
            input_system = nullptr;
            cs_io = nullptr;
            g_pGameEntitySystem = nullptr;
            hwnd = nullptr;
            error_logger::ErrorLogger::Get().Log("Interfaces", "All interfaces destroyed", 0);
        }
        catch (const std::exception& e) {
            error_logger::ErrorLogger::Get().LogException("Interfaces Destroy", e);
        }
        catch (...) {
            error_logger::ErrorLogger::Get().Log("Interfaces", "Unknown error during destroy", 1);
        }
    }
}
