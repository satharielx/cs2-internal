#pragma once
#include "skins.h"
#include "config.h"
#include <commdlg.h>
#include <shellapi.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <set>
#include <algorithm>
#include <filesystem>
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "shell32.lib")

namespace loadout_web {
    inline std::string status;
    inline std::string Json(const std::string& value) {
        std::ostringstream out; out << '"';
        for (unsigned char c : value) {
            if (c == '"' || c == '\\') out << '\\' << c;
            else if (c < 32) out << "\\u" << std::hex << std::setw(4) << std::setfill('0') << int(c);
            else out << c;
        }
        out << '"'; return out.str();
    }
    inline bool PickFile(bool save, std::filesystem::path& path) {
        wchar_t file[MAX_PATH]{};
        OPENFILENAMEW dialog{}; dialog.lStructSize = sizeof(dialog);
        dialog.hwndOwner = GetActiveWindow(); dialog.lpstrFile = file; dialog.nMaxFile = MAX_PATH;
        dialog.lpstrFilter = save ? L"Catalog JSON\0*.json\0\0" : L"Loadout\0*.loadout\0\0";
        dialog.lpstrDefExt = save ? L"json" : L"loadout";
        dialog.Flags = OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | (save ? OFN_OVERWRITEPROMPT : OFN_FILEMUSTEXIST);
        if (!(save ? GetSaveFileNameW(&dialog) : GetOpenFileNameW(&dialog))) return false;
        path = file; return true;
    }
    inline void ExportCatalog() {
        std::filesystem::path path; if (!PickFile(true, path)) return;
        std::lock_guard<std::recursive_mutex> lock(config::mutex);
        std::ofstream out(path, std::ios::binary);
        out << "{\"version\":1,\"knife\":" << skins::selected_knife_id << ",\"glove\":" << skins::selected_glove_kit << ",\"skins\":[";
        bool comma = false;
        for (const auto& s : skins::skin_database) {
            if (comma) out << ','; comma = true;
            out << "{\"id\":" << s.weapon_id << ",\"paint\":" << s.paint_kit << ",\"name\":" << Json(s.name)
                << ",\"weapon\":" << Json(s.weapon_name) << ",\"stattrak\":" << (s.stattrak_available ? "true" : "false") << '}';
        }
        out << "],\"gloves\":["; comma = false;
        for (const auto& g : skins::glove_database) {
            if (comma) out << ','; comma = true;
            out << "{\"paint\":" << g.paint_kit << ",\"name\":" << Json(g.name) << '}';
        }
        out << "],\"loadout\":["; comma = false;
        for (const auto& [id, c] : skins::user_skins) {
            if (comma) out << ','; comma = true;
            out << "{\"id\":" << id << ",\"paint\":" << c.paint_kit << ",\"seed\":" << c.seed
                << ",\"wear\":" << (std::isfinite(c.wear) ? c.wear : 0.01f) << ",\"stattrak\":" << (c.stattrak ? "true" : "false")
                << ",\"count\":" << c.stattrak_count << ",\"name\":" << Json(c.name_tag) << '}';
        }
        out << "]}"; out.close();
        status = out.fail() ? "Could not write catalog." : "Catalog exported. Open it in the web editor.";
    }
    // Parse into temporary storage; malformed files never partially change a loadout.
    inline bool Parse(std::istream& in, std::map<int, skins::PlayerSkinConfig>& result, int& knife, int& glove) {
        std::string magic; int version;
        if (!(in >> magic >> version >> knife >> glove) || magic != "NEPHILIM_LOADOUT" || version != 1) return false;
        const std::set<int> knives{500,503,505,506,507,508,509,512,514,515,516,517,518,519,520,521,522,523,525,526};
        if (!knives.count(knife) || glove < 0) return false;
        if (glove && std::none_of(skins::glove_database.begin(), skins::glove_database.end(), [glove](const auto& g) { return g.paint_kit == glove; })) return false;
        std::map<int, skins::PlayerSkinConfig> parsed;
        while (in >> std::ws && in.peek() != EOF) {
            skins::PlayerSkinConfig c; int stat; std::string hex;
            if (!(in >> c.weapon_id >> c.paint_kit >> c.seed >> c.wear >> stat >> c.stattrak_count >> hex)) return false;
            if (parsed.size() >= 128 || parsed.count(c.weapon_id) || c.paint_kit < 0 || c.seed < 0 || c.seed > 1000 ||
                !std::isfinite(c.wear) || c.wear < 0 || c.wear > 1 || stat < 0 || stat > 1 || c.stattrak_count < 0 || c.stattrak_count > 99999) return false;
            const auto match = std::find_if(skins::skin_database.begin(), skins::skin_database.end(), [&](const auto& s) { return s.weapon_id == c.weapon_id && s.paint_kit == c.paint_kit; });
            if (c.paint_kit && match == skins::skin_database.end()) return false;
            if (!c.paint_kit && !knives.count(c.weapon_id) && std::none_of(skins::skin_database.begin(), skins::skin_database.end(), [&](const auto& s) { return s.weapon_id == c.weapon_id; })) return false;
            if (stat && (match == skins::skin_database.end() || !match->stattrak_available)) return false;
            if (hex != "-") {
                if (hex.size() > 62 || hex.size() % 2) return false;
                for (size_t i = 0; i < hex.size(); i += 2) {
                    auto digit = [](char ch) { return ch >= '0' && ch <= '9' ? ch - '0' : ch >= 'a' && ch <= 'f' ? ch - 'a' + 10 : -1; };
                    int hi = digit(hex[i]), lo = digit(hex[i + 1]);
                    if (hi < 0 || lo < 0 || hi * 16 + lo < 32) return false;
                    c.name_tag += static_cast<char>(hi * 16 + lo);
                }
            }
            c.stattrak = stat != 0; parsed.emplace(c.weapon_id, c);
        }
        if (in.bad()) return false;
        result = std::move(parsed); return true;
    }
    inline void ImportLoadout() {
        std::filesystem::path path; if (!PickFile(false, path)) return;
        std::error_code error; auto size = std::filesystem::file_size(path, error);
        if (error || size > 65536) { status = "Loadout is unreadable or too large."; return; }
        std::ifstream in(path); std::map<int, skins::PlayerSkinConfig> next; int knife = 0, glove = 0;
        std::lock_guard<std::recursive_mutex> lock(config::mutex);
        if (!in || !Parse(in, next, knife, glove)) { status = "Invalid loadout. Export the current catalog and try again."; return; }
        skins::user_skins = std::move(next); skins::selected_knife_id = knife; skins::selected_glove_kit = glove;
        status = "Loadout imported. Enable Skin Changer to apply.";
    }
    inline void OpenEditor() {
        HMODULE module{}; wchar_t path[MAX_PATH]{};
        if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCWSTR>(&OpenEditor), &module) || !GetModuleFileNameW(module, path, MAX_PATH)) { status = "Cannot locate editor."; return; }
        auto editor = std::filesystem::path(path).parent_path() / L"ui" / L"index.html";
        if (reinterpret_cast<INT_PTR>(ShellExecuteW(nullptr, L"open", editor.c_str(), nullptr, nullptr, SW_SHOWNORMAL)) <= 32)
            status = "Cannot open editor. Keep the ui folder next to the DLL.";
    }
}
