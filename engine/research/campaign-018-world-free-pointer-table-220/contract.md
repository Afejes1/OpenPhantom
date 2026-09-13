# world_free_pointer_table_220

cdecl void op_world_free_pointer_table_220(OP_WORLD *world). NULL returns. Signed int count0x220, inline void* entries beginning0x224. One signed index local loops against live count. For each nonnull entry call cdecl void op_release(void*) then clear CURRENT same entry after callback. Zero count at end even when original negative. Skip NULL entries. Field0x620 begins next table's count, bounding the first view to255 pointer slots; this is a partial view, not recovered complete capacity/validation logic. If using255 entries, document the inferred view extent; fixtures use only0..3 entries. Full guarded storage, count negative/zero, NULL gaps, order, current slot replacement cleared after callback, next slot mutation/live count expansion/contraction, untouched/unvisited entries retained except explicit callbacks. Callee495452 one stack pointer, no extra argument. Two callers: world destructor and graphics cleanup. Full118 bytes to table620 routine.

Team report: bapworld_freePtrArray0x220; reported not_done.

Original interval and all call operands were independently checked in Ghidra and read-only PE evidence before assignment. No original bytes or decompiler output are included.
