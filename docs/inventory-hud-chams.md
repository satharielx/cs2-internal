# Inventory, HUD refresh, and model inspection

Implemented against the researched **Steam build 26598758**, on 16 November 2026. The corrected DLL is `build/econ-fix/manager.dll`. The earlier `build/inventory-chams` artifact predates the fixes below.

## Local and native inventory

Choose a finish in **Skin Changer**, adjust wear/seed/name/StatTrak, and click **Add to inventory**. The **Inventory** tab stores multiple items, including several finishes for the same weapon. **Equip local loadout** copies the selected item to the existing weapon/knife/glove cosmetic path and enables the skin changer.

**Save inventory** and **Load inventory** use a separate `NEPHILIM_INVENTORY 1` file; the default is `%LOCALAPPDATA%/Nephilim/Inventory/collection.inv`. Files contain cosmetic definitions, not runtime pointers or native-session IDs. Loading validates the complete file before replacing the collection. Limits include 512 items, 1 MiB files, finite wear in 0..1, seeds in 0..1000, and names up to 128 bytes. Native items must be removed before a collection is replaced.

**Publish to native inventory** queues client-cache insertion. **Remove native and local item** queues removal. Engine operations execute in frame callbacks, not the renderer/UI thread, with at most one operation per callback and a reentrancy guard. These are session-only client-cache objects, not items granted by Steam or the game coordinator. Native equip/trade/ownership operations are not implemented.

The bridge requires the researched client PDB identity and architecture. It resolves unique factory/manager signatures, validates manager/inventory/cache/item RTTI, reads the inventory accessor's displacement, assigns IDs/positions above observed entries, creates an engine-owned econ item, inserts it into the native shared-object cache, and sends the native inventory creation event. Removal checks the original inventory/cache, econ-item class, item ID, and account before removing/notifying/destroying an owned item.

The UI distinguishes local, queued, published, and failed operations. Publication now requires a matching native inventory item view; cache insertion alone reports failure and retains the object for explicit removal. Native econ attributes are written before cache insertion: paint kit (6), seed (7), wear (8), optional StatTrak count/type (80/81), and optional custom name (111). Numeric values are read back before publication. Paint/seed/wear use float bits; StatTrak uses raw uint bits. The engine owns attribute allocations and string copies. Native cosmetic appearance, StatTrak presentation, and coordinator refresh behavior still require live validation of the corrected DLL.

ID allocation excludes reserved default/loadout markers (`0xF...`), includes owned cache objects without views, and masks inventory-position flag bits before choosing the next position. The earlier implementation incorrectly included default IDs and packed position flags. Engine faults now report the operation and exception address and keep native operations disabled until a game restart. This avoids silently retrying partially completed engine mutations.

Weapon finish rarity now comes from the installed item schema's generated metadata, rather than the downloaded catalog's Common default. Publication writes the native rarity bits at econ item +0x32 before cache insertion; other packed fields are preserved. Neo-Noir and Printstream resolve to Covert. The app catalog uses the same metadata with its zero-based enum. Existing native entries need recreation with the updated DLL; local saved items need no file migration. Details and reproduction are in the econ investigation below.

Shutdown requests cleanup while frame callbacks still exist and waits up to 1.5 seconds. Failed/uncertain ownership is retained rather than deleting an unverified object. Retained native objects have engine vtables/allocations, not pointers to DLL callback implementations.

## HUD names without dropping the weapon

Current `client.dll` inspection confirms:

| Routine | RVA | Evidence |
|---|---|---|
| ClearWeaponHUD | `0xEA5CA0` | Two arguments: selection base and entry index; removes a 0x48-byte entry and decrements count at +0x50 |
| Weapon HUD tick | `0xEA9800` | Timer at tick +0x44; cached active handle at tick +0x50; computes primary base with `LEA RBP,[RBX-0x98]` |
| Selection updater | `0xED1380` | Receives that primary base and passes it unchanged to ClearWeaponHUD |

The previous code subtracted **0x68**, following older notes. For this build that reads the wrong count. The refresh now uses **tick this -0x98**. Hook installation additionally checks the observed primary-base instruction family before enabling the layout-dependent path.

Refresh preserves selection entries and invalidates the cached handle/timer **before** forwarding the native tick, exactly once. The native changed-active-weapon path can update the name in the same callback. Empty selections also complete the invalidation. Requests are coalesced by revision and reapplied when the HUD object changes. `ClearWeaponHUD` removes entries and panels; the native updater calls it for invalid weapons. The earlier forced clear-all approach was inappropriate for name changes and could create duplicate HUD entries.

Custom-name writes and clearing are independent of material-builder readiness. Name-only edits do not regenerate unchanged materials. Name restoration requests a refresh even if material restoration needs a later retry. A manual **Refresh weapon icons and names** button is available in **Skin Changer → Debug**.

Raw current-build Ghidra evidence is [hud-current-ghidra.txt](research/cs2-26598758/hud-current-ghidra.txt). The older [knife/HUD notes](knife-hud-validation.md) are historical and must not supply the current base adjustment.

