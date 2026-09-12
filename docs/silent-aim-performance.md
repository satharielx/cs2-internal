Silent-aim performance follow-up

The subtick hook previously waited for the settings mutex, which Present also holds through ESP and menu rendering. The hook now tries the mutex once. If another thread owns it, the original serializer runs unchanged and aim is skipped for that callback. This avoids waiting for a busy rendering pass; it can reduce aim updates during sustained contention.

Target snapshots retain their 8 ms expiry. Geometry is recalculated only after a snapshot refresh or a local eye-position change. Current source angles, FOV, range, and team/visibility settings still participate in each selection, and the selected entity's serial and health are checked before writes. Game-state reads use one snapshot inside the aim routine instead of repeated state locks and a module-loader lookup. History verification is diagnostic and now sampled at 4 Hz per callback thread, rather than reading history and flags for every write.

The native aim diagnostics show **Geometry updates** and **Busy-settings skips**. **Verified history samples** is a sample count, not the total number of writes.

Validation: the feature regression fixture exercises 64 distinct player pawns and 1,000 callbacks, checks cache refresh on movement/expiry/weapon changes, current-angle/FOV filtering, and rejects dead targets. The callback fixture holds the settings mutex on a separate thread while 1,000 serializers complete, verifying that every original is forwarded without an aim write. Both test runners pass. One local synthetic comparison measured 25.4 ms before and 16.7 ms after per 1,000 crowded-map aim callbacks; this excludes the actual engine serializer and is not an in-game FPS measurement.

Rebuild the DLL to use the changes. Live FPS and frame-time improvement remain unverified.
