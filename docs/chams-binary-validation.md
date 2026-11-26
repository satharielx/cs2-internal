# Chams function discovery, 2026-10-25

Read-only inspection of the installed CS2 binaries under
`C:/Program Files (x86)/Steam/steamapps/common/Counter-Strike Global Offensive/game`.
The binaries were not loaded, executed, patched, or injected during this inspection.

| Module | SHA-256 |
| --- | --- |
| client.dll | `ccaf4e4926babc052994502b19ec61198a11008a39dce712921b93dd993be9f5` |
| scenesystem.dll | `0f17ccdc083290a496e6ac13e4fe54413077ad2b83b7dfd281749cffd2733d9b` |
| materialsystem2.dll | `2fe9750177006c24aacd0f86b14efbc00d04ca61d86f27f1b07b263eefb06d69` |
| tier0.dll | `f96f4f9666be66ee3dd42a015b305d954b5dd5582b0608a74e50548df426584d` |

All addresses below are RVAs, not process addresses or proposed hardcoded hooks.

## GeneratePrimitives

The descriptor RTTI is in **scenesystem.dll**, not client.dll. Following the
MSVC type descriptor through its complete-object locator to the primary vtable gives:

| RTTI name | Vtable RVA | Slot 4 target RVA |
| --- | --- | --- |
| `.?AVCAnimatableSceneObjectDesc@@` | `0x5E5C90` | `0x7D7C0` |
| `.?AVCBaseSceneObjectDesc@@` | `0x5E5D70` | `0x7D7C0` |

Both tables point to the same implementation, confirming this part of the supplied
write-up for this binary. A runtime resolver should validate the descriptor RTTI
and read its vtable slot, rather than using the recorded RVA.

Entry disassembly establishes these register roles:

- RCX: descriptor/first argument, saved to entry stack +8.
- RDX: scene object, saved in R14; its +0xA8 field is accessed immediately.
- R8: rendering context, saved in RSI and used through its +0x10 field.
- R9: output buffer, saved in R13; its +0xC count and +8 capacity are accessed later.

These are observed argument roles, not a fully validated C++ prototype or return type.

The entry at `0x7D812` loads `[RDX + 0xA8]`. The following instructions load
the holder's array pointer at +0x3E8, count at +0x3F0, and element zero.
The primitive path accesses the output buffer count at +0xC, its array at +0,
and writes material data at primitive +0x20.

One discrepancy with the supplied write-up: at RVA `0x7E903`, the secondary
primitive's +0x20 is assigned R12; +0x28 is initially assigned R12 and can then
be replaced by the result of a virtual call on that material (slot +0x30).
Consequently, the claim that +0x28 is simply a stable hash is not established
for this build. It should not be treated as another writable material pointer.

The generated schema independently has `CSkeletonInstance::m_modelState = 0x140`
(decimal 320), matching the start of the supplied forward chain. The linkage
at model state +0xD8 and its scene-object +0x20 still need live identity validation.

## Material pipeline

`client.dll` references `materials/dev/glowproperty.vmat` at string RVA
`0x1C675B0`, from initialization code at `0xD19BA`. This establishes the resource
name's presence, not the claimed shader-compilation performance benefit.

Following `.?AVCMaterialSystem2@@` RTTI in **materialsystem2.dll** gives primary
vtable RVA `0x12D920`:

| Slot | Function RVA | Observed behavior |
| --- | --- | --- |
| 14 | `0x37200` | Resource lookup; writes a handle through the second argument, returns that output pointer. Consistent with the existing skybox lookup wrapper. |
| 29 | `0x3ADE0` | Material-creation candidate: allocates a 0x7E0 object, initializes it using the name/KV-style arguments, registers a `vmat` resource, writes the output handle and returns the output pointer. |

Slot 29 preserves entry RDX as the output location, R8 as a name passed into
the resource-name builder, and R9 as the initialization input. It also consumes
a 32-bit fifth argument and a byte-sized sixth argument. Their precise semantics,
the KV object layout and ownership/release rules remain unverified. Do not call it
with a guessed four-argument prototype.

The text KV3 loader is exported by **tier0.dll**, at RVA `0x124950`:

```text
?LoadKV3@@YA_NPEAVKeyValues3@@PEAVCUtlString@@PEBDAEBUKV3ID_t@@2I@Z
```

Its decorated signature describes a bool return and arguments corresponding to
`KeyValues3*`, `CUtlString*`, text, `const KV3ID_t&`, another string, and unsigned
flags. Export resolution avoids a signature scan for this loader. client.dll
imports this overload at IAT RVA `0x1AAFA48`; observed indirect call sites include
RVAs `0x226FF1`, `0x2C173D`, and `0x9CA213`.

## Reproduction and remaining validation

```powershell
python tests/inspect_chams_binary.py "C:/Program Files (x86)/Steam/steamapps/common/Counter-Strike Global Offensive/game/csgo/bin/win64/client.dll"
```

This standard-library-only inspector resolves the descriptor and material-system
vtables from RTTI, checks that both descriptor slot-4 targets agree, verifies the
override field loads, reports the relevant material slots, and locates the text
loader export. It fails when the expected static layout is absent.

Function bodies were inspected with Visual Studio `dumpbin /disasm:nobytes`.
Static discovery does not prove material creation/destruction safety, live pawn
membership, render-thread synchronization, cached-primitive invalidation, or
unload restoration. The inspection itself did not execute game functions.

