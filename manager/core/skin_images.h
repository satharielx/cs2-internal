#pragma once
#include <filesystem>
#include <fstream>
#include <future>
#include <unordered_map>
#include <iomanip>
#include <sstream>
#include <cctype>
#pragma comment(lib, "ole32.lib")
#include <wincodec.h>
#include <wrl/client.h>

namespace skin_images {
    using Microsoft::WRL::ComPtr;
    struct Catalog { std::filesystem::path root; std::unordered_map<std::string,std::string> images; };
    struct Pixels { UINT w=0,h=0; std::vector<unsigned char> data; };
    struct Texture { ComPtr<ID3D11ShaderResourceView> view; UINT w=0,h=0; int used=0; };
    inline Catalog catalog;
    inline std::future<Catalog> catalogJob;
    inline std::future<Pixels> imageJob;
    inline std::string pending;
    inline std::unordered_map<std::string,Texture> textures;
    inline bool started=false;
    inline int lastFrame=-1;
    inline std::string Normalize(std::string s) {
        std::string result;
        for(unsigned char c:s) if(c<128 && std::isalnum(c)) result+=static_cast<char>(std::tolower(c));
        return result;
    }
    inline std::string Field(const std::string& line,const char* key) {
        auto p=line.find(std::string("\"")+key+"\"");
        if(p==std::string::npos || (p=line.find(':',p))==std::string::npos) return {};
        p=line.find('"',p+1); if(p==std::string::npos) return {};
        std::string value;
        while(++p<line.size()) {
            char c=line[p]; if(c=='"') return value;
            if(c!='\\') { value+=c; continue; }
            if(++p>=line.size()) return {};
            c=line[p];
            if(c=='u') {
                if(p+4>=line.size()) return {};
                unsigned code=0;
                for(int i=0;i<4;++i) {
                    const char h=line[++p];
                    const int digit=h>='0' && h<='9'?h-'0':h>='a' && h<='f'?h-'a'+10:h>='A' && h<='F'?h-'A'+10:-1;
                    if(digit<0) return {}; code=code*16+digit;
                }
                if(code<0x80) value+=char(code);
                else if(code<0x800) { value+=char(0xC0|(code>>6)); value+=char(0x80|(code&63)); }
                else { value+=char(0xE0|(code>>12)); value+=char(0x80|((code>>6)&63)); value+=char(0x80|(code&63)); }
            } else if(c=='"' || c=='\\' || c=='/') value+=c;
            else if(c=='n') value+='\n';
            else if(c=='r') value+='\r';
            else if(c=='t') value+='\t';
            else return {};
        }
        return {};
    }
    inline std::string Key(std::string weapon,std::string name) {
        auto pipe=name.find(" | "); if(pipe!=std::string::npos) name.erase(0,pipe+3);
        for(const char* wear:{" (Factory New)"," (Minimal Wear)"," (Field-Tested)"," (Well-Worn)"," (Battle-Scarred)"}) {
            const std::string suffix=wear;
            if(name.size()>=suffix.size() && name.compare(name.size()-suffix.size(),suffix.size(),suffix)==0) name.resize(name.size()-suffix.size());
        }
        return Normalize(weapon)+"/"+Normalize(name);
    }
    inline Catalog ReadCatalog(const std::filesystem::path& dll) {
        Catalog result;
        for(const auto& root: {dll/"skins_images",dll/"core"/"skins_images",std::filesystem::path(__FILE__).parent_path()/"skins_images"}) {
            std::ifstream in(root/"output.jsonl"); if(!in) continue;
            result.root=root; std::string line;
            while(std::getline(in,line)) {
                auto id=Field(line,"imageid"), weapon=Field(line,"weapon"), name=Field(line,"name");
                if(id.empty() || id.size()>16 || id.find_first_not_of("0123456789")!=std::string::npos || weapon.empty() || name.empty()) continue;
                result.images.emplace(Key(weapon,name),id);
            }
            break;
        }
        return result;
    }
    inline Pixels Decode(const std::filesystem::path& file) {
        Pixels pixels;
        const HRESULT initialized=CoInitializeEx(nullptr,COINIT_MULTITHREADED);
        { // Release COM objects before balancing initialization.
            ComPtr<IWICImagingFactory> factory; ComPtr<IWICBitmapDecoder> decoder;
            ComPtr<IWICBitmapFrameDecode> frame; ComPtr<IWICBitmapScaler> scaler; ComPtr<IWICFormatConverter> converter;
            if(SUCCEEDED(CoCreateInstance(CLSID_WICImagingFactory,nullptr,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&factory))) &&
               SUCCEEDED(factory->CreateDecoderFromFilename(file.c_str(),nullptr,GENERIC_READ,WICDecodeMetadataCacheOnLoad,&decoder)) &&
               SUCCEEDED(decoder->GetFrame(0,&frame))) {
                UINT w=0,h=0; frame->GetSize(&w,&h);
                if(w && h && w<=8192 && h<=8192) {
                    const float scale=(std::min)(1.f,320.f/(std::max)(w,h));
                    pixels.w=(std::max)(1u,UINT(w*scale)); pixels.h=(std::max)(1u,UINT(h*scale));
                    if(SUCCEEDED(factory->CreateBitmapScaler(&scaler)) && SUCCEEDED(scaler->Initialize(frame.Get(),pixels.w,pixels.h,WICBitmapInterpolationModeFant)) &&
                       SUCCEEDED(factory->CreateFormatConverter(&converter)) && SUCCEEDED(converter->Initialize(scaler.Get(),GUID_WICPixelFormat32bppRGBA,WICBitmapDitherTypeNone,nullptr,0,WICBitmapPaletteTypeCustom))) {
                        pixels.data.resize(pixels.w*pixels.h*4);
                        if(FAILED(converter->CopyPixels(nullptr,pixels.w*4,UINT(pixels.data.size()),pixels.data.data()))) pixels.data.clear();
                    }
                }
            }
        }
        if(SUCCEEDED(initialized)) CoUninitialize();
        return pixels;
    }
    inline void Reset() {
        if(catalogJob.valid()) { catalogJob.wait(); catalog=catalogJob.get(); }
        if(imageJob.valid()) imageJob.wait();
        imageJob={}; pending.clear(); textures.clear(); lastFrame=-1;
    }
    inline void Tick(const std::string& dll) {
        if(!started) { started=true; catalogJob=std::async(std::launch::async,[dll]{return ReadCatalog(dll);}); }
        if(catalogJob.valid() && catalogJob.wait_for(std::chrono::seconds(0))==std::future_status::ready) catalog=catalogJob.get();
        const int frame=ImGui::GetFrameCount(); if(frame==lastFrame) return; lastFrame=frame;
        // Only evict between frames: ImGui draw commands retain raw texture pointers.
        for(auto it=textures.begin();it!=textures.end();) {
            if(textures.size()>96 && it->second.used<frame-1) it=textures.erase(it); else ++it;
        }
        if(imageJob.valid() && imageJob.wait_for(std::chrono::seconds(0))==std::future_status::ready) {
            auto pixels=imageJob.get(); auto& entry=textures[pending]; entry.used=frame;
            if(!pixels.data.empty() && interfaces::d3d11_device) {
                D3D11_TEXTURE2D_DESC desc{}; desc.Width=pixels.w; desc.Height=pixels.h; desc.MipLevels=desc.ArraySize=1;
                desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM; desc.SampleDesc.Count=1; desc.Usage=D3D11_USAGE_IMMUTABLE; desc.BindFlags=D3D11_BIND_SHADER_RESOURCE;
                D3D11_SUBRESOURCE_DATA data{}; data.pSysMem=pixels.data.data(); data.SysMemPitch=pixels.w*4;
                ComPtr<ID3D11Texture2D> texture;
                if(SUCCEEDED(interfaces::d3d11_device->CreateTexture2D(&desc,&data,&texture)))
                    interfaces::d3d11_device->CreateShaderResourceView(texture.Get(),nullptr,&entry.view);
                entry.w=pixels.w; entry.h=pixels.h;
            }
            pending.clear();
        }
    }
    inline void Draw(int weapon,const std::string& finish,ImVec2 pos,ImVec2 size) {
        const auto found=catalog.images.find(Key(skins::GetWeaponName(weapon),finish));
        auto* draw=ImGui::GetWindowDrawList();
        if(found!=catalog.images.end()) {
            const auto& id=found->second; auto it=textures.find(id);
            if(it!=textures.end()) {
                auto& texture=it->second; texture.used=ImGui::GetFrameCount();
                if(texture.view) {
                    const float scale=(std::min)(size.x/texture.w,size.y/texture.h);
                    const ImVec2 extent(texture.w*scale,texture.h*scale), origin=pos+(size-extent)*.5f;
                    draw->AddImage((ImTextureID)texture.view.Get(),origin,origin+extent); return;
                }
            } else if(!imageJob.valid() && interfaces::d3d11_device) {
                pending=id; const auto file=catalog.root/"images"/(id+".png");
                imageJob=std::async(std::launch::async,[file]{return Decode(file);});
            }
        }
        const bool failed=found!=catalog.images.end() && textures.find(found->second)!=textures.end();
        const char* label=found==catalog.images.end() || failed ? "No preview" : "Loading preview";
        draw->AddText(pos+(size-ImGui::CalcTextSize(label))*.5f,IM_COL32(125,139,158,255),label);
    }
}
