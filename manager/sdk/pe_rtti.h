#pragma once
#include "feature_support.h"
#include <vector>
#include <string_view>

namespace sdk::rtti {
    // Read initialized PE sections once. All results are addresses in this loaded image.
    class Image {
        struct Block { uintptr_t address; std::vector<unsigned char> bytes; };
        std::vector<Block> blocks_;
        uintptr_t base_ = 0;
        uint32_t size_ = 0;
    public:
        explicit Image(HMODULE module) {
            base_ = reinterpret_cast<uintptr_t>(module);
            IMAGE_DOS_HEADER dos{}; IMAGE_NT_HEADERS64 nt{};
            if (!base_ || !read_memory(base_,dos) || dos.e_magic!=IMAGE_DOS_SIGNATURE ||
                dos.e_lfanew<0 || dos.e_lfanew>0x100000 || !read_memory(base_+dos.e_lfanew,nt) ||
                nt.Signature!=IMAGE_NT_SIGNATURE || nt.OptionalHeader.Magic!=IMAGE_NT_OPTIONAL_HDR64_MAGIC ||
                nt.FileHeader.NumberOfSections>96 || nt.OptionalHeader.SizeOfImage>0x20000000) return;
            size_=nt.OptionalHeader.SizeOfImage;
            const auto headers=base_+dos.e_lfanew+offsetof(IMAGE_NT_HEADERS64,OptionalHeader)+nt.FileHeader.SizeOfOptionalHeader;
            for(unsigned i=0;i<nt.FileHeader.NumberOfSections;++i) {
                IMAGE_SECTION_HEADER section{};
                if(!read_memory(headers+i*sizeof(section),section)) {blocks_.clear(); return;}
                const auto count=(std::min)(section.SizeOfRawData,section.Misc.VirtualSize);
                if(!(section.Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA) ||
                    (section.Characteristics & IMAGE_SCN_MEM_EXECUTE) || !count || count>0x2000000 ||
                    section.VirtualAddress>=size_ || count>size_-section.VirtualAddress) continue;
                Block block{base_+section.VirtualAddress,std::vector<unsigned char>(count)};
                SIZE_T transferred=0;
                if(ProfileRead(GetCurrentProcess(),reinterpret_cast<void*>(block.address),block.bytes.data(),count,&transferred) && transferred==count)
                    blocks_.push_back(std::move(block));
            }
        }
        bool Contains(uintptr_t address,size_t bytes=1) const {
            return size_ && address>=base_ && address-base_<size_ && bytes<=size_-(address-base_);
        }
        uintptr_t Vtable(std::string_view name) const {
            uintptr_t result=0;
            for(const auto& names:blocks_) {
                for(size_t n=16;n+name.size()<names.bytes.size();++n) {
                    if(std::memcmp(names.bytes.data()+n,name.data(),name.size()) || names.bytes[n+name.size()]!=0) continue;
                    const uint32_t type=static_cast<uint32_t>(names.address+n-16-base_);
                    for(const auto& locators:blocks_) for(size_t p=0;p+24<=locators.bytes.size();p+=4) {
                        uint32_t col[6]{}; std::memcpy(col,locators.bytes.data()+p,24);
                        if(col[0]!=1 || col[1]!=0 || col[3]!=type || col[5]!=locators.address+p-base_) continue;
                        const uintptr_t locator=locators.address+p;
                        for(const auto& tables:blocks_) for(size_t t=0;t+16<=tables.bytes.size();t+=8) {
                            uintptr_t value=0; std::memcpy(&value,tables.bytes.data()+t,8);
                            if(value!=locator) continue;
                            const auto found=tables.address+t+8;
                            if(result && result!=found) return 0;
                            result=found;
                        }
                    }
                }
            }
            return result;
        }
    };
    inline bool Executable(uintptr_t address) {
        MEMORY_BASIC_INFORMATION info{};
        return address && ProfileQuery(reinterpret_cast<void*>(address),&info,sizeof(info)) && info.State==MEM_COMMIT &&
            !(info.Protect & (PAGE_GUARD|PAGE_NOACCESS)) &&
            (info.Protect & (PAGE_EXECUTE|PAGE_EXECUTE_READ|PAGE_EXECUTE_READWRITE|PAGE_EXECUTE_WRITECOPY));
    }
}
