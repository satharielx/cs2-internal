# Copilot Instructions

## General Guidelines
- Use offset-based address resolution over pattern scanning for CS2 DLL injection.
- Use the pattern: `GetModuleHandleA("module.dll") + 0xOFFSET` instead of `sdk::find_pattern()` and `sdk::resolve_absolute_rip_address()`.

## Offsets
- Key offsets for CS2 (updated to latest values from `cs2-dumper`):
  - `client.dll`
    - `cs_io = 0x2317080` (dwCSGOInput)
    - `entity_list = 0x24ABF98` (dwEntityList)
    - `local_controller = 0x22F0FB8` (dwLocalPlayerController)
    - `view_matrix = 0x230CC90` (dwViewMatrix)
  - `rendersystemdx11.dll`
    - `swapchain = 0x7D50F0`