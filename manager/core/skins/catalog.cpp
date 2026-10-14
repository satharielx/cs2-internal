#include "catalog_sources.h"
#include "../skins.h"
#include "../debug_console.h"
#include <wininet.h>
#include <algorithm>
#include <cctype>
#pragma comment(lib, "wininet.lib")

namespace skins {
    // Http.
    static std::string HTTPGet(const std::string& url) {
        std::string r;
        HINTERNET h = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (!h) return r;
        DWORD timeout = 5000;
        InternetSetOptionA(h, INTERNET_OPTION_CONNECT_TIMEOUT, &timeout, sizeof(timeout));
        InternetSetOptionA(h, INTERNET_OPTION_RECEIVE_TIMEOUT, &timeout, sizeof(timeout));
        DWORD flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE;
        if (url.find("https://") == 0) flags |= INTERNET_FLAG_SECURE;
        HINTERNET hu = InternetOpenUrlA(h, url.c_str(), NULL, 0, flags, 0);
        if (hu) {
            char buf[8192]; DWORD rd = 0;
            while (r.size() < 16 * 1024 * 1024 && InternetReadFile(hu, buf, sizeof(buf) - 1, &rd) && rd > 0) r.append(buf, rd);
            InternetCloseHandle(hu);
        }
        InternetCloseHandle(h);
        return r;
    }

    static int ExtractInt(const std::string& obj, const std::string& key) {
        auto s = "\"" + key + "\":";
        auto p = obj.find(s); if (p == std::string::npos) return 0;
        p += s.size();
        while (p < obj.size() && !std::isdigit(obj[p]) && obj[p] != '-') p++;
        if (p < obj.size() && obj[p] == '"') p++;
        auto e = p;
        while (e < obj.size() && std::isdigit(obj[e])) e++;
        if (e > p) { try { return std::stoi(obj.substr(p, e - p)); } catch (...) {} }
        return 0;
    }

    static std::string ExtractStr(const std::string& obj, const std::string& key) {
        auto s = "\"" + key + "\":";
        auto p = obj.find(s); if (p == std::string::npos) return "";
        p += s.size();
        while (p < obj.size() && obj[p] != '"') p++;
        if (p >= obj.size()) return "";
        p++;
        auto e = obj.find('"', p);
        return (e != std::string::npos) ? obj.substr(p, e - p) : "";
    }

    // Skin database.
    void LoadSkinsFromAPI() {
        skin_database.clear(); skins_by_weapon.clear(); glove_database.clear();
        debug_console::Console::Get().Success("[SKINS] Fetching skin database...");
        try {
            auto data = HTTPGet(skin_catalog_sources::weapon_catalog);
            size_t pos = 0;
            while ((pos = data.find('{', pos)) != std::string::npos) {
                auto end = data.find('}', pos); if (end == std::string::npos) break;
                auto obj = data.substr(pos, end - pos + 1);
                int  wid = ExtractInt(obj, "weapon_defindex");
                int  pk = ExtractInt(obj, "paint");
                auto pn = ExtractStr(obj, "paint_name");
                if (wid > 0 && pk > 0 && !pn.empty()) {
                    auto sn = pn;
                    auto wd = GetWeaponName(wid);
                    auto sep = pn.find(" | ");
                    if (sep != std::string::npos) sn = pn.substr(sep + 3);
                    SkinInfo sk(pk, sn, wd, wid, RARITY_COMMON, true);
                    skin_database.push_back(sk);
                    skins_by_weapon[wd].push_back(sk);
                }
                pos = end + 1;
            }
            debug_console::Console::Get().Success("[SKINS] Loaded %zu weapon skins", skin_database.size());

            auto gdata = HTTPGet(skin_catalog_sources::glove_catalog);
            pos = 0;
            while ((pos = gdata.find('{', pos)) != std::string::npos) {
                auto end = gdata.find('}', pos); if (end == std::string::npos) break;
                auto obj = gdata.substr(pos, end - pos + 1);
                int  pk = ExtractInt(obj, "paint");
                auto pn = ExtractStr(obj, "paint_name");
                if (pk > 0 && !pn.empty()) {
                    auto gn = pn;
                    auto sep = pn.find(" | ");
                    if (sep != std::string::npos) gn = pn.substr(sep + 3);
                    glove_database.push_back(GloveInfo(pk, gn, ExtractInt(obj, "weapon_defindex")));
                }
                pos = end + 1;
            }
            debug_console::Console::Get().Success("[SKINS] Loaded %zu glove skins", glove_database.size());
            if (skin_database.empty() && glove_database.empty()) InitializeSkinDatabase();
        }
        catch (...) {
            debug_console::Console::Get().Error("[SKINS] API failed, using fallback");
            InitializeSkinDatabase();
        }
    }

    void InitializeSkinDatabase() {
        skin_database.clear(); skins_by_weapon.clear(); glove_database.clear();
        skin_database.push_back(SkinInfo(282, "Redline", "AK-47", WEAPON_AK47, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(180, "Vulcan", "AK-47", WEAPON_AK47, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(344, "Dragon Lore", "AWP", WEAPON_AWP, RARITY_CONTRABAND, true));
        skin_database.push_back(SkinInfo(279, "Asiimov", "AWP", WEAPON_AWP, RARITY_ANCIENT, true));
        skin_database.push_back(SkinInfo(309, "Howl", "M4A4", WEAPON_M4A1, RARITY_CONTRABAND, true));
        skin_database.push_back(SkinInfo(360, "Cyrex", "M4A1-S", WEAPON_M4A1_SILENCER, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(37, "Blaze", "Desert Eagle", WEAPON_DEAGLE, RARITY_ANCIENT, false));
        skin_database.push_back(SkinInfo(277, "Orion", "USP-S", WEAPON_USP_SILENCER, RARITY_LEGENDARY, true));
        skin_database.push_back(SkinInfo(38, "Fade", "Glock-18", WEAPON_GLOCK, RARITY_ANCIENT, false));
        for (const auto& s : skin_database) skins_by_weapon[s.weapon_name].push_back(s);
        glove_database.push_back(GloveInfo(10018, "Sport Gloves | Superconductor", 5030));
        glove_database.push_back(GloveInfo(10037, "Sport Gloves | Pandora's Box", 5030));
        glove_database.push_back(GloveInfo(10048, "Sport Gloves | Vice", 5030));
    }

    bool IsDatabaseLoaded() { return !skin_database.empty() || !glove_database.empty(); }

}
