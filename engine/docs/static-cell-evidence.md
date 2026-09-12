# Static-cell surface collection

The team manifest lists 0x004064B0 as bapdraw_gatherCell in bp/bapdraw.c,
reported not done. The authored candidate is
[collection_candidate.cpp](../research/surface-emission/collection_candidate.cpp).
It remains outside the accepted registry: the complete extent is now 896 bytes,
but relocation positions and instruction sequences still differ.

## Observed contract

The complete body and its sole caller at 0x00405651 were inspected. Ordinary
world traversal 0x00404730 checks the cell's bounds and passes the cell pointer
and two float cell-center coordinates after processing its movers. This routine
then applies surface-group culling and emission. Its non-runtime callee,
0x0047E34D, is the already identified affine point transform; its body writes
three basis dot products plus translation. The other two calls are the existing
CRT __ftol conversions in the emission path. There are no imported API or string
references in this complete routine.

The cell has an unsigned count byte at +0 and a 16-byte header. Surface records
follow immediately, with a 64-byte stride. A 16-bit 0xFFFF tag at surface+0 or
flag 0x80 at +0x2C skips a record. The candidate uses an unsigned tag comparison
to reproduce the observed full-word immediate encoding; this does not recover
the original source typedef or other meanings of that field.

Room rejection tests surface+0x38 against byte 0x0059DF44. If rejected, the byte
at surface+0x1E counts additional records to skip, beyond the current record.
It is added to both the loop index and surface pointer, and to the extra-record
counter at 0x0059DFA4. The normal loop increment still follows. A nonzero room
skip resets the last-height sentinel to 255; a zero skip leaves that sentinel
unchanged. This outer room rejection does not increment the inner emitter's
rejection counter at 0x0059DF98.

## Height groups and frustum rejection

The last examined height begins at 255. Equal heights, or render flag 4 at
surface+0x28, bypass the group transform/test. Otherwise the input point is
(center X, center Y, float(height) + 0.5), transformed using the 48-byte camera
matrix at 0x0059DEC8. World traversal copies this from camera+8 and initializes
the slopes and padding. Height 255 therefore bypasses the first group test too.

The observed rejection comparisons, in order, are:

1. depth * vertical slope < projected Z - vertical padding;
2. -(depth * vertical slope) > projected Z + vertical padding;
3. -(depth * horizontal slope) > projected X + horizontal padding;
4. depth * horizontal slope < projected X - horizontal padding;
5. depth + float32(1.2247449) >= far limit.

Depth is transformed Y. Vertical slope/padding are 0x0059DEC4/0x0059DEA8;
horizontal slope/padding are 0x0059DF7C/0x0059DF3C; the far limit is 0x0059DF94.
The first four finite bounds are inclusive; the far acceptance is strict.
Unlike the legacy scan-plane culler, this routine has no separate positive-depth
gate. The source retains the observed negative constants in subtraction forms.
The original x87 branches have specific unordered behavior; modern C comparisons
are not the validation oracle. The fixture exercises selected NaNs, including
an unordered far limit, and the byte comparator still rejects instruction drift.

A rejected height group increments 0x0059DFAC once, adds surface+0x1D to the
extra-record counter at 0x0059DFA8, and advances by that many extra records.
It resets last height to 255, even when the extra skip is zero. An examined
nonculled surface updates last height after emission processing, including when
emission itself rejects the surface. Invalid tags and flag-0x80 records leave
last height unchanged.

## Emission and capacity

The remaining inlined body follows the current emitter's authored-opacity,
room fade, visibility, duplicate and material-bucket rules. Its second room-mask
check is preserved. The fixture can change that mask in its controlled transform
callback to exercise the reread; this is a synthetic dependency-side-effect test,
not a claim that the original affine callee changes room state.

The unsigned 8192-entry guard occurs only at function entry. The loop does not
recheck it for each accepted surface. A fixture beginning at 8191 and submitting
two records therefore ends at 8193. Deliberately larger owned fixture storage
makes that observation safe; it establishes neither the original allocation's
capacity nor valid gameplay preconditions. No new capacity guard is invented.

## Compiler and verification evidence

The span is 895 body bytes, including two out-of-line rejection blocks after the
main RET, plus one NOP before the next function at 0x00406830. The pending target
binds all 37 DIR32 and three REL32 operands: 160 address bytes in 896 total bytes.
Full-width and single-precision constants, all call targets and all padding stay
in scope. There are no byte exclusions or new compiler waivers.

The first authored source produced 880 bytes with both VC5 frontends. Explicit
rejection predicates, unsigned sentinel comparison and initialization order
bring the retained C++ candidate to the required 896-byte extent and 40 COFF
relocations. Their positions still disagree with the original, so the strict
result is unresolved, not a match. Remaining work includes x87 operand ordering,
branch layout and inlined-emitter register choices.

The original VC5 RTM compiler and linker built the focused fixtures without
warnings. The collector fixture passed 4,184 checks over record traversal,
extra skips, sentinel caching, all four finite side boundaries, far-boundary
neighbors, selected unordered cases, all 256 packed opacity values, room masks,
queue order and duplicate handling at x87 24/53/64-bit precision. Its affine
callee is controlled fixture code. No full-game execution, modern-engine
regression, full accepted-function rebuild or recovery checkpoint was performed.
See the [focused receipt](static-cell-batch-20260912.json).

Ghidra's descriptive name, three-argument signature and evidence comment were
saved. The plain C prototype was accepted through the function-name API. It
records the cell pointer and two float coordinates established by the callee's
stack reads and the caller's three pushes at 0x0040564E..0x00405650. Caller
decompilation was refreshed after the metadata change.
