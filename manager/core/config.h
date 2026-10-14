#pragma once
#include <Windows.h>
#include <mutex>

namespace config {
    inline std::recursive_mutex mutex;
#define CONFIG_VALUE(group, type, name, ...) namespace group { inline type name = __VA_ARGS__; }
#define CONFIG_ARRAY(group, type, name, count, ...) namespace group { inline type name[count] = __VA_ARGS__; }
#define CONFIG_TEXT(group, name, count, ...) namespace group { inline char name[count] = __VA_ARGS__; }
#include "config_schema.inc"
#undef CONFIG_VALUE
#undef CONFIG_ARRAY
#undef CONFIG_TEXT
}
