# Sun and cloud geometry filtering

On 2026-10-25, the supplied BaseDrawArray signature matched the installed
`scenesystem.dll` exactly once, at RVA `0x7A540`. SHA-256:
`0f17ccdc083290a496e6ac13e4fe54413077ad2b83b7dfd281749cffd2733d9b`.
The existing sky-array pattern matched RVA `0x154FB0`.

Disassembly at BaseDrawArray's entry preserves R8 as the primitive array and
R9D as its count. The loop beginning at `0x7A5E5` reads the scene-object pointer
at entry +0x18 and advances by 0x70 per entry. The material pointer at +0x20 is
the same primitive field established by the GeneratePrimitives inspection.
The integration uses the supplied seven-argument DrawArray calling convention.
These observations are static validation; live appearance and FPS remain unverified.

`core/scene_visuals.h` adds an optional BaseDrawArray filter. It reads material
names through validated/executable vtable slot zero. Each render thread has a
bounded direct-mapped cache with a one-second expiry. Name matching uses
case-insensitive basename tokens, avoiding unrelated sun/cloud words in map
directories or names such as sunset_wall and cloudy_window.

Selected entries are omitted by forwarding contiguous surviving runs in their
original order. The filter never blackens materials, nulls engine renderables,
or mutates the engine array. No restoration writes are needed to disable this
geometry filter. A batch with no matches is forwarded once. Invalid counts,
unknown materials and unreadable names pass through. A batch requiring more
than 32 surviving runs is forwarded unchanged to bound additional submissions.
Material-cache misses perform guarded engine name lookup; hits do not repeat it.
When both geometry options are disabled, the original receives its arguments
directly without inspecting materials.

The existing sky draw hook separately handles Hide Sky Background by omitting
that sky pass. No worldrenderer scene-node/model pointers are cleared. Remove
Sun also retains the previous reversible dynamic-directional-light control.
Sun/cloud geometry can be filtered only when recognizable material names reach
this draw path. A sun or cloud painted into the sky texture cannot be separated
from that texture by this filter; hiding the background removes that whole pass.

All controls are under View & Camera and persist through the existing config
schema. Filter flags are published once per Present while settings are locked;
render threads do not acquire the configuration lock. Hook failure leaves the
other visual systems available. Shutdown clears all removal flags before hooks
are disabled and callbacks drained.

Validation:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_scene_visuals_regressions.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_feature_regressions.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_config_regressions.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_swapchain_hook_callbacks_smoke.ps1
```

The scene-filter fixture uses owned primitive arrays and mock material names;
it checks independent and combined removals, unchanged entries, preserved
arguments/order, cache reuse, all-hidden batches, invalid inputs and bounded
fallback. It does not execute the game's draw function.
