/*
Module Name: Error logging utilities and diagnostics
Authors: sathariel, martinmarinov
Product: Nephilimgate Multicheat
Tools used: imgui, a2x-cs2dumper
© 2026 sathariel & martinmarinov
*/

#include "error_logger.h"
#include <iostream>
#include <iomanip>

namespace error_logger {
    ErrorLogger& ErrorLogger::Get() {
        static ErrorLogger instance;
        static bool initialized = false;
        if (!initialized) {
            instance.CreateLogFile();
            initialized = true;
        }
        return instance;
    }

    void ErrorLogger::CreateLogFile() {
        std::lock_guard<std::mutex> lock(log_mutex);
        try {
            log_file.open(log_filename, std::ios::app);
            if (log_file.is_open()) {
                file_open = true;
                std::string init_msg = "======= CS2 Cheat Error Log Started at " + GetTimestamp() + " =======\n";
                log_file << init_msg;
                log_file.flush();
                OutputDebugStringA(init_msg.c_str());
            }
        }
        catch (...) {
            OutputDebugStringA("ERROR_LOGGER: Failed to open log file\n");
        }
    }

    std::string ErrorLogger::GetTimestamp() {
        time_t now = time(nullptr);
        struct tm timeinfo;
        localtime_s(&timeinfo, &now);
        
        std::ostringstream oss;
        oss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string ErrorLogger::GetErrorString(DWORD error_code) {
        char* buffer = nullptr;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            error_code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&buffer,
            0,
            nullptr
        );

        std::string result = buffer ? buffer : "Unknown error";
        if (buffer) LocalFree(buffer);
        return result;
    }

    void ErrorLogger::Log(const char* category, const char* message, int error_code) {
        if (!category || !message) return;

        std::lock_guard<std::mutex> lock(log_mutex);

        std::string timestamp = GetTimestamp();
        std::ostringstream oss;
        oss << "[" << timestamp << "] [" << category << "] " << message;
        if (error_code != 0) {
            oss << " (Code: " << error_code << ")";
        }
        oss << "\n";

        std::string log_msg = oss.str();

        WriteToDebugOutput(log_msg.c_str());
        WriteToFile(log_msg.c_str());
        WriteToConsoleWindow(log_msg.c_str());
    }

    void ErrorLogger::LogException(const char* context, const std::exception& ex) {
        if (!context) return;

        std::lock_guard<std::mutex> lock(log_mutex);

        std::string timestamp = GetTimestamp();
        std::ostringstream oss;
        oss << "[" << timestamp << "] [EXCEPTION] " << context << ": " << ex.what() << "\n";

        std::string log_msg = oss.str();
        WriteToDebugOutput(log_msg.c_str());
        WriteToFile(log_msg.c_str());
        WriteToConsoleWindow(log_msg.c_str());
    }

    void ErrorLogger::LogWin32Error(const char* context, DWORD error_code) {
        if (!context) return;

        std::lock_guard<std::mutex> lock(log_mutex);

        std::string error_str = GetErrorString(error_code);
        std::string timestamp = GetTimestamp();
        std::ostringstream oss;
        oss << "[" << timestamp << "] [WIN32_ERROR] " << context << ": " << error_str 
            << " (Code: 0x" << std::hex << error_code << std::dec << ")\n";

        std::string log_msg = oss.str();
        WriteToDebugOutput(log_msg.c_str());
        WriteToFile(log_msg.c_str());
        WriteToConsoleWindow(log_msg.c_str());
    }

    void ErrorLogger::LogHResult(const char* context, HRESULT hr) {
        if (!context) return;

        std::lock_guard<std::mutex> lock(log_mutex);

        std::string timestamp = GetTimestamp();
        std::ostringstream oss;
        oss << "[" << timestamp << "] [HRESULT_ERROR] " << context << ": 0x" 
            << std::hex << hr << std::dec << "\n";

        std::string log_msg = oss.str();
        WriteToDebugOutput(log_msg.c_str());
        WriteToFile(log_msg.c_str());
        WriteToConsoleWindow(log_msg.c_str());
    }

    void ErrorLogger::LogPointerError(const char* context, uintptr_t ptr) {
        if (!context) return;

        std::lock_guard<std::mutex> lock(log_mutex);

        std::string timestamp = GetTimestamp();
        std::ostringstream oss;
        oss << "[" << timestamp << "] [POINTER_ERROR] " << context << ": Invalid pointer 0x"
            << std::hex << ptr << std::dec << "\n";

        std::string log_msg = oss.str();
        WriteToDebugOutput(log_msg.c_str());
        WriteToFile(log_msg.c_str());
        WriteToConsoleWindow(log_msg.c_str());
    }

    void ErrorLogger::LogInitError(const char* system, const char* reason) {
        if (!system || !reason) return;

        std::lock_guard<std::mutex> lock(log_mutex);

        std::string timestamp = GetTimestamp();
        std::ostringstream oss;
        oss << "[" << timestamp << "] [INIT_ERROR] System: " << system << " | Reason: " << reason << "\n";

        std::string log_msg = oss.str();
        WriteToDebugOutput(log_msg.c_str());
        WriteToFile(log_msg.c_str());
        WriteToConsoleWindow(log_msg.c_str());
    }

    void ErrorLogger::LogCrashInfo(const char* location, const char* details) {
        if (!location || !details) return;

        std::lock_guard<std::mutex> lock(log_mutex);

        std::string timestamp = GetTimestamp();
        std::ostringstream oss;
        oss << "[" << timestamp << "] [CRASH_WARNING] Location: " << location << " | Details: " << details << "\n";

        std::string log_msg = oss.str();
        WriteToDebugOutput(log_msg.c_str());
        WriteToFile(log_msg.c_str());
        WriteToConsoleWindow(log_msg.c_str());
    }

    void ErrorLogger::WriteToDebugOutput(const char* message) {
        if (!message) return;
        try {
            OutputDebugStringA(message);
        }
        catch (...) {
        }
    }

    void ErrorLogger::WriteToFile(const char* message) {
        if (!message) return;
        try {
            if (!file_open) {
                CreateLogFile();
            }
            if (log_file.is_open()) {
                log_file << message;
                log_file.flush();
            }
        }
        catch (...) {
        }
    }

    void ErrorLogger::WriteToConsoleWindow(const char* message) {
        if (!message) return;
        try {
            HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
            if (console_handle != INVALID_HANDLE_VALUE) {
                DWORD written = 0;
                WriteConsoleA(console_handle, message, strlen(message), &written, nullptr);
            }
        }
        catch (...) {
        }
    }
}
