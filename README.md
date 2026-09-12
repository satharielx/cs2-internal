# ⚔️ NEPHILIMGATE

### Advanced Internal Counter-Strike 2 Framework

**A modern C++20 internal project built around Source 2 entity handling, signature resolution, DirectX 11 rendering, gameplay systems, skin customization, and runtime diagnostics.**

![Version](https://img.shields.io/badge/version-0.98-7c3aed?style=for-the-badge)
![C++](https://img.shields.io/badge/C++-20-00599C?style=for-the-badge\&logo=cplusplus)
![Platform](https://img.shields.io/badge/platform-Windows-0078D6?style=for-the-badge\&logo=windows)
![Renderer](https://img.shields.io/badge/renderer-DirectX%2011-107C10?style=for-the-badge)
![UI](https://img.shields.io/badge/UI-ImGui-00ADD8?style=for-the-badge)
![License](https://img.shields.io/badge/license-MIT-green?style=for-the-badge)

> **Latest Release:** `v0.98` — September 11, 2026

---

## ✦ About

**NEPHILIMGATE** is an internal Counter-Strike 2 project focused on building a modular and maintainable framework around the constantly changing Source 2 environment.

The project combines **signature pattern resolution**, generated Source 2 schemas, centralized game-state tracking, checked entity access, DirectX 11 rendering, and dedicated runtime diagnostics.

### Core Systems

* 🎯 Aim assistance
* 👁️ Player ESP
* 🎯 Recoil Control System
* ⚡ Triggerbot
* 🏃 Bunny Hop
* 🗺️ Radar ESP
* ✨ NoFlash
* 🎨 Weapon Skin Changer
* 🔪 Knife Changer
* 🧤 Glove Changer
* 🧬 Runtime Signature Resolution
* 🖥️ Responsive ImGui Interface
* 🔬 Runtime Diagnostics
* 🧪 Regression & Hook Tests

> **Development Status:** Active development. Counter-Strike 2 updates frequently change signatures, offsets, schemas, engine structures, and runtime behavior.

---

# 🚀 Version 0.98

### `11/09/2026`

Version **0.98** is one of the largest compatibility, stability, and architecture updates made to NEPHILIMGATE so far.

The update focuses on restoring functionality affected by recent Counter-Strike 2 changes, correcting entity resolution, improving hook behavior, repairing visual features, and renovating the user interface.

## ⭐ Major Changes

| System                          | Status | v0.98                                    |
| ------------------------------- | :----: | ---------------------------------------- |
| 🎨 Skin Changer                 |    ✅   | Working correctly                        |
| 🎯 Silent Aimbot                |   🧪   | Working — optimization still in progress |
| 🖥️ Interface                   |    ✅   | Redesigned and renovated                 |
| 🛡️ VAC Live compatibility work |   ⚠️   | Added                                    |
| 👁️ ESP                         |    ✅   | Entity handling repaired and expanded    |
| 🔪 Knife Changer                |    ✅   | Model / arms handling fixed              |
| 🗺️ Radar ESP                   |    ✅   | Entity resolution corrected              |
| ✨ NoFlash                       |    ✅   | Incorrect field access fixed             |

> **Anti-cheat Notice:** No build can be guaranteed permanently undetected or safe against VAC / VAC Live.

---

# 🔥 v0.98 Hotfixes

## 🎯 Silent Aimbot

* Fixed Silent Aimbot calling the wrong hooked function.
* Corrected subtick callback handling.
* Fixed target-angle processing.
* Improved entity validation.
* Improved player death / respawn handling.
* Added additional runtime debugging information.
* Corrected floating-point angle handling.
* Improved target acquisition reliability.
* Silent Aimbot is currently functional.
* Further caching and performance optimization is still planned.

---

## 🧬 Entity Resolution

Major changes were made to how critical game objects are resolved.

Fixed:

* **Entity List**
* **Local Player Pawn**
* **Local Player Controller**

Important runtime addresses now make greater use of **signature pattern resolution** instead of depending entirely on static offsets.

This improves reliability after updates and helps prevent stale entity state across:

* Player deaths
* Respawns
* Map changes
* Disconnects
* Reconnects
* Controller changes
* Pawn replacement

---

## 📦 Offset Update

Updated outdated offsets using the recent Counter-Strike 2 build:

```text
10/09/2026
03:56:02 AM
UTC+2
```

Generated Source 2 schema information is now used throughout more of the project to reduce duplicated or stale offset definitions.

---

# 👁️ ESP

ESP failures caused by **Entity List invalidation** were repaired.

## Player ESP Features

* ✅ Box ESP
* ✅ Skeleton ESP
* ✅ Health Bar
* ✅ Player Name
* ✅ Player Distance
* ✅ Snaplines
* ✅ Viewpoint / direction information
* ✅ Team filtering
* ✅ Maximum-distance filtering
* ✅ Configurable colors

## Validation

The renderer performs validation for:

* Entity pointers
* Player health
* Bone positions
* Team information
* Maximum distance
* World-to-screen projection

The skeleton renderer has also been updated around newer Counter-Strike 2 entity and bone handling.

---

# 🎯 Aim System

NEPHILIMGATE contains separate normal and silent aiming paths.

## Aim Features

| Feature          | Description                            |
| ---------------- | -------------------------------------- |
| Normal Aim       | Traditional target-angle adjustment    |
| Silent Aim       | Subtick-based target-angle handling    |
| FOV              | Configurable target acquisition radius |
| Smoothing        | Adjustable aim interpolation           |
| Team Check       | Ignores teammates                      |
| Spotted Check    | Uses CS2 spotted-state information     |
| Maximum Distance | Limits target selection                |
| Auto Shoot       | Fires after target acquisition         |
| Pause Key        | Temporarily disables aim processing    |

## Aim Diagnostics

The runtime debugging system can expose:

```text
Resolved Controllers
Resolved Pawns
Living Entities
Enemy Entities
Spotted Entities
Valid Bones
Targets In Range
Targets In FOV
Selected Pawn
Selected Player Name
Input Angles
Target Angles
Output Angles
Subtick Callback Count
Angle Write Count
```

---

# 🎯 Recoil Control System

The project includes a standalone **RCS — Recoil Control System**.

Features:

* ✅ Enable / Disable
* ✅ Adjustable recoil strength
* ✅ Aim-punch compensation
* ✅ Normal aim integration
* ✅ Silent aim integration
* ✅ Shooting-state handling

The current implementation follows the newer aim-punch services structure instead of relying on older direct recoil fields.

---

# ⚡ Triggerbot

Triggerbot functionality includes:

* ✅ Enable / Disable
* ✅ Activation key
* ✅ Configurable firing delay
* ✅ Enemy validation
* ✅ Target-change handling
* ✅ Menu-state input release
* ✅ Aim pause integration
* ✅ Auto Shoot integration

Current default activation:

```text
Left Shift
```

---

# 🎨 Skin Changer

The skin system is one of the largest subsystems in NEPHILIMGATE.

It supports complete per-weapon customization rather than only changing paint-kit IDs.

## Weapon Customization

* ✅ Paint Kit
* ✅ Seed
* ✅ Wear
* ✅ StatTrak
* ✅ StatTrak Count
* ✅ Custom Name Tag
* ✅ Per-weapon configuration

## Skin Utilities

* ✅ Apply configured skins
* ✅ Clear skins
* ✅ Default preset
* ✅ Factory New preset
* ✅ Random seed generation
* ✅ Refresh changed skins
* ✅ Weapon diagnostics

Skin changes are synchronized with the game's frame stages rather than being performed directly inside the UI renderer.

---

# 🔪 Knife Changer

Dedicated knife handling exists separately from ordinary firearm skins.

Supported functionality:

* ✅ Knife definition replacement
* ✅ Knife paint kits
* ✅ Knife models
* ✅ Active-knife validation
* ✅ Mesh-group handling
* ✅ Arms-model preservation
* ✅ Frame-stage refresh
* ✅ Material refresh

## Supported Knife Definitions

```text
Bayonet
Classic Knife
Flip Knife
Gut Knife
Karambit
M9 Bayonet
Huntsman Knife
Falchion Knife
Bowie Knife
Butterfly Knife
Shadow Daggers
Paracord Knife
Survival Knife
Ursus Knife
Navaja Knife
Nomad Knife
Stiletto Knife
Talon Knife
Skeleton Knife
Kukri Knife
```

### v0.98 Knife Fix

Fixed an issue where changing the knife model could remove or corrupt the player's hand / arms model.

Existing arms mesh groups are now preserved more carefully during knife replacement.

---

# 🧤 Glove Changer

Glove customization includes:

* ✅ Glove entity resolution
* ✅ Paint Kit selection
* ✅ Model selection
* ✅ Frame-stage updates
* ✅ Change detection
* ✅ Suppression of unnecessary refreshes

This reduces unnecessary updates when the selected glove has not changed.

---

# 🌐 Skin Database

The project contains a skin database / downloader layer.

Available skin information includes:

```text
Weapon ID
Paint Kit
Skin Name
Rarity
StatTrak Availability
```

Downloaded skin information can also be serialized into a local file.

---

# 🗺️ Radar ESP

Radar functionality uses its own player enumeration path.

This allows Radar ESP to work independently of whether regular ESP rendering is currently enabled.

### v0.98 Fix

Fixed Radar ESP reading incorrect or stale player entities.

---

# ✨ NoFlash

NoFlash removes the flashbang visual effect.

### v0.98 Fix

Fixed NoFlash accessing an incorrect / outdated player field.

The module now performs checked access against the appropriate flash-related property.

---

# 🏃 Movement

## Bunny Hop

Bunny Hop is currently implemented.

It handles:

* ✅ Jump press
* ✅ Jump release
* ✅ Player state
* ✅ Feature disable
* ✅ Menu blocking
* ✅ Cleanup during unload

Additional movement functionality is planned.

---

# 🧠 Game-State System

NEPHILIMGATE contains a centralized runtime game-state layer.

The snapshot tracks:

```text
In-Game State
Entity List
Entity System
Local Player Controller
Local Player Pawn
Resolution Status
```

The state is refreshed as the game changes to reduce stale pointer reuse.

This is particularly important during:

```text
Player Death
Player Respawn
Map Change
Disconnect
Reconnect
Pawn Replacement
Controller Replacement
```

---

# 🧬 Signature & Pattern Resolution

One of the core architectural improvements is the increasing use of runtime signature resolution.

The project contains memory-pattern scanning and RIP-relative address-resolution utilities.

Conceptually:

```text
Signature / Pattern Scanner
            ↓
Runtime Address Resolution
            ↓
Entity / Engine Pointer
            ↓
Generated Source 2 Schema
            ↓
Player / Weapon / Game Fields
```

This makes the project less dependent on a single layer of hardcoded addresses.

---

# 🪝 Hooking Architecture

The project contains hook handling around major rendering and gameplay paths.

Current architecture includes:

```text
DirectX 11 Swap Chain
Present
Resize Handling
CreateMove
Subtick Processing
FrameStageNotify
Input Integration
```

**MinHook** is included as the primary hooking library.

---

# 🖥️ User Interface

Version **0.98** introduces a major UI redesign.

The menu uses:

```text
Dear ImGui
DirectX 11
FontAwesome
Windows Imaging Component
```

## UI Improvements

* ✅ Responsive scaling
* ✅ Screen-resolution-aware sizing
* ✅ Better window-mode handling
* ✅ Redesigned sidebar
* ✅ Animated cards
* ✅ Animated tab transitions
* ✅ FontAwesome icons
* ✅ Custom logo support
* ✅ Improved typography
* ✅ Better feature grouping
* ✅ Runtime status indicators
* ✅ Debug information
* ✅ Renderer-resource recovery

---

# 🔬 Diagnostics

NEPHILIMGATE includes a dedicated debugging system.

Diagnostics exist for:

* Game state
* Pattern resolution
* Entity resolution
* Aim state
* Silent Aim callbacks
* Selected target information
* Weapon state
* Weapon services
* Material refresh
* Renderer state
* Hook state
* Error logging

This helps distinguish between failures caused by:

```text
Signature
Offset
Hook
Entity
Controller
Pawn
Weapon Services
Game State
Rendering
```

---

# 🧪 Testing

The repository contains several regression and smoke-test suites.

## Feature Regression Tests

Coverage includes:

```text
Angle Calculations
Yaw Normalization
World-To-Screen Validation
Entity Boundaries
Entity Serial Validation
Player Indices 32 / 33 / 64
Weapon Vector Bounds
Protected Memory Boundaries
Stage Gating
Weapon Refresh
Fallback Restoration
Glove Paint / Model Handling
Trigger Delay
Target Changes
Radar Independence
NoFlash
Bunny Hop
Game-State Handling
Aim Paths
```

## Pattern Scanner Tests

```text
Pattern Syntax
Pattern Boundaries
Memory Protections
```

## DirectX 11 / Swap Chain Tests

```text
D3D11 WARP Rendering
Resource Ownership
Capture
Resize
Swap Chain Replacement
Callback Forwarding
ImGui Context Ownership
Hook Activation
Hook Rollback
```

---

# 🧱 Project Structure

```text
cs2-internal/
│
├── manager/
│   ├── api/
│   │   ├── hook/
│   │   └── module/
│   │
│   ├── core/
│   │   ├── features.cpp
│   │   ├── features.h
│   │   ├── hooks.cpp
│   │   ├── hooks.h
│   │   ├── game_state.cpp
│   │   ├── game_state.h
│   │   ├── pattern_resolver.*
│   │   ├── skins.cpp
│   │   ├── skins.h
│   │   ├── skins_downloader.*
│   │   ├── skin_menu.*
│   │   ├── menu_advanced.*
│   │   ├── debug_console.*
│   │   ├── error_logger.*
│   │   └── Hook_FrameStageNotify.*
│   │
│   ├── sdk/
│   │
│   └── external/
│       ├── imgui/
│       ├── minhook/
│       ├── offsets/
│       └── source2sdk/
│
├── sdk/
├── tests/
├── docs/
├── build/
├── manager.slnx
└── LICENSE.txt
```

---

# ⚙️ Technology

| Component          | Technology                   |
| ------------------ | ---------------------------- |
| Language           | C++20                        |
| Platform           | Windows x64                  |
| Compiler           | MSVC                         |
| Renderer           | DirectX 11                   |
| Interface          | Dear ImGui                   |
| Hooking            | MinHook                      |
| Game Data          | Generated Source 2 Schemas   |
| Runtime Resolution | Signature / Pattern Scanning |
| Build              | Visual Studio / MSBuild      |

---

# 🚧 Planned / Incomplete Features

These systems already have foundations or menu pages but should currently be considered incomplete:

* ⏳ Weapon / dropped-item ESP
* ⏳ Grenade ESP
* ⏳ Bomb world information
* ⏳ Grenade trajectory prediction
* ⏳ Grenade lineups
* ⏳ Spectator list
* ⏳ Additional HUD customization
* ⏳ Additional movement features
* ⏳ Persistent Save / Load configuration backend
* ⏳ Silent Aim caching / performance optimization

---

# 🗺️ Roadmap

## v0.99

Planned focus:

```text
Silent Aim Optimization
Entity Respawn Improvements
Performance Profiling
Additional Runtime Validation
Expanded ESP
Configuration Persistence
UI Polish
Reduced Static Address Dependency
Skin / Material Stability
```

## v1.0

The goal of **v1.0** is a stable architectural baseline where every implemented subsystem is clearly separated, validated, configurable, and maintainable.

---

# ⚠️ Disclaimer

This repository is provided **as-is** for research, reverse-engineering, and educational purposes.

Counter-Strike 2, Steam, VAC, and VAC Live are trademarks or properties of their respective owners.

This project is not affiliated with or endorsed by Valve Corporation.

Using software that modifies or interacts with protected multiplayer software may violate the applicable terms of service and can result in account restrictions or bans.

**No permanent anti-cheat safety or undetected status is guaranteed.**

---

# 📜 License

Distributed under the **MIT License**.

Copyright © 2026
**sathariel & martinmarinov**

---

# ⚔️ NEPHILIMGATE

### Source 2 changes. The framework adapts.

`C++20` • `DirectX 11` • `ImGui` • `Source 2` • `Pattern Resolution`

**Version 0.98 — September 11, 2026**
