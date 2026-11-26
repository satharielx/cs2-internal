# Binary research index

- [CS2 Steam build 26598758 — 16 November 2026](cs2-26598758/overview.md): whole-installation PE/Ghidra inventory, architecture, module roles, function entry points, exports, RTTI, PDB identities and reproducible scripts.
- [Runtime redesign based on this research](../runtime-redesign.md): corrected ambiguous signatures, code-section/architecture validation, checked address resolution, and staged startup/cleanup. The installed-binary audit covers 25 current signatures and corroborates the frame callback with RTTI.

Use hashes and the recorded Steam build to match evidence to binaries. Keep observed exports/RTTI separate from inferred subsystem roles and unconfirmed interface strings.
