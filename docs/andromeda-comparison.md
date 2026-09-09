Compared the local initialization and rendering code with [Andromeda-CS2-Base at commit 006ff8b](https://github.com/or75/Andromeda-CS2-Base/tree/006ff8bea0ada49face963b9e733b5ca1a97e03b).

The subsequent [feature review](andromeda-feature-review.md) covers shared reads, aiming/RCS, ESP, input/misc features, weapon settings and glove selection, with a newer build at `build/feature-review/manager.dll`.

The review covered `CHook_Loader`, `CSDK_Loader`, SDK interface capture, `MemoryEngine`, `CAndromedaGUI`, and the Present, ResizeBuffers and CreateSwapChain callbacks. Changes target concrete startup and rendering defects in this project.

| Area | Finding and applied fix |
| --- | --- |
| Swap-chain creation | The reference's [CreateSwapChain callback](https://github.com/or75/Andromeda-CS2-Base/blob/006ff8bea0ada49face963b9e733b5ca1a97e03b/Andromeda-CS2-Base/Andromeda-CS2-Base/CS2/Hook/Hook_CreateSwapChain.cpp) releases its render target before factory creation. This project also owns a reference to the selected swap chain. Added the matching overlay hook and release of owned renderer resources before same-window replacement. The original runs once outside the renderer mutex, and the next Present initializes resources. Foreign windows preserve the selected renderer. |
| Hook installation | The reference's [hook loader](https://github.com/or75/Andromeda-CS2-Base/blob/006ff8bea0ada49face963b9e733b5ca1a97e03b/Andromeda-CS2-Base/Andromeda-CS2-Base/CS2/CHook_Loader.cpp) prepares its hook list before activation. Removed early activation from local hook helpers. Resolution and preparation now share the rollback boundary, followed by one checked activation call. Present, ResizeBuffers and CreateSwapChain are required rendering hooks. |
| Pattern scanning | Local callers checked for null, but the scanner threw for missing patterns/modules. Normal misses now return null. Added strict token parsing, final-position matching, range-size checks and skipping of inaccessible/guard pages. Readability checks now query mappings instead of deliberately dereferencing untrusted addresses inside a C++ catch block. |
| Optional interfaces | Local `cs_io` and `input_system` captures have no consumers in this build, yet failures aborted rendering startup. They now report unavailable state without aborting the independent renderer. Pointer slots are validated for a complete pointer-sized read. |
| ImGui ownership | The reference's [GUI implementation](https://github.com/or75/Andromeda-CS2-Base/blob/006ff8bea0ada49face963b9e733b5ca1a97e03b/Andromeda-CS2-Base/Andromeda-CS2-Base/AndromedaClient/CAndromedaGUI.cpp) stores and selects its context. The local renderer now owns an explicit context, selects it while handling frames/input/resize, restores the previous context, and destroys only its own context. |
| Menu toggles | Switched Insert/F1 toggles to key release to avoid repeated toggles from key-down autorepeat. |

The existing COM-based Present capture, inferred render-target format, back-buffer release on resize, and complete output-merger reference restoration remain the basis of rendering.

Validation passed:

- Debug x64 project build, producing `build/andromeda-review/manager.dll`.
- `tests/run_pattern_scanner_smoke.ps1`: malformed signatures, wildcards, final candidate, missing module, inaccessible/guard pages and matches spanning readable regions.
- `tests/run_swapchain_resources_smoke.ps1`: native D3D11 WARP capture, failed queries, ownership, resize success/failure recovery and replacement/destruction.
- `tests/run_swapchain_hook_callbacks_smoke.ps1`: production callbacks with WARP, exactly-once forwarding, unlocked original calls, foreign-window isolation, creation failure recovery, external ImGui-context preservation, optional-pattern startup and required-hook rollback. Hook registration and unrelated gameplay/UI dependencies are stubbed in this harness.

Run each test script with `powershell -NoProfile -ExecutionPolicy Bypass -File <script-path>`.

The build used `TrackFileAccess=false` and `ScanSourceForModuleDependencies=false` to avoid this sandbox's MSBuild file-tracker access failure; project compiler settings were unchanged. Existing conversion/encoding warnings remain. Live Steam-overlay signature matching, full ImGui input behavior and gameplay integration require an in-game check; no game process was used during validation.
