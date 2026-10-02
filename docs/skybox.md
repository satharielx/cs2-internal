# Skybox changer (method 2)

Open **View & Visuals**, enter a game-relative sky material path, and select **Apply skybox**. Compiled .vmat_c names are normalized to .vmat. **Restore map sky** or disabling **Custom skybox** refreshes the original map material.

The feature resolves client.dll!UpdateSkybox using the updateskybox signature from [CS Patterns](https://www.cspatterns.dev/json). On the game-thread stage-6 callback, it finds env_sky entities, writes the custom handle into the generated C_EnvSky::m_hSkyMaterial field, and calls UpdateSkybox(entity). It then puts the map handle back into the entity field immediately. The intended result is a refreshed scene using the selected material while the entity retains its original material for restoration. This corrects the pasted method-2 example's assignment to a local variable.

Material replacement does not use a skybox detour or scene-world resolver. The existing frame-stage callback drives the feature. While enabled, an entity scan runs at most once per second unless a new selection is requested. Unchanged entities are not refreshed. Identity pointers, serial-bearing entity handles, and map-material handles detect replacement entities. Disconnects/entity-system changes clear cached state. Unload requests restoration on the game thread and waits up to 1.5 seconds; restoration cannot be guaranteed if the game thread stops.

Material lookup uses VMaterialSystem2_001 slot 14. Missing/error materials leave the displayed sky unchanged. Assets must already be accessible to the engine; the separate resource-precache loader omitted from the original article is still not implemented.

The signature, virtual index, and schema layout require verification against the running game. Mocked-engine tests verify the field write during UpdateSkybox, original field preservation, restoring the map sky, entity reuse, unchanged-frame behavior, missing materials, and disconnect cleanup. They do not prove the current game retains the refreshed scene material after the temporary substitution. Test applying, restoring, map changes, and unloading in a local session.

Run tests/run_skybox_regressions.ps1 for regression checks.

## Sky color
In View & Visuals or Main Misc Settings, enable **Custom sky color**, choose **Sky color**, and adjust **Sky brightness** (default 3). Color works independently of the custom material toggle and does not request cl_fullupdate.

The optional scenesystem.dll DrawArray hook uses the signature from the supplied reference, reads the scene object at draw-data +0x18, and temporarily writes three RGB floats at object +0xE8. It restores the original RGB after the draw; disabling tint requires no cached entity pointers. The callback participates in the existing unload drain. Missing signatures or hook creation failures appear beside the controls and do not prevent other features from loading.

These offsets and the seven-argument function signature come from the supplied reference and still require live-game verification. Only the first scene object is addressed, matching that reference; no unverified draw-record stride is assumed. Regression tests cover tint scaling, restoration, the adjacent fourth float, disabled/disconnected behavior, and invalid counts.