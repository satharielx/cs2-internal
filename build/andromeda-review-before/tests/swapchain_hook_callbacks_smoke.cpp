// Compile the actual callback in this translation unit so a second thread can
// verify its private renderer mutex is unlocked during the original DXGI call.
// /Gy and /OPT:REF discard unrelated hooks; D3D resources use production code.
#include "../manager/core/hooks.cpp"
#include <wrl/client.h>
#include <cstdio>
#include <thread>
#include "swapchain_hook_unused_stubs.h"

using Microsoft::WRL::ComPtr;

namespace error_logger {
ErrorLogger& ErrorLogger::Get() { static ErrorLogger logger; return logger; }
void ErrorLogger::Log(const char*, const char*, int) {}
void ErrorLogger::LogException(const char*, const std::exception&) {}
void ErrorLogger::LogHResult(const char*, HRESULT) {}
void ErrorLogger::LogPointerError(const char*, uintptr_t) {}
void ErrorLogger::LogInitError(const char*, const char*) {}
}
namespace debug_console {
Console& Console::Get() { static Console console; return console; }
void Console::Success(const char*, ...) {}
}
namespace ImGui {
ImGuiContext* GetCurrentContext() { return nullptr; }
}
void ImGui_ImplDX11_InvalidateDeviceObjects() {}

static void Check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

static void CheckHR(HRESULT result, const char* message) {
    if (FAILED(result)) {
        std::fprintf(stderr, "%s: HRESULT 0x%08lX\n", message, result);
        throw std::runtime_error(message);
    }
}

struct HiddenWindow {
    HWND handle = CreateWindowExW(0, L"STATIC", L"Swap-chain callback smoke test",
        WS_OVERLAPPEDWINDOW, 0, 0, 640, 480, nullptr, nullptr,
        GetModuleHandleW(nullptr), nullptr);
    HiddenWindow() { Check(handle != nullptr, "Create hidden window"); }
    ~HiddenWindow() { if (handle) DestroyWindow(handle); }
};

static ComPtr<IDXGISwapChain> MakeSwapChain(HWND window) {
    DXGI_SWAP_CHAIN_DESC desc{};
    desc.BufferDesc.Width = 640;
    desc.BufferDesc.Height = 480;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = 1;
    desc.OutputWindow = window;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    ComPtr<IDXGISwapChain> chain;
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    CheckHR(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr,
        0, nullptr, 0, D3D11_SDK_VERSION, &desc, chain.GetAddressOf(),
        device.GetAddressOf(), nullptr, context.GetAddressOf()), "Create WARP swap chain");
    return chain;
}

struct ExpectedCall {
    IDXGISwapChain* chain;
    UINT buffer_count;
    UINT width;
    UINT height;
    DXGI_FORMAT format;
    UINT flags;
    bool tracked;
};

static ExpectedCall expected{};
static unsigned original_calls = 0;
static HRESULT original_result = E_UNEXPECTED;

static HRESULT __stdcall OriginalResize(IDXGISwapChain* chain, UINT buffer_count,
    UINT width, UINT height, DXGI_FORMAT format, UINT flags) {
    ++original_calls;
    Check(chain == expected.chain && buffer_count == expected.buffer_count &&
        width == expected.width && height == expected.height &&
        format == expected.format && flags == expected.flags,
        "ResizeBuffers must forward every original argument unchanged");
    Check(s_active_callbacks.load() == 1, "Callback must remain tracked during DXGI");
    Check(s_resizes_in_progress == (expected.tracked ? 1u : 0u),
        "Only the selected swap chain must suppress rendering while DXGI resizes");
    bool mutex_available = false;
    std::thread window_thread([&] {
        mutex_available = s_render_mutex.try_lock();
        if (mutex_available) s_render_mutex.unlock();
    });
    window_thread.join();
    Check(mutex_available, "Original ResizeBuffers must run outside the renderer mutex");
    if (expected.tracked) {
        Check(!interfaces::d3d11_render_target_view,
            "Release the selected render target before original ResizeBuffers");
        ComPtr<ID3D11RenderTargetView> bound;
        interfaces::d3d11_device_context->OMGetRenderTargets(1, bound.GetAddressOf(), nullptr);
        Check(!bound, "Unbind the selected render target before original ResizeBuffers");
    }
    original_result = chain->ResizeBuffers(buffer_count, width, height, format, flags);
    return original_result;
}

