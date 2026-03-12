/*
Module Name: Minimal CFunctionList stub for optional runtime function list usage
Authors: sathariel, martinmarinov
Product: Nephilimgate Multicheat
Tools used: imgui, a2x-cs2dumper
© 2026 sathariel & martinmarinov
*/

#pragma once

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

class CFunctionList;

extern CFunctionList* GetFunctionList();
