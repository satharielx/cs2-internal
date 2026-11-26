#pragma once
#include "skins.h"
#include "config.h"
#include "skin_hud.h"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <algorithm>

namespace inventory {
    enum class NativeState { Local, AddPending, Published, RemovePending, Failed };
    struct Item {
        uint64_t id=0;
        skins::PlayerSkinConfig skin;
        NativeState native=NativeState::Local;
        uint64_t native_id=0;
        uintptr_t native_object=0,native_inventory=0,native_cache=0;
        std::string message;
    };
    inline std::vector<Item> items;
    inline uint64_t next_id=1;
    inline std::string status="Local cosmetic collection";
    inline constexpr size_t maximum_items=512;
    inline bool Valid(const skins::PlayerSkinConfig& s) {
        return s.weapon_id>0&&s.weapon_id<=65535&&s.paint_kit>=0&&s.paint_kit<=100000&&s.seed>=0&&s.seed<=1000&&
            std::isfinite(s.wear)&&s.wear>=0&&s.wear<=1&&s.stattrak_count>=0&&s.name_tag.size()<=128&&
            s.name_tag.find_first_of("\r\n\0",0,3)==std::string::npos;
    }
    inline uint64_t Add(const skins::PlayerSkinConfig& skin) {
        std::lock_guard<std::recursive_mutex> lock(config::mutex);
        if(!Valid(skin)||items.size()>=maximum_items||!next_id||next_id==UINT64_MAX) {status="Invalid item or collection limit reached";return 0;}
        const auto id=next_id++;Item item;item.id=id;item.skin=skin;items.push_back(std::move(item));
        status="Item added to local inventory";return id;
    }
    inline bool Equip(uint64_t id) {
        std::lock_guard<std::recursive_mutex> lock(config::mutex);
        const auto found=std::find_if(items.begin(),items.end(),[&](const Item& i){return i.id==id;});
        if(found==items.end()||!Valid(found->skin)) return false;
        skins::user_skins[found->skin.weapon_id]=found->skin;
        if(skins::IsKnife(found->skin.weapon_id)) {skins::selected_knife_id=found->skin.weapon_id;skins::ApplyKnife();}
        if(found->skin.weapon_id>=5027&&found->skin.weapon_id<=5035) skins::selected_glove_kit=found->skin.paint_kit;
        config::skin_changer::enabled=true;skin_hud::Request();status="Local loadout equipped";return true;
    }
    inline bool Save(const std::filesystem::path& path) {
        std::lock_guard<std::recursive_mutex> lock(config::mutex);
        auto temporary=path;temporary+=L".tmp";
        std::ofstream out(temporary,std::ios::binary|std::ios::trunc);
        if(!out) {status="Cannot create inventory file";return false;}
        out<<"NEPHILIM_INVENTORY 1\n";
        for(const auto& item:items) {const auto& s=item.skin;out<<item.id<<' '<<s.weapon_id<<' '<<s.paint_kit<<' '<<s.seed<<' '
            <<std::setprecision(9)<<s.wear<<' '<<s.stattrak<<' '<<s.stattrak_count<<' '<<std::quoted(s.name_tag)<<'\n';}
        out.close();
        if(!out||!MoveFileExW(temporary.c_str(),path.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)) {status="Inventory save failed";return false;}
        status="Local inventory saved";return true;
    }
    inline bool Load(const std::filesystem::path& path) {
        std::lock_guard<std::recursive_mutex> lock(config::mutex);
        if(std::any_of(items.begin(),items.end(),[](const Item& i){return i.native_object||i.native==NativeState::AddPending;})) {
            status="Remove native items before replacing the local collection";return false;
        }
        auto fail=[](){status="Invalid inventory file; collection preserved";return false;};
        std::error_code ec;if(std::filesystem::file_size(path,ec)>1024*1024||ec) {status="Cannot read inventory file";return false;}
        std::ifstream in(path,std::ios::binary);std::string line;
        if(!std::getline(in,line)||line!="NEPHILIM_INVENTORY 1") return fail();
        std::vector<Item> next;uint64_t maximum=0;
        while(std::getline(in,line)) {
            if(line.empty()) continue;
            Item item;auto& s=item.skin;std::istringstream row(line);int stat=0;
            if(!(row>>item.id>>s.weapon_id>>s.paint_kit>>s.seed>>s.wear>>stat>>s.stattrak_count>>std::quoted(s.name_tag))||
                !item.id||item.id==UINT64_MAX||(stat!=0&&stat!=1)||!Valid(s)||next.size()>=maximum_items||
                std::any_of(next.begin(),next.end(),[&](const Item& i){return i.id==item.id;})) return fail();
            row>>std::ws;if(!row.eof()) return fail();
            s.stattrak=stat!=0;maximum=(std::max)(maximum,item.id);next.push_back(std::move(item));
        }
        if(in.bad()) return fail();
        items=std::move(next);next_id=maximum+1;status="Local inventory loaded; native publication is explicit";return true;
    }
    inline void Publish(uint64_t id) {
        std::lock_guard<std::recursive_mutex> lock(config::mutex);
        for(auto& item:items) if(item.id==id&&!item.native_object) {item.native=NativeState::AddPending;item.message="Waiting for game-thread native publication";}
    }
    inline void Remove(uint64_t id) {
        std::lock_guard<std::recursive_mutex> lock(config::mutex);
        const auto found=std::find_if(items.begin(),items.end(),[&](const Item& i){return i.id==id;});
        if(found==items.end()) return;
        if(found->native_object) {found->native=NativeState::RemovePending;found->message="Waiting for native removal";}
        else items.erase(found);
    }
    void OnFrameStage(int stage);
    void Shutdown();
}
