#pragma once
#include <array>
#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <charconv>
#include <cmath>
#include <algorithm>
#include <locale>
#include <filesystem>
#include <fstream>

namespace model_inspector {
    struct Point {float x=0,y=0,z=0;};
    struct Mesh {std::vector<Point> vertices;std::vector<std::array<unsigned,3>> triangles;};
    inline constexpr size_t max_vertices=20000,max_triangles=20000,max_bytes=2*1024*1024;
    inline bool Normalize(Mesh& mesh) {
        if(mesh.vertices.empty() || mesh.triangles.empty()) return false;
        Point low=mesh.vertices[0],high=low;
        for(const auto& p:mesh.vertices) {
            if(!std::isfinite(p.x)||!std::isfinite(p.y)||!std::isfinite(p.z)) return false;
            low={(std::min)(low.x,p.x),(std::min)(low.y,p.y),(std::min)(low.z,p.z)};
            high={(std::max)(high.x,p.x),(std::max)(high.y,p.y),(std::max)(high.z,p.z)};
        }
        const float extent=(std::max)({high.x-low.x,high.y-low.y,high.z-low.z});
        if(!std::isfinite(extent)||extent<1e-6f) return false;
        const Point center{low.x+(high.x-low.x)*.5f,low.y+(high.y-low.y)*.5f,low.z+(high.z-low.z)*.5f};
        for(auto& p:mesh.vertices) p={(p.x-center.x)/extent*2,(p.y-center.y)/extent*2,(p.z-center.z)/extent*2};
        return true;
    }
    inline bool ParseObj(std::string_view text,Mesh& output,std::string& error) {
        auto fail=[&](const char* reason){error=reason;return false;};
        if(text.size()>max_bytes) return fail("OBJ exceeds 2 MiB");
        Mesh mesh;std::istringstream input{std::string(text)};input.imbue(std::locale::classic());
        std::string line;
        while(std::getline(input,line)) {
            if(line.size()>8192) return fail("OBJ line is too long");
            std::istringstream row(line);row.imbue(std::locale::classic());std::string kind;row>>kind;
            if(kind.empty()||kind[0]=='#') continue;
            if(kind=="v") {
                Point p;
                if(!(row>>p.x>>p.y>>p.z)||!std::isfinite(p.x)||!std::isfinite(p.y)||!std::isfinite(p.z)) return fail("Invalid OBJ vertex");
                if(mesh.vertices.size()>=max_vertices) return fail("OBJ exceeds 20,000 vertices");
                mesh.vertices.push_back(p);
            } else if(kind=="f") {
                std::vector<unsigned> face;std::string token;
                while(row>>token) {
                    if(token[0]=='#') break;
                    const auto slash=token.find('/');const auto part=std::string_view(token).substr(0,slash);
                    int index=0;const auto parsed=std::from_chars(part.data(),part.data()+part.size(),index);
                    if(parsed.ec!=std::errc{}||parsed.ptr!=part.data()+part.size()||!index) return fail("Invalid OBJ face index");
                    const auto resolved=index<0?static_cast<long long>(mesh.vertices.size())+index:static_cast<long long>(index)-1;
                    if(resolved<0||resolved>=static_cast<long long>(mesh.vertices.size())||face.size()>=64) return fail("OBJ face references an unavailable vertex");
                    face.push_back(static_cast<unsigned>(resolved));
                }
                if(face.size()<3) return fail("OBJ face has fewer than three vertices");
                for(size_t i=1;i+1<face.size();++i) {
                    if(mesh.triangles.size()>=max_triangles) return fail("OBJ exceeds 20,000 triangles");
                    if(face[0]!=face[i]&&face[i]!=face[i+1]&&face[0]!=face[i+1]) mesh.triangles.push_back({face[0],face[i],face[i+1]});
                }
            }
        }
        if(!Normalize(mesh)) return fail("OBJ has no usable geometry");
        output=std::move(mesh);error.clear();return true;
    }
    inline bool LoadObjFile(const std::filesystem::path& path,Mesh& mesh,std::string& error) {
        try {
            std::error_code ec;
            if(std::filesystem::file_size(path,ec)>max_bytes||ec) {error="Cannot read mesh, or file exceeds 2 MiB";return false;}
            std::ifstream input(path,std::ios::binary);
            std::string text(max_bytes+1,'\0');input.read(text.data(),static_cast<std::streamsize>(text.size()));
            text.resize(static_cast<size_t>(input.gcount()));
            if(input.bad()||text.size()>max_bytes) {error="Mesh read failed or file grew beyond 2 MiB";return false;}
            return ParseObj(text,mesh,error);
        } catch(const std::exception& e) {error=e.what();return false;}
    }
    inline Mesh Mannequin() {
        Mesh mesh;
        auto box=[&](Point low,Point high) {
            const unsigned n=static_cast<unsigned>(mesh.vertices.size());
            for(unsigned i=0;i<8;++i) mesh.vertices.push_back({i&1?high.x:low.x,i&2?high.y:low.y,i&4?high.z:low.z});
            constexpr unsigned faces[][3]={{0,2,1},{1,2,3},{4,5,6},{5,7,6},{0,1,4},{1,5,4},{2,6,3},{3,6,7},{0,4,2},{2,4,6},{1,3,5},{3,7,5}};
            for(auto& f:faces) mesh.triangles.push_back({n+f[0],n+f[1],n+f[2]});
        };
        box({-11,-6,38},{11,6,65});box({-7,-6,68},{7,6,82});box({-9,-5,30},{9,5,39});
        box({-20,-5,42},{-13,5,63});box({13,-5,42},{20,5,63});
        box({-21,-4,24},{-14,4,43});box({14,-4,24},{21,4,43});
        box({-10,-5,3},{-2,5,31});box({2,-5,3},{10,5,31});
        box({-10,-11,0},{-2,5,5});box({2,-11,0},{10,5,5});
        Normalize(mesh);return mesh;
    }
    struct View {float yaw=3.54f,pitch=.15f,distance=3.4f,pan_x=0,pan_y=0;};
    inline Point Rotate(Point p,const View& view) {
        const float x=p.x*std::cos(view.yaw)-p.y*std::sin(view.yaw);
        const float depth=p.x*std::sin(view.yaw)+p.y*std::cos(view.yaw);
        return {x,p.z*std::cos(view.pitch)-depth*std::sin(view.pitch),
            view.distance-depth*std::cos(view.pitch)-p.z*std::sin(view.pitch)};
    }
}
