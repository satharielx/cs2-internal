#pragma once
#include "config.h"
#include "skins.h"
#include "config/value_codec.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <functional>
#include <set>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <type_traits>
namespace config_store {
    inline std::string status;
    inline void Initialize() {
        if(!entries.empty()) return;
#include "config_registry.inc"
    }
    inline void Reset() {
        std::lock_guard<std::recursive_mutex> lock(config::mutex); Initialize();
        for(auto& e:entries) e.set(e.defaults,true);
        skins::user_skins.clear(); skins::selected_glove_kit=0; skins::selected_knife_id=skins::WEAPON_KNIFE_BAYONET;
        status="Defaults restored; all features disabled";
    }
    inline bool Save(const std::filesystem::path& path) {
        std::lock_guard<std::recursive_mutex> lock(config::mutex); Initialize();
        auto temporary=path; temporary += L".tmp";
        std::ofstream out(temporary,std::ios::binary|std::ios::trunc);
        if(!out) {status="Cannot create configuration";return false;}
        out << "NEPHILIM_CONFIG 1\n";
        for(auto& e:entries) out << "setting " << std::quoted(e.key) << ' ' << std::quoted(e.get()) << '\n';
        out << "loadout " << skins::selected_knife_id << ' ' << skins::selected_glove_kit << '\n';
        for(auto& [id,s]:skins::user_skins) out << "skin " << id << ' ' << s.weapon_id << ' ' << s.paint_kit << ' ' << s.seed << ' ' << std::setprecision(9) << s.wear << ' ' << s.stattrak << ' ' << s.stattrak_count << ' ' << std::quoted(s.name_tag) << '\n';
        out.close(); if(!out || !MoveFileExW(temporary.c_str(),path.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)) {status="Configuration save failed";return false;}
        status="Configuration saved";return true;
    }
    inline bool Load(const std::filesystem::path& path) {
        std::lock_guard<std::recursive_mutex> lock(config::mutex); Initialize();
        auto fail=[](){status="Invalid configuration; current settings preserved";return false;};
        std::error_code ec; if(std::filesystem::file_size(path,ec)>1024*1024 || ec) {status="Cannot read configuration";return false;}
        std::ifstream in(path,std::ios::binary); std::string line;
        if(!std::getline(in,line)||line!="NEPHILIM_CONFIG 1") return fail();
        std::map<std::string,std::string> values; std::map<int,skins::PlayerSkinConfig> loadout;
        int knife=skins::WEAPON_KNIFE_BAYONET,glove=0; bool hasLoadout=false;
        while(std::getline(in,line)) {
            std::istringstream row(line); std::string kind; if(!(row>>kind)) continue;
            if(kind=="setting") {std::string key,value;if(!(row>>std::quoted(key)>>std::quoted(value))||!values.emplace(key,value).second)return fail();}
            else if(kind=="loadout") {if(hasLoadout||!(row>>knife>>glove)||!skins::IsKnife(knife)||glove<0||glove>100000) return fail();hasLoadout=true;}
            else if(kind=="skin") {
                int id; skins::PlayerSkinConfig s;
                if(!(row>>id>>s.weapon_id>>s.paint_kit>>s.seed>>s.wear>>s.stattrak>>s.stattrak_count>>std::quoted(s.name_tag)) ||
                    id<=0||id>65535||s.weapon_id<=0||s.weapon_id>65535||s.paint_kit<0||s.paint_kit>100000||s.seed<0||s.seed>1000||!std::isfinite(s.wear)||s.wear<0||s.wear>1||s.stattrak_count<0||s.name_tag.size()>128||!loadout.emplace(id,s).second) return fail();
            } else return fail();
            row>>std::ws;if(!row.eof())return fail();
        }
        if(in.bad()||!hasLoadout)return fail();
        for(auto& [key,value]:values) {
            auto e=std::find_if(entries.begin(),entries.end(),[&](const Entry& e){return e.key==key;});
            if(e==entries.end()||!e->set(value,false))return fail();
        }
        for(auto& e:entries) {auto value=values.find(e.key);e.set(value==values.end()?e.defaults:value->second,true);}
        skins::user_skins=std::move(loadout);skins::selected_knife_id=knife;skins::selected_glove_kit=glove;
        status="Configuration loaded";return true;
    }
}
