The loadout editor uses plain HTML, CSS, and JavaScript with no package install or web server. Open `manager/ui/index.html` directly, or use **Open Web Editor** in the native Inventory Changer. Builds copy the `ui` folder beside the DLL; distribute that folder with the DLL.

1. In the native Inventory Changer, load the database and choose **Export Catalog**.
2. In the web editor, choose **Open catalog** and select that JSON file. It includes the database and current loadout.
3. Search finishes or select the Knives category. Save a finish and its wear, seed, optional StatTrak, and name tag. Saving a knife also selects that knife model. The colored card graphics are decorative, not skin previews.
4. Choose **Export loadout**, then **Import Loadout** in the native menu. Import replaces the configured weapon/knife loadout and glove selection. Enable Skin Changer to apply it through the existing frame-stage code.

The browser editor is a separate window, not an embedded replacement for the native overlay. Changes transfer explicitly through files; browser saves do not change the running game. Drafts are stored locally when browser storage is available. Knife finishes are filtered by weapon ID from the loaded database; the small offline fallback database has no knife finishes, so load the online database for those options. The native knife panel also provides paintkit selection.

Loadout format: `NEPHILIM_LOADOUT 1 <knife ID> <glove paintkit>` followed by one row per item: `<weapon ID> <paintkit> <seed> <wear> <StatTrak 0/1> <kills> <UTF-8 name encoded as lowercase hex, or ->`. Imports are size limited, validated against the current catalog, and committed only after the entire file passes validation. Name tags are limited to 31 UTF-8 bytes.

Checks: `node --test tests/web_loadout.test.js`, `powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_loadout_import.ps1`, and `powershell -NoProfile -ExecutionPolicy Bypass -File tests/run_feature_regressions.ps1`.
