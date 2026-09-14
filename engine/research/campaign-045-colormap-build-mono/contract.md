# colormap_build_mono

Build a16384-byte64x256 table from a768-byte RGB palette. Columns0..31 are identity across64rows; for columns32..255, row63 is identity and rows62..0 accumulate stored binary32 steps toward targetRGB over63 increments. Nearest callback is cdecl uint(palette+3,254,red,green,blue), three floats by value; lowbyte result plus1 is stored. Full428-byte target,1 call operand.

Team not_done verified by numeric address. Authored guarded disjoint palette/table fixtures use independent literal binary32 oracles, callback order/last-write/current-unwritten checks and full final snapshots. No nearest-search implementation claim; callback return exercises allbyte values. Saved/restored maskedPC53 round-nearest. Native compilation only; authored fixture runs in locked Docker. Per-function15minute/10attempt cap and5non-improvement stop; no exclusions or waivers.
