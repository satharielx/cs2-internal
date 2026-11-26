# Saved research context

The user requested persistent CS2 binary research. Start with
`docs/research/README.md` and `docs/research/cs2-26598758/overview.md` for the
16 November 2026 snapshot of Steam build 26598758. Complete per-module evidence,
hashes, symbols and Ghidra scripts are alongside that overview. The local Ghidra
project is `build/cs2-research/projects/CS2_26598758_Inventory.gpr`.

Treat recorded RVAs and prior validation notes as build-specific evidence.
Interface-like strings are candidates, not confirmed runtime registrations.
The overview uses PE metadata and selected-export decompilation; it does not
claim exhaustive analysis of every internal function.

Runtime changes based on that research are documented in
`docs/runtime-redesign.md`. Startup/hook/interface/resolver discovery now uses
unique executable-section matches. The current 25-pattern binary audit and
frame-callback RTTI corroboration are saved in
`docs/research/cs2-26598758/runtime-signature-validation.json` and can be repeated
with `tests/inspect_runtime_signatures.py` after a game update.

The app's Runtime Inspector is documented in `docs/runtime-inspector.md`.
Its generated 103-entry catalog compares architecture and embedded PDB identity,
not live feature compatibility. It exposes read-only module/export browsing and
explicit clipboard reports. Regenerate the catalog from the saved inventory
with `docs/research/cs2-26598758/generate_runtime_baseline.py` when adopting a
deliberately researched new baseline.

Current inventory/HUD/chams changes and native ABI evidence are documented in
`docs/inventory-hud-chams.md`. For build 26598758, the weapon HUD selection base
is tick this **-0x98**, not the historical -0x68. Native inventory publication is
experimental session-only client-cache integration, gated by the research PDB
identity. The 3D inspector is standalone OBJ/diagnostic geometry, not a native
VMDL renderer. Preserve those confidence limits in future changes.

Native inventory fault follow-up is saved in
`docs/research/cs2-26598758/econ-investigation.md`. Default item IDs (`0xF...`)
and packed inventory-position flags must not enter native ID allocation.
Cosmetics require SO attributes before cache insertion, with float finish bits
and raw integer StatTrak bits. Publication requires a matching native item view.
Name refresh must preserve HUD selection entries; do not clear all entries.
The corrected artifact is `build/econ-fix/manager.dll`; live corrected behavior
is still unverified. Do not reset the engine-fault latch to retry partial writes.

Weapon rarity follow-up: native rarity is bits 11..14 of econ item +0x32,
not the app's zero-based SkinRarity enum. `skins/rarity_baseline.inc` derives
from installed items_game.txt via `tests/generate_skin_rarity.py`; preserve
prefab resolution and repeated KV sections. Use combined definition/finish
rarity for publication and UI. Evidence is appended to econ-investigation.md.

The 3D inspector now defaults to locally exported Seal Team 6 variant E,
not the diagnostic mannequin. See `docs/agent-model-inspector.md` and
`tests/prepare_agent_model.py`. Real assets use `agents/models/ctm_st6/`;
the similarly named `characters/models/` assets are dummies. Keep the output
`models` folder beside the DLL. It remains a static geometry/tint viewer,
without game textures, animations or native VMDL rendering. All retained
preview faces render; do not reintroduce per-frame triangle skipping.
