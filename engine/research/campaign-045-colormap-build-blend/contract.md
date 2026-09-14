# colormap_build_blend

Build a65536-byte256x256 table from a768-byte RGB palette. Outer RGB is captured once per row, inner RGB read sequentially; compute outer*blend+(1-blend)*inner in observed x87 order. Call nearest(palette+3,254,three floats byvalue), store lowbyte plus1. Full375-byte target,3 immutable1.0f operands and1 call operand. No blend clamping.

Team not_done verified by numeric address. Authored guarded disjoint palette/table fixtures use independent literal binary32 oracles, callback order/last-write/current-unwritten checks and full final snapshots. No nearest-search implementation claim; callback return exercises allbyte values. Saved/restored maskedPC53 round-nearest. Native compilation only; authored fixture runs in locked Docker. Per-function15minute/10attempt cap and5non-improvement stop; no exclusions or waivers.
