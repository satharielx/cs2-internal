// Compiles the real interfaces.cpp and exercises its renderer using D3D11 WARP.
// The only stubs are logging and unused game module lookups.
#include "../manager/core/interfaces.h"
#include "../manager/core/error_logger.h"
#include "../manager/core/debug_console.h"
#include <wrl/client.h>
#include <cstdio>
#include <stdexcept>

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
namespace sdk {
std::uint8_t* find_pattern(const char*, const char*) { return nullptr; }
std::uint8_t* resolve_absolute_rip_address(std::uint8_t*, std::size_t, std::size_t) {
    return nullptr;
}
}

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
    HWND handle = CreateWindowExW(0, L"STATIC", L"Swap-chain resource smoke test",
        WS_OVERLAPPEDWINDOW, 0, 0, 640, 480, nullptr, nullptr,
        GetModuleHandleW(nullptr), nullptr);
    HiddenWindow() { Check(handle != nullptr, "Create hidden window"); }
    ~HiddenWindow() { if (handle) DestroyWindow(handle); }
};

struct Resources {
    IDXGISwapChain* chain = interfaces::swap_chain;
    ID3D11Device* device = interfaces::d3d11_device;
    ID3D11DeviceContext* context = interfaces::d3d11_device_context;
    ID3D11RenderTargetView* target = interfaces::d3d11_render_target_view;
    HWND window = interfaces::hwnd;

    bool Unchanged() const {
        return chain == interfaces::swap_chain && device == interfaces::d3d11_device &&
            context == interfaces::d3d11_device_context &&
            target == interfaces::d3d11_render_target_view && window == interfaces::hwnd;
    }
};

static void CheckEmpty() {
    Check(!interfaces::swap_chain && !interfaces::d3d11_device &&
        !interfaces::d3d11_device_context && !interfaces::d3d11_render_target_view &&
        !interfaces::hwnd, "Renderer resources must all be null");
}

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

// An actual IDXGISwapChain interface that fails at a selected query. Successful
// preceding queries use WARP, including the acquired device and context refs.
class FailingSwapChain final : public IDXGISwapChain {
public:
    enum class Stage { Description, Window, Device, BackBuffer };
    FailingSwapChain(IDXGISwapChain* real, Stage stage) : real_(real), stage_(stage) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void** output) override {
        if (output) *output = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return ++references_; }
    ULONG STDMETHODCALLTYPE Release() override { return --references_; }
    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID, UINT, const void*) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(REFGUID, const IUnknown*) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID, UINT*, void*) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetParent(REFIID, void**) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetDevice(REFIID iid, void** output) override {
        if (stage_ == Stage::Device) { *output = nullptr; return E_NOINTERFACE; }
        return real_->GetDevice(iid, output);
    }
    HRESULT STDMETHODCALLTYPE Present(UINT, UINT) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetBuffer(UINT, REFIID, void** output) override {
        *output = nullptr;
        return DXGI_ERROR_INVALID_CALL;
    }
    HRESULT STDMETHODCALLTYPE SetFullscreenState(BOOL, IDXGIOutput*) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetFullscreenState(BOOL*, IDXGIOutput**) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetDesc(DXGI_SWAP_CHAIN_DESC* desc) override {
        if (stage_ == Stage::Description) return E_FAIL;
        HRESULT result = real_->GetDesc(desc);
        if (stage_ == Stage::Window) desc->OutputWindow = nullptr;
        return result;
    }
    HRESULT STDMETHODCALLTYPE ResizeBuffers(UINT, UINT, UINT, DXGI_FORMAT, UINT) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE ResizeTarget(const DXGI_MODE_DESC*) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetContainingOutput(IDXGIOutput**) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetFrameStatistics(DXGI_FRAME_STATISTICS*) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetLastPresentCount(UINT*) override { return E_NOTIMPL; }

private:
    IDXGISwapChain* real_;
    Stage stage_;
    ULONG references_ = 1;
};

static void CheckRejectedCapture(IDXGISwapChain* candidate) {
    Resources before;
    bool threw = false;
    try { interfaces::create_d3d11_resources(candidate); }
    catch (const std::exception&) { threw = true; }
    Check(threw, "Invalid capture must throw");
    Check(before.Unchanged(), "Failed capture must leave all published resources intact");
}

