# UI and runtime optimization checks

The menu uses a compact dark workspace with searchable navigation, native selectable rows, consistent cards, and a scoped theme. Theme settings are restored after drawing so the console and overlays keep their own styles. The saved accent color still controls the menu accent. Existing feature defaults and saved config keys are unchanged.

Grenade prediction renders every frame, but recomputes a moving path at most once per 33 ms. Pawn, weapon handle and throw-strength changes rebuild immediately; idle previews refresh after 100 ms. Surface inspection also samples at 33 ms. The simulation keeps its 64 Hz step and 192-step maximum. Each path point is projected once per frame. Eye height now reads the three networked view-offset components used by the SDK.

The synthetic moving-preview regression runs 250 frames at 4 ms intervals: 5,184 simulation traces, compared with the previous maximum of 48,000 for a changed view every frame. This measures trace workload, not live FPS. The tradeoff is up to 33 ms between motion-driven prediction updates.

Standalone RCS reads only local state; it no longer scans player targets when the aimbot is disabled. The regression verifies operation without an entity list, correct compensation, baseline resets on disable/respawn, and zero angle writes during 1,000 unchanged updates. The normal aim path reuses its recoil sample and current-angle read.

Camera punch removal reads one game-state snapshot per callback, copies local angle data through guarded access, validates the writable destination, and skips an already matching angle. Tests cover unchanged angles, nonfinite input, read-only destinations, and zero ReadProcessMemory/WriteProcessMemory calls during the override.

Run tests/run_all.ps1 for all 15 suites. The Release x64 DLL build is a separate check. Live appearance, trajectory accuracy and frame-time improvement still need an in-game check.


## Workspace UI and console

Navigation uses packaged Font Awesome Free Solid glyphs and a fixed array of
frame-rate-independent highlight animations. The custom cursor is drawn once at
the final overlay pass; text editing and resize cursors retain their familiar
shapes. Mouse capture still follows the existing menu/console ownership logic.

The console keeps at most 1,000 entries and 64 commands. It formats rows when
logging, copies a snapshot only when logs change, and clips drawing to visible
rows. It never holds the log mutex during ImGui drawing. The file sink keeps its
file open, serializes writes separately, and flushes warnings/errors immediately.
Normal lines are buffered; an abrupt process exit can lose buffered file output.

Commands (F1 opens the console):

- `help`, `status`, `fps`, `history`
- `clear`, `close`, `echo <text>`
- `filter <text>` or `filter off`; comma-separated terms and `-exclusions` work
- `autoscroll on` / `autoscroll off`
- `config save <profile>` / `config load <profile>`

Profiles use letters, digits, underscores or hyphens, and are stored under
`%LOCALAPPDATA%/Nephilim/Configs`. Config commands use the same validated store as
the menu. Up/Down recalls history and restores an unfinished draft; Tab completes
command names. Commands run locally and do not execute shell commands.

`tests/run_console_regressions.ps1` checks commands, literal percent signs,
bounded retention, clipped geometry, filtering, concurrent logging and cursor
ownership. Live game frame times still need measurement on the target machine.
