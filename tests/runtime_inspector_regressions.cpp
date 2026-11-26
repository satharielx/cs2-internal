#include "../manager/core/runtime_inspector.h"
#include "../manager/core/ui/runtime_page.cpp"
#include <set>
#include <stdexcept>
#include <cstring>
#include "imgui_preview.h"

namespace game_state { Snapshot GetSnapshot() { return {}; } }
namespace menu_advanced { void SectionHeader(const char* text, const char*) { ImGui::TextUnformatted(text); } }

using namespace runtime_inspector;
static void Check(bool value, const char* message) { if (!value) throw std::runtime_error(message); }
struct Fixture {
    std::vector<unsigned char> bytes = std::vector<unsigned char>(0x6000);
    IMAGE_NT_HEADERS64& Nt() { return *reinterpret_cast<IMAGE_NT_HEADERS64*>(bytes.data()+0x80); }
    IMAGE_EXPORT_DIRECTORY& Exports() { return *reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(bytes.data()+0x1100); }
    IMAGE_DEBUG_DIRECTORY& Debug() { return *reinterpret_cast<IMAGE_DEBUG_DIRECTORY*>(bytes.data()+0x1800); }
    template<class T> void Put(size_t offset, T value) { std::memcpy(bytes.data()+offset, &value, sizeof(value)); }
    void String(size_t offset, const char* value) { std::memcpy(bytes.data()+offset, value, std::strlen(value)+1); }
    Fixture() {
        auto& dos = *reinterpret_cast<IMAGE_DOS_HEADER*>(bytes.data());
        dos.e_magic = IMAGE_DOS_SIGNATURE; dos.e_lfanew = 0x80;
        auto& nt = Nt(); nt.Signature = IMAGE_NT_SIGNATURE;
        nt.FileHeader.Machine = IMAGE_FILE_MACHINE_AMD64;
        nt.FileHeader.NumberOfSections = 3;
        nt.FileHeader.SizeOfOptionalHeader = sizeof(IMAGE_OPTIONAL_HEADER64);
        nt.OptionalHeader.Magic = IMAGE_NT_OPTIONAL_HDR64_MAGIC;
        nt.OptionalHeader.SizeOfImage = static_cast<DWORD>(bytes.size());
        nt.OptionalHeader.NumberOfRvaAndSizes = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;
        nt.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT] = {0x1100,0x400};
        nt.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG] = {0x1800,sizeof(IMAGE_DEBUG_DIRECTORY)};
        auto* sections = IMAGE_FIRST_SECTION(&nt);
        sections[0].VirtualAddress = 0x1000; sections[0].Misc.VirtualSize = 0x2000; sections[0].Characteristics = IMAGE_SCN_MEM_READ;
        sections[1].VirtualAddress = 0x3000; sections[1].Misc.VirtualSize = 0x1000; sections[1].Characteristics = IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_EXECUTE;
        sections[2].VirtualAddress = 0x4000; sections[2].Misc.VirtualSize = 0x1000; sections[2].Characteristics = IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_WRITE;
        auto& e = Exports(); e.Base = 1; e.NumberOfFunctions = 4; e.NumberOfNames = 3;
        e.AddressOfFunctions = 0x1200; e.AddressOfNames = 0x1240; e.AddressOfNameOrdinals = 0x1250;
        Put<DWORD>(0x1200,0x3000); Put<DWORD>(0x1204,0x4000); Put<DWORD>(0x1208,0x1300); Put<DWORD>(0x120c,0x3010);
        Put<DWORD>(0x1240,0x1400); Put<DWORD>(0x1244,0x1420); Put<DWORD>(0x1248,0x1440);
        Put<WORD>(0x1250,0); Put<WORD>(0x1252,1); Put<WORD>(0x1254,2);
        String(0x1400,"CreateInterface"); String(0x1420,"g_Value"); String(0x1440,"Forwarded"); String(0x1300,"KERNEL32.Sleep");
        auto& debug = Debug(); debug.Type = IMAGE_DEBUG_TYPE_CODEVIEW; debug.AddressOfRawData = 0x1900; debug.SizeOfData = 64;
        Put<DWORD>(0x1900,0x53445352);
        const GUID guid{0x419b8bb8,0x78e2,0x4868,{0x8a,0x21,0x18,0x1a,0xbc,0x98,0x43,0x49}};
        Put(0x1904,guid); Put<DWORD>(0x1914,56); String(0x1918,"client.pdb");
    }
    ImageInfo Inspect() const { return InspectImage(bytes.data(),bytes.size()); }
    ExportList List() const { return InspectExports(bytes.data(),bytes.size()); }
};

