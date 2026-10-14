#pragma once
#include <array>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <functional>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <type_traits>
#include <initializer_list>

// Parse and format settings without touching files or game state.
namespace config_store {
    struct Entry { std::string key, defaults; std::function<std::string()> get; std::function<bool(const std::string&, bool)> set; };
    inline std::vector<Entry> entries;
    template<class T> std::string Encode(T value) { std::ostringstream out; out << std::setprecision(9) << value; return out.str(); }
    template<class T> void Add(const char* key, T& target, T initial) {
        entries.push_back({key, Encode(initial), [&target]{return Encode(target);}, [&target](const std::string& text, bool apply){
            std::istringstream in(text); T value{}; if (!(in >> value)) return false;
            in >> std::ws; if (!in.eof()) return false;
            if constexpr (std::is_floating_point_v<T>) if (!std::isfinite(value) || std::abs(value)>1000000) return false;
            if (apply) target=value; return true;
        }});
    }
    template<class T, size_t N> void Add(const char* key, T (&target)[N], std::initializer_list<std::type_identity_t<T>> initial) {
        std::array<T,N> defaults{}; std::copy(initial.begin(),initial.end(),defaults.begin());
        auto encode=[](const auto& values){std::ostringstream out; out << std::setprecision(9); for(auto value:values) out << value << ' '; return out.str();};
        entries.push_back({key,encode(defaults),[&target,encode]{return encode(target);},[&target](const std::string& text,bool apply){
            std::array<T,N> values{}; std::istringstream in(text);
            for(auto& value:values) { if(!(in>>value)) return false; if constexpr(std::is_floating_point_v<T>) if(!std::isfinite(value)||std::abs(value)>1000000) return false; }
            in>>std::ws; if(!in.eof()) return false; if(apply) std::copy(values.begin(),values.end(),target); return true;
        }});
    }
    template<size_t N> void Add(const char* key, char (&target)[N], const char* initial) {
        entries.push_back({key,initial,[&target]{return std::string(target);},[&target](const std::string& value,bool apply){
            if(value.size()>=N || value.find('\0')!=std::string::npos) return false;
            if(apply) {std::memset(target,0,N);std::memcpy(target,value.data(),value.size());} return true;
        }});
    }
}
