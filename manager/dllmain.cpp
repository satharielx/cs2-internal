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
#include "core/pattern_resolver.h"
#include "core/runtime_startup.h"
#include "core/constants/hooks_signatures.h"
#include "sdk/mem.h"

static_assert(sizeof(void*) == 8, "The CS2 runtime requires an x64 DLL");

namespace {
    class RuntimeSession {
        bool interfaces_started_ = false;
        bool hooks_started_ = false;
        bool state_started_ = false;
        bool aim_started_ = false;
        template<class Stop>
        static bool StopStage(bool& started, const char* name, Stop stop) noexcept {
            if (!started) return true;
            try { stop(); started = false; return true; }
            catch (...) {
                try { error_logger::ErrorLogger::Get().Log("Cleanup", name, 1); } catch (...) {}
                return false;
            }
        }
    public:
        void Start() {
            // Discovery can allocate resources before throwing.
            interfaces_started_ = true;
            interfaces::create();
            // The monitor consumes completed interface discovery and supplies the
            // snapshot needed by frame callbacks during setup and restoration.
            state_started_ = true;
            game_state::Start();
            // hooks::create rolls back its own partial setup on failure.
            hooks::create();
            hooks_started_ = true;
            // The mutating feature worker starts only after resolver/hook setup.
            aim_started_ = true;
            features::StartAimbotThread();
        }
        bool Stop() noexcept {
            // Keep dependencies alive if a worker could not be joined.
            if (!StopStage(aim_started_, "Failed to stop feature worker", features::StopAimbotThread)) return false;
            if (!StopStage(hooks_started_, "Failed to stop hooks", hooks::destroy)) return false;
            if (!StopStage(state_started_, "Failed to stop state monitor", game_state::Stop)) return false;
            if (!StopStage(interfaces_started_, "Failed to release interfaces", interfaces::destroy)) return false;
            pattern_resolver::Reset();
            return true;
        }
    };

    bool WaitForRuntime() {
        const auto loaded = [](const char* module) { return GetModuleHandleA(module) != nullptr; };
        const auto began = GetTickCount64();
        std::string last_status;
        for (;;) {
            if (GetAsyncKeyState(VK_END) & 1) return false;
            auto missing = runtime_startup::MissingModules(loaded);
            std::string status;
            if (!missing.empty()) status = "Waiting for: " + missing;
            else {
                // Check actual mandatory entry points instead of sleeping ten seconds.
                // Optional game features are discovered independently afterward.
                const bool ready = sdk::find_unique_pattern("gameoverlayrenderer64.dll", runtime_patterns::hooks::present) &&
                    sdk::find_unique_pattern("gameoverlayrenderer64.dll", runtime_patterns::hooks::resize_buffers) &&
                    sdk::find_unique_pattern("gameoverlayrenderer64.dll", runtime_patterns::hooks::create_swap_chain);
                if (ready) return true;
                status = "Required modules loaded; waiting for compatible Steam overlay entry points";
            }
            if (status != last_status) {
                error_logger::ErrorLogger::Get().Log("Startup", status.c_str(), 0);
                std::printf("[WAIT] %s\n", status.c_str());
                last_status = status;
            }
            if (GetTickCount64() - began >= 45000) {
                throw std::runtime_error(missing.empty() ?
                    "Rendering signatures unavailable after 45 seconds; the Steam overlay integration needs revalidation" :
                    runtime_startup::TimeoutReason(loaded));
            }
            Sleep(250);
        }
    }
}

DWORD WINAPI MainThread(LPVOID instance) {
    const bool owns_console = AllocConsole() != FALSE;
    FILE* output = nullptr;
    if (owns_console) {
        freopen_s(&output, "CONOUT$", "w", stdout);
        freopen_s(&output, "CONOUT$", "w", stderr);
        SetConsoleTitleA("NEPHILIMGATE - Startup");
    }
    RuntimeSession session;
    try {
        error_logger::ErrorLogger::Get().Log("Startup", "Starting x64 runtime", 0);
        if (WaitForRuntime()) {
            session.Start();
            if (owns_console) SetConsoleTitleA("NEPHILIMGATE - Ready");
            std::puts("[READY] INSERT = menu | F1 = debug console | END = unload");
            error_logger::ErrorLogger::Get().Log("Startup", "Runtime initialized; rendering awaits Present", 0);
            while (!(GetAsyncKeyState(VK_END) & 1)) Sleep(100);
        }
    }
    catch (const std::exception& error) {
        std::fprintf(stderr, "[ERROR] %s\n", error.what());
        try { error_logger::ErrorLogger::Get().LogException("Startup", error); } catch (...) {}
    }
    catch (...) {
        std::fputs("[ERROR] Unexpected runtime initialization failure\n", stderr);
        try { error_logger::ErrorLogger::Get().Log("Startup", "Unexpected failure", 1); } catch (...) {}
    }
    if (!session.Stop()) {
        std::fputs("[ERROR] Cleanup incomplete; module retained to keep live callbacks valid\n", stderr);
        return 1;
    }
    if (owns_console) FreeConsole();
    FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}

BOOL APIENTRY DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);
        const HANDLE thread = CreateThread(nullptr, 0, MainThread, module, 0, nullptr);
        if (!thread) return FALSE;
        CloseHandle(thread);
    }
    return TRUE;
}
