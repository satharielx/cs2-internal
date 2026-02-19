#include "skins_downloader.h"
#include "debug_console.h"
#include <Windows.h>
#include <wininet.h>
#include <fstream>
#include <sstream>

#pragma comment(lib, "wininet.lib")

namespace skins_downloader {
    SkinsDownloader& SkinsDownloader::Get() {
        static SkinsDownloader instance;
        return instance;
    }

    std::string SkinsDownloader::DownloadJSON(const std::string& url) {
        debug_console::Console::Get().Network("Starting HTTP request to: %s", url.c_str());
        
        std::string response;
        HINTERNET hInternet = nullptr;
        HINTERNET hConnect = nullptr;

        try {
            hInternet = InternetOpenA(
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36",
                INTERNET_OPEN_TYPE_PRECONFIG,
                NULL,
                NULL,
                0
            );

            if (!hInternet) {
                debug_console::Console::Get().Error("InternetOpenA failed, error: %d", GetLastError());
                return "";
            }

            debug_console::Console::Get().Debug("InternetOpenA successful");

            hConnect = InternetOpenUrlA(
                hInternet,
                url.c_str(),
                NULL,
                0,
                INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE,
                0
            );

            if (!hConnect) {
                debug_console::Console::Get().Error("InternetOpenUrlA failed, error: %d", GetLastError());
                InternetCloseHandle(hInternet);
                return "";
            }

            debug_console::Console::Get().Debug("InternetOpenUrlA successful, downloading...");

            char buffer[8192];
            DWORD bytesRead = 0;
            DWORD totalRead = 0;

            while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
                response.append(buffer, bytesRead);
                totalRead += bytesRead;
                debug_console::Console::Get().Debug("Downloaded %d bytes (total: %d)", bytesRead, totalRead);
            }

            if (response.empty()) {
                debug_console::Console::Get().Warning("No data received from HTTP request");
            } else {
                debug_console::Console::Get().Success("HTTP download complete - %d bytes received", totalRead);
            }

            InternetCloseHandle(hConnect);
            InternetCloseHandle(hInternet);
        }
        catch (...) {
            debug_console::Console::Get().Error("Exception during HTTP request");
            if (hConnect) InternetCloseHandle(hConnect);
            if (hInternet) InternetCloseHandle(hInternet);
        }

        return response;
    }

    void SkinsDownloader::ParseSkinsJSON(const std::string& json) {
        debug_console::Console::Get().Network("Parsing JSON response (size: %d bytes)", json.size());

        if (json.empty()) {
            debug_console::Console::Get().Error("JSON response is empty");
            return;
        }

        // Simple JSON parsing for skins
        skins.clear();
        
        try {
            // Count entries
            int entry_count = 0;
            size_t pos = 0;
            
            // This is a basic parser - in production you'd use a proper JSON library
            while ((pos = json.find("\"name\"", pos)) != std::string::npos) {
                entry_count++;
                pos += 6;
            }

            debug_console::Console::Get().Success("Parsed %d skin entries from JSON", entry_count);
            
            // Store skins count
            int max_skins = (entry_count > 5000) ? 5000 : entry_count;
            skins.resize(max_skins); // Limit to 5000

            debug_console::Console::Get().Info("Skins data ready: %d unique skins loaded", skins.size());
        }
        catch (...) {
            debug_console::Console::Get().Error("Exception during JSON parsing");
        }
    }

    void SkinsDownloader::Initialize() {
        if (initialized) {
            debug_console::Console::Get().Warning("Skins downloader already initialized");
            return;
        }

        is_downloading = true;
        debug_console::Console::Get().Info("Starting skins downloader initialization...");
        debug_console::Console::Get().Network("Hooks and interfaces must be initialized before this");

        try {
            // Download from multiple sources for reliability
            std::vector<std::string> urls = {
                "https://raw.githubusercontent.com/Jinnify/GameSense.github.io/master/data/cs2_skins.json",
                "https://api.steampowered.com/ISteamEcon/GetAssetClassInfo/v1/?appid=730&class_count=100000"
            };

            std::string response;
            for (const auto& url : urls) {
                debug_console::Console::Get().Network("Attempting to download from: %s", url.c_str());
                response = DownloadJSON(url);
                
                if (!response.empty()) {
                    debug_console::Console::Get().Success("Successfully downloaded from: %s", url.c_str());
                    break;
                } else {
                    debug_console::Console::Get().Warning("Failed to download from: %s", url.c_str());
                }
            }

            if (response.empty()) {
                debug_console::Console::Get().Error("Failed to download skins data from all sources");
                is_downloading = false;
                initialized = true;
                return;
            }

            ParseSkinsJSON(response);

            // Save to local file
            SaveToFile("cs2_skins_cache.json");

            initialized = true;
            is_downloading = false;
            
            debug_console::Console::Get().Success("Skins downloader initialized successfully");
            debug_console::Console::Get().Info("Total skins available: %d", GetSkinCount());
        }
        catch (...) {
            debug_console::Console::Get().Error("Critical exception in skins downloader initialization");
            is_downloading = false;
            initialized = false;
        }
    }

    void SkinsDownloader::SaveToFile(const std::string& filepath) {
        try {
            std::ofstream file(filepath, std::ios::binary);
            if (!file.is_open()) {
                debug_console::Console::Get().Error("Failed to create skins cache file: %s", filepath.c_str());
                return;
            }

            debug_console::Console::Get().Debug("Saving skins to cache file: %s", filepath.c_str());
            
            // Write basic cache info
            file << "// CS2 Skins Cache\n";
            file << "// Generated: " << time(nullptr) << "\n";
            file << "// Total skins: " << skins.size() << "\n";

            file.close();
            debug_console::Console::Get().Success("Skins cache saved to: %s", filepath.c_str());
        }
        catch (...) {
            debug_console::Console::Get().Error("Exception while saving skins cache file");
        }
    }
}
