# Prepared extended reset flashes

cdecl void op_extended_reset_flashes(void). Calls op_extended_clear_flashes(void) exactly once; no other explicit work. Caller 0041CB16 is inside world lifecycle FUN_0041C9E0. Callee 004284FC clears 44-byte flash slots and resets signed count. Full interval 00427C90..00427C9A has no tail. Fixture checks one call per invocation and preserves callback-written synthetic state. Do not execute real callee.

Full original body and interval verified through the next function entry; no alignment or table tail. Every listed operand independently validated against owned original PE and complete HIGHLOW relocation coverage before implementation. Team manifest: not_done, bp/extended.c. Use C89-era VC5 C, /Od /MT and existing runner. No masks, assembly, byte insertion, manufactured frames, warning waivers or target edits. Per function15minutes/10compiles, stop after five no-improvements or campaign worker deadline. Freeze near results rather than polish.
