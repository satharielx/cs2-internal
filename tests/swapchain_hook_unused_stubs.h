#pragma once
#include <cstdlib>

inline ImGuiContext* test_current_context = nullptr;
inline unsigned test_menu_resets = 0;
inline bool test_hook_setup = false;
inline bool test_missing_overlay_pattern = false;
inline unsigned test_create_failure = 0;
inline unsigned test_creates = 0;
inline unsigned test_enables = 0;
inline unsigned test_uninitializes = 0;
inline unsigned test_removes = 0;

// hooks.cpp shares its translation unit with unrelated game and ImGui setup
// hooks. MSVC requires their symbols even though /OPT:REF discards them. Abort
// if the callback under test unexpectedly reaches any of these dependencies.
namespace sdk {
std::uint8_t* find_pattern(const char* module, const char*) {
    if (!test_hook_setup) std::abort();
    static std::uint8_t placeholder = 0;
    return !test_missing_overlay_pattern && std::strcmp(module, "gameoverlayrenderer64.dll") == 0
        ? &placeholder : nullptr;
}
std::uint8_t* resolve_absolute_rip_address(std::uint8_t*, std::size_t, std::size_t) { std::abort(); }
}
namespace features {
void SetInputBlocked(bool) {}
void ReleaseInputs() {}
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
void ResetRendererResources() { ++test_menu_resets; }
void RenderMainMenu() { std::abort(); }
}
namespace debug_console {
void Console::Info(const char*, ...) { if (!test_hook_setup) std::abort(); }
void Console::Warning(const char*, ...) { if (!test_hook_setup) std::abort(); }
void Console::Error(const char*, ...) { std::abort(); }
void Console::Render() { std::abort(); }
}
namespace hooks {
fnFrameStageNotify FrameStageNotify_o = nullptr;
void RegisterOnFrameStageNotify(OnFrameStageNotifyCallback) { std::abort(); }
void __fastcall Hook_FrameStageNotify(CSource2Client*, int) { std::abort(); }
}
namespace pattern_resolver {
bool Initialize() { if (!test_hook_setup) std::abort(); return false; }
}
namespace ImGui {
void SetCurrentContext(ImGuiContext* context) { test_current_context = context; }
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
MH_STATUS WINAPI MH_Initialize() { if (!test_hook_setup) std::abort(); return MH_OK; }
MH_STATUS WINAPI MH_Uninitialize() { ++test_uninitializes; return MH_OK; }
MH_STATUS WINAPI MH_CreateHook(LPVOID, LPVOID, LPVOID*) {
    ++test_creates;
    return test_creates == test_create_failure ? MH_ERROR_UNSUPPORTED_FUNCTION : MH_OK;
}
MH_STATUS WINAPI MH_RemoveHook(LPVOID) { ++test_removes; return MH_OK; }
MH_STATUS WINAPI MH_EnableHook(LPVOID target) {
    if (target != MH_ALL_HOOKS || test_creates != 3) std::abort();
    ++test_enables;
    return MH_OK;
}
MH_STATUS WINAPI MH_DisableHook(LPVOID) { return MH_OK; }
}
