# Runtime Inspector

Open **Miscellaneous → Runtime Inspector** in the native menu. The page adds three features based on the [CS2 binary research](research/cs2-26598758/overview.md).

## Build identity dashboard

The module catalog covers the 102 binaries researched in Steam build **26598758**, plus the Steam overlay dependency. The page shows which modules are loaded, their subsystem, architecture and named-export count, and compares their embedded PDB GUID/age with the saved research identity.

- **Match:** architecture and embedded symbol identity match the research snapshot.
- **Changed:** architecture or a comparable PDB identity differs from the snapshot.
- **Unverified:** the baseline or loaded module has no comparable PDB identity.
- **Unreadable:** the mapped image or metadata could not be validated.
- **Not loaded:** the library is absent from this process. Many optional tools/backends are normally absent; use **Loaded only** to hide them.

The header separately shows actual D3D11 capture, frame-callback resolution, the game-state reader status, and missing required render dependencies. Backend DLL presence is not treated as proof that its renderer is active. Identity matching does not establish live feature compatibility or verify every byte of a patched module.

Check the loaded path when investigating a difference: a system copy of a bundled library, such as `d3dcompiler_47.dll`, can differ from the researched game copy without establishing that CS2 itself changed. Identity differences are diagnostic information and do not automatically toggle features.

The first visit captures a snapshot. **Refresh modules** updates it; snapshot age stays visible. There is no per-frame module enumeration or automatic signature rescan. Selecting a module reads its export metadata on demand. Modules are pinned during inspection and released afterward; absent modules are not loaded.

## Searchable module and export browser

Filter modules by name, subsystem, or identity status. Select a row to see its loaded path, expected and observed symbol identities, and exported entries. The export filter searches names and forwarded targets; ImGui's comma-separated inclusion and `-exclusion` syntax is supported.

The export table distinguishes **Code**, **Data**, and **Forwarder** entries and retains ordinal-only exports as `#ordinal`. Addresses are displayed as RVAs. It uses clipped rendering so a library with tens of thousands of exports does not create a draw row for every symbol. Exported functions and interface factories are not called by the inspector. Matching PDBs are not downloaded, and embedded identity records are not presented as loaded debug symbols.

If a module's observed identity changes between snapshot capture and export inspection, the export request is rejected and asks for a refresh. This prevents pairing newer exports with an older module identity.

## Copyable diagnostics

**Copy diagnostic report** copies the module snapshot, identity comparisons, paths, counts, inspection failures, and current renderer/frame/session health to the clipboard. **Copy filtered exports** copies only the selected module's visible export set as tab-separated text, including ordinal, RVA, kind, and forwarder. Clipboard actions require an explicit button click; the app does not upload reports.

## Ownership and baseline maintenance

`core/runtime_inspector.cpp` owns the checked PE/CodeView/export reader and snapshot/report logic. It has no engine ABI or ImGui dependency. `core/ui/runtime_page.cpp` owns menu state, filters, selection, clipped tables, and clipboard actions. Snapshots own strings/metadata rather than retaining raw pointers into inspected images.

`core/runtime_research_baseline.inc` is generated from the saved research inventory. Reproduce the current catalog with:

```powershell
python docs/research/cs2-26598758/generate_runtime_baseline.py
```

The extra Steam-overlay entry has no researched PDB baseline and is intentionally unverified. Its callback signatures were audited separately in the runtime redesign. Do not treat the baseline as updated merely because a game update occurred.

Adopting a new baseline requires a new researched inventory, updating the generator's input and the `researched_build` identifier, then regenerating and verifying the catalog.

## Validation

Run `powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_runtime_inspector_regressions.ps1`. Tests cover x64 and x86 mapped-image metadata, GUID endianness/age formatting, identity changes, exported code/data/forwarders/ordinal-only entries, malformed records, inaccessible pages, loaded-only module access, stale snapshots, report content, filter exclusions, and the production ImGui page with 30,000 export rows. The test also writes `tests/out/runtime_inspector/runtime-inspector.bmp` for offline layout inspection.

The inspector suite is included in `tests/run_all.ps1`. Release output is `build/runtime-inspector/manager.dll`. These tests do not inject the app or establish live CS2 behavior.

Validation completed: **all 18 regression suites passed** and the **Release x64 build succeeded**. Both the standalone export-browser preview and the integrated native-menu preview were visually inspected. The latter is `tests/out/features/runtime-page.bmp`. Artifact identity is recorded in `build/runtime-inspector/validation.json`; build and full-suite logs are `build/runtime-inspector-build.log` and `build/runtime-inspector-all-tests.log`.
