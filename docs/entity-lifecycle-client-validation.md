# Entity lifecycle signatures: installed client inspection

Read-only inspection on 2026-10-15 of:
`C:/Program Files (x86)/Steam/steamapps/common/Counter-Strike Global Offensive/game/csgo/bin/win64/client.dll`.

SHA-256: `44af2aa0d2c64f90d273444cb393b8b679094f0b02136dbdb89d4ec82a560008`.
This differs from the older binary recorded in the camera/trace/subtick notes.

Both existing patterns in `core/constants/hooks_signatures.h` match exactly once
in executable sections, at function entry:

| Pattern | RVA | Observed last return |
| --- | --- | --- |
| `entity_added` | `0x9F6E40` | `0x9F6ED8` |
| `entity_removed` | `0x9F78C0` | `0x9F7961` |

Disassembly uses RCX as the entity system, RDX as the entity pointer, and R8D as
the 32-bit handle. Both mask the handle with `0x7FFF`, check the invalid handle,
and compare the index with `0x4000`. The add function increments system counters
at `+0x2120` / `+0x2124`; the remove function decrements them. Both iterate the
listener array at system `+0x2158` with its count at `+0x2150`. Add dispatches the
listener's first virtual function; remove dispatches the function at `+0x10`.
These observations support the identity of the matched lifecycle functions and
the current three argument types. Neither inspected tail establishes a stable
pointer return; return semantics were not changed as part of signature validation.

The PE runtime-function table splits these bodies into multiple unwind regions;
the end of the first region is not the end of the function.

No replacement signatures are warranted for this file. Static matching does not
prove every runtime event reaches the registry. The supplied profiler's zero
live entity lookups reflects the prior cache-only target scan, not evidence of
missing hook patterns. Keep live target enumeration independent of cache completeness.

Reproduce the signature check without loading or executing the DLL:

```powershell
python tests/inspect_entity_signatures.py "C:/Program Files (x86)/Steam/steamapps/common/Counter-Strike Global Offensive/game/csgo/bin/win64/client.dll"
```

Function bodies were inspected with Visual Studio `dumpbin /disasm:nobytes`
using image-base-adjusted ranges. The game binary was not modified.
