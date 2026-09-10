#pragma once
#include <fstream>
// Offline preview of the font-atlas-only menu, using ImGui's actual draw data.
inline void SaveMenuPreview(const char* path, const unsigned char* atlas, int aw, int ah) {
    const int width=1280, height=900;
    std::vector<unsigned char> output(width*height*4,24);
    auto* data=ImGui::GetDrawData();
    auto edge=[](ImVec2 a,ImVec2 b,ImVec2 p) { return (p.x-a.x)*(b.y-a.y)-(p.y-a.y)*(b.x-a.x); };
    for(int list=0;list<data->CmdListsCount;++list) {
        const auto* dl=data->CmdLists[list];
        for(const auto& cmd:dl->CmdBuffer) {
            if(cmd.UserCallback) continue;
            for(unsigned i=0;i+2<cmd.ElemCount;i+=3) {
                const auto& a=dl->VtxBuffer[cmd.VtxOffset+dl->IdxBuffer[cmd.IdxOffset+i]];
                const auto& b=dl->VtxBuffer[cmd.VtxOffset+dl->IdxBuffer[cmd.IdxOffset+i+1]];
                const auto& c=dl->VtxBuffer[cmd.VtxOffset+dl->IdxBuffer[cmd.IdxOffset+i+2]];
                const float area=edge(a.pos,b.pos,c.pos);
                if(std::abs(area)<0.0001f) continue;
                int left=(int)std::max({0.0f,cmd.ClipRect.x,std::min({a.pos.x,b.pos.x,c.pos.x})});
                int top=(int)std::max({0.0f,cmd.ClipRect.y,std::min({a.pos.y,b.pos.y,c.pos.y})});
                int right=(int)std::min({(float)width,cmd.ClipRect.z,std::max({a.pos.x,b.pos.x,c.pos.x})+1});
                int bottom=(int)std::min({(float)height,cmd.ClipRect.w,std::max({a.pos.y,b.pos.y,c.pos.y})+1});
                for(int y=top;y<bottom;++y) for(int x=left;x<right;++x) {
                    const ImVec2 p(x+.5f,y+.5f);
                    const float wa=edge(b.pos,c.pos,p)/area, wb=edge(c.pos,a.pos,p)/area, wc=1-wa-wb;
                    if(wa<0 || wb<0 || wc<0) continue;
                    const int tx=std::clamp((int)((a.uv.x*wa+b.uv.x*wb+c.uv.x*wc)*aw),0,aw-1);
                    const int ty=std::clamp((int)((a.uv.y*wa+b.uv.y*wb+c.uv.y*wc)*ah),0,ah-1);
                    auto channel=[&](int shift) { return ((a.col>>shift)&255)*wa+((b.col>>shift)&255)*wb+((c.col>>shift)&255)*wc; };
                    const float alpha=channel(24)*atlas[(ty*aw+tx)*4+3]/65025.0f;
                    auto* pixel=&output[(y*width+x)*4];
                    for(int ch=0;ch<3;++ch) pixel[2-ch]=(unsigned char)(channel(ch*8)*alpha+pixel[2-ch]*(1-alpha));
                    pixel[3]=255;
                }
            }
        }
    }
    BITMAPFILEHEADER header{}; header.bfType=0x4D42; header.bfOffBits=sizeof(header)+sizeof(BITMAPINFOHEADER); header.bfSize=header.bfOffBits+(DWORD)output.size();
    BITMAPINFOHEADER info{}; info.biSize=sizeof(info); info.biWidth=width; info.biHeight=-height; info.biPlanes=1; info.biBitCount=32;
    std::ofstream file(path,std::ios::binary);
    file.write((char*)&header,sizeof(header)); file.write((char*)&info,sizeof(info)); file.write((char*)output.data(),output.size());
}
