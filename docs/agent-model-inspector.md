# CS2 agent model inspector

Implemented 17 November 2026. The inspector defaults to real Seal Team 6
variant E geometry from the user's installed CS2 assets. It is a static,
shaded geometry preview with the selected chams tint, orbit, pan, zoom,
wireframe, auto-rotation and reset. It does not render game textures,
animations, live pawn poses or exact Source 2 materials.

Open **Players ESP > Chams & glow > 3D model inspector**. The agent loads
once when the section opens. **Load Seal Team 6** reloads it; **Custom
geometry** retains OBJ import and the explicitly labeled diagnostic mesh.
Failure preserves the previous mesh and reports the missing asset.

## Installation

Use `build/agent-inspector.zip`, or copy both:

- `build/econ-fix/manager.dll`
- `build/econ-fix/models/` beside that DLL

Asset lookup is relative to the module containing the inspector code,
independent of CS2's current working directory. Existing DLL reload rules
still apply. This change does not inject or reload code into a running game.

## Asset preparation

The real resource is
`agents/models/ctm_st6/ctm_st6_variante.vmdl_c`. The similarly named
`characters/models/ctm_st6/` resources in this installation are 24-vertex
dummy models and must not be used for the preview.

The official [Source 2 Viewer CLI](https://s2v.app/ValveResourceFormat/guides/command-line)
21.1 exports the resource and dependencies from the local VPK to GLB.
Downloaded release: `cli-windows-x64.zip` from the official
[21.1 release](https://github.com/ValveResourceFormat/ValveResourceFormat/releases/tag/21.1).
Archive SHA256:
`CD01665E4D31D70C1081546A697EB5654F30718000FEF1A2BB0654AE044D659E`.
The CLI is a build-time tool, not a runtime requirement.

Reproduce from the repository root:

```powershell
python tests/prepare_agent_model.py --cli build/agent-tools/vrf/Source2Viewer-CLI.exe --vpk 'C:\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive\game\csgo\pak01_dir.vpk' --output build/agent-assets
```

The converter retains third-person body/default gloves, excludes first-person
arms/sleeves and optional defuse-kit geometry, converts Y-up to Z-up, and
rejects placeholder models. Offline vertex clustering bounds the preview
below 10,000 triangles. All retained preview faces render; the old runtime
triangle-skipping behavior has been removed. The original exported GLB stays
under `build/agent-assets` for inspection.

This asset contains 19,807 source vertices and 27,893 source triangles.
The prepared OBJ contains 4,244 vertices and 9,688 triangles, with SHA256
`B2054AFC607DBF2C4C08B0C2CBFF2C4B5D991EE10F98BE2EDD911828295E17E1`.
`seal_team_6.json` records its source resource, counts and hash. MSBuild
copies the locally generated OBJ/metadata into the output `models` folder.
The converter and app use bounded files/geometry and preserve the last
loaded mesh on parse failure. Valve assets are sourced from the local game;
they are not embedded into repository source or downloaded from model sites.

## Validation

The release build, inventory/model regressions and feature regressions passed.
The model suite loads the packaged agent, verifies non-placeholder geometry
within the triangle limit, exercises the production UI, and saves
`tests/out/inventory_model/model-inspector.bmp`. That image was visually
checked for a complete, forward-facing agent. Live CS2 integration of this
updated DLL has not been exercised.
