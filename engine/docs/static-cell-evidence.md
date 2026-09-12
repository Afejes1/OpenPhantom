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

The first authored source produced 880 bytes. Rejection predicates, unsigned
sentinel comparison and initialization order first brought it to 896 bytes,
but its relocation positions and several floating-point branches disagreed.
The current candidate verifies all forty operands within that complete span.
Five instruction bytes remain different, at +0xF4 through +0xF8: the original
performs FCOMPP followed by ADD ESP,12; the candidate schedules the stack cleanup
first. Every other byte agrees after strictly verified address resolution.
This remains a mismatch, with no instruction-order exception or excluded bytes.

The second and third side-bound branches reject on ordered greater-than.
Writing the candidate rejection with the upper bound on the left of a less-than
comparison reproduces those branches with VC5. Retaining the negative horizontal
extent in a float local reproduces its reuse and subsequent negation for the
fourth bound. The preceding source's reversed x87 operands differed for unordered
inputs, despite agreeing on the finite boundary cases already tested.

Two new cases make only an upper bound unordered: negative infinite projected Z
with positive infinite vertical padding, and the corresponding X/horizontal
case. Static instruction analysis predicts acceptance, while the preceding
authored candidate rejected both in all three x87 precision modes. The current
candidate accepts them. Two positive-infinity cases instead make a lower-bound
comparison unordered and must reject; they also pass. These fixtures execute
only authored code with controlled affine results, never the original game.

Explicit byte-mask temporaries remove an extra copy in the inlined fade path.
Initializing the bucket before its guard restores the original register choices.
An authored inline packed-opacity helper restores the byte-register copy/shift
order; it generates no separate function section or additional matched-function
claim. The remaining five-byte stack-cleanup discrepancy persists with the two
approved optimized profiles, first-bound inline/local expression forms, and
several private processor/debug-information diagnostics. Those diagnostics do
not change the accepted compiler policy. Related mask changes in the separate
emitters corrected relocation counts but not positions, so those sources remain
unchanged in this checkpoint.

VC5 RTM and the original linker built all three focused fixtures without
warnings. The collector now passes 4,208 checks at x87 24/53/64-bit precision;
the emission and 3,038-check culling fixtures also pass. Six synthetic tests
separately verify recorded artifact hashes, complete section coverage, missing
or empty listings, object/listing modification and removal, wrong symbols, and
the difference between COFF metadata hashes and function-section hashes.
No full regression, accepted-function rebuild, or recovery checkpoint was run.
The fourteen-function accepted history is unchanged. See the
[current focused receipt](static-cell-ordering-20260912.json); the
[earlier receipt](static-cell-batch-20260912.json) preserves the initial collector
checkpoint and its narrower fixture coverage.

Ghidra's descriptive name, three-argument signature and evidence comment were
saved. The plain C prototype was accepted through the function-name API. It
records the cell pointer and two float coordinates established by the callee's
stack reads and the caller's three pushes at 0x0040564E..0x00405650. Caller
decompilation was refreshed after the metadata change.
