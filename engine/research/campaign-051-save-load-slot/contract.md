# save_load_slot

Call slot_path(slot), then load_saved_game(path,1); the returned value is unused in observed callers and this wrapper is modeled void. Manifest save_writeSlotNoBroadcast is retained only as team provenance, not behavioral proof.

All strings, buffers and callback services are authored. No original game, real file deletion/load, lifecycle dispatch or disk API execution occurs. Callback ordering, returned pointer identity, signed inputs, complete expected snapshots and bounded writes are checked as applicable. No byte exclusions or warning waivers. Shared acceptance and whole-executable identity remain separate.