static void CheckQueryFailures(IDXGISwapChain* real) {
    for (auto stage : { FailingSwapChain::Stage::Description, FailingSwapChain::Stage::Window,
        FailingSwapChain::Stage::Device, FailingSwapChain::Stage::BackBuffer }) {
        FailingSwapChain failing(real, stage);
        CheckRejectedCapture(&failing);
    }
}

static void CheckBackBuffer(UINT width, UINT height) {
    ComPtr<ID3D11Resource> resource;
    interfaces::d3d11_render_target_view->GetResource(resource.GetAddressOf());
    ComPtr<ID3D11Texture2D> texture;
    CheckHR(resource.As(&texture), "Query render-target texture");
    D3D11_TEXTURE2D_DESC desc{};
    texture->GetDesc(&desc);
    Check(desc.Width == width && desc.Height == height, "Render target must match back-buffer size");
}

int main() {
    try {
        HiddenWindow first_window;
        HiddenWindow second_window;
        auto first = MakeSwapChain(first_window.handle);
        auto second = MakeSwapChain(second_window.handle);

        interfaces::destroy_d3d11_resources();
        CheckEmpty();
        CheckRejectedCapture(nullptr);
        CheckQueryFailures(first.Get());
        CheckEmpty();
        std::puts("PASS: null and staged query failures leave empty state");

        interfaces::create_d3d11_resources(first.Get());
        Check(interfaces::swap_chain == first.Get(), "Capture callback swap chain");
        Check(interfaces::d3d11_device && interfaces::d3d11_device_context &&
            interfaces::d3d11_render_target_view, "Capture complete renderer state");
        Check(interfaces::hwnd == first_window.handle, "Capture actual swap-chain window");
        CheckBackBuffer(640, 480);
        first.Reset(); // The published chain must own an independent COM ref.
        CheckBackBuffer(640, 480);
        CheckRejectedCapture(nullptr);
        CheckQueryFailures(second.Get());
        std::puts("PASS: first capture owns resources; failed replacement preserves them");

        interfaces::d3d11_device_context->OMSetRenderTargets(1,
            &interfaces::d3d11_render_target_view, nullptr);
        interfaces::d3d11_device_context->OMSetRenderTargets(0, nullptr, nullptr);
        interfaces::destroy_render_target();
        CheckHR(interfaces::swap_chain->ResizeBuffers(0, 800, 600, DXGI_FORMAT_UNKNOWN, 0),
            "Resize after unbinding/releasing target");
        interfaces::create_render_target();
        CheckBackBuffer(800, 600);
        auto* target = interfaces::d3d11_render_target_view;
        interfaces::create_render_target();
        Check(target == interfaces::d3d11_render_target_view, "Existing target is reused");
        std::puts("PASS: real ResizeBuffers and target recreation");

        ComPtr<ID3D11Texture2D> held_buffer;
        CheckHR(interfaces::swap_chain->GetBuffer(0, IID_PPV_ARGS(held_buffer.GetAddressOf())),
            "Hold reference to force ResizeBuffers failure");
        interfaces::destroy_render_target();
        Check(FAILED(interfaces::swap_chain->ResizeBuffers(0, 1024, 768, DXGI_FORMAT_UNKNOWN, 0)),
            "Outstanding back-buffer reference must reject resize");
        held_buffer.Reset();
        interfaces::create_render_target();
        CheckBackBuffer(800, 600);
        std::puts("PASS: failed resize recovers the unchanged back buffer");

        interfaces::create_d3d11_resources(second.Get());
        Check(interfaces::swap_chain == second.Get(), "Replacement captures the supplied chain");
        Check(interfaces::hwnd == second_window.handle, "Replacement uses its own window");
        CheckBackBuffer(640, 480);
        interfaces::create_d3d11_resources(second.Get());
        CheckBackBuffer(640, 480);
        second.Reset();
        interfaces::destroy_d3d11_resources();
        CheckEmpty();
        interfaces::destroy_d3d11_resources();
        CheckEmpty();
        std::puts("PASS: replacement, repeated capture, and idempotent destruction");
        return 0;
    }
    catch (const std::exception& ex) {
        std::fprintf(stderr, "FAIL: %s\n", ex.what());
        interfaces::destroy_d3d11_resources();
        return 1;
    }
}
