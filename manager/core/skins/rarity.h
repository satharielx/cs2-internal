#pragma once
#include <algorithm>
#include <iterator>

namespace skins::rarity {
    struct RarityEntry { int id,value; };
    #include "rarity_baseline.inc"
    template<size_t N> inline int Lookup(const RarityEntry (&entries)[N],int id) {
        const auto found=std::lower_bound(std::begin(entries),std::end(entries),id,
            [](const RarityEntry& entry,int key){return entry.id<key;});
        return found!=std::end(entries)&&found->id==id?found->value:-1;
    }
    // Paint-kit rarity is relative to its weapon definition. Native econ rarity
    // is one-based; SkinRarity used by the app is zero-based.
    inline int Native(int definition,int paint) {
        const auto base=Lookup(item_rarities,definition);
        if(base<0) return -1;
        if(!paint) return base;
        const auto finish=Lookup(paint_rarities,paint);
        if(finish<0) return -1;
        return (std::clamp)(base+finish-1,0,finish==7?7:6);
    }
}
