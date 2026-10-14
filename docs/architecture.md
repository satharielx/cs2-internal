# Project structure and maintenance

`manager/core` owns runtime lifecycle, feature coordination and the menu. `manager/sdk` owns checked engine-memory access and reusable entity/projection operations. `manager/external` contains third-party libraries and generated schemas; refactors should not rewrite those files.

## Settings

`core/config_schema.inc` is the source of truth for all 95 setting names, types and default values. `config.h` expands it into runtime variables; `config_registry.inc` expands it into persistence registrations. Add a setting to the schema once. Keep feature switches disabled by default.

`core/config/value_codec.h` handles typed values independently of file I/O. `config_store.h` owns versioned file storage, complete validation before application, loadout persistence and reset. Configuration changes and loadout access use `config::mutex`. Config files remain compatible with version 1.

## Entity and rendering boundaries

`sdk/world_entities.h` discovers and validates world entities. It does not depend on ImGui, menu state or feature settings. `sdk/projection.h` contains world-to-screen math. The feature coordinator decides when to scan and what to render. `core/grenade_trails.h` stores bounded trail history independently of drawing.

## Skin runtime constants

`core/skins/signatures.h` owns engine signature strings as namespaced `inline constexpr` values. `core/skins/offsets.h` derives item/scene/inventory offsets from generated schema members. `core/skins/knife_data.h` owns the immutable knife model/subclass table. Keep runtime caches and resolved function pointers in implementation files; adding them to these headers would expose mutable implementation state.

## Skin catalog ownership

`core/skins/catalog.cpp` owns catalog retrieval and fallback data. `core/skins/names.cpp` owns weapon labels and rarity metadata. These are independently compiled translation units; runtime entity/material/inventory mutation stays in `skins.cpp`. Catalog code has no dependency on renderer or engine-hook internals.

## Menu ownership

`menu_advanced.cpp` owns renderer resources, main navigation, the player/aim pages and the skin browser. `core/ui/settings_pages.cpp` owns the trigger, world, view/environment, HUD, movement, grenade and configuration pages. `core/ui/widgets.cpp` owns shared widget rendering and card animation state, exposed through `widgets.h`. Cards use native ImGui layout without per-frame animation bookkeeping. `ui/theme.h` scopes the compact workspace theme so it cannot leak into the console or overlays.

`menu_advanced.cpp` is the active menu. `skin_menu.h` and `skins_meun.cpp` are retained as historical source but excluded from the Visual Studio project. Do not route active UI through them.

## Validation

Run `powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_all.ps1` from the repository root for the regression suite. Run MSBuild on `manager/manager.vcxproj` with `Configuration=Release` and `Platform=x64` for the DLL build. Tests use owned memory; they do not establish live engine compatibility or anti-cheat safety.

## Remaining structural work

`features.cpp`, `hooks.cpp`, `skins.cpp` and `menu_advanced.cpp` still hold several responsibilities. Future extraction should move state together with its operations behind explicit interfaces and preserve callback ordering, lock ownership, handle validation and restoration. Splitting these files into textual includes would change file sizes without fixing dependencies. The current refactor establishes independent components and one settings schema; it is not a complete rewrite of those runtime systems.

## Constants and signatures

`core/constants` groups hook, game-state, interface, resolver and engine-trace patterns by subsystem. They retain the previous byte patterns and call-site/entry resolution behavior. `constants/features.h` owns shared geometry constants; `ui/catalog.h` owns weapon categories and navigation labels; `ui/icons.h` owns font codepoints. Skin catalog source URLs and cache filenames live in `skins/catalog_sources.h`. Mutable state does not belong in these definition headers. Leave short, local implementation constants close to their use when sharing them would add an unnecessary dependency.
