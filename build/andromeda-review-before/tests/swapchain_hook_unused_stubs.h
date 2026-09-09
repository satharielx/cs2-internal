#pragma once
#include <cstdlib>

// hooks.cpp shares its translation unit with unrelated game and ImGui setup
// hooks. MSVC requires their symbols even though /OPT:REF discards them. Abort
// if the callback under test unexpectedly reaches any of these dependencies.
namespace sdk {
std::uint8_t* find_pattern(const char*, const char*) { std::abort(); }
std::uint8_t* resolve_absolute_rip_address(std::uint8_t*, std::size_t, std::size_t) { std::abort(); }
}
namespace features {
void RenderESP() { std::abort(); }
void RunSilentAimSubTick(DWORD*, sdk::C_CSPlayerPawn*) { std::abort(); }
void StartAimbotThread() { std::abort(); }
void BunnyHop() { std::abort(); }
void NoFlash() { std::abort(); }
void TriggerBot() { std::abort(); }
void RadarHack() { std::abort(); }
}
namespace skins {
void ApplyAllSkins() { std::abort(); }
void ApplyKnifeSkins() { std::abort(); }
void SetCurrentFrameStage(int) { std::abort(); }
}
namespace menu_advanced {
bool LoadIconFont() { std::abort(); }
void ResetRendererResources() { std::abort(); }
void RenderMainMenu() { std::abort(); }
}
namespace debug_console {
void Console::Info(const char*, ...) { std::abort(); }
void Console::Warning(const char*, ...) { std::abort(); }
void Console::Error(const char*, ...) { std::abort(); }
void Console::Render() { std::abort(); }
}
namespace hooks {
fnFrameStageNotify FrameStageNotify_o = nullptr;
void RegisterOnFrameStageNotify(OnFrameStageNotifyCallback) { std::abort(); }
void __fastcall Hook_FrameStageNotify(CSource2Client*, int) { std::abort(); }
}
namespace pattern_resolver {
bool Initialize() { std::abort(); }
}
namespace ImGui {
ImGuiContext* CreateContext(ImFontAtlas*) { std::abort(); }
void DestroyContext(ImGuiContext*) { std::abort(); }
ImGuiIO& GetIO() { std::abort(); }
void NewFrame() { std::abort(); }
void EndFrame() { std::abort(); }
void Render() { std::abort(); }
ImDrawData* GetDrawData() { std::abort(); }
void StyleColorsDark(ImGuiStyle*) { std::abort(); }
}
ImFont* ImFontAtlas::AddFontDefault(const ImFontConfig*) { std::abort(); }
ImFont* ImFontAtlas::AddFontFromFileTTF(const char*, float, const ImFontConfig*, const ImWchar*) { std::abort(); }
bool ImGui_ImplWin32_Init(void*) { std::abort(); }
void ImGui_ImplWin32_Shutdown() { std::abort(); }
void ImGui_ImplWin32_NewFrame() { std::abort(); }
LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM) { std::abort(); }
bool ImGui_ImplDX11_Init(ID3D11Device*, ID3D11DeviceContext*) { std::abort(); }
void ImGui_ImplDX11_Shutdown() { std::abort(); }
void ImGui_ImplDX11_NewFrame() { std::abort(); }
void ImGui_ImplDX11_RenderDrawData(ImDrawData*) { std::abort(); }
extern "C" {
MH_STATUS WINAPI MH_Initialize() { std::abort(); }
MH_STATUS WINAPI MH_Uninitialize() { std::abort(); }
MH_STATUS WINAPI MH_CreateHook(LPVOID, LPVOID, LPVOID*) { std::abort(); }
MH_STATUS WINAPI MH_RemoveHook(LPVOID) { std::abort(); }
MH_STATUS WINAPI MH_EnableHook(LPVOID) { std::abort(); }
MH_STATUS WINAPI MH_DisableHook(LPVOID) { std::abort(); }
}
