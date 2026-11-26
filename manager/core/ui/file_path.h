#pragma once
#include <filesystem>
#include <string_view>
namespace menu_advanced {
    inline std::filesystem::path PathFromUtf8(std::string_view value) {
        std::u8string text; text.reserve(value.size());
        for(unsigned char c:value) text.push_back(static_cast<char8_t>(c));
        return std::filesystem::path(text);
    }
}
