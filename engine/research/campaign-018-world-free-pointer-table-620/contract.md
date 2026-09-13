# world_free_pointer_table_620

cdecl void op_world_free_pointer_table_620(OP_WORLD *world). Same observed algorithm as table220 routine, but signed count0x620 and inline entries0x624. NULL returns; signed live bound; nonnull entry forwarded to cdecl void op_release(void*), then CURRENT slot cleared; final count0 even for initial negative. Partial view only: repeating1024-byte table pattern suggests255 slots, but full original capacity is not independently established. Use byte-addressed/typed pointer access if avoiding an inferred array extent; do not claim a recovered capacity. No compiler operation depends on sizeof the view. Fixtures only0..3 valid slots, full guarded storage, NULL holes, signed bounds, order and callback field/slot mutations; other caller-visible words unchanged. Both destructor and graphics cleanup callers verified. Full118 bytes to tail cleanup. Do not change offsets, order or callback ABI to improve score.

Team report: bapworld_freePtrArray0x220 | bapworld_freePtrArray0x620; reported not_done.

Original interval and all call operands were independently checked in Ghidra and read-only PE evidence before assignment. No original bytes or decompiler output are included.
