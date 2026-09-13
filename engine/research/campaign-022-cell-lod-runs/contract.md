# Static contract

cdecl void(OP_CELL*), no NULL guard. Full 258-byte body, no globals/calls/operands. Unsigned count at0, 255 owned records at0x10 stride0x40. Material u16 at0; flags u16 at0x2C; mask u8 at0x38; run u8 at0x1E. Current sentinel or flag0x80 retains old run; next sentinel stops; current-mask subset of next OR next flag0x80 includes next; mismatch stops; cap254; write eligible run. Caller0041F2CE supplies the same cell. All branches and widths checked against complete instructions.

Team manifest: bapmap_buildLodRuns is not_done (source line 1491).
