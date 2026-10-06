# Subtick serializer inspection

Read-only inspection of the same installed client documented in `trace-client-validation.md` (SHA-256 `7081d87fd49961a5d82e45abd0f0b8f8f05deeb3cb849837c1759716221c6977`).

The existing SubTickAngle signature matches exactly once, at RVA `0xCFA130`. The PE runtime-function table gives its end as `0xCFA713`. No signature change was needed.

Observed ABI:

- RCX: source record; RDX: destination history message; R8B: mode.
- XMM3: fourth argument (float). Entry RSP+0x28: fifth argument (float). Entry RSP+0x30: sixth argument (optional pawn pointer).
- The function has no defined return value. The call site at RVA `0xD020B4` sets those arguments and does not consume a return. The hook and trampoline types now return void, replacing the inferred int64 return.

Observed serialization:

- Source +0/+4 are copied to history +0x60/+0x64, with presence bits 0x200/0x400.
- Source +8/+0xC are conditionally copied to history +0x68/+0x6C, with bits 0x800/0x1000.
- Source angle floats +0x10/+0x14/+0x18 are copied into the message referenced by history +0x18, at message +0x18/+0x1C/+0x20. History presence bit 1 and angle-message bits 1/2/4 are set by the engine.
- The serializer also reads optional source fields through +0x50. The hook continues forwarding the original full source record; it does not replace it with a truncated angle-only buffer.

The existing angle offsets and history verification match this build. The hook still changes only the three angle floats temporarily and restores them after the original serializer. Engine tick counts, fractions, interpolation and presence-bit generation remain engine-owned; a 64-tick server does not justify rewriting these fields.

Callback tests cover all six arguments, exactly-once forwarding, untouched tick words, restored source angles, serialized angles/presence flags, disabled forwarding and lock contention. Live-game/server behavior is not established by static inspection or synthetic tests.
