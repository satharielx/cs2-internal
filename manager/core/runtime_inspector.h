#pragma once
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace runtime_inspector {
    inline constexpr const char* researched_build = "26598758";
    struct Baseline {
        const char* name;
        const char* role;
        std::uint16_t machine;
        const char* pdb_identity;
    };
    enum class Identity { Missing, Match, Changed, Unverified, Invalid };
    struct Section { std::uint32_t rva, size; bool executable; };
    struct ImageInfo {
        bool valid = false;
        std::uint16_t machine = 0;
        std::uint32_t size = 0, exports = 0, named_exports = 0;
        std::string pdb_identity, pdb_path, error;
        std::vector<Section> sections;
        std::uint32_t export_rva = 0, export_size = 0;
    };
    struct Export {
        std::string name, forwarder;
        std::uint32_t ordinal = 0, rva = 0;
        bool executable = false;
    };
    struct ExportList {
        std::vector<Export> entries;
        std::string error;
        std::string pdb_identity;
    };
    struct Module {
        std::string name, role, path, expected_identity;
        Identity identity = Identity::Missing;
        ImageInfo image;
    };
    struct Snapshot {
        std::vector<Module> modules;
        std::uint64_t captured_at_ms = 0;
        std::size_t loaded = 0, matched = 0, changed = 0, unverified = 0, invalid = 0;
    };
    std::span<const Baseline> Catalog();
    // Reads mapped images in this process with checked, bounded memory reads.
    // Does not load modules or call their exports/factories.
    ImageInfo InspectImage(const void* base, std::size_t mapped_size);
    ExportList InspectExports(const void* base, std::size_t mapped_size);
    Identity Compare(const Baseline& baseline, const ImageInfo& image);
    Snapshot Capture(std::span<const Baseline> catalog = Catalog());
    ExportList CaptureExports(const char* module_name, const char* expected_identity = nullptr);
    const char* IdentityLabel(Identity value);
    const char* Architecture(std::uint16_t machine);
    std::string Report(const Snapshot& snapshot);
}
