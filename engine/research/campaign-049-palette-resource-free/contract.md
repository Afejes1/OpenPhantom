# palette_resource_free

Capture the argument in a typed local, call colormap_free_entry on it, then pool_release on that same pointer. Callback global mutations must not change the second argument. Null is exercised only through non-dereferencing authored callbacks; this does not claim the real free-entry function accepts null.

All storage and strings are authored. Complete guarded map/name/global snapshots and callback order are checked. Original program execution, copied game data, byte exclusions and warning waivers are prohibited. Individual functions remain separate from shared acceptance and whole-executable identity.
