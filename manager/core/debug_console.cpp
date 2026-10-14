#include "debug_console.h"
#include "../external/imgui/imgui.h"
#include <cstdarg>
#include <cstdio>
#include <algorithm>
#include <Windows.h>
#include <iomanip>
#include <sstream>
#include "ui/theme.h"
#include <cstring>
#include "config_store.h"
#include <fstream>

namespace debug_console {
    Console& Console::Get() {
        static Console instance;
        return instance;
    }

    const char* Console::GetLevelString(LogLevel level) const {
        switch (level) {
            case LOG_INFO:    return "[INFO]";
            case LOG_SUCCESS: return "[OK]";
            case LOG_WARNING: return "[!]";
            case LOG_ERROR:   return "[ERROR]";
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

        std::string file_stamp;
        {
            std::lock_guard<std::mutex> lock(log_mutex);
            logs.emplace_back(level, buffer);
            auto& entry = logs.back();
            tm local{};
            localtime_s(&local, &entry.timestamp);
            char stamp[16]{};
            strftime(stamp, sizeof(stamp), "%H:%M:%S", &local);
            file_stamp = stamp;
            entry.display = std::string(stamp) + "  " + GetLevelString(level) + "  " + buffer;
            std::replace(entry.display.begin(), entry.display.end(), '\n', ' ');
            std::replace(entry.display.begin(), entry.display.end(), '\r', ' ');
            ++revision;

            // Keep only recent logs
            if (logs.size() > MAX_LOGS) {
                logs.erase(logs.begin());
            }
        }

        // Write to the debugger and console too.
        const char* prefix = GetLevelString(level);
        OutputDebugStringA(prefix);
        OutputDebugStringA(" ");
        OutputDebugStringA(buffer);
        OutputDebugStringA("\n");
        printf("%s %s\n", prefix, buffer);

        // Keep the file open and serialize sink writes separately from UI snapshots.
        static std::mutex file_mutex;
        std::lock_guard<std::mutex> file_lock(file_mutex);
        static std::ofstream file("manager_console.log", std::ios::app);
        if (file) {
            file << file_stamp << ' ' << prefix << ' ' << buffer << '\n';
            if (level == LOG_ERROR || level == LOG_WARNING) file.flush();
        }
    }

    void Console::Info(const char* format, ...) {
        if (!format) return;
        va_list args;
        va_start(args, format);
        char buffer[1024];
        vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, format, args);
        va_end(args);
        Log(LOG_INFO, "%s", buffer);
    }

