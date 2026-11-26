#include "runtime_inspector.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <psapi.h>
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <optional>
#include <sstream>

namespace runtime_inspector {
    namespace {
        constexpr Baseline research_catalog[]{
#include "runtime_research_baseline.inc"
        };
        struct Reader {
            uintptr_t base;
            std::size_t size;
            bool Contains(std::size_t rva, std::size_t count) const {
                return base && size <= UINTPTR_MAX - base && rva <= size && count <= size - rva;
            }
            bool Bytes(std::size_t rva, void* out, std::size_t count) const {
                if (!Contains(rva, count)) return false;
                if (!count) return true;
                SIZE_T read = 0;
                return ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<const void*>(base+rva),
                    out, count, &read) && read == count;
            }
            template<class T> bool Read(std::size_t rva, T& value) const { return Bytes(rva, &value, sizeof(value)); }
            std::optional<std::string> String(std::size_t rva, std::size_t limit = 8192) const {
                if (!Contains(rva, 1)) return {};
                limit = (std::min)({limit, size-rva, size_t{8192}});
                std::string value;
                std::array<char, 64> chunk{};
                while (value.size() < limit) {
                    const auto count = (std::min)(chunk.size(), limit-value.size());
                    if (!Bytes(rva+value.size(), chunk.data(), count)) return {};
                    const auto end = static_cast<const char*>(std::memchr(chunk.data(), 0, count));
                    if (end) { value.append(chunk.data(), static_cast<std::size_t>(end-chunk.data())); return value; }
                    value.append(chunk.data(), count);
                }
                return {};
            }
        };
        std::string SymbolIdentity(const GUID& guid, DWORD age) {
            char value[48]{};
            sprintf_s(value, "%08lX%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X%lX",
                guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2],
                guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7], age);
            return value;
        }
        struct Pin {
            HMODULE handle = nullptr;
            explicit Pin(const char* name) { if (name && *name) GetModuleHandleExA(0, name, &handle); }
            ~Pin() { if (handle) FreeLibrary(handle); }
            Pin(const Pin&) = delete;
            Pin& operator=(const Pin&) = delete;
        };
        MODULEINFO ModuleBounds(HMODULE handle) {
            MODULEINFO info{};
            if (handle) K32GetModuleInformation(GetCurrentProcess(), handle, &info, sizeof(info));
            return info;
        }
    }

    std::span<const Baseline> Catalog() { return research_catalog; }

    ImageInfo InspectImage(const void* base, std::size_t mapped_size) {
        ImageInfo result;
        Reader reader{reinterpret_cast<uintptr_t>(base), mapped_size};
        auto fail = [&](const char* error) { result.valid = false; result.error = error; return result; };
        IMAGE_DOS_HEADER dos{};
        DWORD signature = 0;
        IMAGE_FILE_HEADER file{};
        if (!reader.Read(0, dos) || dos.e_magic != IMAGE_DOS_SIGNATURE || dos.e_lfanew < 0 ||
            !reader.Read(static_cast<std::size_t>(dos.e_lfanew), signature) || signature != IMAGE_NT_SIGNATURE ||
            !reader.Read(static_cast<std::size_t>(dos.e_lfanew)+4, file)) return fail("Unreadable or invalid PE header");
        const auto optional = static_cast<std::size_t>(dos.e_lfanew)+4+sizeof(file);
        WORD magic = 0;
        if (!reader.Read(optional, magic)) return fail("Optional header unavailable");
        IMAGE_DATA_DIRECTORY debug{}, exports{};
        DWORD directory_count = 0;
        if (magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC && file.Machine == IMAGE_FILE_MACHINE_AMD64) {
            IMAGE_OPTIONAL_HEADER64 header{};
            if (file.SizeOfOptionalHeader < sizeof(header) || !reader.Read(optional, header)) return fail("Truncated x64 header");
            result.size = header.SizeOfImage;
            directory_count = header.NumberOfRvaAndSizes;
            debug = header.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG];
            exports = header.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
        } else if (magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC && file.Machine == IMAGE_FILE_MACHINE_I386) {
            IMAGE_OPTIONAL_HEADER32 header{};
            if (file.SizeOfOptionalHeader < sizeof(header) || !reader.Read(optional, header)) return fail("Truncated x86 header");
            result.size = header.SizeOfImage;
            directory_count = header.NumberOfRvaAndSizes;
            debug = header.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG];
            exports = header.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
        } else return fail("Unsupported or inconsistent PE architecture");
        result.machine = file.Machine;
        if (!result.size || result.size > mapped_size || !file.NumberOfSections || file.NumberOfSections > 96)
            return fail("Image extent or section count invalid");
        reader.size = result.size;
        const auto table = optional+file.SizeOfOptionalHeader;
        for (unsigned i = 0; i < file.NumberOfSections; ++i) {
            IMAGE_SECTION_HEADER section{};
            if (!reader.Read(table+i*sizeof(section), section)) return fail("Section table unreadable");
            const auto size = section.Misc.VirtualSize ? section.Misc.VirtualSize : section.SizeOfRawData;
            if (!reader.Contains(section.VirtualAddress, size)) return fail("Section outside image");
            result.sections.push_back({section.VirtualAddress, size, (section.Characteristics & IMAGE_SCN_MEM_EXECUTE) != 0});
        }
        if (directory_count > IMAGE_DIRECTORY_ENTRY_EXPORT && exports.VirtualAddress) {
            IMAGE_EXPORT_DIRECTORY directory{};
            if (exports.Size < sizeof(directory) || !reader.Contains(exports.VirtualAddress, exports.Size) ||
                !reader.Read(exports.VirtualAddress, directory) || directory.NumberOfFunctions > 1000000 ||
                directory.NumberOfNames > 1000000 ||
                !reader.Contains(directory.AddressOfFunctions, static_cast<size_t>(directory.NumberOfFunctions)*4) ||
                !reader.Contains(directory.AddressOfNames, static_cast<size_t>(directory.NumberOfNames)*4) ||
                !reader.Contains(directory.AddressOfNameOrdinals, static_cast<size_t>(directory.NumberOfNames)*2))
                return fail("Export directory invalid");
            result.exports = directory.NumberOfFunctions;
            result.named_exports = directory.NumberOfNames;
            result.export_rva = exports.VirtualAddress;
            result.export_size = exports.Size;
        }
        if (directory_count > IMAGE_DIRECTORY_ENTRY_DEBUG && debug.VirtualAddress) {
            if (!reader.Contains(debug.VirtualAddress, debug.Size) || debug.Size % sizeof(IMAGE_DEBUG_DIRECTORY) ||
                debug.Size/sizeof(IMAGE_DEBUG_DIRECTORY) > 4096) return fail("Debug directory invalid");
            for (std::size_t i = 0; i < debug.Size/sizeof(IMAGE_DEBUG_DIRECTORY); ++i) {
                IMAGE_DEBUG_DIRECTORY entry{};
                if (!reader.Read(debug.VirtualAddress+i*sizeof(entry), entry)) return fail("Debug entry unreadable");
                if (entry.Type != IMAGE_DEBUG_TYPE_CODEVIEW) continue;
                DWORD cv = 0, age = 0;
                GUID guid{};
                const auto rva = static_cast<std::size_t>(entry.AddressOfRawData);
                if (entry.SizeOfData < 4 || !reader.Contains(rva, entry.SizeOfData) ||
                    !reader.Read(rva, cv)) return fail("CodeView record invalid");
                if (cv != 0x53445352) continue; // RSDS; old CodeView formats have no matching GUID identity.
                if (entry.SizeOfData < 25) return fail("Truncated RSDS identity");
                if (!reader.Read(rva+4, guid) || !reader.Read(rva+20, age)) return fail("CodeView identity unreadable");
                auto path = reader.String(rva+24, (std::min)(static_cast<size_t>(entry.SizeOfData)-24, size_t{8192}));
                if (!path) return fail("CodeView path unterminated or unreadable");
                result.pdb_identity = SymbolIdentity(guid, age);
                result.pdb_path = std::move(*path);
                break;
            }
        }
        result.valid = true;
        return result;
    }

    ExportList InspectExports(const void* base, std::size_t mapped_size) {
        ExportList result;
        const auto image = InspectImage(base, mapped_size);
        if (!image.valid) { result.error = image.error; return result; }
        result.pdb_identity = image.pdb_identity;
        if (!image.export_rva) return result;
        Reader reader{reinterpret_cast<uintptr_t>(base), image.size};
        auto fail = [&](const char* error) { result.entries.clear(); result.error = error; return result; };
        IMAGE_EXPORT_DIRECTORY directory{};
        if (!reader.Read(image.export_rva, directory)) return fail("Export directory became unreadable");
        // Counts and extents come from the validated copy, not re-read mutable counts.
        if (directory.NumberOfFunctions != image.exports || directory.NumberOfNames != image.named_exports)
            return fail("Export directory changed during inspection");
        std::vector<DWORD> functions(image.exports), names(image.named_exports);
        std::vector<WORD> ordinals(image.named_exports);
        if (!reader.Bytes(directory.AddressOfFunctions, functions.data(), functions.size()*sizeof(DWORD)) ||
            !reader.Bytes(directory.AddressOfNames, names.data(), names.size()*sizeof(DWORD)) ||
            !reader.Bytes(directory.AddressOfNameOrdinals, ordinals.data(), ordinals.size()*sizeof(WORD)))
            return fail("Export tables unreadable");
        std::vector<bool> named(functions.size());
        auto append = [&](std::string name, std::size_t index) {
            if (index >= functions.size() || index > UINT32_MAX-directory.Base) return false;
            const auto rva = functions[index];
            if (!rva || !reader.Contains(rva, 1)) return false;
            Export entry{std::move(name), {}, directory.Base+static_cast<DWORD>(index), rva, false};
            if (rva >= image.export_rva && rva-image.export_rva < image.export_size) {
                auto target = reader.String(rva, image.export_size-(rva-image.export_rva));
                if (!target) return false;
                entry.forwarder = std::move(*target);
            } else for (const auto& section : image.sections)
                if (rva >= section.rva && rva-section.rva < section.size) entry.executable = section.executable;
            result.entries.push_back(std::move(entry));
            return true;
        };
        for (std::size_t i = 0; i < names.size(); ++i) {
            auto name = reader.String(names[i]);
            if (!name || !append(std::move(*name), ordinals[i])) return fail("Export name, ordinal, or target invalid");
            named[ordinals[i]] = true;
        }
        for (std::size_t i = 0; i < functions.size(); ++i)
            if (functions[i] && !named[i] && !append("#"+std::to_string(directory.Base+i), i))
                return fail("Ordinal export target invalid");
        std::sort(result.entries.begin(), result.entries.end(), [](const Export& a, const Export& b) { return a.name < b.name; });
        return result;
    }

    Identity Compare(const Baseline& baseline, const ImageInfo& image) {
        if (!image.valid) return Identity::Invalid;
        if (image.machine != baseline.machine) return Identity::Changed;
        if (!baseline.pdb_identity || !*baseline.pdb_identity || image.pdb_identity.empty()) return Identity::Unverified;
        return image.pdb_identity == baseline.pdb_identity ? Identity::Match : Identity::Changed;
    }

    Snapshot Capture(std::span<const Baseline> catalog) {
        Snapshot snapshot;
        snapshot.captured_at_ms = GetTickCount64();
        for (const auto& spec : catalog) {
            Module module;
            module.name = spec.name;
            module.role = spec.role;
            module.expected_identity = spec.pdb_identity ? spec.pdb_identity : "";
            Pin pin(spec.name);
            if (pin.handle) {
                ++snapshot.loaded;
                const auto bounds = ModuleBounds(pin.handle);
                module.image = InspectImage(bounds.lpBaseOfDll, bounds.SizeOfImage);
                module.identity = Compare(spec, module.image);
                std::array<wchar_t, 32768> path{};
                const auto length = GetModuleFileNameW(pin.handle, path.data(), static_cast<DWORD>(path.size()));
                if (length && length < path.size()) {
                    const auto bytes = WideCharToMultiByte(CP_UTF8, 0, path.data(), static_cast<int>(length), nullptr, 0, nullptr, nullptr);
                    if (bytes > 0) {
                        module.path.resize(bytes);
                        WideCharToMultiByte(CP_UTF8, 0, path.data(), static_cast<int>(length), module.path.data(), bytes, nullptr, nullptr);
                    }
                }
                switch (module.identity) {
                    case Identity::Match: ++snapshot.matched; break;
                    case Identity::Changed: ++snapshot.changed; break;
                    case Identity::Unverified: ++snapshot.unverified; break;
                    case Identity::Invalid: ++snapshot.invalid; break;
                    default: break;
                }
            }
            snapshot.modules.push_back(std::move(module));
        }
        return snapshot;
    }

    ExportList CaptureExports(const char* module_name, const char* expected_identity) {
        Pin pin(module_name);
        if (!pin.handle) return {{}, "Module is not loaded"};
        const auto bounds = ModuleBounds(pin.handle);
        auto result = InspectExports(bounds.lpBaseOfDll, bounds.SizeOfImage);
        if (expected_identity && *expected_identity && result.pdb_identity != expected_identity) {
            result.entries.clear();
            result.error = "Module identity changed or became unavailable; refresh the module snapshot";
        }
        return result;
    }
    const char* IdentityLabel(Identity value) {
        switch (value) {
            case Identity::Match: return "Research identity match";
            case Identity::Changed: return "Changed since research";
            case Identity::Unverified: return "Identity unavailable";
            case Identity::Invalid: return "Invalid / unreadable image";
            default: return "Not loaded";
        }
    }
    const char* Architecture(std::uint16_t machine) {
        if (machine == IMAGE_FILE_MACHINE_AMD64) return "x86-64";
        if (machine == IMAGE_FILE_MACHINE_I386) return "x86";
        return "Unknown";
    }
    std::string Report(const Snapshot& snapshot) {
        std::ostringstream out;
        out << "NEPHILIMGATE Runtime Inspector\nResearch baseline: Steam build " << researched_build
            << "\nSnapshot uptime (ms): " << snapshot.captured_at_ms << "\nLoaded: " << snapshot.loaded
            << " | Match: " << snapshot.matched << " | Changed: " << snapshot.changed
            << " | Unverified: " << snapshot.unverified << " | Invalid: " << snapshot.invalid
            << "\nIdentity compares architecture and embedded PDB GUID/age; it does not validate live feature behavior.\n";
        for (const auto& module : snapshot.modules) {
            out << "\n" << module.name << " | " << module.role << " | " << IdentityLabel(module.identity) << '\n';
            if (module.identity == Identity::Missing) continue;
            out << "Path: " << module.path << "\nArchitecture: " << Architecture(module.image.machine)
                << " | Image bytes: " << module.image.size << " | Export slots: " << module.image.exports
                << " | Named exports: " << module.image.named_exports << "\nExpected identity: " << module.expected_identity
                << "\nObserved identity: " << module.image.pdb_identity << '\n';
            if (!module.image.error.empty()) out << "Inspection error: " << module.image.error << '\n';
        }
        return out.str();
    }
}
