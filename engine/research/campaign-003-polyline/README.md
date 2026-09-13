# Submit screen-space polyline strip

This case reconstructs the 478-byte `op_submit_polyline` function at WMAIN `0x00408735` using the reviewed campaign packet and a natural four-vertex aggregate.

The candidate preserves allocation ordering, projection arguments, per-segment depth rejection, every vertex field, signed width behavior, and the quad callback contract. The fixture covers null allocation permutations and call ordering, zero/one counts, one and multiple segments, first/second/later invalid depths, NaN depth, negative and zero widths, callback arguments, all eight output fields, and input preservation.

The best VC5 `/Od /MT` result has the full 478-byte extent, the original 140-byte local layout, and all seven verified operands at their expected offsets. Eight bytes differ beginning at offset 364, confined to instruction ordering for the first aggregate copy: the original loads the source address before the copy count, while VC5 emits those operations in the opposite order from the readable structure assignment. No artificial temporary or source distortion was added.

Final native report: `engine/build/campaign-001-submit_polyline-63bab7a4fb51/research-result.json`. Final locked-Docker report: `engine/build/campaign-001-submit_polyline-5b09738ce9d9/research-result.json`. The fixture passed 35 checks with no failures.
The coordinator independently verified the frozen source and fixture. The
[public receipt](result.json) records **8/478 differing bytes (1.674%)**, with
all seven operands verified and 35 focused assertions passing. This is a
near match for later work, not an accepted or exact function. Both standard
verification commands return 1 because the mismatch remains:

```powershell
python engine/research/campaign-001/verify.py --case engine/research/campaign-003-polyline --runner native --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE
python engine/research/campaign-001/verify.py --case engine/research/campaign-003-polyline --runner docker --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE --behavior --expect-report <native-report-path>
```
