#pragma once
#include "../manager/external/imgui/imgui_impl_dx11.h"
#pragma comment(lib,"d3d11.lib")
inline void TestSkinImagesAndPreview() {
    auto* previousContext=ImGui::GetCurrentContext();
    menu_advanced::ResetRendererResources();
    auto* previewContext=ImGui::CreateContext();
    ImGui::SetCurrentContext(previewContext);
    ImGui::GetIO().IniFilename=nullptr; ImGui::GetIO().DisplaySize=ImVec2(1280,900); ImGui::GetIO().DeltaTime=1.f/60;
    const auto root=std::filesystem::path(__FILE__).parent_path().parent_path()/"manager"/"core";
    auto catalog=skin_images::ReadCatalog(root);
    assert(catalog.images.size()>1000);
    assert(catalog.images.at(skin_images::Key("UMP-45","Caramel"))=="1");
    assert(skin_images::Field("{\"name\":\"J\\u00f6rmungandr\"}","name")=="J\xC3\xB6rmungandr");
    assert(skin_images::Field("{\"name\":\"broken\\uQQQQ\"}","name").empty());
    assert(skin_images::Key("Karambit","StatTrak knife | Doppler (Factory New)")==skin_images::Key("Karambit","Doppler"));
    const auto pixels=skin_images::Decode(catalog.root/"images"/"1.png");
    assert(!pixels.data.empty() && pixels.w<=320 && pixels.h<=320);
    assert(skin_images::Decode(catalog.root/"images"/"missing.png").data.empty());
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    assert(SUCCEEDED(D3D11CreateDevice(nullptr,D3D_DRIVER_TYPE_WARP,nullptr,0,nullptr,0,D3D11_SDK_VERSION,&device,nullptr,&context)));
    interfaces::d3d11_device=device.Get();
    assert(ImGui_ImplDX11_Init(device.Get(),context.Get()));
    menu_advanced::LoadIconFont();
    D3D11_TEXTURE2D_DESC desc{}; desc.Width=1280; desc.Height=900; desc.MipLevels=desc.ArraySize=1;
    desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM; desc.SampleDesc.Count=1; desc.BindFlags=D3D11_BIND_RENDER_TARGET;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> target;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> view;
    assert(SUCCEEDED(device->CreateTexture2D(&desc,nullptr,&target)));
    assert(SUCCEEDED(device->CreateRenderTargetView(target.Get(),nullptr,&view)));
    auto saved=skins::skin_database;
    skins::skin_database.clear();
    std::ifstream in(catalog.root/"output.jsonl"); std::string line; std::set<std::string> names;
    while(std::getline(in,line) && names.size()<24) {
        if(skin_images::Field(line,"weapon")!="AK-47") continue;
        auto name=skin_images::Field(line,"name");
        auto sep=name.find(" | "); if(sep==std::string::npos) continue; name.erase(0,sep+3);
        auto wear=name.rfind(" ("); if(wear!=std::string::npos) name.resize(wear);
        if(names.insert(name).second) skins::skin_database.emplace_back(int(names.size()),name,"AK-47",7,skins::RARITY_RARE,true);
    }
    assert(!skins::skin_database.empty());
    skins::user_skins[7].paint_kit=skins::skin_database.front().paint_kit;
    menu_advanced::selected_tab=8;
    for(int frame=0;frame<60;++frame) {
        if(skin_images::imageJob.valid()) skin_images::imageJob.wait();
        ImGui_ImplDX11_NewFrame(); ImGui::NewFrame(); menu_advanced::RenderMainMenu(); ImGui::Render();
        const float clear[4]={.04f,.05f,.07f,1}; context->ClearRenderTargetView(view.Get(),clear);
        auto* raw=view.Get(); context->OMSetRenderTargets(1,&raw,nullptr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
    assert(!skin_images::textures.empty());
    for(const auto& [id,t]:skin_images::textures) assert(t.view);
    desc.BindFlags=0; desc.Usage=D3D11_USAGE_STAGING; desc.CPUAccessFlags=D3D11_CPU_ACCESS_READ;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> staging; assert(SUCCEEDED(device->CreateTexture2D(&desc,nullptr,&staging)));
    context->CopyResource(staging.Get(),target.Get()); D3D11_MAPPED_SUBRESOURCE mapped{};
    assert(SUCCEEDED(context->Map(staging.Get(),0,D3D11_MAP_READ,0,&mapped)));
    BITMAPFILEHEADER file{}; BITMAPINFOHEADER info{};
    file.bfType=0x4D42; file.bfOffBits=sizeof(file)+sizeof(info); file.bfSize=file.bfOffBits+1280*900*4;
    info.biSize=sizeof(info); info.biWidth=1280; info.biHeight=-900; info.biPlanes=1; info.biBitCount=32;
    std::ofstream out("skin-studio.bmp",std::ios::binary); out.write((char*)&file,sizeof(file)); out.write((char*)&info,sizeof(info));
    for(int y=0;y<900;++y) {
        auto* row=(unsigned char*)mapped.pData+y*mapped.RowPitch;
        for(int x=0;x<1280;++x) { unsigned char bgra[]={row[x*4+2],row[x*4+1],row[x*4],255}; out.write((char*)bgra,4); }
    }
    context->Unmap(staging.Get(),0);
    skin_images::Reset(); assert(skin_images::textures.empty());
    ImGui_ImplDX11_Shutdown(); interfaces::d3d11_device=nullptr;
    menu_advanced::ResetRendererResources();
    ImGui::DestroyContext(previewContext); ImGui::SetCurrentContext(previousContext);
    skins::skin_database=std::move(saved);
}