static HRESULT InvokeResize(IDXGISwapChain* chain, UINT width, UINT height, bool tracked) {
    expected = { chain, 1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0, tracked };
    original_calls = 0;
    original_result = E_UNEXPECTED;
    const HRESULT result = hkResizeBuffers(chain, expected.buffer_count, width, height,
        expected.format, expected.flags);
    Check(original_calls == 1, "Original ResizeBuffers must be called exactly once");
    Check(result == original_result, "Return the original DXGI result unchanged");
    Check(s_resizes_in_progress == 0 && s_active_callbacks.load() == 0,
        "Resize and callback tracking must finish on both success and failure");
    return result;
}

int main() {
    try {
        HiddenWindow selected_window;
        HiddenWindow foreign_window;
        auto selected = MakeSwapChain(selected_window.handle);
        auto foreign = MakeSwapChain(foreign_window.handle);
        hooks::oResizeBuffers = &OriginalResize;
        interfaces::create_d3d11_resources(selected.Get());
        interfaces::d3d11_device_context->OMSetRenderTargets(
            1, &interfaces::d3d11_render_target_view, nullptr);

        CheckHR(InvokeResize(selected.Get(), 800, 600, true), "Selected resize success");
        interfaces::create_render_target();
        Check(interfaces::d3d11_render_target_view != nullptr,
            "Recreate selected render target after successful resize");

        // Retaining a real back-buffer reference forces DXGI to reject resize.
        ComPtr<ID3D11Texture2D> retained_buffer;
        CheckHR(selected->GetBuffer(0, IID_PPV_ARGS(retained_buffer.GetAddressOf())),
            "Retain back buffer to force resize failure");
        Check(InvokeResize(selected.Get(), 1024, 768, true) == DXGI_ERROR_INVALID_CALL,
            "Return DXGI failure without a second original call");
        retained_buffer.Reset();
        interfaces::create_render_target();
        Check(interfaces::d3d11_render_target_view != nullptr,
            "Recreate selected render target from retained buffer after failed resize");

        auto* active_target = interfaces::d3d11_render_target_view;
        auto* active_device = interfaces::d3d11_device;
        auto* active_context = interfaces::d3d11_device_context;
        interfaces::d3d11_device_context->OMSetRenderTargets(1, &active_target, nullptr);
        CheckHR(InvokeResize(foreign.Get(), 320, 240, false), "Foreign resize success");
        Check(interfaces::swap_chain == selected.Get() &&
            interfaces::d3d11_render_target_view == active_target &&
            interfaces::d3d11_device == active_device &&
            interfaces::d3d11_device_context == active_context &&
            interfaces::hwnd == selected_window.handle,
            "Foreign-chain resize must preserve selected renderer resources");
        ComPtr<ID3D11RenderTargetView> bound;
        active_context->OMGetRenderTargets(1, bound.GetAddressOf(), nullptr);
        Check(bound.Get() == active_target,
            "Foreign-chain resize must preserve the selected output-merger binding");
        bound.Reset();

        active_context->OMSetRenderTargets(0, nullptr, nullptr);
        interfaces::destroy_d3d11_resources();
        std::puts("PASS: production ResizeBuffers success/failure, exactly-once forwarding, foreign-chain isolation, and unlocked original DXGI call");
        return 0;
    }
    catch (const std::exception& ex) {
        interfaces::destroy_d3d11_resources();
        std::fprintf(stderr, "FAIL: %s\n", ex.what());
        return 1;
    }
}
