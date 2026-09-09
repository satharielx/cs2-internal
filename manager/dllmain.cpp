#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <cstdio>
#include <stdexcept>
#include <string>
#include "core/interfaces.h"
#include "core/hooks.h"
#include "core/error_logger.h"
#include "core/game_state.h"
#include "core/features.h"

DWORD WINAPI MainThread(LPVOID instance) {
// Spawn a native Windows console for early debug output
AllocConsole();
FILE* fp = nullptr;
freopen_s(&fp, "CONOUT$", "w", stdout);
freopen_s(&fp, "CONOUT$", "w", stderr);
freopen_s(&fp, "CONIN$", "r", stdin);  // Also redirect input
SetConsoleTitleA("CS2 Debug Console - Loading...");
    
// Make console visible and focused
HWND consoleWnd = GetConsoleWindow();
if (consoleWnd) {
    ShowWindow(consoleWnd, SW_SHOW);
    SetForegroundWindow(consoleWnd);
}
    
printf("========================================\n");
printf("  CS2 CHEAT DEBUG CONSOLE\n");
printf("========================================\n");
printf("[+] Console allocated successfully\n");
printf("[+] Injection started...\n");
printf("========================================\n\n");

// Initialize error logger at startup
error_logger::ErrorLogger::Get().Log("MainThread", "CS2 Cheat injection started", 0);
printf("[LOG] Error logger initialized\n");
    
// Show immediate message box to confirm injection
MessageBoxA(nullptr, 
    "DLL Injected!\n\n"
    "Debug console is now open.\n"
    "Waiting for CS2 modules...\n\n"
    "This will take 10-15 seconds.", 
    "CS2 Cheat - Initializing", 
    MB_ICONINFORMATION);
    
// Wait for critical CS2 modules with timeout
DWORD maxWait = 45000;  // 45 seconds max
const ULONGLONG waitStarted = GetTickCount64();
const char* requiredModules[] = {
    "client.dll", "engine2.dll", "rendersystemdx11.dll", "gameoverlayrenderer64.dll"
};

error_logger::ErrorLogger::Get().Log("Module Loading", "Waiting for CS2 modules...", 0);
printf("[WAIT] Waiting for CS2 modules to load...\n");
    
    std::string lastMissing;
    for (;;) {
        std::string missing;
        for (const char* module : requiredModules) {
            if (!GetModuleHandleA(module)) {
                if (!missing.empty()) missing += ", ";
                missing += module;
            }
        }
        if (missing.empty()) break;
        if (missing != lastMissing) {
            const std::string message = "Waiting for: " + missing;
            error_logger::ErrorLogger::Get().Log("Module Loading", message.c_str(), 0);
            lastMissing = missing;
        }
        if (GetTickCount64() - waitStarted >= maxWait) {
            const std::string message = "Timed out after 45 seconds. Missing modules:\n" + missing +
                "\n\nSee cs2_cheat_errors.log for details.";
            error_logger::ErrorLogger::Get().Log("Module Loading", message.c_str(), 1);
            MessageBoxA(nullptr, message.c_str(), "Injection Failed", MB_ICONERROR);
            FreeConsole();
            FreeLibraryAndExitThread((HMODULE)instance, 0);
            return 0;
        }
        Sleep(200);
    }

    // Verify modules are loaded
    HMODULE client_dll = GetModuleHandleA("client.dll");
    HMODULE engine2_dll = GetModuleHandleA("engine2.dll");
    HMODULE render_dll = GetModuleHandleA("rendersystemdx11.dll");
    
    error_logger::ErrorLogger::Get().Log("Module Loading", "All modules loaded successfully", 0);

    // Start game state monitor thread
    game_state::Start();

    // Extra buffer for module initialization
    error_logger::ErrorLogger::Get().Log("Initialization", "Waiting 10 seconds for module initialization...", 0);
    Sleep(10000);

    try {
        error_logger::ErrorLogger::Get().Log("Interfaces", "Creating game interfaces...", 0);
        interfaces::create();
        error_logger::ErrorLogger::Get().Log("Interfaces", "Game interfaces created successfully", 0);
        
        error_logger::ErrorLogger::Get().Log("Hooks", "Creating rendering hooks...", 0);
        hooks::create();
        error_logger::ErrorLogger::Get().Log("Hooks", "Rendering hooks created successfully", 0);

        // Start aimbot on its own thread so it never blocks ESP rendering
        features::StartAimbotThread();
        
        error_logger::ErrorLogger::Get().Log("MainThread", "Cheat injected successfully!", 0);
        MessageBoxA(nullptr, "Injected successfully!\n\nF1 = Toggle Debug Console (auto-open)\nINSERT = Toggle Menu\nEND = Unload\n\nErrors logged to cs2_cheat_errors.log", "CS2 Cheat Ready", MB_ICONINFORMATION);
    }
    catch (const std::exception& e) {
        error_logger::ErrorLogger::Get().LogException("MainThread", e);
        hooks::destroy();
        features::StopAimbotThread();
        game_state::Stop();
        interfaces::destroy();
        char buf[1024];
        sprintf_s(buf, "Initialization failed:\n\n%s\n\nCheck cs2_cheat_errors.log for details\n\nThe cheat will now unload.", e.what());
        MessageBoxA(nullptr, buf, "Error", MB_ICONERROR);
        FreeLibraryAndExitThread((HMODULE)instance, 0);
        return 0;
    }
    catch (...) {
        error_logger::ErrorLogger::Get().Log("MainThread", "Unknown exception during initialization", 1);
        hooks::destroy();
        features::StopAimbotThread();
        game_state::Stop();
        interfaces::destroy();
        MessageBoxA(nullptr, "Unknown exception during initialization!\n\nCheck cs2_cheat_errors.log for details\n\nThe cheat will now unload.", "Critical Error", MB_ICONERROR);
        FreeLibraryAndExitThread((HMODULE)instance, 0);
        return 0;
    }

    // Main loop - wait for unload key
    error_logger::ErrorLogger::Get().Log("MainThread", "Entering main loop. Press END to unload.", 0);
    while (!(GetAsyncKeyState(VK_END) & 1)) {
        Sleep(100);
    }

    // Cleanup
    error_logger::ErrorLogger::Get().Log("Cleanup", "Starting cleanup...", 0);
    try {
        hooks::destroy();
        features::StopAimbotThread();
        game_state::Stop();
        interfaces::destroy();
        error_logger::ErrorLogger::Get().Log("Cleanup", "Cleanup completed successfully", 0);
    }
    catch (const std::exception& ex) {
        error_logger::ErrorLogger::Get().LogException("Cleanup", ex);
    }
    catch (...) {
        error_logger::ErrorLogger::Get().Log("Cleanup", "Unknown error during cleanup", 1);
    }

    FreeConsole();
    FreeLibraryAndExitThread((HMODULE)instance, 0);
    return 0;
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
        if (hThread) {
            CloseHandle(hThread);
        }
    }
    return TRUE;
}
