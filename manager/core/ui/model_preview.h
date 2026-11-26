#pragma once
#include "../model_inspector.h"
#include "../../external/imgui/imgui.h"
#include <filesystem>
#include <fstream>
#include "file_path.h"

namespace menu_advanced {
    inline std::filesystem::path AgentMeshPath() {
        HMODULE module=nullptr;
        if(!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCWSTR>(&AgentMeshPath),&module)) return {};
        std::array<wchar_t,32768> path{};
        const auto length=GetModuleFileNameW(module,path.data(),static_cast<DWORD>(path.size()));
        if(!length||length>=path.size()) return {};
        return std::filesystem::path(path.data()).parent_path()/L"models"/L"seal_team_6.obj";
    }
    inline void RenderModelInspector(const float color[3]) {
        using namespace model_inspector;
        static Mesh mesh;static View view;
        static char path[512]{};static std::string message;
        static bool attempted=false;
        static bool wire=false,rotate=false;
        if(!ImGui::CollapsingHeader("3D model inspector")) return;
        auto loadAgent=[&] {
            std::string error;
            if(LoadObjFile(AgentMeshPath(),mesh,error)) {view={};message="Seal Team 6 - variant E | CS2 agent geometry";}
            else message="Seal Team 6 mesh unavailable. Place the supplied models folder beside manager.dll, then reload.";
        };
        if(!attempted) {attempted=true;loadAgent();}
        ImGui::TextUnformatted("Agent: Seal Team 6 (variant E)");
        if(ImGui::Button("Load Seal Team 6")) loadAgent();
        ImGui::SameLine();if(ImGui::Button("Reset view")) view={};
        ImGui::TextDisabled("Static agent mesh with preview tint. Drag to orbit, right-drag to pan, wheel to zoom.");
        ImGui::TextDisabled("Game textures, animations and exact game shaders are not shown.");
        if(ImGui::TreeNode("Custom geometry")) {
        ImGui::InputTextWithHint("##obj_path","Path to an OBJ mesh",path,sizeof(path));
        if(ImGui::Button("Load OBJ")) {
            std::string error;
            if(LoadObjFile(PathFromUtf8(path),mesh,error)) {view={};message="Imported OBJ geometry";}
            else message=error;
        }
        ImGui::SameLine();if(ImGui::Button("Diagnostic mesh")) {mesh=Mannequin();view={};message="Preview geometry: diagnostic mannequin";}
        ImGui::TreePop();
        }
        ImGui::Checkbox("Wireframe",&wire);ImGui::SameLine();ImGui::Checkbox("Auto rotate",&rotate);
        ImGui::TextWrapped("%s",message.c_str());
        ImGui::Text("%zu vertices | %zu triangles",mesh.vertices.size(),mesh.triangles.size());
        const ImVec2 origin=ImGui::GetCursorScreenPos(),size{ImGui::GetContentRegionAvail().x,300};
        ImGui::InvisibleButton("##model_orbit",size,ImGuiButtonFlags_MouseButtonLeft|ImGuiButtonFlags_MouseButtonRight);
        auto& io=ImGui::GetIO();
        if(ImGui::IsItemActive()) {
            if(ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {view.yaw+=io.MouseDelta.x*.01f;view.pitch=std::clamp(view.pitch+io.MouseDelta.y*.01f,-1.3f,1.3f);}
            if(ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {view.pan_x+=io.MouseDelta.x;view.pan_y+=io.MouseDelta.y;}
        }
        if(ImGui::IsItemHovered()) view.distance=std::clamp(view.distance*std::exp(-io.MouseWheel*.12f),1.6f,8.f);
        if(rotate) view.yaw+=io.DeltaTime*.5f;
        struct Face {std::array<ImVec2,3> points;float depth,light;};
        std::vector<Face> faces;faces.reserve(mesh.triangles.size());
        const float scale=(std::min)(size.x,size.y)*1.3f;
        for(size_t t=0;t<mesh.triangles.size();++t) {
            std::array<Point,3> points;
            for(unsigned i=0;i<3;++i) points[i]=Rotate(mesh.vertices[mesh.triangles[t][i]],view);
            if(points[0].z<.15f||points[1].z<.15f||points[2].z<.15f) continue;
            Face face{};
            for(unsigned i=0;i<3;++i) {face.points[i]={origin.x+size.x*.5f+view.pan_x+points[i].x/points[i].z*scale,
                origin.y+size.y*.5f+view.pan_y-points[i].y/points[i].z*scale};face.depth+=points[i].z/3;}
            Point a{points[1].x-points[0].x,points[1].y-points[0].y,points[1].z-points[0].z};
            Point b{points[2].x-points[0].x,points[2].y-points[0].y,points[2].z-points[0].z};
            Point n{a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};
            const float length=std::sqrt(n.x*n.x+n.y*n.y+n.z*n.z);
            face.light=length>1e-6f?.3f+.7f*std::abs((n.x*.3f+n.y*.5f+n.z*.8f)/length):.3f;
            faces.push_back(face);
        }
        std::sort(faces.begin(),faces.end(),[](const Face& a,const Face& b){return a.depth>b.depth;});
        auto* draw=ImGui::GetWindowDrawList();draw->PushClipRect(origin,{origin.x+size.x,origin.y+size.y},true);
        draw->AddRectFilled(origin,{origin.x+size.x,origin.y+size.y},IM_COL32(17,24,32,255),6);
        for(const auto& face:faces) {
            const auto tint=ImGui::ColorConvertFloat4ToU32({std::clamp(color[0]*face.light,0.f,1.f),std::clamp(color[1]*face.light,0.f,1.f),std::clamp(color[2]*face.light,0.f,1.f),1});
            draw->AddTriangleFilled(face.points[0],face.points[1],face.points[2],tint);
            if(wire) draw->AddTriangle(face.points[0],face.points[1],face.points[2],IM_COL32(220,240,255,110));
        }
        draw->PopClipRect();
    }
}
