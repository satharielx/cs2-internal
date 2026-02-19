#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <Windows.h>
#include <mutex>

namespace error_logger {
    class ErrorLogger {
    public:
        static ErrorLogger& Get();
        
        void Log(const char* category, const char* message, int error_code = 0);
        void LogException(const char* context, const std::exception& ex);
        void LogWin32Error(const char* context, DWORD error_code);
        void LogHResult(const char* context, HRESULT hr);
        void LogPointerError(const char* context, uintptr_t ptr);
        void LogInitError(const char* system, const char* reason);
        void LogCrashInfo(const char* location, const char* details);
        
        // File logging
        void CreateLogFile();
        void WriteToFile(const char* message);
        void WriteToDebugOutput(const char* message);
        void WriteToConsoleWindow(const char* message);
        
        // Utility
        std::string GetTimestamp();
        std::string GetErrorString(DWORD error_code);
        
    private:
        ErrorLogger() = default;
        
        std::mutex log_mutex;
        std::ofstream log_file;
        bool file_open = false;
        const char* log_filename = "cs2_cheat_errors.log";
    };
}
