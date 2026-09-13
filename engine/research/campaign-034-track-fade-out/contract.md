# track_fade_out

For valid track0..3 with zero flags, return0 without any write. Otherwise replace flags with (flags & ~4)|8, set rate+28 to1/duration when duration>0 or literal1 otherwise, return1. Whole object preservation and finite dyadic boundaries checked; nonfinite path statically observed only.

All unresolved record bytes are opaque. Four tracks established by original initialization/removal callers, not an inferred unbounded pool. Full span includes native compiler alignment. Team manifest not_done. Pinned VC5/O2/MT, no byte exclusions or artificial padding, no original execution. Ten attempts/fifteen minutes; stop after five non-improving attempts.
