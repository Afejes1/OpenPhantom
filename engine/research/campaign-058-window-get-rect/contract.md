# window_get_rect

No-argument cdecl returns the address of the live16-byte global, not a copy. Tests verify identity, safely mutate selected fields through that pointer, then check global state and repeated identity. Only owned storage is dereferenced.

Independent static review confirmed99 full bytes, nine data operands, cdecl ABI and caller context. No external calls or alignment tails. Fixtures vary216 initial states across six word patterns, preserving all documented globals and guarded input. Global adjacency is not assumed as a fixture guard. No original executable, OS operation, exclusion, warning waiver or whole-executable claim.
