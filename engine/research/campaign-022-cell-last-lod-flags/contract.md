# Static contract

cdecl void(OP_CELL*), no NULL guard. Full249-byte body, no operands. Current sentinel retains byte0x2E. Scan later records; sentinel OR mask containment clears last flag; reaching end sets flag. Only bit8 changes. Preserve original semantically unused count local because it emits code. Unsigned count/masks and ushort sentinel independently checked. Caller0041F2CE supplies same cell.

Team manifest: bapmap_markLodRunTail is not_done (source line 1489).