static void ParserChecks() {
    Fixture image;
    const auto info = image.Inspect();
    Check(info.valid && info.machine == 0x8664 && info.exports == 4 && info.named_exports == 3, "Read PE metadata");
    Check(info.pdb_identity == "419B8BB878E248688A21181ABC98434938" && info.pdb_path == "client.pdb", "Correct GUID endianness and hexadecimal age");
    const auto list = image.List();
    Check(list.error.empty() && list.entries.size() == 4, "Include named and ordinal-only exports");
    const auto find = [&](const char* name) -> const Export& {
        for (const auto& e : list.entries) if (e.name == name) return e;
        throw std::runtime_error("Missing fixture export");
    };
    Check(find("CreateInterface").executable && find("CreateInterface").rva == 0x3000, "Classify executable export");
    Check(!find("g_Value").executable && find("g_Value").forwarder.empty(), "Exported data is not code");
    Check(find("Forwarded").forwarder == "KERNEL32.Sleep", "Classify forwarded export");
    Check(find("#4").ordinal == 4, "Ordinal-only name retained");
    Check(list.pdb_identity == info.pdb_identity, "Export details retain the inspected image identity");
    {
        Fixture x86;
        const auto old = x86.Nt();
        std::array<IMAGE_SECTION_HEADER,3> sections{};
        std::memcpy(sections.data(), IMAGE_FIRST_SECTION(&x86.Nt()), sizeof(sections));
        IMAGE_NT_HEADERS32 nt{};
        nt.Signature = IMAGE_NT_SIGNATURE; nt.FileHeader = old.FileHeader;
        nt.FileHeader.Machine = IMAGE_FILE_MACHINE_I386;
        nt.FileHeader.SizeOfOptionalHeader = sizeof(IMAGE_OPTIONAL_HEADER32);
        nt.OptionalHeader.Magic = IMAGE_NT_OPTIONAL_HDR32_MAGIC;
        nt.OptionalHeader.SizeOfImage = old.OptionalHeader.SizeOfImage;
        nt.OptionalHeader.NumberOfRvaAndSizes = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;
        std::memcpy(nt.OptionalHeader.DataDirectory,old.OptionalHeader.DataDirectory,sizeof(nt.OptionalHeader.DataDirectory));
        std::memcpy(x86.bytes.data()+0x80,&nt,sizeof(nt));
        std::memcpy(x86.bytes.data()+0x80+sizeof(nt),sections.data(),sizeof(sections));
        Check(x86.Inspect().valid && x86.Inspect().machine == IMAGE_FILE_MACHINE_I386 && x86.List().entries.size() == 4,
            "Inspect legacy x86 metadata without executing it");
    }
    Baseline baseline{"client.dll","Client",0x8664,"419B8BB878E248688A21181ABC98434938"};
    Check(Compare(baseline,info) == Identity::Match, "Compare researched identity");
    baseline.pdb_identity = "DIFFERENT";
    Check(Compare(baseline,info) == Identity::Changed, "Detect updated identity");
    baseline.pdb_identity = "";
    Check(Compare(baseline,info) == Identity::Unverified, "Missing baseline is not compatibility proof");
    baseline.machine = 0x14c;
    Check(Compare(baseline,info) == Identity::Changed, "Architecture mismatch detected independently of PDB");
    image.Put<WORD>(0x1250,5);
    Check(!image.List().error.empty() && image.List().entries.empty(), "Invalid ordinal rejects whole export list");
    image = Fixture{}; image.Exports().NumberOfNames = 0xffffffff;
    Check(!image.Inspect().valid, "Bound export-table counts");
    image = Fixture{}; image.Debug().SizeOfData = 24;
    Check(!image.Inspect().valid, "Reject truncated RSDS");
    image = Fixture{}; image.Debug().SizeOfData = 16; image.Put<DWORD>(0x1900,0x3031424e);
    Check(image.Inspect().valid && image.Inspect().pdb_identity.empty(), "Old CodeView identity stays unverified");
    image = Fixture{}; image.Debug().AddressOfRawData = 0xfffffff0;
    Check(!image.Inspect().valid, "Reject CodeView RVA overflow");
    image = Fixture{}; image.Debug().SizeOfData = 25; image.bytes[0x1918] = 'X';
    Check(!image.Inspect().valid, "Reject unterminated bounded PDB path");
    image = Fixture{}; image.Nt().OptionalHeader.SizeOfImage = 0xffffffff;
    Check(!image.Inspect().valid, "Do not trust image size beyond supplied mapping");
    Check(!InspectImage(nullptr,0x6000).valid, "Null image safe");
    auto* inaccessible = VirtualAlloc(nullptr,0x1000,MEM_RESERVE|MEM_COMMIT,PAGE_NOACCESS);
    Check(inaccessible != nullptr, "Allocate inaccessible fixture");
    const bool rejected = !InspectImage(inaccessible,0x1000).valid;
    VirtualFree(inaccessible,0,MEM_RELEASE);
    Check(rejected,"Inaccessible image rejected without AV");
}

