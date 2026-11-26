# Econ investigation after failed native publication

Date: 16 November 2026. Steam build 26598758, client PDB
`419B8BB878E248688A21181ABC98434938`, SHA256
`99a6720b29d8b477d55f0a2d7fd4333b5df13f354ba723804e996bead1231aa8`.

The user reported a new-item dialog with a named finish but a stock model,
missing inventory entries, duplicated weapon HUD icons, and the app's generic
"disabled after an engine fault" status. The old code did not retain the failing
operation/address, so the exact old exception site cannot be recovered from
that status. The findings below identify concrete implementation defects;
they do not establish that every reported symptom has been resolved live.

## Evidence

`tests/probe_econ_process.py PID --output build/econ-live-layout.json` uses
query/read process access only. It corroborated manager, inventory, shared cache,
type cache, econ item and item-view vtables. It does not execute game functions.
The layout is build-specific; verify client identity before reusing it.

The live view vector contained four reserved `0xF...` default/loadout markers.
The old allocator selected its maximum item ID from all views, then incremented
it, producing another reserved ID. Inventory position values included high flag
bits (e.g. `0xC0000007`); these were also incorrectly included in the maximum.
Allocation now excludes default markers, masks the lower 30 position bits, and
includes owned SO cache entries that do not have a view.

Existing owned CEconItems have an attribute blob at +0x20: a four-byte header,
count byte +2, and 16-byte entries with definition index +0 and value +8.
Paint kit (6), seed (7) and wear (8) are 32-bit floats. StatTrak count (80) and
type (81) are raw uint32 values. A live counter of 90 was stored as `0x5A`.
The factory starts with no attribute blob. The earlier view-only decoration
never populated the SO, so it could not establish native finish appearance.

Ghidra's CEconItem copier `0x10E9450` identifies schema accessor `0x1189B30`,
definition count +0x150, definition pointer array +0x158, attribute type pointer
definition +0x18, and type vtable slot 6 (set from native string). The live type
tables corroborated slot 6 as `0x11547A0` for indices 6/7/8/80/81 and
`0x1154860` for custom name 111. The numeric wrapper calls `0x10DF990`, which
stores the supplied 32 bits through native attribute allocation and callbacks.
The corrected bridge calls that writer with exact bits and reads them back.
Custom names use the native string wrapper, preserving engine ownership.

`SOCreated` reaches `0x10EDE00`, which initializes an item view via `0x111AE10`
and appends it only after initialization succeeds. Cache addition is therefore
insufficient evidence of successful publication. A missing matching view now
reports failure and retains the owned object for removal.

HUD updater `0xED1380` invokes `ClearWeaponHUD` (`0xEA5CA0`) for invalid weapon
entries. Clearing all entries on a name revision interferes with native panel
ownership. Refresh now invalidates the active-weapon handle and timer, preserves
the entry array/count, and forwards the original tick once.

## Retained decompilation

- [Schema and econ copier](econ-schema-copy-ghidra.txt)
- [Attribute type wrappers](econ-attribute-setters-ghidra.txt)
- [Native numeric/string storage](econ-raw-attributes-ghidra.txt)
- [Original constructors and cache](inventory-native-ghidra.txt)
- [Inventory layouts and events](inventory-layout-ghidra.txt)

Reproduce selected functions using `tests/run_ghidra_functions.py` against the
saved read-only project. Production changes are in `manager/core/inventory.cpp`
and `manager/core/skin_hud.h`; user behavior and validation limits are recorded
in `docs/inventory-hud-chams.md`.

The corrected DLL has not been exercised in live CS2. Tests use owned fixtures
and cannot prove engine compatibility or coordinator persistence. A game restart
is needed to discard the previous session's faulted/partially registered state.

## Weapon finish rarity follow-up

The user subsequently confirmed native weapon finishes appeared, but their
inventory rarity strips were white; knives displayed rarity. Factory-created
weapon SOs retained rarity zero. The remote catalog also assigned every entry
`RARITY_COMMON`, so using that catalog as the native source would remain wrong.

Ghidra `0x1130E80` reads `(uint16_t(secondary_this+0x2A)>>11)&15`, with mask
`0x7800`. The secondary econ interface is primary +8, so the packed rarity
field is primary +0x32, bits 11..14. Its fallback explains why knives with base
rarity 6 can look correct while ordinary weapons with override zero look white.
The corrected write preserves all other bits, including quality/origin/level.
See [rarity getter evidence](econ-rarity-ghidra.txt).

`tests/generate_skin_rarity.py` extracts `scripts/items/items_game.txt` from
the installed `pak01_dir.vpk`, merges repeated KV sections, resolves item
prefabs, and generates `manager/core/skins/rarity_baseline.inc`. It records 1,479
paint-kit rarities plus item-definition rarities. Finish rarity combines the
base item and paint-kit values as `base + paint - 1`, capped at Covert (6),
or Contraband (7) for a Contraband paint kit. Regular weapons have base 2;
AWP Neo-Noir (803) and Desert Eagle Printstream (962) have paint rarity 5,
producing Covert (6). Redline is Classified (5), Blaze Restricted (4), and
Howl Contraband (7). The UI uses the result minus one for its zero-based enum.

Publication applies rarity before cache insertion. Unknown finish metadata
fails that operation rather than publishing an item with a false white rarity.
Unpainted items retain the existing factory/fallback behavior. Local saved
collections need no format migration: rarity derives from weapon and paint IDs.
Existing native session entries must be recreated with the corrected DLL.

The focused inventory suite verifies both reported finishes, intermediate and
Contraband grades, knife preservation, unknown IDs, sorted metadata, and packed
field preservation at the actual cache-add boundary. Release build and focused
tests pass; the corrected rarity display still needs live confirmation.
