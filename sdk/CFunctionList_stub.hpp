#pragma once

// Minimal stub for CFunctionList to allow optional use of function list
// This file intentionally provides lightweight definitions so skins.cpp
// can attempt to use GetFunctionList() if available at runtime. The
// real implementation should replace this stub.

struct CBasePattern {
    const char* name = nullptr;
    const char* pattern = nullptr;
    const char* module = nullptr;
    void* resolved = nullptr;

    CBasePattern() = default;
    CBasePattern(const char* n, const char* p, const char* m) : name(n), pattern(p), module(m), resolved(nullptr) {}

    void Set(const char* n, const char* p, const char* m) {
        name = n; pattern = p; module = m; resolved = nullptr;
    }

    void SetResolved(void* addr) { resolved = addr; }

    void* GetFunction() const { return resolved; }
};

// Forward declaration for the real CFunctionList defined elsewhere.
class CFunctionList;

// The real project should provide an implementation of GetFunctionList().
// We do not provide a default here to avoid conflicting definitions.
extern CFunctionList* GetFunctionList();
