#pragma once
#include "constants/engine_trace_signatures.h"
#include "../sdk/entity.h"
#include <cstddef>

// Layouts checked against the installed client.dll disassembly (2026-10-06).
// Live-game behavior still requires validation; unavailable traces never pass.
namespace engine_trace {
    // MASK_SHOT | CONTENTS_SKY, using the supplied CTraceMasks definitions.
    inline constexpr uint64_t shot_mask = 0x1ULL | 0x2ULL | 0x1000ULL |
        0x2000ULL | 0x40000ULL | 0x80000ULL | 0x100000ULL;
    inline constexpr uint64_t visibility_mask = shot_mask | 0x8ULL;
    static_assert(visibility_mask == 0x1C300BULL);
    inline std::atomic<bool> faulted{false};
    inline std::atomic<const char*> status{"Not initialized"};
    struct alignas(16) Hit {
        void* surface; void* entity; void* hitbox;
        char pad1[0x10]; uint32_t contents; char pad2[0x4A];
        sdk::Vector3 start, end, normal, position;
        char pad3[4]; float fraction; char pad4[0xB];
        bool all_solid; char pad5[0x54];
    };
    struct Surface { char data[0x38]; };
    struct Entry {
        float start, end, damage; int secondary;
        uint16_t surface_start, surface_end; uint8_t flags, pad[3];
    };
    struct Array { int size; char pad[4]; Entry* data; char tail[8]; };
    struct alignas(16) Data {
        char pad[24]; Surface surfaces[128]; char pad2[8];
        Array entries; Entry inline_entries[8];
        sdk::Vector3 start, end; char tail[12];
    };
    struct alignas(16) Filter { char data[164]; };
    static_assert(offsetof(Hit, fraction) == 0xAC && sizeof(Hit) == 0x110);
    static_assert(offsetof(Data, entries) == 0x1C20 && sizeof(Data) == 0x1D20);
    static_assert(offsetof(Data, start) == 0x1CF8 && offsetof(Hit, all_solid) == 0xBB);
    static_assert(sizeof(Entry) == 24 && sizeof(Filter) == 176);
    using InitData = void(__fastcall*)(Data*);
    using InitHit = void(__fastcall*)(Hit*);
    using InitFilter = void*(__fastcall*)(Filter*, uintptr_t, uint64_t, int, int);
    using Create = void(__fastcall*)(Data*, sdk::Vector3, sdk::Vector3, Filter*, int, bool);
    using GetHit = void(__fastcall*)(Data*, Hit*, float, void*);
    struct Functions {
        InitData init_data{}; InitHit init_hit{}; InitFilter init_filter{};
        Create create{}; GetHit get_hit{};
        bool Ready() const { return init_data && init_hit && init_filter && create && get_hit; }
    };
    inline Functions functions;
    inline const char* MissingStatus() {
        if (!functions.init_data) return "Trace signatures missing: InitTraceData";
        if (!functions.init_hit) return "Trace signatures missing: InitTraceInfo";
        if (!functions.init_filter) return "Trace signatures missing: InitFilter";
        if (!functions.create) return "Trace signatures missing: CreateTrace";
        if (!functions.get_hit) return "Trace signatures missing: GetTraceInfo";
        return "Trace functions resolved";
    }
    inline void Initialize() {
        functions = {
            reinterpret_cast<InitData>(sdk::find_pattern("client.dll", runtime_patterns::engine_trace::init_data)),
            reinterpret_cast<InitHit>(sdk::find_pattern("client.dll", runtime_patterns::engine_trace::init_hit)),
            reinterpret_cast<InitFilter>(sdk::find_pattern("client.dll", runtime_patterns::engine_trace::init_filter)),
            reinterpret_cast<Create>(sdk::find_pattern("client.dll", runtime_patterns::engine_trace::create_trace)),
            reinterpret_cast<GetHit>(sdk::find_pattern("client.dll", runtime_patterns::engine_trace::get_hit))
        };
        faulted = false;
        status = functions.Ready() ? "Ready; awaiting trace" : MissingStatus();
    }
    enum class Result { Unavailable, Failed, Blocked, Visible };
    inline Result Trace(const Functions& fn, const sdk::Vector3& start,
        const sdk::Vector3& end, uintptr_t skip, uintptr_t target, uint64_t mask) {
        if (!fn.Ready() || !mask) return Result::Unavailable;
        if (!skip || !target || !sdk::finite(start) || !sdk::finite(end)) return Result::Failed;
        const auto delta = end - start;
        if (!sdk::finite(delta) || delta.Length() <= 0) return Result::Failed;
        Data data{}; Filter filter{}; Hit hit{};
        fn.init_data(&data);
        fn.init_filter(&filter, skip, mask, 4, 7);
        fn.create(&data, start, delta, &filter, 4, true);
        if (data.entries.size < 0 || data.entries.size > 128) return Result::Failed;
        if (!data.entries.size) return Result::Visible;
        Entry entry{};
        if (!data.entries.data || !sdk::read_memory(reinterpret_cast<uintptr_t>(data.entries.data), entry)) return Result::Failed;
        const unsigned index = entry.surface_end & 0x7FFF;
        if (index >= 128 || !std::isfinite(entry.start) || entry.start < 0 || entry.start > 1) return Result::Failed;
        fn.init_hit(&hit);
        fn.get_hit(&data, &hit, entry.start, &data.surfaces[index]);
        if (!std::isfinite(hit.fraction) || hit.fraction < 0 || hit.fraction > 1) return Result::Failed;
        if (hit.all_solid) return Result::Blocked;
        // A 0.97 tolerance could accept a wall near the destination.
        return reinterpret_cast<uintptr_t>(hit.entity) == target || hit.fraction == 1.f
            ? Result::Visible : Result::Blocked;
    }
    // Keep SEH separate from code that owns C++ objects.
    inline Result GuardedTrace(const sdk::Vector3& start, const sdk::Vector3& end,
        uintptr_t skip, uintptr_t target) {
        __try { return Trace(functions, start, end, skip, target, visibility_mask); }
        __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ||
            GetExceptionCode() == EXCEPTION_IN_PAGE_ERROR ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
            faulted = true;
            return Result::Failed;
        }
    }
    inline Result Visible(const sdk::Vector3& start, const sdk::Vector3& end,
        uintptr_t skip, uintptr_t target) {
        if (faulted) { status = "Trace fault; disabled until reload"; return Result::Failed; }
        const auto result = GuardedTrace(start, end, skip, target);
        status = faulted ? "Trace fault; disabled until reload" :
            result == Result::Unavailable ? MissingStatus() :
            result == Result::Failed ? "Trace returned invalid data or failed" :
            result == Result::Blocked ? "Blocked by collision" : "Clear line of sight";
        return result;
    }
}