static void LiveAndReportChecks() {
    Check(Catalog().size() == 103, "102 researched modules plus overlay dependency");
    std::set<std::string> names;
    for (const auto& spec : Catalog()) Check(names.insert(spec.name).second, "Unique catalog module names");
    const Baseline specs[]{{"kernel32.dll","Owned process system dependency",0x8664,""},
        {"nephilim_inspector_missing_94832.dll","Missing test module",0x8664,""}};
    const auto snapshot = Capture(specs);
    Check(snapshot.loaded == 1 && snapshot.unverified == 1 && snapshot.modules[1].identity == Identity::Missing,
        "Inspect only loaded modules; do not load missing libraries");
    Check(!GetModuleHandleA(specs[1].name), "Missing dependency remains unloaded");
    Check(snapshot.modules[0].image.valid && !snapshot.modules[0].path.empty(), "Read real mapped Windows image");
    const auto exports = CaptureExports("kernel32.dll");
    Check(exports.error.empty() && !exports.entries.empty(), "Browse real loaded export table without calling exports");
    Check(!CaptureExports(specs[1].name).error.empty(), "Missing-module export request reported");
    Check(CaptureExports("kernel32.dll","mismatched snapshot identity").entries.empty() &&
        !CaptureExports("kernel32.dll","mismatched snapshot identity").error.empty(), "Reject stale snapshot identity");
    const auto report = Report(snapshot);
    Check(report.find("26598758") != std::string::npos && report.find("Identity unavailable") != std::string::npos &&
        report.find(specs[1].name) != std::string::npos, "Report identifies baseline and uncertainty/missing modules");
}

static void UiChecks() {
    menu_advanced::InspectorView view;
    view.exports.entries = {{"Factory",{},1,0x1000,true},{"Forwarded","Factory.Target",2,0x2000,false},
        {"literal %s %n",{},3,0x3000,false}};
    strcpy_s(view.export_filter.InputBuf,"Factory"); view.export_filter.Build(); view.FilterExports();
    Check(view.export_rows.size() == 2,"Search export names and forwarders");
    strcpy_s(view.export_filter.InputBuf,"-Factory"); view.export_filter.Build(); view.FilterExports();
    Check(view.export_rows.size() == 1 && view.export_rows[0] == 2,"Exclusion filter applies to combined export/forwarder text");
    ImGui::CreateContext();
    auto& io = ImGui::GetIO(); io.DisplaySize={1200,1400}; io.DeltaTime=1.f/60; io.IniFilename=nullptr;
    unsigned char* pixels; int width,height;
    io.Fonts->GetTexDataAsRGBA32(&pixels,&width,&height);
    for (int i = 0; i < 3; ++i) {
        ImGui::NewFrame(); ImGui::SetNextWindowSize({1100,1300});
        ImGui::Begin("Inspector test"); menu_advanced::RenderRuntimeTab(); ImGui::End(); ImGui::Render();
        Check(ImGui::GetDrawData()->TotalVtxCount > 0,"Render production inspector page");
    }
    auto& current = menu_advanced::CurrentInspector();
    current.snapshot = {};
    Module module; module.name="client.dll"; module.role="Client gameplay"; module.identity=Identity::Match;
    Fixture fixture; module.image=fixture.Inspect(); module.expected_identity=module.image.pdb_identity;
    module.path="C:/CS2/game/csgo/bin/win64/client.dll";
    current.snapshot.modules.push_back(module); current.snapshot.loaded=1; current.snapshot.matched=1;
    current.attempted=true; current.selected=0; current.exports=fixture.List();
    strcpy_s(current.export_filter.InputBuf,""); current.export_filter.Build();
    for (unsigned i = 0; i < 30000; ++i) current.exports.entries.push_back({"AdditionalExport"+std::to_string(i),{},i+10,0x3000,true});
    current.FilterExports();
    for (int i = 0; i < 3; ++i) {
        ImGui::NewFrame(); ImGui::SetNextWindowPos({10,10}); ImGui::SetNextWindowSize({1100,1250});
        ImGui::Begin("Inspector test"); menu_advanced::RenderRuntimeTab(); ImGui::End(); ImGui::Render();
    }
    Check(ImGui::GetDrawData()->TotalVtxCount < 30000,"Clip large export tables instead of rendering 30000 rows");
    SaveMenuPreview("runtime-inspector.bmp",pixels,width,height);
    ImGui::DestroyContext();
}

int main() {
    try {
        ParserChecks(); LiveAndReportChecks(); UiChecks();
        std::puts("PASS: PE/PDB identity, malformed and inaccessible images, code/data/forwarders/ordinals, loaded-only inspection, report and inspector UI");
        return 0;
    } catch (const std::exception& error) { std::fprintf(stderr,"FAIL: %s\n",error.what()); return 1; }
}
