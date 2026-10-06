#pragma once
#include "interfaces.h"
#include "../sdk/entity.h"
#include <dxgi.h>

using namespace sdk;

namespace hooks {
    void create();
    void destroy();

    // x64 entity-system callback: RCX=system, RDX=entity, R8D=handle.
    // Keep the original pointer-sized return value intact.
    using EntityLifecycleFn = void*(__fastcall*)(void* system, void* entity, uint32_t handle);
    inline EntityLifecycleFn OnAddEntity_o = nullptr;
    inline EntityLifecycleFn OnRemoveEntity_o = nullptr;
    void* __fastcall Hook_OnAddEntity(void* system, void* entity, uint32_t handle);
    void* __fastcall Hook_OnRemoveEntity(void* system, void* entity, uint32_t handle);


    using tPresent = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);
    inline tPresent oPresent = nullptr;

    using tResizeBuffers = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
    inline tResizeBuffers oResizeBuffers = nullptr;

    using tCreateSwapChain = HRESULT(__stdcall*)(IDXGIFactory*, IUnknown*, DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**);
    inline tCreateSwapChain oCreateSwapChain = nullptr;

    inline WNDPROC oWndProc = nullptr;
}