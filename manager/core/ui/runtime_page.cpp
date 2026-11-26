#include "../menu_advanced.h"
#include "widgets.h"
#include "../runtime_inspector.h"
#include "../runtime_startup.h"
#include "../interfaces.h"
#include "../game_state.h"
#include <windows.h>
#include <algorithm>
#include <cstdio>
#include <sstream>

namespace menu_advanced {
    namespace {
        struct InspectorView {
            runtime_inspector::Snapshot snapshot;
            runtime_inspector::ExportList exports;
            ImGuiTextFilter module_filter, export_filter;
            std::vector<std::size_t> export_rows;
            int selected = -1;
            bool attempted = false, loaded_only = true;
            std::string error, action;
            void Refresh() {
                attempted = true;
                try {
                    auto next = runtime_inspector::Capture();
                    snapshot = std::move(next);
                    selected = -1;
                    exports = {};
                    export_rows.clear();
                    error.clear();
                    action = "Module snapshot refreshed";
                } catch (const std::exception& e) { error = e.what(); }
            }
            void FilterExports() {
                export_rows.clear();
                for (std::size_t i = 0; i < exports.entries.size(); ++i) {
                    const auto& e = exports.entries[i];
                    const auto searchable = e.name+" "+e.forwarder;
                    if (export_filter.PassFilter(searchable.c_str())) export_rows.push_back(i);
                }
            }
            void Select(int index) {
                selected = index;
                try { exports = runtime_inspector::CaptureExports(snapshot.modules[index].name.c_str(),
                    snapshot.modules[index].image.pdb_identity.c_str()); }
                catch (const std::exception& e) { exports = {{}, e.what()}; }
                FilterExports();
            }
        };
        InspectorView& CurrentInspector() { static InspectorView view; return view; }
        ImVec4 IdentityColor(runtime_inspector::Identity identity) {
            using runtime_inspector::Identity;
            if (identity == Identity::Match) return {.35f,.85f,.55f,1};
            if (identity == Identity::Changed || identity == Identity::Invalid) return {1,.55f,.35f,1};
            return ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled);
        }
        const char* ShortIdentity(runtime_inspector::Identity identity) {
            using runtime_inspector::Identity;
            switch (identity) {
                case Identity::Match: return "Match";
                case Identity::Changed: return "Changed";
                case Identity::Unverified: return "Unverified";
                case Identity::Invalid: return "Unreadable";
                default: return "Not loaded";
            }
        }
        std::string LiveHealth() {
            const auto state = game_state::GetSnapshot();
            std::ostringstream out;
            out << "D3D11 capture: " << (interfaces::d3d11_device && interfaces::swap_chain ? "ready" : "waiting")
                << "\nFrame callback: " << (interfaces::frame_stage_notify ? "resolved" : "unavailable")
                << "\nSession: " << state.status << '\n';
            return out.str();
        }
    }

    void RenderRuntimeTab() {
        using namespace runtime_inspector;
        auto& view = CurrentInspector();
        if (!view.attempted) view.Refresh();
        SectionHeader("Runtime Inspector", ICON_FA_COG);
        ImGui::TextWrapped("Research baseline: Steam build %s. Identity matches compare embedded PDB GUID/age and architecture; they do not prove live feature compatibility.", researched_build);
        ImGui::TextUnformatted(LiveHealth().c_str());
        if (ImGui::Button("Refresh modules")) view.Refresh();
        ImGui::SameLine();
        if (ImGui::Button("Copy diagnostic report")) {
            const auto report = Report(view.snapshot)+"\nLive health at copy time:\n"+LiveHealth();
            ImGui::SetClipboardText(report.c_str());
            view.action = "Diagnostic report copied";
        }
        ImGui::SameLine();
        ImGui::Checkbox("Loaded only", &view.loaded_only);
        if (!view.error.empty()) ImGui::TextWrapped("Inspection failed: %s", view.error.c_str());
        if (!view.action.empty()) ImGui::TextDisabled("%s", view.action.c_str());
        ImGui::Text("Loaded %zu | Match %zu | Changed %zu | Unverified %zu | Unreadable %zu",
            view.snapshot.loaded, view.snapshot.matched, view.snapshot.changed, view.snapshot.unverified, view.snapshot.invalid);
        const auto age = view.snapshot.captured_at_ms ? (GetTickCount64()-view.snapshot.captured_at_ms)/1000 : 0;
        ImGui::TextDisabled("Snapshot age: %llu s. Modules are inspected only on opening this page or Refresh.", age);
        bool missing_dependency = false;
        for (const auto* required : runtime_startup::required_modules) {
            const auto found = std::find_if(view.snapshot.modules.begin(), view.snapshot.modules.end(),
                [&](const Module& m) { return m.name == required; });
            if (found == view.snapshot.modules.end() || found->identity == Identity::Missing || found->identity == Identity::Invalid) {
                ImGui::TextWrapped("Required dependency unavailable: %s", required);
                missing_dependency = true;
            }
        }
        if (!missing_dependency) ImGui::TextDisabled("Required rendering modules are present; D3D11 capture above reports the actual device.");
        ImGui::TextUnformatted("Find modules or subsystems");
        view.module_filter.Draw("##runtime_module_filter", -1);
        if (ImGui::BeginTable("runtime_modules", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
            ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable, {0,210})) {
            ImGui::TableSetupColumn("Module");
            ImGui::TableSetupColumn("Subsystem");
            ImGui::TableSetupColumn("Architecture");
            ImGui::TableSetupColumn("Named exports");
            ImGui::TableSetupColumn("Research identity");
            ImGui::TableHeadersRow();
            for (std::size_t i = 0; i < view.snapshot.modules.size(); ++i) {
                const auto& module = view.snapshot.modules[i];
                if (view.loaded_only && module.identity == Identity::Missing) continue;
                const auto searchable = module.name+" "+module.role+" "+IdentityLabel(module.identity);
                if (!view.module_filter.PassFilter(searchable.c_str())) continue;
                ImGui::PushID(static_cast<int>(i));
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                if (ImGui::Selectable(module.name.c_str(), view.selected == static_cast<int>(i), ImGuiSelectableFlags_SpanAllColumns))
                    view.Select(static_cast<int>(i));
                ImGui::TableNextColumn(); ImGui::TextUnformatted(module.role.c_str());
                ImGui::TableNextColumn(); ImGui::TextUnformatted(module.identity == Identity::Missing ? "-" : Architecture(module.image.machine));
                ImGui::TableNextColumn(); ImGui::Text("%u", module.image.named_exports);
                ImGui::TableNextColumn(); ImGui::TextColored(IdentityColor(module.identity), "%s", ShortIdentity(module.identity));
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", IdentityLabel(module.identity));
                ImGui::PopID();
            }
            ImGui::EndTable();
        }
        if (view.selected < 0 || static_cast<std::size_t>(view.selected) >= view.snapshot.modules.size()) {
            ImGui::TextDisabled("Select a module to browse its exported code, data, and forwarders.");
            return;
        }
        const auto& module = view.snapshot.modules[view.selected];
        ImGui::Separator();
        ImGui::TextUnformatted(module.name.c_str());
        ImGui::TextWrapped("%s", module.path.c_str());
        ImGui::TextWrapped("Expected identity: %s", module.expected_identity.empty() ? "No PDB baseline" : module.expected_identity.c_str());
        ImGui::TextWrapped("Observed identity: %s", module.image.pdb_identity.empty() ? "Unavailable" : module.image.pdb_identity.c_str());
        if (!module.image.error.empty()) ImGui::TextWrapped("%s", module.image.error.c_str());
        if (!view.exports.error.empty()) ImGui::TextWrapped("Export inspection: %s", view.exports.error.c_str());
        ImGui::TextDisabled("RVA is relative to this image. Exported data and forwarders are not local callable functions.");
        ImGui::TextUnformatted("Find exports or forwarded targets");
        if (view.export_filter.Draw("##runtime_export_filter", -1)) view.FilterExports();
        if (ImGui::Button("Copy filtered exports")) {
            std::ostringstream out;
            out << module.name << "\nName\tOrdinal\tRVA\tKind\tForwarder\n";
            for (const auto index : view.export_rows) {
                const auto& e = view.exports.entries[index];
                out << e.name << '\t' << e.ordinal << "\t0x" << std::hex << e.rva << std::dec << '\t'
                    << (!e.forwarder.empty() ? "Forwarder" : e.executable ? "Code" : "Data") << '\t' << e.forwarder << '\n';
            }
            ImGui::SetClipboardText(out.str().c_str());
            view.action = "Filtered exports copied";
        }
        ImGui::SameLine(); ImGui::TextDisabled("%zu / %zu entries", view.export_rows.size(), view.exports.entries.size());
        if (ImGui::BeginTable("runtime_exports", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
            ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable, {0,230})) {
            ImGui::TableSetupColumn("Export"); ImGui::TableSetupColumn("RVA / ordinal");
            ImGui::TableSetupColumn("Kind"); ImGui::TableSetupColumn("Forwarder");
            ImGui::TableHeadersRow();
            ImGuiListClipper clipper;
            clipper.Begin(static_cast<int>(view.export_rows.size()));
            while (clipper.Step()) for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row) {
                const auto& e = view.exports.entries[view.export_rows[row]];
                ImGui::TableNextRow(); ImGui::TableNextColumn(); ImGui::TextUnformatted(e.name.c_str());
                ImGui::TableNextColumn(); ImGui::Text("0x%X / #%u", e.rva, e.ordinal);
                ImGui::TableNextColumn(); ImGui::TextUnformatted(!e.forwarder.empty() ? "Forwarder" : e.executable ? "Code" : "Data");
                ImGui::TableNextColumn(); ImGui::TextUnformatted(e.forwarder.c_str());
            }
            ImGui::EndTable();
        }
    }
}
