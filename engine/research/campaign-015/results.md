# Campaign 015 results

**Five exact functions,287 full-span bytes, plus one research candidate.** All122
focused fixture assertions pass after independent source and test review.

| Function | Address | Full bytes | Status | Focused checks |
| --- | --- | ---: | --- | ---: |
| [extended_reset_flashes](../campaign-015-extended-reset-flashes/README.md) | 0x00427c90 | 10 | Exact | 4 |
| [extended_load_colormap](../campaign-015-extended-load-colormap/README.md) | 0x00428844 | 73 | Exact | 40 |
| [extended_free_colormaps](../campaign-015-extended-free-colormaps/README.md) | 0x0042888d | 68 | Exact | 16 |
| [extended_advance_cel](../campaign-015-extended-advance-cel/README.md) | 0x00428db6 | 56 | Exact | 18 |
| [extended_attach_colormap](../campaign-015-extended-attach-colormap/README.md) | 0x00428dee | 117 | Research: 6 differing bytes | 12 |
| [extended_load_material](../campaign-015-extended-load-material/README.md) | 0x00428e63 | 80 | Exact | 32 |

All six candidates used their first natural C source experiment. The cel-advance
helper matches raw bytes; the other four exact helpers match after all14 address
operands are verified. Colormap attachment has6 differing bytes over117 (5.128205%);
that exceeds the5% scheduling threshold and remains research, with no known
behavioral defect. Full spans are unchanged, including every original byte.

Review verified actual cdecl loader/free contracts, live count and pointer reloads,
signed cel/count comparisons, callback mutation timing, fallback returns, and
preserved storage. It caught two fixture oracle problems: a sentinel prevented the
expected colormap insertion, and attachment expectations were initially snapshotted
after the call. Both were corrected before successful final verification. Parent
review added explicit mutation expectations, signed/unsigned boundaries, full
array/guard observations and owned sentinel pointers. No candidate source rewrite
was needed. Names and evidence comments for the six recovered functions were
saved in Ghidra; affected callers were re-decompiled.

Six worker compiles,12 final native/Docker candidate compiles and six successful
fixture builds/executions produced the final receipts. Two earlier fixture builds
failed under strict VC5 warnings: do-while-zero CHECK caused C4127, and memset on
an array of const-char pointers caused C4090/C4022. Warning-safe check functions
and typed initialization fixed the tests. Those attempts add four candidate
compiles and two failed fixture compiles, with zero fixture executions; their
source/report hashes remain in [review](review.json) and [metrics](metrics.json).
No failures are hidden and no warning waiver was added.

All six Docker cases used the existing explicit native-listing fallback after
complete source/toolchain and function/fixup agreement. The original empty Docker
listings remain preserved privately. Every final source snapshot and receipt is
checked against staged Git blobs. The matcher and shared build tools are unchanged.
Full regression was already completed at the preceding acceptance checkpoint and
was not repeated here. Accepted history remains21functions/21records; these are
focused research results pending shared ABI integration. Original game content,
compiler binaries, generated objects and decompiler output remain outside Git.
The original was never executed or patched; whole-executable identity is pending.
