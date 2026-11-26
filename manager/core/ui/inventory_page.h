#pragma once
#include "../inventory.h"
#include "../../external/imgui/imgui.h"
#include "file_path.h"

namespace menu_advanced {
    inline void RenderInventoryCollection() {
        static uint64_t selected=0;
        static ImGuiTextFilter filter;
        static char file[512]{};
        if(!file[0]) {
            wchar_t appdata[MAX_PATH]{};const auto n=GetEnvironmentVariableW(L"LOCALAPPDATA",appdata,MAX_PATH);
            const auto base=n&&n<MAX_PATH?std::filesystem::path(appdata):std::filesystem::temp_directory_path();
            const auto value=(base/"Nephilim"/"Inventory"/"collection.inv").u8string();
            const auto size=(std::min)(value.size(),sizeof(file)-1);std::memcpy(file,value.data(),size);file[size]=0;
        }
        ImGui::TextWrapped("Save multiple cosmetic items, equip a local loadout, or publish a session-only item to CS2's native client inventory. Native publication requires the researched client identity; it does not grant Steam ownership.");
        ImGui::InputText("Inventory file",file,sizeof(file));
        if(ImGui::Button("Save inventory")) {
            const auto path=PathFromUtf8(file);std::error_code ec;
            if(!path.parent_path().empty()) std::filesystem::create_directories(path.parent_path(),ec);
            if(ec) inventory::status="Cannot create inventory directory";else inventory::Save(path);
        }
        ImGui::SameLine();if(ImGui::Button("Load inventory")) inventory::Load(PathFromUtf8(file));
        ImGui::TextWrapped("%s",inventory::status.c_str());
        ImGui::TextUnformatted("Find saved items");filter.Draw("##inventory_filter",-1);
        if(ImGui::BeginTable("local_inventory",4,ImGuiTableFlags_Borders|ImGuiTableFlags_RowBg|ImGuiTableFlags_ScrollY,{0,230})) {
            ImGui::TableSetupColumn("Item");ImGui::TableSetupColumn("Paint / seed");ImGui::TableSetupColumn("Wear");ImGui::TableSetupColumn("Native state");ImGui::TableHeadersRow();
            for(const auto& item:inventory::items) {
                const auto label=std::string(skins::GetWeaponName(item.skin.weapon_id))+" | "+item.skin.name_tag;
                if(!filter.PassFilter(label.c_str())) continue;
                ImGui::PushID(static_cast<int>(item.id));ImGui::TableNextRow();ImGui::TableNextColumn();
                if(ImGui::Selectable(label.c_str(),selected==item.id,ImGuiSelectableFlags_SpanAllColumns)) selected=item.id;
                ImGui::TableNextColumn();ImGui::Text("%d / %d",item.skin.paint_kit,item.skin.seed);
                ImGui::TableNextColumn();ImGui::Text("%.4f",item.skin.wear);
                ImGui::TableNextColumn();
                using inventory::NativeState;
                ImGui::TextUnformatted(item.native==NativeState::Published?"Published":item.native==NativeState::AddPending?"Queued add":
                    item.native==NativeState::RemovePending?"Queued remove":item.native==NativeState::Failed?"Failed / see details":"Local");
                ImGui::PopID();
            }
            ImGui::EndTable();
        }
        const auto found=std::find_if(inventory::items.begin(),inventory::items.end(),[&](const inventory::Item& i){return i.id==selected;});
        if(found==inventory::items.end()) {ImGui::TextDisabled("Choose finishes in the browser and use Add to inventory.");return;}
        const bool native_present=found->native_object!=0;
        ImGui::Text("Saved item #%llu | Native item ID: %llu",found->id,found->native_id);
        if(!found->message.empty()) ImGui::TextWrapped("%s",found->message.c_str());
        if(ImGui::Button("Equip local loadout")) inventory::Equip(selected);
        ImGui::SameLine();ImGui::BeginDisabled(native_present||found->native==inventory::NativeState::AddPending);
        if(ImGui::Button("Publish to native inventory")) inventory::Publish(selected);
        ImGui::EndDisabled();ImGui::SameLine();
        if(ImGui::Button(native_present?"Remove native and local item":"Remove saved item")) inventory::Remove(selected);
        ImGui::TextDisabled("Native cache changes run on the frame callback; local items are saved independently.");
    }
}
