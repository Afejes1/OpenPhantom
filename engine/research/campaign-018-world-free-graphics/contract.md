# world_free_graphics

cdecl int op_world_free_graphics(OP_WORLD *world). First word load_state. NULL returns1. Nonnull with load_state!=0 calls op_world_free_materials(world), op_world_free_pointer_table_220(world), op_world_free_pointer_table_620(world) in order; all cdecl void(OP_WORLD*) with caller/callee stacks verified. Returns0 for every nonnull world, including unloaded. Gate load_state once; callbacks changing it do not suppress subsequent calls. No target stores. Fixtures NULL, zero/positive/high-bit states, order, callback-visible world state, each callback mutations retained and full guarded object. No direct caller XREF found; callees also used by destroy_b3d_world. Full64 bytes to read_b3d_chunk_header.

Team report: bapworld_freeGraphics; reported not_done.

Original interval and all call operands were independently checked in Ghidra and read-only PE evidence before assignment. No original bytes or decompiler output are included.
