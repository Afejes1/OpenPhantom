# window_set_rect

Void cdecl copies a16-byte four-word record from a captured source pointer into the global. Static disassembly proves ordered reads/stores for words0..3; snapshots prove final value/preservation, not intermediate concurrent observation. Neutral unsigned words avoid inferred coordinate signedness. Disjoint guarded source and exact self-alias are covered; NULL and partial overlap are excluded.

Independent static review confirmed99 full bytes, nine data operands, cdecl ABI and caller context. No external calls or alignment tails. Fixtures vary216 initial states across six word patterns, preserving all documented globals and guarded input. Global adjacency is not assumed as a fixture guard. No original executable, OS operation, exclusion, warning waiver or whole-executable claim.
