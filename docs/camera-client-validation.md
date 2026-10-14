Inspected the installed `game/csgo/bin/win64/client.dll` read-only on 2026-10-07. SHA-256: `7081d87fd49961a5d82e45abd0f0b8f8f05deeb3cb849837c1759716221c6977`.

The internet reference OverrideView and ApplyViewPunch signatures have zero matches. The input angle accessor signature matches once at RVA `0xB70D40`; RCX is input, EDX is the slot, and RAX returns an angle pointer. Slot zero returns input +0x688 when the buffered entry count at +0xB50 is zero; otherwise it selects the last +0x440-sized entry through +0xB58 and returns entry +0x430.

MSVC RTTI for ClientModeCSNormal resolves the primary vtable at RVA `0x1C9EE00`. Slot 15 points to RVA `0xB9C680`. This function takes the instance in RCX and view setup in RDX. At `0xB9C7FB` it passes setup +0x4B8 as the camera CalcView angle output (R8), with setup +0x4A0 as position output (RDX), to camera-services slot 33. At `0xB9C859` it writes an integer output to setup +0x4F8: the reference's angle offset must not be used. The replacement entry signature uniquely matches `0xB9C680`.

The input singleton reference signature uniquely matches RVA `0xBF3969`; its RIP-relative load resolves RVA `0x223A320` and immediately calls the verified angle accessor. Runtime resolves this reference instead of using a potentially mismatched generated global offset.

The hook forwards the original first, then copies input angles into only setup +0x4B8 when enabled and the local pawn is alive. It does not write input angles, pawn recoil state, or aim-punch services. The standalone RCS feature remains independently controlled. Owned-buffer tests check the angle destination, unchanged adjacent fields/input, and rejection of invalid angles. This validation does not establish live gameplay appearance or third-person compatibility.

Reference compared: https://github.com/thebloomstart/cs2-internal/blob/main/cstrike/core/hooks.h and its SDK viewsetup/input definitions. Current disassembly supersedes the reference's five-argument prototype and 0x4F8 layout.