    void Console::Success(const char* format, ...) {
        if (!format) return;
        va_list args;
        va_start(args, format);
        char buffer[1024];
        vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, format, args);
        va_end(args);
        Log(LOG_SUCCESS, "%s", buffer);
    }

    void Console::Warning(const char* format, ...) {
        if (!format) return;
        va_list args;
        va_start(args, format);
        char buffer[1024];
        vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, format, args);
        va_end(args);
        Log(LOG_WARNING, "%s", buffer);
    }

    void Console::Error(const char* format, ...) {
        if (!format) return;
        va_list args;
        va_start(args, format);
        char buffer[1024];
        vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, format, args);
        va_end(args);
        Log(LOG_ERROR, "%s", buffer);
    }

    void Console::Debug(const char* format, ...) {
        if (!format) return;
        va_list args;
        va_start(args, format);
        char buffer[1024];
        vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, format, args);
        va_end(args);
        Log(LOG_DEBUG, "%s", buffer);
    }

    void Console::Pointer(const char* name, uintptr_t address, uintptr_t value, bool valid) {
        std::stringstream ss;
        ss << std::hex << std::uppercase;
        
        if (valid) {
            ss << name << " - Address: 0x" << address << " | Value: 0x" << value << " | Status: VALID";
            Log(LOG_POINTER, "%s", ss.str().c_str());
        } else {
            ss << name << " - Address: 0x" << address << " | Value: 0x" << value << " | Status: INVALID";
            Log(LOG_POINTER, "%s", ss.str().c_str());
        }
    }

    void Console::Network(const char* format, ...) {
        if (!format) return;
        va_list args;
        va_start(args, format);
        char buffer[1024];
        vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, format, args);
        va_end(args);
        Log(LOG_NETWORK, "%s", buffer);
    }

    void Console::Clear() {
        std::lock_guard<std::mutex> lock(log_mutex);
        logs.clear();
        ++revision;
    }


    void Console::ExecuteCommand(const std::string& input) {
        const auto first = input.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return;
        const std::string command = input.substr(first, input.find_last_not_of(" \t\r\n") - first + 1);
        if (history.empty() || history.back() != command) history.push_back(command);
        if (history.size() > 64) history.erase(history.begin());
        history_position = -1;
        history_draft.clear();
        Info("> %s", command.c_str());
        std::istringstream stream(command);
        std::string verb, argument;
        stream >> verb;
        std::getline(stream >> std::ws, argument);
        if (verb == "help") {
            Info("help | clear | status | fps | history | echo <text> | filter <text|off> | autoscroll <on|off> | config <save|load> <profile> | close");
            Info("Up/Down: command history. Tab: complete command. Filters support comma-separated terms and -exclusions.");
        } else if (verb == "clear") Clear();
        else if (verb == "close") is_visible = false;
        else if (verb == "echo") Info("%s", argument.c_str());
        else if (verb == "filter") {
            snprintf(filter_buffer, sizeof(filter_buffer), "%s", argument == "off" ? "" : argument.c_str());
            filter_dirty = true;
        } else if (verb == "autoscroll" && (argument == "on" || argument == "off")) {
            auto_scroll = argument == "on";
            Info("Auto-scroll %s", argument.c_str());
        } else if (verb == "config") {
            std::istringstream args(argument);
            std::string action, name, extra;
            args >> action >> name >> extra;
            if ((action != "save" && action != "load") || name.empty() || name.size() > 48 || !extra.empty() ||
                name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-") != std::string::npos) {
                Warning("Usage: config <save|load> <profile>. Use letters, digits, _ or - for the profile name.");
                return;
            }
            wchar_t local[MAX_PATH]{};
            const DWORD length = GetEnvironmentVariableW(L"LOCALAPPDATA", local, MAX_PATH);
            if (!length || length >= MAX_PATH) { Error("LOCALAPPDATA is unavailable."); return; }
            auto folder = std::filesystem::path(local) / "Nephilim" / "Configs";
            std::error_code error;
            if (action == "save") std::filesystem::create_directories(folder, error);
            if (error) { Error("Cannot create configuration directory."); return; }
            const auto path = folder / (name + ".cfg");
            std::lock_guard<std::recursive_mutex> lock(config::mutex);
            const bool ok = action == "save" ? config_store::Save(path) : config_store::Load(path);
            Log(ok ? LOG_SUCCESS : LOG_ERROR, "%s: %s", name.c_str(), config_store::status.c_str());
        } else if (verb == "history") {
            for (size_t i = 0; i < history.size(); ++i) Info("%zu  %s", i + 1, history[i].c_str());
        } else if (verb == "status") {
            Info("Log buffer: %zu / %d. History: %zu / 64. Auto-scroll: %s.", GetLogs().size(), MAX_LOGS, history.size(), auto_scroll ? "on" : "off");
        } else if (verb == "fps") {
            if (ImGui::GetCurrentContext()) Info("UI: %.1f FPS, %.2f ms/frame", ImGui::GetIO().Framerate, 1000.f / (std::max)(1.f, ImGui::GetIO().Framerate));
            else Warning("No active UI context.");
        } else Warning("Unknown command or invalid arguments. Type help.");
    }

    int Console::InputCallback(ImGuiInputTextCallbackData* data) {
        auto& console = *static_cast<Console*>(data->UserData);
        std::string replacement;
        if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory) {
            if (console.history.empty()) return 0;
            if (data->EventKey == ImGuiKey_UpArrow) {
                if (console.history_position == -1) {
                    console.history_draft = data->Buf;
                    console.history_position = static_cast<int>(console.history.size()) - 1;
                } else if (console.history_position > 0) --console.history_position;
            } else if (console.history_position >= 0 && ++console.history_position >= static_cast<int>(console.history.size())) {
                console.history_position = -1;
            }
            replacement = console.history_position < 0 ? console.history_draft : console.history[console.history_position];
        } else if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion) {
            if (std::strchr(data->Buf, ' ')) return 0;
            static constexpr const char* commands[] = {"help", "clear", "status", "fps", "history", "echo", "filter", "autoscroll", "config", "close"};
            int matches = 0;
            for (const char* candidate : commands) {
                if (std::strncmp(candidate, data->Buf, data->BufTextLen) != 0) continue;
                if (matches++ == 0) replacement = candidate;
                else {
                    size_t common = 0;
                    while (common < replacement.size() && candidate[common] == replacement[common]) ++common;
                    replacement.resize(common);
                }
            }
            if (!matches) return 0;
            if (matches == 1) replacement += ' ';
        } else return 0;
        data->DeleteChars(0, data->BufTextLen);
        data->InsertChars(0, replacement.c_str());
        return 0;
    }

    void Console::Render() {
        if (!is_visible) return;
        menu_advanced::WorkspaceTheme theme;
        ImGui::SetNextWindowSize(ImVec2(960, 460), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSizeConstraints({520, 280}, {1920, 1400});
        if (!ImGui::Begin("Console / Workspace", &is_visible, ImGuiWindowFlags_NoCollapse)) {
            ImGui::End();
            return;
        }
        // Only copy new rows. Never hold the producer lock while drawing.
        if (revision.load() != rendered_revision) {
            std::unique_lock<std::mutex> lock(log_mutex, std::try_to_lock);
            if (lock.owns_lock()) {
                display_logs = logs;
                rendered_revision = revision.load();
                filter_dirty = true;
            }
        }
        ImGui::SetNextItemWidth((std::max)(120.f, ImGui::GetContentRegionAvail().x - 340.f));
        filter_dirty |= ImGui::InputTextWithHint("##log_filter", "Search logs...", filter_buffer, sizeof(filter_buffer));
        ImGui::SameLine();
        ImGui::SetNextItemWidth(110);
        filter_dirty |= ImGui::Combo("##level", &level_filter, "All levels\0Info\0Success\0Warning\0Error\0Debug\0Pointer\0Network\0");
        ImGui::SameLine();
        ImGui::Checkbox("Follow", &auto_scroll);
        ImGui::SameLine();
        if (ImGui::Button("Clear")) Clear();
        if (filter_dirty) {
            ImGuiTextFilter filter(filter_buffer);
            visible_rows.clear();
            for (int i = 0; i < static_cast<int>(display_logs.size()); ++i) {
                const auto& row = display_logs[i];
                if ((!level_filter || row.level == level_filter - 1) && filter.PassFilter(row.display.c_str())) visible_rows.push_back(i);
            }
            filter_dirty = false;
        }
        ImGui::TextDisabled("%zu matching / %zu retained    |    Type help for commands", visible_rows.size(), display_logs.size());
        ImGui::Separator();
        ImGui::BeginChild("LogRows", {0, -ImGui::GetFrameHeightWithSpacing()}, ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);
        const bool was_at_bottom = ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 1.f;
        ImGuiListClipper clipper;
        clipper.Begin(static_cast<int>(visible_rows.size()));
        while (clipper.Step()) {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
                const auto& row = display_logs[visible_rows[i]];
                float r, g, b, a;
                GetLevelColor(row.level, r, g, b, a);
                ImGui::PushStyleColor(ImGuiCol_Text, {r, g, b, a});
                ImGui::TextUnformatted(row.display.c_str());
                ImGui::PopStyleColor();
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::PushTextWrapPos(600);
                    ImGui::TextUnformatted(row.message.c_str());
                    ImGui::PopTextWrapPos();
                    ImGui::EndTooltip();
                }
            }
        }
        if (auto_scroll && was_at_bottom) ImGui::SetScrollHereY(1.f);
        ImGui::EndChild();
        ImGui::SetNextItemWidth(-1);
        if (ImGui::InputTextWithHint("##command", "> Enter command... (Tab completes, Up/Down recalls)", command_buffer, sizeof(command_buffer),
                ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackCompletion, InputCallback, this)) {
            ExecuteCommand(command_buffer);
            command_buffer[0] = 0;
            ImGui::SetKeyboardFocusHere(-1);
        }
        ImGui::End();
    }
}
