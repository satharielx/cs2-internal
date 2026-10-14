#include "../manager/core/config_store.h"
#include <cassert>
#include <iostream>
int main() {
    config_store::Reset();
    for(auto& e:config_store::entries) if(e.defaults=="0") assert(e.get()=="0");
    assert(!config::rcs::enabled && !config::viewmodel::no_aim_punch && !config::grenade::trajectory && !config::aimbot::silent_aim);
    auto path=std::filesystem::temp_directory_path()/"nephilim-config-regression.cfg";
    config::aimbot::enabled=true;config::aimbot::fov=7.25f;config::esp::box=true;
    config::lighting::night_mode=true;config::skybox::enabled=true;
    std::strcpy(config::skybox::material,"materials/skybox/custom sky.vmat");
    skins::PlayerSkinConfig skin;skin.weapon_id=7;skin.paint_kit=282;skin.seed=15;skin.wear=.123f;skin.name_tag="quoted \"name\"";
    skins::user_skins[7]=skin;skins::selected_glove_kit=10006;
    std::map<std::string,std::string> expected;
    for(auto& e:config_store::entries)expected[e.key]=e.get();
    assert(config_store::Save(path));config_store::Reset();assert(skins::user_skins.empty());
    assert(config_store::Load(path));
    for(auto& e:config_store::entries)assert(e.get()==expected.at(e.key));
    assert(skins::user_skins.at(7).name_tag==skin.name_tag && skins::selected_glove_kit==10006);
    {std::ofstream out(path);out<<"NEPHILIM_CONFIG 1\nsetting \"aimbot.enabled\" \"0\"\nsetting \"aimbot.fov\" \"nan\"\nloadout 500 0\n";}
    assert(!config_store::Load(path));assert(config::aimbot::enabled && config::aimbot::fov==7.25f && skins::user_skins.size()==1);
    std::filesystem::remove(path);config_store::Reset();
    std::cout<<"Config persistence regressions passed: "<<config_store::entries.size()<<" settings, loadout, reset and malformed-file rollback\n";
}
