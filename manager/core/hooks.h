#pragma once
#include "interfaces.h"
#include <dxgi.h>

namespace hooks {
    void create();
    void destroy();

    using tPresent = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);
    inline tPresent oPresent = nullptr;

    using tResizeBuffers = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
    inline tResizeBuffers oResizeBuffers = nullptr;

    using tCreateSwapChain = HRESULT(__stdcall*)(IDXGIFactory*, IUnknown*, DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**);
    inline tCreateSwapChain oCreateSwapChain = nullptr;

    inline WNDPROC oWndProc = nullptr;
}