// Use the same settings definitions as config.h.
#define CONFIG_VALUE(group, type, name, ...) Add(#group "." #name, config::group::name, __VA_ARGS__);
#define CONFIG_ARRAY(group, type, name, count, ...) Add(#group "." #name, config::group::name, __VA_ARGS__);
#define CONFIG_TEXT(group, name, count, ...) Add(#group "." #name, config::group::name, __VA_ARGS__);
#include "config_schema.inc"
#undef CONFIG_VALUE
#undef CONFIG_ARRAY
#undef CONFIG_TEXT
