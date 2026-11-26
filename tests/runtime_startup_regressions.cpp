// Exercise the production startup session with owned fake services. No game loading.
#include "../manager/dllmain.cpp"
#include <vector>
#include <string_view>

namespace {
    std::vector<std::string> events;
    int fail_start = 0;
    int fail_stop = 0;
    bool state_available = false;
    void Enter(const char* name, int stage, bool stopping = false) {
        events.emplace_back(name);
        if ((stopping ? fail_stop : fail_start) == stage) throw std::runtime_error(name);
    }
    void Check(bool value, const char* message) { if (!value) throw std::runtime_error(message); }
}
namespace error_logger {
    ErrorLogger& ErrorLogger::Get() { static ErrorLogger logger; return logger; }
    void ErrorLogger::Log(const char*, const char*, int) {}
    void ErrorLogger::LogException(const char*, const std::exception&) {}
}
namespace interfaces {
    void create() { Enter("interfaces.start", 1); }
    void destroy() { Enter("interfaces.stop", 1, true); }
}
namespace hooks {
    void create() {
        try { Enter("hooks.start", 3); }
        catch (...) { events.emplace_back("hooks.rollback"); throw; }
    }
    void destroy() {
        Check(state_available, "Retain the state provider during frame-callback restoration");
        Enter("hooks.stop", 3, true);
    }
}
namespace game_state {
    void Start() { state_available = true; Enter("state.start", 2); }
    void Stop() { Enter("state.stop", 2, true); state_available = false; }
}
namespace features {
    void StartAimbotThread() { Enter("features.start", 4); }
    void StopAimbotThread() { Enter("features.stop", 4, true); }
}
namespace pattern_resolver { void Reset() { events.emplace_back("resolver.reset"); } }
namespace sdk {
    std::uint8_t* find_pattern(const char*, const char*) { return nullptr; }
    std::uint8_t* find_unique_pattern(const char*, const char*) { return nullptr; }
}

int main() {
    try {
        const auto all_loaded = [](const char*) { return true; };
        Check(runtime_startup::MissingModules(all_loaded).empty(), "Ready dependency set");
        const auto only_vulkan = [](const char* name) {
            return std::string_view(name) != "rendersystemdx11.dll" && std::string_view(name) != "gameoverlayrenderer64.dll";
        };
        const auto missing = runtime_startup::MissingModules(only_vulkan);
        Check(missing == "rendersystemdx11.dll, gameoverlayrenderer64.dll", "List only missing dependencies");
        const auto reason = runtime_startup::TimeoutReason(only_vulkan);
        Check(reason.find("Vulkan") != std::string::npos && reason.find("Steam overlay") != std::string::npos,
            "Explain actual unsupported dependencies");
        Check(runtime_startup::TimeoutReason(all_loaded).find("Vulkan") == std::string::npos,
            "Do not infer active Vulkan merely because its module is present");

        for (int stage = 0; stage <= 4; ++stage) {
            events.clear(); fail_start = stage; fail_stop = 0;
            RuntimeSession session;
            bool threw = false;
            try { session.Start(); } catch (const std::exception&) { threw = true; }
            Check(threw == (stage != 0), "Inject startup failure at each phase");
            Check(session.Stop(), "Rollback completes");
            std::vector<std::string> expected;
            const char* starts[]{"interfaces.start", "state.start", "hooks.start", "features.start"};
            const int attempted = stage ? stage : 4;
            for (int i = 0; i < attempted; ++i) expected.emplace_back(starts[i]);
            if (stage == 3) expected.emplace_back("hooks.rollback");
            if (attempted >= 4) expected.emplace_back("features.stop");
            if (stage == 0 || stage == 4) expected.emplace_back("hooks.stop");
            if (attempted >= 2) expected.emplace_back("state.stop");
            expected.emplace_back("interfaces.stop");
            expected.emplace_back("resolver.reset");
            Check(events == expected, "Rollback order and partial-start cleanup");
            events.clear();
            Check(session.Stop(), "Repeated stop succeeds");
            Check(events == std::vector<std::string>{"resolver.reset"}, "Stopped services are not destroyed twice");
        }
        events.clear(); fail_start = 0;
        RuntimeSession session;
        session.Start();
        events.clear(); fail_stop = 4;
        Check(!session.Stop(), "Failure to join a worker prevents teardown");
        Check(events == std::vector<std::string>{"features.stop"}, "Retain worker dependencies on join failure");
        events.clear(); fail_stop = 0;
        Check(session.Stop(), "Cleanup can retry after failure");
        Check(events == std::vector<std::string>{"features.stop", "hooks.stop", "state.stop", "interfaces.stop", "resolver.reset"},
            "Retry preserves the full remaining cleanup order");
        std::puts("PASS: startup dependencies, each partial failure, worker-first cleanup and cleanup retry");
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "FAIL: %s\n", error.what());
        return 1;
    }
}
