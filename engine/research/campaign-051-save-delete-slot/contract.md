# save_delete_slot

Call slot_path(slot), then delete_file(path). Return0 for backend0, else1. The actual backend wraps file deletion with0 success/-1 failure. Manifest save_slotExists conflicts with this delete-slot behavior; do not call it an existence test.

All strings, buffers and callback services are authored. No original game, real file deletion/load, lifecycle dispatch or disk API execution occurs. Callback ordering, returned pointer identity, signed inputs, complete expected snapshots and bounded writes are checked as applicable. No byte exclusions or warning waivers. Shared acceptance and whole-executable identity remain separate.
