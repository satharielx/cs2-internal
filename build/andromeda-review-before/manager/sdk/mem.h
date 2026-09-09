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

    // Safe pointer validation - checks if pointer is readable
    inline bool is_valid_ptr(uintptr_t ptr) {
        if (ptr == 0 || ptr == 0xFFFFFFFF || ptr == 0xFFFFFFFFFFFFFFFF) return false;
        if (ptr < 0x1000) return false;  // Kernel memory
        
        try {
            // Try to read a single byte
            volatile uint8_t test = *reinterpret_cast<uint8_t*>(ptr);
            return true;
        }
        catch (...) {
            return false;
        }
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

    // Resolve RIP-relative address (x64 instruction addressing)
    // instruction: pointer to the instruction
    // offset_to_displacement: offset to the displacement value in the instruction
    // instruction_size: total size of the instruction
    std::uint8_t* resolve_absolute_rip_address(std::uint8_t* instruction,
        std::size_t offset_to_displacement,
        std::size_t instruction_size);

}  // namespace sdk