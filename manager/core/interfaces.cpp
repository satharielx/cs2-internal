#include "interfaces.h"
#include "error_logger.h"
#include "debug_console.h"
#include <stdexcept>
#include <format>

namespace interfaces {
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
        if (!swap_chain_dx11 || !swap_chain_dx11->swap_chain) {
            error_logger::ErrorLogger::Get().Log("Render Target", "SwapChain invalid", 1);
            throw std::runtime_error("No swapchain");
        }
        ID3D11Texture2D* bb = nullptr;
        if (FAILED(swap_chain_dx11->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&bb))) {
            error_logger::ErrorLogger::Get().Log("Render Target", "GetBuffer failed", 1);
            throw std::runtime_error("GetBuffer failed");
        }
        if (FAILED(d3d11_device->CreateRenderTargetView(bb, nullptr, &d3d11_render_target_view))) {
            error_logger::ErrorLogger::Get().Log("Render Target", "CreateRenderTargetView failed", 1);
            bb->Release();
            throw std::runtime_error("CreateRTV failed");
        }
        bb->Release();
        error_logger::ErrorLogger::Get().Log("Render Target", "Created successfully", 0);
    }

    void destroy_render_target() {
        if (d3d11_render_target_view) {
            d3d11_render_target_view->Release();
            d3d11_render_target_view = nullptr;
            error_logger::ErrorLogger::Get().Log("Render Target", "Destroyed", 0);
        }
    }

    static void create_d3d11_resources() {
        auto& con = debug_console::Console::Get();

        {
            con.Info("[D3D11] Scanning pattern in rendersystemdx11.dll...");

            HMODULE render_mod = GetModuleHandleA("rendersystemdx11.dll");
            con.Pointer("rendersystemdx11.dll base", reinterpret_cast<uintptr_t>(render_mod), 0, render_mod != nullptr);

            std::uint8_t* address =
                sdk::find_pattern("rendersystemdx11.dll", "48 89 2D ? ? ? ? 66 0F 7F 05");
            con.Pointer("pattern address", reinterpret_cast<uintptr_t>(address), 0, address != nullptr);

            std::uint8_t* resolved = sdk::resolve_absolute_rip_address(address, 3, 7);
            con.Pointer("resolved RIP address", reinterpret_cast<uintptr_t>(resolved), 0, resolved != nullptr);

            // Log each dereference level of the triple pointer
            uintptr_t deref1 = *reinterpret_cast<uintptr_t*>(resolved);
            con.Pointer("deref level 1 (*resolved)", reinterpret_cast<uintptr_t>(resolved), deref1, deref1 != 0);

            uintptr_t deref2 = *reinterpret_cast<uintptr_t*>(deref1);
            con.Pointer("deref level 2 (**resolved)", deref1, deref2, deref2 != 0);

            swap_chain_dx11 = reinterpret_cast<sdk::interface_swap_chain_dx11*>(deref2);
            con.Pointer("swap_chain_dx11", 0, reinterpret_cast<uintptr_t>(swap_chain_dx11), swap_chain_dx11 != nullptr);

            if (swap_chain_dx11 == nullptr) {
                con.Error("[D3D11] FAILED: swap_chain_dx11 is null after double deref");
                throw std::runtime_error("failed to capture interface_swap_chain_dx11");
            }
            con.Success("[D3D11] swap_chain_dx11 captured");
        }

        con.Pointer("swap_chain_dx11->swap_chain", reinterpret_cast<uintptr_t>(swap_chain_dx11),
                     reinterpret_cast<uintptr_t>(swap_chain_dx11->swap_chain),
                     swap_chain_dx11->swap_chain != nullptr);

        if (swap_chain_dx11->swap_chain == nullptr) {
            con.Error("[D3D11] FAILED: swap_chain_dx11->swap_chain is null (padding is outdated)");
            throw std::runtime_error("swap_chain_dx11 padding is outdated.");
        }

        IDXGISwapChain* swap_chain = swap_chain_dx11->swap_chain;
        con.Success("[D3D11] IDXGISwapChain* = 0x%llX", reinterpret_cast<uintptr_t>(swap_chain));

        if (FAILED(swap_chain->GetDevice(__uuidof(ID3D11Device),
                                         reinterpret_cast<void**>(&d3d11_device)))) {
            con.Error("[D3D11] FAILED: GetDevice from swap chain");
            throw std::runtime_error("failed to get d3d11 device from swap chain");
        }
        con.Pointer("d3d11_device", reinterpret_cast<uintptr_t>(d3d11_device), 0, d3d11_device != nullptr);

        d3d11_device->GetImmediateContext(&d3d11_device_context);
        if (d3d11_device_context == nullptr) {
            con.Error("[D3D11] FAILED: device context is null");
            throw std::runtime_error("d3d11 device context is null");
        }
        con.Pointer("d3d11_device_context", reinterpret_cast<uintptr_t>(d3d11_device_context), 0, d3d11_device_context != nullptr);

        {
            ID3D11Texture2D* back_buffer = nullptr;
            if (FAILED(swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                             reinterpret_cast<void**>(&back_buffer)))) {
                con.Error("[D3D11] FAILED: GetBuffer for back buffer");
                throw std::runtime_error("failed to get back buffer from swap chain");
            }

            if (back_buffer == nullptr) {
                con.Error("[D3D11] FAILED: back buffer is null");
                throw std::runtime_error("back buffer is null");
            }
            con.Pointer("back_buffer", reinterpret_cast<uintptr_t>(back_buffer), 0, true);

            if (FAILED(d3d11_device->CreateRenderTargetView(back_buffer, nullptr,
                                                            &d3d11_render_target_view))) {
                back_buffer->Release();
                con.Error("[D3D11] FAILED: CreateRenderTargetView");
                throw std::runtime_error("failed to create render target view from back buffer");
            }

            back_buffer->Release();

            if (d3d11_render_target_view == nullptr) {
                con.Error("[D3D11] FAILED: render target view is null after creation");
                throw std::runtime_error("render target view is null");
            }
            con.Pointer("d3d11_render_target_view", reinterpret_cast<uintptr_t>(d3d11_render_target_view), 0, true);
        }

        {
            DXGI_SWAP_CHAIN_DESC swap_chain_desc;
            if (FAILED(swap_chain->GetDesc(&swap_chain_desc))) {
                con.Error("[D3D11] FAILED: GetDesc for swap chain");
                throw std::runtime_error("failed to get swap chain description");
            }

            hwnd = swap_chain_desc.OutputWindow;
            con.Pointer("hwnd", reinterpret_cast<uintptr_t>(hwnd), 0, hwnd != nullptr);

            if (hwnd == nullptr) {
                con.Error("[D3D11] FAILED: hwnd is null");
                throw std::runtime_error("swap chain hwnd is null");
            }
        }

        con.Success("[D3D11] All resources created successfully");
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

        try {
            error_logger::ErrorLogger::Get().Log("D3D11", "Initializing D3D11 resources...", 0);
            create_d3d11_resources();
            error_logger::ErrorLogger::Get().Log("D3D11", "Initialization completed", 0);
        }
        catch (const std::exception& e) {
            error_logger::ErrorLogger::Get().LogException("D3D11 Initialization", e);
            throw std::runtime_error(std::string("D3D11 initialization failed: ") + e.what());
        }
        catch (...) {
            error_logger::ErrorLogger::Get().Log("D3D11", "Unknown exception during initialization", 1);
            throw;
        }


    }

    void destroy() {
        try {
            destroy_render_target();
            if (d3d11_device_context) {
                d3d11_device_context->Release();
                d3d11_device_context = nullptr;
                error_logger::ErrorLogger::Get().Log("D3D11", "Device context released", 0);
            }
            if (d3d11_device) {
                d3d11_device->Release();
                d3d11_device = nullptr;
                error_logger::ErrorLogger::Get().Log("D3D11", "Device released", 0);
            }
            swap_chain_dx11 = nullptr;
            input_system = nullptr;
            cs_io = nullptr;
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
