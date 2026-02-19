#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <queue>
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
        const std::vector<LogEntry>& GetLogs() const { return logs; }
        
        void Render();
        void ToggleVisibility() { is_visible = !is_visible; }
        bool IsVisible() const { return is_visible; }
        
    private:
        Console() = default;
        
        std::vector<LogEntry> logs;
        std::mutex log_mutex;
        static const int MAX_LOGS = 1000;
        bool is_visible = true;
        bool auto_scroll = true;
        
        const char* GetLevelString(LogLevel level) const;
        void GetLevelColor(LogLevel level, float& r, float& g, float& b, float& a) const;
    };
}
