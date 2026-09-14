# save_disk_free_at_least

Call the stdcall GetDiskFreeSpaceA import slot with NULL root and four distinct output pointers. Model that slot as an external typed function pointer at its verified IAT address; the fixture points it only at authored code. API failure returns1 without reading outputs. On success multiply free_clusters*sectors*bytes in unsigned32 arithmetic, including wrap, then return1 only when requested is strictly less than available; equality returns0. Independent literal products cover zero and overflow; total-clusters is ignored.

All strings, buffers and callback services are authored. No original game, real file deletion/load, lifecycle dispatch or disk API execution occurs. Callback ordering, returned pointer identity, signed inputs, complete expected snapshots and bounded writes are checked as applicable. No byte exclusions or warning waivers. Shared acceptance and whole-executable identity remain separate.
