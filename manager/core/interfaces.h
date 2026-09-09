#pragma once
#include <d3d11.h>
#include "../sdk/interfaces/cs_io.h"
#include "../sdk/interfaces/input_system.h"

class CGameEntitySystem;

namespace interfaces {
    void create();
    void destroy();
    CGameEntitySystem* GameEntitySystem();

    inline sdk::interface_cs_io* cs_io = nullptr;
    inline sdk::interface_input_system* input_system = nullptr;
    // Owned COM reference captured from the Present callback.
    inline IDXGISwapChain* swap_chain = nullptr;
    // Client interface pointer (IBaseClientDLL / CHLClient) captured via CreateInterface
    inline void* client = nullptr;

    inline ID3D11Device* d3d11_device = nullptr;
    inline ID3D11DeviceContext* d3d11_device_context = nullptr;
    inline ID3D11RenderTargetView* d3d11_render_target_view = nullptr;
    inline HWND hwnd = nullptr;

    void create_render_target();
    void destroy_render_target();
    void create_d3d11_resources(IDXGISwapChain* chain);
    void destroy_d3d11_resources();
}
