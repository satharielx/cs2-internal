#include "../manager/core/debug_console.cpp"
#include "../manager/core/ui/cursor.h"
#include <cassert>
#include <thread>
#include <iostream>
#include "imgui_preview.h"
int main() {
    auto& console = debug_console::Console::Get();
    console.Clear();
    console.Info("%s", "100% complete %s %n");
    assert(console.GetLogs().back().message == "100% complete %s %n");
    console.ExecuteCommand("echo literal %s %n");
    assert(console.GetLogs().back().message == "literal %s %n");
    console.ExecuteCommand("config save ../escape");
    assert(console.GetLogs().back().level == debug_console::LOG_WARNING);
    console.ExecuteCommand("unknown");
    assert(console.GetLogs().back().level == debug_console::LOG_WARNING);
    console.ExecuteCommand("clear");
    assert(console.GetLogs().empty());
    for (int i = 0; i < 1100; ++i) console.Info("row %d", i);
    assert(console.GetLogs().size() == 1000);
    assert(console.GetLogs().front().message == "row 100");
    ImGui::CreateContext();
    auto& io = ImGui::GetIO(); io.DisplaySize={1280,800}; io.DeltaTime=1.f/60; io.IniFilename=nullptr;
    unsigned char* pixels; int width,height;
    io.Fonts->GetTexDataAsRGBA32(&pixels,&width,&height);
    auto frame = [&] { ImGui::NewFrame(); console.Render(); ImGui::Render(); };
    for(int i=0;i<4;++i) frame();
    // A thousand retained rows must not create a thousand rows of geometry.
    assert(ImGui::GetDrawData()->TotalVtxCount < 15000);
    SaveMenuPreview("console.bmp", pixels, width, height);
    const int unfiltered = ImGui::GetDrawData()->TotalVtxCount;
    console.ExecuteCommand("filter impossible_match_943758");
    frame(); frame();
    assert(ImGui::GetDrawData()->TotalVtxCount < unfiltered);
    console.ExecuteCommand("filter off");
    std::thread producer([&] { for(int i=0;i<100;++i) console.Info("worker %d",i); });
    for(int i=0;i<50;++i) { frame(); assert(console.GetLogs().size() <= 1000); }
    producer.join();
    ImGui::NewFrame();
    io.MousePos={100,100}; ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
    menu_advanced::DrawWorkspaceCursor(true);
    assert(io.MouseDrawCursor && ImGui::GetMouseCursor()==ImGuiMouseCursor_None);
    menu_advanced::DrawWorkspaceCursor(false);
    assert(!io.MouseDrawCursor);
    ImGui::Render();
    console.ExecuteCommand("close"); assert(!console.IsVisible());
    ImGui::DestroyContext();
    std::cout << "Console regressions passed: literal formatting, commands, bounded logs, clipped rendering, filters, concurrent logging and cursor ownership.\n";
}
