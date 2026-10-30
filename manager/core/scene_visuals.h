#pragma once
#include "../sdk/pe_rtti.h"
#include <array>
#include <atomic>
#include <cstring>

namespace scene_visuals {
    enum : unsigned {Sun=1,Clouds=2,Sky=4};
    using DrawArray=void(__fastcall*)(uintptr_t,uintptr_t,uintptr_t,int,uintptr_t,uintptr_t,uintptr_t);
    inline std::atomic<unsigned> flags{0};
    inline std::atomic<const char*> status{"Geometry filter not initialized"};
    inline std::atomic<unsigned long long> skipped{0},fallbacks{0};
    inline constexpr size_t stride=0x70,material_offset=0x20;

    inline bool Separator(char c) {return !(c>='a' && c<='z') && !(c>='0' && c<='9');}
    inline bool Token(const char* name,const char* word) {
        const auto length=std::strlen(word);
        for(const char* p=name;(p=std::strstr(p,word))!=nullptr;++p)
            if((p==name || Separator(p[-1])) && (!p[length] || Separator(p[length]) ||
                (p[length]>='0' && p[length]<='9') || (p[length]=='s' && (!p[length+1] || Separator(p[length+1]))))) return true;
        return false;
    }
    inline unsigned ClassifyName(const char* input) {
        if(!input) return 0;
        char name[256]{};
        size_t length=0;
        for(;length+1<sizeof(name) && input[length];++length) {
            const char c=input[length];
            name[length]=c>='A' && c<='Z' ? c+('a'-'A') : c=='\\' ? '/' : c;
        }
        if(length+1==sizeof(name)) return 0;
        // Match the asset's basename, not unrelated words in map/directory names.
        const char* leaf=std::strrchr(name,'/');leaf=leaf ? leaf+1 : name;
        unsigned kind=0;
        if(Token(leaf,"sun") || Token(leaf,"sunflare") || Token(leaf,"sunshaft")) kind|=Sun;
        if(Token(leaf,"cloud") || Token(leaf,"sky_clouds")) kind|=Clouds;
        return kind;
    }
    inline unsigned ReadKind(uintptr_t material) {
        if(!material) return 0;
        // No material calls are made until its vtable method is executable.
        uintptr_t table=0,function=0;
        if(!sdk::CopyLocalMemory(material,&table,sizeof(table)) || !table ||
            !sdk::CopyLocalMemory(table,&function,sizeof(function)) || !sdk::rtti::Executable(function)) return 0;
        char name[256]{};
        bool valid=false;
        __try {
            const auto text=reinterpret_cast<const char*(__fastcall*)(uintptr_t)>(function)(material);
            if(text) for(size_t i=0;i<sizeof(name);++i) {
                name[i]=text[i];
                if(!name[i]) {valid=true;break;}
            }
        } __except(GetExceptionCode()==EXCEPTION_ACCESS_VIOLATION || GetExceptionCode()==EXCEPTION_IN_PAGE_ERROR
            ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {return 0;}
        return valid ? ClassifyName(name) : 0;
    }
    inline unsigned Kind(uintptr_t material,ULONGLONG now) {
        struct Entry {uintptr_t material=0;ULONGLONG until=0;unsigned kind=0;};
        static thread_local std::array<Entry,256> cache{};
        auto& entry=cache[((material>>4)^(material>>12)) & 255];
        if(entry.material!=material || now>=entry.until) entry={material,now+1000,ReadKind(material)};
        return entry.kind;
    }
    inline void Draw(DrawArray original,uintptr_t desc,uintptr_t context,uintptr_t list,int count,
        uintptr_t view,uintptr_t layer,uintptr_t unknown) {
        const unsigned remove=flags.load(std::memory_order_relaxed) & (Sun|Clouds);
        if(!remove || !list || count<=0 || count>4096 || static_cast<size_t>(count)>((UINTPTR_MAX-list)/stride)) {
            original(desc,context,list,count,view,layer,unknown);return;
        }
        // Plan first, so a pathological alternating list can fall back without
        // submitting any geometry twice. Engine render entries remain unchanged.
        std::array<unsigned char,4096> hide{};
        const auto now=GetTickCount64();
        unsigned hidden=0,runs=0;
        bool passing=false;
        for(int i=0;i<count;++i) {
            uintptr_t material=0;
            if(sdk::CopyLocalMemory(list+static_cast<size_t>(i)*stride+material_offset,&material,sizeof(material)))
                hide[i]=(Kind(material,now)&remove)!=0;
            hidden+=hide[i];
            if(!hide[i] && !passing) ++runs;
            passing=!hide[i];
        }
        if(!hidden || runs>32) {
            if(runs>32) fallbacks.fetch_add(1,std::memory_order_relaxed);
            original(desc,context,list,count,view,layer,unknown);return;
        }
        skipped.fetch_add(hidden,std::memory_order_relaxed);
        int start=-1;
        for(int i=0;i<=count;++i) {
            if(i<count && !hide[i]) {if(start<0) start=i;}
            else if(start>=0) {
                original(desc,context,list+static_cast<size_t>(start)*stride,i-start,view,layer,unknown);
                start=-1;
            }
        }
    }
}
