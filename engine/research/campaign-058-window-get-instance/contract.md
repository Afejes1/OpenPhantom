# window_get_instance

No-argument cdecl returns the live32-bit instance word. Tests verify full-word values and intervening global mutation while preserving window, rectangle and owned input state.

Independent static review confirmed99 full bytes, nine data operands, cdecl ABI and caller context. No external calls or alignment tails. Fixtures vary216 initial states across six word patterns, preserving all documented globals and guarded input. Global adjacency is not assumed as a fixture guard. No original executable, OS operation, exclusion, warning waiver or whole-executable claim.
