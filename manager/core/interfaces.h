#pragma once
#include <d3d11.h>
#include "../sdk/interfaces/cs_io.h"
#include "../sdk/interfaces/input_system.h"
#include "../sdk/interfaces/swap_chain_dx11.h"

namespace interfaces {
    void create();
    void destroy();

    inline sdk::interface_cs_io* cs_io = nullptr;
    inline sdk::interface_input_system* input_system = nullptr;
    inline sdk::interface_swap_chain_dx11* swap_chain_dx11 = nullptr;

    inline ID3D11Device* d3d11_device = nullptr;
    inline ID3D11DeviceContext* d3d11_device_context = nullptr;
    inline ID3D11RenderTargetView* d3d11_render_target_view = nullptr;
    inline HWND hwnd = nullptr;

    void create_render_target();
    void destroy_render_target();
}