## Project integration

`core/chams.cpp` now uses this discovery in an optional hook. Initialization
resolves both descriptor primary vtables from RTTI, verifies matching slot-4
targets and the entry's holder/array/count loads, validates the material-system
vtable, and checks the material-creation implementation fingerprint. Missing
helpers leave chams unavailable without preventing the other hooks from starting.

The existing client KV arena helpers were also inspected: constructor RVA
`0x19B6AC0`, root getter `0x19C4B40` (root at arena +0x78), and cleanup thunk
`0x19B7110`, called from RVA `0x2270EE`. Cleanup reaches a destructor that releases
arena allocations; its optional delete size is 0x1190. The integration uses an
aligned 0x1400-byte arena and resolves these helpers by the patterns in
`constants/chams_signatures.h`, not their RVAs. The offline inspector checks
those patterns and verifies the material fingerprint equals vtable slot 29.

Material creation uses slot 29 with output binding, resource name, parsed KV root,
32-bit flags zero and a final bool true. These arguments and resource ownership
still require live validation. The KV arena is cleaned after creation, including
failed loads. Registered material bindings are treated as engine-owned; the module
does not guess a release-function ABI. Materials are cached by RGB/depth mode,
color edits settle for 250 ms before creation, and at most 64 definitions are
registered per module lifetime. Reusing an existing color reuses its cached binding.
Opaque and depth-disabled definitions use the supplied shader and resource names.
Through-wall mode selects the depth-disabled material; it is not a two-color,
two-pass implementation.

Frame-stage updates enumerate serial-validated living, non-local pawns, respect
the ESP team filter, and verify each forward-linked scene object's exact vtable.
They install persistent holder overrides, independent of ImGui ESP enablement.
Membership uses atomically published owned snapshots, avoiding a two-buffer reuse
race. A shared draw guard allows concurrent original GeneratePrimitives calls;
frame-stage field updates and ordinary removals take its exclusive side. Nested
draw callbacks reuse their thread's read guard. Reentrant removals retain the
owned backing arrays until readers finish. There is one active override element,
backed by 256 slots to cover accepted original counts during lifecycle transitions.

Original array/count values are preserved, including existing engine arrays.
The removal callback restores before the engine frees a pawn. Disable, disconnect,
death/filter changes and shutdown also retire overrides. If another subsystem has
replaced our array, restoration leaves that replacement alone. Unwritable holders
are skipped. A failed final restoration retains its small process-heap backing
allocation rather than leaving a pointer into freed storage or an unloaded DLL.
Shutdown restores while draw hooks can still pin snapshots, then the existing hook
manager disables hooks and drains callbacks before removing trampolines.

Settings are in Players ESP: Enable chams, Chams through walls, and Chams color.
Status, tracked/applied counts and callback matches are available there. The
preview mannequin reflects the chosen chams color. Configuration save/load/reset
uses the existing schema registry.

Validation:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_chams_regressions.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_feature_regressions.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_config_regressions.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_swapchain_hook_callbacks_smoke.ps1
```

The chams fixture uses owned buffers and mock material functions; it does not
execute the engine material factory. It checks material definitions/cleanup,
cached creation, persistent overrides, original-array restoration, another
renderer's replacement, read-only holders, team/death/serial filtering, removal,
disconnect, concurrent snapshot lifetime and a draw overlapping restoration.
Live model appearance, cached-primitive response and FPS are not verified.

## Performance follow-up

The initial implementation performed a descriptor memory read, atomic shared
snapshot acquisition and a linear membership scan for every GeneratePrimitives
call, including unrelated scene objects. It also held the exclusive draw barrier
while enumerating entities. These were unnecessary costs for a persistent override.

The draw callback now only holds the existing read-side lifetime guard, batches
its diagnostic counter in groups of 64, and forwards the original arguments and
return value. It performs no process-memory queries/reads/writes or membership
scans. Frame-stage enumeration runs outside the draw barrier. An unchanged set
of installed overrides keeps the same published snapshot and does no writes or
exclusive draw locking; actual override changes still use the exclusive guard.
Published/material record fields are atomic for the outside-barrier validation.

The owned-buffer regression fixture measured 20,000 callbacks at approximately
0.23 ms and asserts zero memory API calls. It also checks that repeated unchanged
frames perform no writes and preserve the existing snapshot. This measures CPU
wrapper overhead with a mocked original, not GPU cost or live game FPS.

The subsequent write audit removed 256 interlocked exchanges from each backing
array update. Ordinary stores run only when its material pointer changes, under
the existing exclusive draw guard. A count-only repair now writes only that count;
it does not refill the backing array or rewrite its base. Unchanged frames remain
write-free. If the engine or another renderer replaces an installed array, that
record is marked conflicted and no longer causes an unsuccessful exclusive retry
every frame. Toggling chams retires it and permits a new installation.

Players ESP diagnostics now show successful holder writes per second and total,
logical backing-array material updates, conflicts, and material creations. The
write counter includes installation/restoration and should settle at zero with
unchanged live objects/settings. A large FPS drop with zero writes needs further
material/GPU investigation; this counter does not measure GPU work. Regression
coverage includes count-only repair, unchanged write/update counters and stable
publication after an external replacement.
