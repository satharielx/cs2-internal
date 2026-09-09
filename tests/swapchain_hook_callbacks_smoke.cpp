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
ImGuiContext* GetCurrentContext() { return test_current_context; }
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

static bool expected_replacement = false;
static unsigned create_calls = 0;
static HRESULT create_result = E_UNEXPECTED;

static HRESULT __stdcall OriginalCreate(IDXGIFactory* factory, IUnknown* device,
    DXGI_SWAP_CHAIN_DESC* desc, IDXGISwapChain** chain) {
    ++create_calls;
    Check(s_creations_in_progress == (expected_replacement ? 1u : 0u), "Track matching-window creation");
    if (expected_replacement) {
        Check(!interfaces::swap_chain && !interfaces::d3d11_render_target_view &&
            !interfaces::d3d11_device && !interfaces::d3d11_device_context,
            "Release owned resources before factory creates replacement");
    }
    bool mutex_available = false;
    std::thread window_thread([&] {
        mutex_available = s_render_mutex.try_lock();
        if (mutex_available) s_render_mutex.unlock();
    });
    window_thread.join();
    Check(mutex_available, "Original CreateSwapChain must run outside renderer lock");
    create_result = factory->CreateSwapChain(device, desc, chain);
    return create_result;
}

static void CheckCreation(HWND selected_window, HWND foreign_window, IDXGISwapChain* selected) {
    ComPtr<ID3D11Device> device;
    CheckHR(selected->GetDevice(IID_PPV_ARGS(device.GetAddressOf())), "Get creation device");
    ComPtr<IDXGIDevice> dxgi_device;
    CheckHR(device.As(&dxgi_device), "Get DXGI device");
    ComPtr<IDXGIAdapter> adapter;
    CheckHR(dxgi_device->GetAdapter(adapter.GetAddressOf()), "Get adapter");
    ComPtr<IDXGIFactory> factory;
    CheckHR(adapter->GetParent(IID_PPV_ARGS(factory.GetAddressOf())), "Get factory");
    hooks::oCreateSwapChain = &OriginalCreate;
    DXGI_SWAP_CHAIN_DESC desc{};
    CheckHR(selected->GetDesc(&desc), "Get creation description");
    auto* external_context = reinterpret_cast<ImGuiContext*>(static_cast<uintptr_t>(0x1234));
    test_current_context = external_context;

    // A foreign window must retain the selected overlay resources.
    desc.OutputWindow = foreign_window;
    expected_replacement = false;
    create_calls = 0;
    auto* target = interfaces::d3d11_render_target_view;
    ComPtr<IDXGISwapChain> other;
    HRESULT hr = hkCreateSwapChain(factory.Get(), device.Get(), &desc, other.GetAddressOf());
    CheckHR(hr, "Create foreign chain");
    Check(create_calls == 1 && hr == create_result && interfaces::d3d11_render_target_view == target,
        "Foreign creation preserves active renderer and forwards once");

    desc.OutputWindow = selected_window;
    expected_replacement = true;
    create_calls = 0;
    ComPtr<IDXGISwapChain> replacement;
    hr = hkCreateSwapChain(factory.Get(), device.Get(), &desc, replacement.GetAddressOf());
    CheckHR(hr, "Create same-window replacement");
    Check(create_calls == 1 && hr == create_result && !interfaces::swap_chain,
        "Replacement forwards once and defers resource capture");
    Check(test_current_context == external_context, "Preserve another ImGui context during teardown");
    interfaces::create_d3d11_resources(replacement.Get());

    // Invalid DXGI output must not be inspected or retried by the hook.
    create_calls = 0;
    hr = hkCreateSwapChain(factory.Get(), device.Get(), &desc, nullptr);
    Check(FAILED(hr) && create_calls == 1 && hr == create_result, "Creation failure forwarded once");
    Check(!interfaces::swap_chain && s_creations_in_progress == 0 && s_active_callbacks == 0,
        "Failure leaves state ready for next Present");
    interfaces::create_d3d11_resources(replacement.Get());
    test_current_context = nullptr;
    std::puts("PASS: CreateSwapChain replacement/failure, foreign windows, external ImGui context and unlocked forwarding");
}

static void CheckHookSetup() {
    test_hook_setup = true;
    auto reset = [] {
        test_creates = test_enables = test_uninitializes = test_removes = 0;
        test_create_failure = 0;
        test_missing_overlay_pattern = false;
    };
    reset();
    hooks::create(); // Optional game patterns deliberately return nullptr.
    Check(test_creates == 3 && test_enables == 1, "Enable together after all rendering hooks are prepared");
    hooks::destroy();
    Check(test_uninitializes == 1 && test_removes == 1, "Release prepared hooks on shutdown");

    for (unsigned failure : {1u, 3u}) {
        reset();
        test_create_failure = failure;
        bool rejected = false;
        try { hooks::create(); } catch (const std::exception&) { rejected = true; }
        Check(rejected && test_enables == 0 && test_uninitializes == 1 && test_removes == 1,
            "Required-hook failure rolls back before activation");
    }
    reset();
    test_missing_overlay_pattern = true;
    bool rejected = false;
    try { hooks::create(); } catch (const std::exception&) { rejected = true; }
    Check(rejected && test_creates == 0 && test_enables == 0 && test_uninitializes == 1,
        "Missing overlay signature fails cleanly");
    test_hook_setup = false;
    std::puts("PASS: optional-pattern startup, grouped activation and required-hook rollback");
}

int main() {
    try {
        oCreateMove = [](void* input, uint32_t slot, char active) -> bool {
            Check(input == reinterpret_cast<void*>(0x1234) && slot == 7 && active == 3,
                "CreateMove forwards all three ABI arguments");
            return true;
        };
        Check(hkCreateMove(reinterpret_cast<void*>(0x1234), 7, 3), "CreateMove forwards return value");
        oSubTickAngle = [](DWORD*, void*, char, double, int, sdk::C_CSPlayerPawn*) -> __int64 {
            bool unlocked = false;
            std::thread probe([&] {
                unlocked = config::mutex.try_lock();
                if (unlocked) config::mutex.unlock();
            });
            probe.join();
            Check(unlocked, "Subtick original runs without the settings mutex");
            return 123;
        };
        s_stopping = true;
        Check(hkSubTickAngle(nullptr, nullptr, 0, 0, 0, nullptr) == 123,
            "Subtick forwards while stopping");
        s_stopping = false;
        std::puts("PASS: CreateMove ABI and unlocked subtick forwarding");
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
        CheckCreation(selected_window.handle, foreign_window.handle, selected.Get());
        interfaces::destroy_d3d11_resources();
        CheckHookSetup();
        std::puts("PASS: production ResizeBuffers success/failure, exactly-once forwarding, foreign-chain isolation, and unlocked original DXGI call");
        return 0;
    }
    catch (const std::exception& ex) {
        interfaces::destroy_d3d11_resources();
        std::fprintf(stderr, "FAIL: %s\n", ex.what());
        return 1;
    }
}
