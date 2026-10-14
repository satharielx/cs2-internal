#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <atomic>
#include <cstdint>

struct ImGuiInputTextCallbackData;
#include <mutex>
#include <cstdarg>

namespace debug_console {
    enum LogLevel {
        LOG_INFO,
        LOG_SUCCESS,
        LOG_WARNING,
        LOG_ERROR,
        LOG_DEBUG,
        LOG_POINTER,
        LOG_NETWORK
    };

    struct LogEntry {
        LogLevel level;
        std::string message;
        time_t timestamp;
        std::string display;
        
        LogEntry(LogLevel lvl, const std::string& msg) 
            : level(lvl), message(msg), timestamp(time(nullptr)) {}
    };

    class Console {
    public:
        static Console& Get();
        
        void Log(LogLevel level, const char* format, ...);
        void Info(const char* format, ...);
        void Success(const char* format, ...);
        void Warning(const char* format, ...);
        void Error(const char* format, ...);
        void Debug(const char* format, ...);
        void Pointer(const char* name, uintptr_t address, uintptr_t value, bool valid);
        void Network(const char* format, ...);
        
        void Clear();
        std::vector<LogEntry> GetLogs() const { std::lock_guard<std::mutex> lock(log_mutex); return logs; }
        void ExecuteCommand(const std::string& command);
        
        void Render();
        void ToggleVisibility() { is_visible = !is_visible; }
        bool IsVisible() const { return is_visible; }
        
    private:
        Console() = default;
        
        std::vector<LogEntry> logs;
        mutable std::mutex log_mutex;
        std::atomic<unsigned long long> revision{0};
        unsigned long long rendered_revision = ~0ull;
        std::vector<LogEntry> display_logs;
        std::vector<int> visible_rows;
        std::vector<std::string> history;
        int history_position = -1;
        std::string history_draft;
        char command_buffer[512]{};
        char filter_buffer[256]{};
        bool filter_dirty = true;
        int level_filter = 0;
        static int InputCallback(ImGuiInputTextCallbackData* data);
        static const int MAX_LOGS = 1000;
        bool is_visible = true;
        bool auto_scroll = true;
        
        const char* GetLevelString(LogLevel level) const;
        void GetLevelColor(LogLevel level, float& r, float& g, float& b, float& a) const;
    };
}
