# world_free_tail_blocks

cdecl void op_world_free_tail_blocks(OP_WORLD *world). NULL returns. Signed/unsigned count word at0xCB4 only tested nonzero: if nonzero call op_release(world->sprites at0xCB8), then set spritesNULL. Always set sprite_count0 afterward. Then if LIVE sound_placements pointer0xCC0 nonnull release it and setNULL. Then if LIVE sound_descriptors pointer0xCC8 nonnull release it and setNULL. Other count words0xCBC/0xCC4 are untouched. First branch keyed on count, not pointer: count0 retains a nonnull sprites pointer; nonzero can forwardNULL. Callbackfree cdeclvoid(void*) actualonearg. Fixture validatesNULL-world, all combinations, negative/nonzero first count, NULL forwarding, release order, prior fields alreadycleared at later callbacks, mutations to upcoming pointers picked up live, replacingcurrentpointer stillcleared, untouched counts/fullguardedworld. Backend stub owns no realfree; do not run original. Destructor caller0041CCFE. Full158 bytes to clock setter.

Team report: bapworld_freeTailBlocks; reported not_done.

Original interval and all call operands were independently checked in Ghidra and read-only PE evidence before assignment. No original bytes or decompiler output are included.
