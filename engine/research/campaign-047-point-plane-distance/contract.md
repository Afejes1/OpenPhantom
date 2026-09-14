# point_plane_distance

Cdecl float(point,normal,origin), three const12-byte vector pointers. Return (point.x-origin.x)*normal.x plus correspondingY thenZ products, preserving observed x87 evaluation order. No writes/calls/operands; complete60-byte span with no tail. Callers in directional/omni face-light accumulation pass point,normal,origin and store binary32 result before thresholds. Authored dyadic vector pool with all64 argument-index combinations covers disjoint and exact aliases. Independent literal result words, negative-zero case, full guarded input snapshots; no nonfinite/overflow claims in this fixture.

Team not_done verified by numeric address. Fixtures use authored guarded storage and saved/restored maskedPC53 round-nearest control. Original executable is static evidence only. Independent native/Docker matching, passing focused fixtures and review precede any exact claim; shared acceptance and whole-executable identity remain separate. No exclusions or warning waivers.
