#pragma once
#include <string>
#include <vector>
#include <map>

namespace skins_downloader {
    struct SkinData {
        int weapon_id;
        int paint_kit;
        std::string name;
        std::string rarity;
        bool stattrak_available;
    };

    class SkinsDownloader {
    public:
        static SkinsDownloader& Get();
        
        // Start downloading skins after hooks initialized
        void Initialize();
        bool IsInitialized() const { return initialized; }
        bool IsDownloading() const { return is_downloading; }
        
        // Get downloaded skins
        const std::vector<SkinData>& GetSkins() const { return skins; }
        int GetSkinCount() const { return static_cast<int>(skins.size()); }
        
        // Save to file
        void SaveToFile(const std::string& filepath);
        
    private:
        SkinsDownloader() = default;
        
        std::vector<SkinData> skins;
        bool initialized = false;
        bool is_downloading = false;
        
        std::string DownloadJSON(const std::string& url);
        void ParseSkinsJSON(const std::string& json);
    };
}
