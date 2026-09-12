# Surface-emission, scan-culling and cell-collection research batch

All four functions are reported not done in bp/bapdraw.c. None is accepted by
the matching registry. The existing fourteen-function history remains unchanged.

| Address | Team name | Candidate | Current comparison |
|---|---|---|---|
| 0x00406300 | bapdraw_emitFace | [C++ source](candidate.cpp) | Correct 432-byte extent; relocation positions and register allocation differ |
| 0x00405A50 | bapdrawOld_emitFace | [C source](legacy_candidate.c) | Correct 672-byte extent; relocation inventory and instructions differ |
| 0x00403FA0 | bapdrawOld_cullAgainstPlane | [C source](culling_candidate.c) | Correct 480-byte extent and all 22 relocations; ten instruction bytes differ |
| 0x004064B0 | bapdraw_gatherCell | [C++ source](collection_candidate.cpp) | Correct 896-byte extent and all 40 relocations; five instruction bytes differ |

The JSON target definitions preserve complete original extents, embedded tables,
alignment, constant widths, symbol addends and calls. These pending specifications
do not extend engine/target.json or claim accepted matches. No bytes are excluded.

## Focused commands

From the repository root, using the existing private compiler configuration:

    python engine/research/surface-emission/probe.py compare --reference C:/PrivateGame/WMAIN.EXE
    python engine/research/surface-emission/probe.py behavior

The compare command compiles all four candidates with the original VC5 compiler
and the strict comparator. It returns 1 for nonmatching candidates, 2 for
prerequisite/build failures, and 0 only if all complete spans agree. The behavior
command builds three separate synthetic executables: emission with a controlled
culling stub and the matched bucket helper; the actual culling candidate with a
controlled projection callback; and the collector with a controlled affine
callee. It requires the locked Docker runner and does not read or execute the game.

Each run records commands, compiler/runtime fingerprints, candidate and verifier
source hashes, object/comparison results, VC5 /FAcs assembly listings and fixture
hashes in a fresh private build directory. Schema 2 records object, complete raw
function-section and listing hashes even for unresolved comparisons. Completion
requires source/toolchain freshness and unchanged object/listing hashes.
No command creates an accepted full-build history record. The
[artifact checkpoint](../../docs/surface-artifacts-20260912.md) explains the
listing/report format, with its historical hash receipt. The
[latest collector receipt](../../docs/static-cell-ordering-20260912.json) records
the five-byte mismatch and strengthened fixture.
The [collector receipt](../../docs/static-cell-batch-20260912.json) and
[earlier receipt](../../docs/surface-batch-20260912.json) retain the preceding
four- and three-candidate snapshots.

## Focused fixture coverage

All three fixtures passed with VC5 RTM on 2026-09-12. These are unit fixtures with
controlled callees, not complete game behavior or integrated renderer coverage.

- Emission covers all 256 packed-opacity values, room masks, gates and duplicate
  marking, new-path fade clamping, legacy sixteen-step integer fades, height
  updates and culling-call interactions.
- Culling passes 3,038 checks covering four planes, sixteen selections of two
  corners, nonzero flags, input/state retention, early returns, inclusive bounds,
  signed zero and NaNs at three x87 precisions.
- Collection passes 4,208 checks covering records and extra-group skips, height
  caching and sentinel behavior, four side boundaries, far-boundary neighbors,
  selected NaNs, opposite-infinity cancellation, packed opacity, room masks, queue order and duplicate handling
  at three x87 precisions. Extra owned test storage permits observing the
  entry-only capacity check without inferring the original buffer allocation.

## Remaining compiler work

The newer emitter still needs the original extra callee-saved register and cached
membership/mask schedule. C/C++ language, local names, register hints, packed-byte
caches, signedness and optimization probes have not reproduced it.

The legacy emitter retains converted height on x87 and uses separate calls for
conditional culling extents. That reaches the required 672-byte section, but
relocation inventory and instruction scheduling still disagree.

The culler initializes its loop counter before copying the base point. Its ten
remaining bytes encode early instruction scheduling and swapped EBX/EBP retention
of base Y/Z. Processor/optimizer, arithmetic, inline-vector and table-indexing
probes did not remove them. No new compiler profile was accepted. Before registry
promotion, report its embedded table bytes separately from alignment in aggregate
coverage accounting; accepted functions currently have no such table.

The collector now verifies all forty address operands within the complete
896-byte span. Correct comparison direction and a retained negative horizontal
extent preserve unordered behavior. Explicit mask caching, bucket initialization
before its guard, and an inlined packed-opacity helper reproduce the emitter's
register choices. Only five bytes differ: FCOMPP and ADD ESP,12 trade places at
+0xF4. Its initial setup and source expression boundaries are intentional; do not
simplify them without recompiling and comparing. A fixture pass does not excuse
this instruction-order mismatch.

See [emission evidence](../../docs/surface-emission-evidence.md),
[culling evidence](../../docs/scan-culling-evidence.md) and
[collection evidence](../../docs/static-cell-evidence.md). Continue the batch
before a full original-toolchain acceptance checkpoint. A behavior pass cannot
resolve a byte mismatch.
