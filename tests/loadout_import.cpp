#include "../manager/core/loadout_web.h"
#include <cassert>
#include <iostream>
int main() {
    skins::skin_database.emplace_back(38, "Fade", "Karambit", 507, skins::RARITY_ANCIENT, true);
    std::map<int, skins::PlayerSkinConfig> result; int knife = 0, glove = 0;
    std::istringstream good("NEPHILIM_LOADOUT 1 507 0\n507 38 321 0.0800 1 42 4d79206b6e696665\n");
    assert(loadout_web::Parse(good,result,knife,glove));
    assert(knife == 507 && result.at(507).paint_kit == 38 && result.at(507).name_tag == "My knife");
    for (const char* invalid : {
        "NEPHILIM_LOADOUT 2 507 0", "NEPHILIM_LOADOUT 1 504 0", "NEPHILIM_LOADOUT 1 507 99999",
        "NEPHILIM_LOADOUT 1 507 0\n500 38 0 0.01 0 0 -",
        "NEPHILIM_LOADOUT 1 507 0\n507 38 1001 0.01 0 0 -",
        "NEPHILIM_LOADOUT 1 507 0\n507 38 0 nan 0 0 -",
        "NEPHILIM_LOADOUT 1 507 0\n507 38 0 0.01 0 0 0a",
        "NEPHILIM_LOADOUT 1 507 0\n507 38 0 0.01 0 0 -\n507 38 0 0.01 0 0 -",
        "NEPHILIM_LOADOUT 1 507 0\n507 38 0 0.01 0 0 -\ntrailing garbage"}) {
        std::istringstream in(invalid); assert(!loadout_web::Parse(in,result,knife,glove));
        assert(result.size() == 1 && result.at(507).name_tag == "My knife");
    }
    std::istringstream empty("NEPHILIM_LOADOUT 1 507 0\n");
    assert(loadout_web::Parse(empty,result,knife,glove) && result.empty());
    std::cout << "Loadout import tests passed\n";
}
