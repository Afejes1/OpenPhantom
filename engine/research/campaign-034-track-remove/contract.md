# track_remove

Select track index0..3 (332-byte stride, array+8). If callback+300 is nonnull, call captured callback(thing->userdata at+328,index,0). Callback runs before clearing flags+0,keyframe+296,callback+300; no other field cleared. Null callback path does not dereference thing. Retain other callback mutations and return1. Callback source return type is not inferred; return unused. No invalid index domain claim.

All unresolved record bytes are opaque. Four tracks established by original initialization/removal callers, not an inferred unbounded pool. Full span includes native compiler alignment. Team manifest not_done. Pinned VC5/O2/MT, no byte exclusions or artificial padding, no original execution. Ten attempts/fifteen minutes; stop after five non-improving attempts.
