# directory_close

Capture the original state pointer. If nonnull and position is nonzero, call find_close(captured find_handle), ignoring its result. If state is nonnull, release that same state, with all callback mutations still visible. No field is cleared. Null states and mechanically forwarded handle words are observer-only, not real OS-handle safety claims.

No original program or real file enumeration/close/heap operation is executed. Strings, records and callbacks are authored. Full snapshots and explicit pre/post-callback expectations support the bounded behavior claims. No byte exclusions or warning waivers; individual functions remain distinct from shared acceptance and whole-executable identity.
