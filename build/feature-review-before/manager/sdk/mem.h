#pragma once
#include <cstdint>  // for std::uint8_t
#include <cstddef>  // for std::size_t
#include <windows.h> // for Windows API types and functions

namespace sdk {
    // Padding template for struct alignment
    template <std::size_t Size>
    class padding {
    private:
        std::uint8_t m_pad[Size];
    };

    // Check mappings without dereferencing potentially inaccessible memory.
    // As with any mapping check, the caller must still own the object's lifetime.
    inline bool is_readable_range(uintptr_t ptr, std::size_t size) {
        if (!ptr || !size || size > UINTPTR_MAX - ptr) return false;
        const uintptr_t end = ptr + size;
        while (ptr < end) {
            MEMORY_BASIC_INFORMATION info{};
            if (!VirtualQuery(reinterpret_cast<const void*>(ptr), &info, sizeof(info))) return false;
            constexpr DWORD readable = PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY |
                PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
            if (info.State != MEM_COMMIT || (info.Protect & (PAGE_GUARD | PAGE_NOACCESS)) ||
                !(info.Protect & readable)) return false;
            const uintptr_t base = reinterpret_cast<uintptr_t>(info.BaseAddress);
            if (info.RegionSize > UINTPTR_MAX - base) return false;
            const uintptr_t next = base + info.RegionSize;
            if (next <= ptr) return false;
            ptr = next;
        }
        return true;
    }

    inline bool is_valid_ptr(uintptr_t ptr) {
        return is_readable_range(ptr, 1);
    }

    using PVOID = void*;
    inline auto GetCA(intptr_t address) -> PVOID {
        auto IntData = *(int32_t*)(address + 1);
        auto PtrData = address + 5;

        return (PVOID)(IntData + PtrData);
    }

    template<typename T = intptr_t>
    inline auto GetPtrAddress(intptr_t Address) -> T
    {
        auto IntData = *(int32_t*)(Address + 3);
        auto PtrData = Address + 7;

        return (T)(IntData + PtrData);
    }

    template< typename T >
    inline auto vget(PVOID instance, unsigned int index) -> T
    {
        auto procedure_array = *reinterpret_cast<PDWORD_PTR*>(instance);
        return (T)(procedure_array)[index];
    }

    // Get virtual function from VMT (Virtual Method Table)
    template <class Function = void*, std::size_t Index>
    Function virtual_function_get(void* vmt) {
        return (*static_cast<Function**>(vmt))[Index];
    }

    template<unsigned int Index, typename ReturnType, typename ... Args>
    ReturnType CallVFunc(void* thisptr, Args ... argList)
    {
        using Fn = ReturnType(__thiscall*)(void*, decltype(argList)...);
        return (*static_cast<Fn**>(thisptr))[Index](thisptr, argList...);
    }

    // Find byte pattern in a module
    // pattern format: "48 8B 0D ?? ?? ?? ??" (IDA style)
    std::uint8_t* find_pattern(const char* module_name, const char* pattern);
    // Missing modules/matches and malformed signatures return nullptr.
    // The range scanner skips uncommitted, guard and inaccessible pages.
    std::uint8_t* find_pattern_in_range(std::uint8_t* data, std::size_t size, const char* pattern);

    // Resolve RIP-relative address (x64 instruction addressing)
    // instruction: pointer to the instruction
    // offset_to_displacement: offset to the displacement value in the instruction
    // instruction_size: total size of the instruction
    std::uint8_t* resolve_absolute_rip_address(std::uint8_t* instruction,
        std::size_t offset_to_displacement,
        std::size_t instruction_size);

}  // namespace sdk