## Chams and 3D inspection

The chams material cache now registers a distinct resource name for each quantized RGB/depth key, instead of reusing the same two resource names for every color. Existing settling/debounce, bounded caching, serial validation, persistent holder overrides, ownership-conflict handling, and restoration remain in place. The draw callback retains its no-memory-API fast path.

In **Players ESP → Chams & glow**:

- Counters show tracked/applied objects, material builds, and holder writes per second.
- **Live chams scene bindings** shows handles, entity/scene-object addresses, original material counts, and applied/retired/conflicted state. The UI copies record metadata under its record lock; it does not modify engine state.
- **3D model inspector** provides orbit, pan, zoom, wireframe, auto-rotation, reset, and color-tinted geometry inspection.

The default geometry is now the actual Seal Team 6 variant E agent, exported from the installed game. Keep the supplied `models` folder beside the DLL. The standalone viewer supports orbit, pan, zoom, wireframe, auto-rotation and chams tint, with static geometry rather than game textures or animations. **Custom geometry** retains OBJ import and the diagnostic mannequin. OBJ loading supports polygon fan triangulation and negative/slash-form vertex indices; validates finite bounds and indices; rejects files above 2 MiB, 20,000 vertices or 20,000 triangles; and preserves the previous mesh on failure. The agent is reduced offline below 10,000 triangles; all preview faces render without runtime triangle skipping. See [agent inspector](agent-model-inspector.md) for installation, source paths, conversion and validation.

Current static chams inspection also passes: both scene descriptors' slot 4 resolve to `0x7D7C0`; holder +0xA8, material array +0x3E8, count +0x3F0 are present. Current material-system slots 14/29 are `0x37280` / `0x3AF00`. Runtime resolution uses RTTI/signatures rather than these RVAs.

## Native inventory research anchors

| Evidence | Current observation |
|---|---|
| Econ-item factory | `0x10D7800`, allocates 0x48 bytes and installs `CEconItem` vtables |
| Manager initializer | `0xAC050`, initializes the static `CCSInventoryManager` |
| Local inventory accessor | Manager slot 72, code reads this +0x528D0; runtime extracts the displacement |
| Inventory owner / SO cache | Owner record at +0x10; shared-object cache at +0x68 |
| Native item-view vector | Inventory +0x20, with its current count/data/capacity layout |
| Shared-object insertion / removal | Cache slots 2/3, `0x161F930` / `0x161FB90` |
| Inventory creation / removal notifications | Inventory slots 0/2, `0x83E200` / `0x83E280`; owner record pointer and event 4 |
| Econ fields written | ID +0x10, original ID +0x18, account +0x28, inventory position +0x2C, definition +0x30 |
| Schema accessor | `0x1189B30`, RIP load of initialized system; schema +0x150 definition count, +0x158 definition pointers |
| Attribute type | Definition +0x18; type vtable slot 6 converts from native MSVC string |
| Numeric SO attribute writer | `0x10DF990`, copies a 32-bit value through the engine's typed attribute storage |
| String SO attribute wrapper | `0x1154860`, creates and copies the engine string representation |

The read-only live probe corroborated the current RTTI/layouts, default markers, packed positions, and existing SO attributes. Four default-marker views were present. Owned items stored paint/seed/wear as floats and a StatTrak count of 90 as `0x0000005A`, not float 90. The initial factory objects had no attribute blob: decorating a view did not supply the underlying SO paint attributes. See [econ investigation](research/cs2-26598758/econ-investigation.md) for evidence and reproduction.

Retained evidence: [constructors/cache](research/cs2-26598758/inventory-native-ghidra.txt), [ownership/layout/destructor](research/cs2-26598758/inventory-layout-ghidra.txt), [cache methods/events](research/cs2-26598758/inventory-cache-methods.txt), and [unique-pattern/hash audit](research/cs2-26598758/inventory-native-validation.json). `tests/run_ghidra_functions.py` reproduces read-only function inspection in the saved project.

## Validation and limits

All **20 regression suites passed**. Owned-memory tests verify native call arguments/events, add failure cleanup, reserved ID exclusion, packed position masking, exact numeric attribute bits before cache insertion, missing-view failure, insertion/removal, reentrancy rejection, persistence, parser limits and preview rendering. Production callback tests verify selection preservation, empty selections, pre-tick invalidation and exactly-once forwarding. Name-only edits and clearing with the material builder unavailable are covered. Chams tests retain zero memory API calls across 20,000 draw callbacks and exercise material caching/restoration/concurrency.

The final native attribute changes and fault-reporting fixture were rerun in the focused inventory suite. Release builds and tests do **not** establish live CS2 inventory/skin/HUD/chams behavior. The live process probe reads metadata only; no remote function calls, writes, or injection were performed for validation. Restart CS2 before using the corrected build: the previous faulted session may contain partially registered cache objects and duplicated HUD state.
