Installed client inspected read-only on 2026-10-07, SHA-256 `7081d87fd49961a5d82e45abd0f0b8f8f05deeb3cb849837c1759716221c6977`.

The existing call-site signature uniquely matches RVA `0xED2B07`, calling `0xEA82F0`. ClearWeaponHUD takes RCX selection storage and EDX entry index, removes a 0x48-byte entry, decrements count +0x50, and returns index minus one in EAX. It is `int(void*, int)`, not the project's old three-argument void declaration.

CCSGO_HudWeaponSelection RTTI identifies its secondary vtable at RVA `0x1CE05C8` (secondary base offset 0x98). Slot 6 is its native tick at `0xEABB40`. At `0xEABC89` this tick passes `this - 0x68` to the selection update function at `0xED29F0`, whose native cleanup calls the clear routine with that same selection pointer. Selection count is +0x50 and entries are +0x58. The tick's own +0x50 is a separate cached active-weapon handle; +0x44 is its next-update time.

Both new direct-entry signatures match exactly once. Cosmetic changes increment an atomic revision. The hook first runs the native HUD tick, then clears populated entries for that revision and invalidates its active handle and timer for the next native update. An empty list leaves the request pending instead of acknowledging it without calling ClearWeaponHUD. The Skin Debug tab shows hook ticks, refresh requests, clear calls, and refresh status. No renderer-thread HUD pointers or guessed FindHudElement offsets are used. Count bounds, progress checks, and guarded engine calls prevent unbounded cleanup.

Owned-memory cleanup tests and production callback smoke tests pass. Live icon/label appearance still needs an in-game check.
