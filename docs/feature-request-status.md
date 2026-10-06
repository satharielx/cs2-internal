# Requested feature status

This change adds runtime behavior and controls, but does **not** implement the entire requested list. Tests use owned memory and ImGui frames. Live CS2 rendering and server behavior have not been validated.

## Aimbot and weapon

| Request | Status |
| --- | --- |
| FOV, smooth | Existing normal/silent paths retained. |
| Hitboxes | Select head, neck, chest, or pelvis bone centers. |
| Hitscan | Choose the selected bone nearest the input angle. No collision, penetration, or damage tracing. |
| Shot delay | Normal aim only. Silent aim selects immediately. |
| Kill delay | Normal aim only. Silent aim does not pause after kills. |
| Lock target | Normal aim retains a serial-validated target. Silent aim reselects the closest eligible candidate within FOV each evaluation. |
| Draw FOV | Independent camera-FOV-based overlay, including when player ESP is disabled. |
| Disable when | Flashed, airborne, scoped, menu/console open, or existing pause key held. |
| FOV changer | Temporary render-stage camera FOV override, preserving scoped weapon FOV. |
| Force shoot | Crosshair target must match the selected, ready aim target. Manual trigger retains its separate delay. |
| Autoscope | Scope supported sniper definitions; release synthetic input after 10 ms or on cleanup. |
| Bunnyhop | Existing implementation retained. |
| PSilent | Existing serializer/history silent aim retained; no additional packet-specific PSilent implementation. |
| Lock mouse | Pending verified game mouse-input integration. |
| Head/body point scale | Pending actual hitbox geometry and collision tracing. |
| Minimal damage, hitchance | Pending weapon ballistics, spread/inaccuracy evaluation, and collision/penetration traces. |
| Autostop between shots, smooth auto strafer, accurate walk | Pending verified retrieval/serialization of current user commands. Existing CreateMove receives an input object, not a user-command pointer. |

Bone settings invalidate target caches immediately. The default head path retains geometry reuse across subtick callbacks. Silent writes revalidate target identity and health. Delay state resets when the local pawn or entity system changes.

## Visuals

| Request | Status |
| --- | --- |
| Preview | Schematic menu preview reflecting boxes, bars, labels, skeleton, distance, and snaplines. |
| Box, health bar, nickname, distance, skeleton | Existing renderer retained. |
| Armor bar | Armor percentage beneath player boxes. |
| Weapon text | Active weapon name from the existing definition table. |
| Bomb | Player C4 carrier flag, including carried weapons; separate planted-bomb label. |
| Defuse kit, flashed, scoped, planting, defusing, hostage | Schema-based player status labels. |
| Glow | Configurable player glow fields with serial-validated restoration. Requires live renderer validation. |
| Out of FOV arrows | Direction arrows outside the viewport, including behind the camera. |
| Items on ground | Ownerless weapon and defuse-kit labels. |
| Hit/kill visuals | Animated center-screen rings driven by confirmed hit/kill counters. |
| Night mode | Dim existing hooked light/sky colors; restore after each engine callback. |
| Smoke color | Smoke projectile RGB fields with restoration. Whether existing smoke volumes refresh needs live validation. |
| Grenade warning | Active projectile labels and distance markers; no damage or detonation prediction. |
| Radar | Existing in-game spotted radar retained. |
| External | Separate radar panel inside the existing DX11 overlay; no external executable/window. |
| Scale, alpha | ESP label/arrow/bar size and opacity; independent radar scale/alpha. Bounds remain tied to projected geometry. |
| Chams | Pending verified material construction and draw-object override integration. |
| Grenade trajectory | Pending collision traces, bounce response, and grenade-specific detonation rules. |

World discovery runs at most every 100 ms. Cached entities are serial-validated before use. Glow/smoke restoration preserves newer engine values and recycled entities. Cleanup also runs on input release and hook shutdown.

## Miscellaneous

| Request | Status |
| --- | --- |
| View Model Editor | FOV plus X/Y/Z offsets, temporarily applied during rendering and restored on scope exit. |
| Show player money | Money-service label beside player ESP. |
| Spectator list | Dedicated observer-pawn resolution; list viewers of the local or observed pawn. |
| World modulation | Existing light/sky controls plus night dimming. Independent world/cloud/sun controls remain pending. |
| Taser/knife range | Projected reference circles with the corresponding weapon equipped; no obstacle/reach tracing. |
| Hit marker/hit effect | Confirmed hits drive crosshair marks and expanding rings; confirmed kills use a separate color. |
| Removals | Existing no-flash and visual aim-punch removal retained; other removal types remain pending. |
| Inventory Changer | Existing weapon/knife/glove skin changer retained; actual inventory-item creation remains pending. |
| Viewmodel/hand/glove/sleeve/weapon chams | Pending material and model-part identification/override hooks. |
| Auto Accept | Pending a verified lobby-ready event and accept callback. |
| Spread circle | Pending verified runtime weapon spread/inaccuracy evaluation. |
| Aspect ratio, third person | Pending a verified camera/view-setup hook. |

## Validation

Run with the installed Visual Studio x64 toolchain:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_feature_regressions.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_viewmodel_regressions.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_skybox_regressions.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_swapchain_hook_callbacks_smoke.ps1
```

Coverage includes bone selection, cache invalidation, conditions, shot/kill timing, stale handles, glow/smoke restoration, preservation of engine updates, autoscope input release, scoped force-shoot gating, FOV with ESP disabled, hit/kill overlays, viewmodel/camera restoration, scoped FOV preservation, night sky restoration, and existing hook/feature regressions.

The Release x64 DLL is at `build/feature-expansion-direct/manager.dll`. MSBuild's file tracker failed with access denied here; the full project C/C++ source list was compiled and linked directly with MSVC. The existing `build/manager.dll` was left untouched.

Silent aim uses live target state and hook-maintained entity handles. Wall Check uses the supplied trace signatures/layouts and mask 0x1C300B; normal aim retains Spotted Check. Trace failure rejects targets, and collision-blocked candidates are skipped within the same evaluation. Mock-engine regression tests pass; the engine ABI and live visibility results still need in-game verification. ESP diagnostic status/counters remain in the Players menu to identify the reported live rendering failure.
