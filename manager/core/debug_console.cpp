#include "debug_console.h"
#include "../external/imgui/imgui.h"
#include <cstdarg>
#include <cstdio>
#include <algorithm>
#include <Windows.h>
#include <iomanip>
#include <sstream>

namespace debug_console {
    Console& Console::Get() {
        static Console instance;
        return instance;
    }

    const char* Console::GetLevelString(LogLevel level) const {
        switch (level) {
            case LOG_INFO:    return "[INFO]";
            case LOG_SUCCESS: return "[?]";
            case LOG_WARNING: return "[!]";
            case LOG_ERROR:   return "[?]";
            case LOG_DEBUG:   return "[DBG]";
            case LOG_POINTER: return "[PTR]";
            case LOG_NETWORK: return "[NET]";
            default:          return "[?]";
        }
    }

    void Console::GetLevelColor(LogLevel level, float& r, float& g, float& b, float& a) const {
        a = 1.0f;
        switch (level) {
            case LOG_INFO:
                r = 0.6f; g = 0.8f; b = 1.0f;
                break;
            case LOG_SUCCESS:
                r = 0.2f; g = 1.0f; b = 0.4f;
                break;
            case LOG_WARNING:
                r = 1.0f; g = 0.9f; b = 0.2f;
                break;
            case LOG_ERROR:
                r = 1.0f; g = 0.3f; b = 0.3f;
                break;
            case LOG_DEBUG:
                r = 0.7f; g = 0.7f; b = 0.7f;
                break;
            case LOG_POINTER:
                r = 0.8f; g = 0.6f; b = 1.0f;
                break;
            case LOG_NETWORK:
                r = 1.0f; g = 0.7f; b = 0.2f;
                break;
            default:
                r = 1.0f; g = 1.0f; b = 1.0f;
        }
    }

    void Console::Log(LogLevel level, const char* format, ...) {
        if (!format) return;

        va_list args;
        va_start(args, format);

        char buffer[1024];
        vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, format, args);
        va_end(args);

        {
            std::lock_guard<std::mutex> lock(log_mutex);
            logs.emplace_back(level, buffer);

            // Keep only recent logs
            if (logs.size() > MAX_LOGS) {
                logs.erase(logs.begin());
            }
        }

        // Also print to debug output and native console
        const char* prefix = GetLevelString(level);
        OutputDebugStringA(prefix);
        OutputDebugStringA(" ");
        OutputDebugStringA(buffer);
        OutputDebugStringA("\n");
        printf("%s %s\n", prefix, buffer);
    }

    void Console::Info(const char* format, ...) {
        if (!format) return;
        va_list args;
        va_start(args, format);
        char buffer[1024];
        vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, format, args);
        va_end(args);
        Log(LOG_INFO, buffer);
    }

    void Console::Success(const char* format, ...) {
        if (!format) return;
        va_list args;
        va_start(args, format);
        char buffer[1024];
        vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, format, args);
        va_end(args);
        Log(LOG_SUCCESS, buffer);
    }

    void Console::Warning(const char* format, ...) {
        if (!format) return;
        va_list args;
        va_start(args, format);
        char buffer[1024];
        vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, format, args);
        va_end(args);
        Log(LOG_WARNING, buffer);
    }

    void Console::Error(const char* format, ...) {
        if (!format) return;
        va_list args;
        va_start(args, format);
        char buffer[1024];
        vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, format, args);
        va_end(args);
        Log(LOG_ERROR, buffer);
    }

    void Console::Debug(const char* format, ...) {
        if (!format) return;
        va_list args;
        va_start(args, format);
        char buffer[1024];
        vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, format, args);
        va_end(args);
        Log(LOG_DEBUG, buffer);
    }

    void Console::Pointer(const char* name, uintptr_t address, uintptr_t value, bool valid) {
        std::stringstream ss;
        ss << std::hex << std::uppercase;
        
        if (valid) {
            ss << name << " - Address: 0x" << address << " | Value: 0x" << value << " | Status: VALID";
            Log(LOG_POINTER, ss.str().c_str());
        } else {
            ss << name << " - Address: 0x" << address << " | Value: 0x" << value << " | Status: INVALID";
            Log(LOG_POINTER, ss.str().c_str());
        }
    }

    void Console::Network(const char* format, ...) {
        if (!format) return;
        va_list args;
        va_start(args, format);
        char buffer[1024];
        vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, format, args);
        va_end(args);
        Log(LOG_NETWORK, buffer);
    }

    void Console::Clear() {
        std::lock_guard<std::mutex> lock(log_mutex);
        logs.clear();
    }

    void Console::Render() {
        if (!is_visible) return;

        ImGui::SetNextWindowSize(ImVec2(1200, 400), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin("Advanced Debug Console", &is_visible, ImGuiWindowFlags_NoCollapse)) {
            ImGui::End();
            return;
        }

        // Control buttons
        if (ImGui::Button("Clear", ImVec2(100, 0))) {
            Clear();
        }
        ImGui::SameLine();
        ImGui::Checkbox("Auto-scroll", &auto_scroll);
        ImGui::SameLine();
        ImGui::Text("| Total Logs: %zu / %d", logs.size(), MAX_LOGS);
        ImGui::Separator();

        // Log display
        {
            std::lock_guard<std::mutex> lock(log_mutex);

            ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

            for (const auto& entry : logs) {
                float r, g, b, a;
                GetLevelColor(entry.level, r, g, b, a);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(r, g, b, a));

                // Format timestamp
                time_t t = entry.timestamp;
                struct tm timeinfo;
                localtime_s(&timeinfo, &t);
                char time_str[16];
                strftime(time_str, sizeof(time_str), "%H:%M:%S", &timeinfo);

                ImGui::TextWrapped("[%s] %s %s", time_str, GetLevelString(entry.level), entry.message.c_str());

                ImGui::PopStyleColor();
            }

            if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                ImGui::SetScrollHereY(1.0f);
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }
}
