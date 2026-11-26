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

## Runtime discovery and lifecycle

`dllmain.cpp` owns a staged runtime session: interface discovery, state monitor, hook setup, then feature worker. Teardown joins the feature worker, restores/removes hooks while the monitor still supplies state, then stops the monitor and releases interfaces. A failed stop retains its dependencies and prevents unloading the DLL. `core/runtime_startup.h` owns the required module list and dependency diagnostics. Startup waits for actual mandatory overlay entry points instead of a fixed initialization sleep.

`sdk/mem.cpp` validates loaded PE architecture and section extents, scans code sections, and resolves RIP-relative displacements with checked reads and arithmetic. Use `find_unique_pattern` for callback/global discovery; the existing first-match API remains for explicitly chosen legacy callers. `pattern_resolver::Reset` clears published addresses before initialization and after stopped teardown. `interfaces::frame_stage_notify` is a code address, not a factory-created object.

See [runtime redesign](runtime-redesign.md) for the concrete failure modes, installed-binary signature audit, confidence limits, and verification commands.

## Runtime Inspector

`core/runtime_inspector.cpp` owns bounded inspection of mapped PE images, PDB identities, export metadata, module snapshots and diagnostic reports. It does not invoke engine functions or load missing libraries. `core/runtime_research_baseline.inc` is generated from the saved binary inventory. `core/ui/runtime_page.cpp` owns the Inspector menu page, snapshot refresh, search state, clipped export tables, and explicit clipboard actions. Module enumeration and export inspection occur on demand; no inspector work runs in frame callbacks while the page is closed. See [Runtime Inspector](runtime-inspector.md) for usage and baseline maintenance.

## Cosmetic inventory and model inspection

`core/inventory.h` owns the local item collection and transactional persistence under `config::mutex`; `core/inventory.cpp` owns researched-build-gated native cache operations queued to frame callbacks. Never invoke native inventory methods from ImGui callbacks. The HUD refresh primary base for build 26598758 is tick this -0x98, guarded by the inspected tick layout; older validation notes use a different build's adjustment. `core/model_inspector.h` owns bounded OBJ parsing/geometry math; `ui/model_preview.h` owns standalone preview drawing and controls. It does not extract or render native game-model resources. See [inventory/HUD/chams](inventory-hud-chams.md).

## Constants and signatures

`core/constants` groups hook, game-state, interface, resolver and engine-trace patterns by subsystem. They retain the previous byte patterns and call-site/entry resolution behavior. `constants/features.h` owns shared geometry constants; `ui/catalog.h` owns weapon categories and navigation labels; `ui/icons.h` owns font codepoints. Skin catalog source URLs and cache filenames live in `skins/catalog_sources.h`. Mutable state does not belong in these definition headers. Leave short, local implementation constants close to their use when sharing them would add an unnecessary dependency.
