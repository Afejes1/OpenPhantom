# world_load_via_skip

cdecl int op_world_load_via_skip(OP_STREAM *stream,void *unused_world,OP_HEADER *header). Three stack arguments: first stream, second unused, third header. Header unsigned skip_bytes at0x2D4 (NOT current payload at0x800). Calls cdecl int op_world_skip_chunk(OP_STREAM*,unsigned int bytes) with stream,header skip_bytes, ignores result and returns1. Real skip helper confirmed two stack args by instructions and exact Campaign017 source; Ghidra stale thiscall adds a spurious argument, do not copy that. Use (void)unused_world to avoid /W4 unused parameter warning; no emitted operation expected. Fixtures owned distinct pointers, unused_world NULL/nonnull never dereferenced, header other-field decoys, zero/high-bit sizes, ignored error returns, callback mutations retained, guarded snapshots. No direct caller XREF found. Full32 bytes to finalize_b3d_world_links.

Team report: bapworld_loadViaSkip; reported not_done.

Original interval and all call operands were independently checked in Ghidra and read-only PE evidence before assignment. No original bytes or decompiler output are included.
