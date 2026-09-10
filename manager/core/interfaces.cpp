#include "interfaces.h"
#include "error_logger.h"
#include "debug_console.h"
#include "../sdk/mem.h"
#include "../sdk/feature_support.h"
#include "../external/offsets/offsets.hpp"
#include <stdexcept>
#include <format>
#include <wrl/client.h>

namespace interfaces {
    static uintptr_t entity_system_slot = 0;
    CGameEntitySystem* GameEntitySystem() {
        static ULONGLONG next_scan = 0;
        if (!entity_system_slot && GetTickCount64() >= next_scan) {
            next_scan = GetTickCount64() + 1000;
            auto* instruction = sdk::find_pattern("client.dll", "48 89 0D ? ? ? ? E9 ? ? ? ? CC");
            if (instruction)
                entity_system_slot = reinterpret_cast<uintptr_t>(sdk::resolve_absolute_rip_address(instruction, 3, 7));
        }
        // The global slot survives map changes; the object stored in it may not.
        if (entity_system_slot) return sdk::read_value<CGameEntitySystem*>(entity_system_slot);
        const auto client = reinterpret_cast<uintptr_t>(GetModuleHandleA("client.dll"));
        return client ? sdk::read_value<CGameEntitySystem*>(client +
            cs2_dumper::offsets::client_dll::dwGameEntitySystem) : nullptr;
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

        sdk::entity_lookup = reinterpret_cast<sdk::EntityLookup>(sdk::find_pattern("client.dll",
            "4C 8D 49 10 81 FA ?? ?? 00 00 77 ?? 8B CA C1 F9 09"));
        // These optional interfaces currently have no consumers in this build.
        // Missing game signatures must not prevent the independent overlay hooks.
        cs_io = nullptr;
        input_system = nullptr;
        client = nullptr;
        auto* input_load = sdk::find_pattern("client.dll", "48 8B 0D ? ? ? ? 4C 8D 8F ? ? ? ? 45 33 F6");
        if (input_load) {
            auto* slot = sdk::resolve_absolute_rip_address(input_load, 3, 7);
            if (sdk::is_readable_range(reinterpret_cast<uintptr_t>(slot), sizeof(void*))) {
                auto* candidate = *reinterpret_cast<sdk::interface_cs_io**>(slot);
                if (sdk::is_valid_ptr(reinterpret_cast<uintptr_t>(candidate))) cs_io = candidate;
            }
        }
        if (!cs_io)
            error_logger::ErrorLogger::Get().Log("cs_io", "Optional input signature unavailable", 0);

        try {
            input_system = capture_interface<sdk::interface_input_system>("inputsystem.dll", "InputSystemVersion001");
        }
        catch (const std::exception& e) {
            error_logger::ErrorLogger::Get().LogException("Optional InputSystem", e);
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
            entity_system_slot = 0;
            sdk::entity_lookup = nullptr;
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
