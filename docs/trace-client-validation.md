# Trace adapter: installed client inspection

Inspected the installed `game/csgo/bin/win64/client.dll` on 2026-10-06, read-only. SHA-256: `7081d87fd49961a5d82e45abd0f0b8f8f05deeb3cb849837c1759716221c6977`.

The old GetTraceInfo signature has no matches. The function at RVA `0x885A90` uses `sub rsp, 0x80` encoded as `48 81 EC 80 00 00 00`, rather than the old `48 83 EC` encoding. Its argument usage matches the adapter: RCX trace data, RDX output, XMM2 starting fraction, R9 surface record. It resolves the hit entity into output +8 and writes the fraction at output +0xAC.

Related layout corrections established from disassembly:

- InitTraceData (RVA `0x87F060`) initializes the secondary array at +0x1C20, with its data pointer at +0x1C28 and inline storage at +0x1C38. Surface storage begins at +0x18 and has 128 records of 0x38 bytes, not 0x30. The previous buffer was undersized.
- CreateTrace (RVA `0x883340`) writes start position at +0x1CF8. It does not define a Boolean return value. The adapter now declares it void and validates output instead of testing an undefined AL value.
- GetTraceInfo reads start/delta at +0x1CF8/+0x1D04, writes hit fraction at +0xAC and the solid-state flag at +0xBB. Compile-time assertions pin these offsets.
- The original CreateTrace pattern matched two locations. Its replacement includes the observed start-position write and uniquely matches the intended function.

All five adapter signatures were checked against this file and each matched exactly once. Feature regression tests pass with the corrected layouts/function type. These checks do not establish live visibility correctness; that still requires in-game verification. No game binary was modified or invoked during inspection.
