# Surface-emission, scan-culling and cell-collection research batch

All five functions are reported not done in bp/bapdraw.c. None is accepted by
the matching registry. The existing fourteen-function history remains unchanged.

| Address | Team name | Candidate | Current comparison |
|---|---|---|---|
| 0x00406300 | bapdraw_emitFace | [C++ source](candidate.cpp) | Correct 432-byte extent; relocation positions and register allocation differ |
| 0x00405A50 | bapdrawOld_emitFace | [C source](legacy_candidate.c) | Correct 672-byte extent; relocation inventory and instructions differ |
| 0x00403FA0 | bapdrawOld_cullAgainstPlane | [C source](culling_candidate.c) | Correct 480-byte extent and all 22 relocations; six instruction bytes differ |
| 0x004064B0 | bapdraw_gatherCell | [C++ source](collection_candidate.cpp) | Correct 896-byte extent and all 40 relocations; five instruction bytes differ |
| 0x004056C0 | bapdrawOld_gatherCell | [C source](legacy_collection_candidate.c) | Correct 816-byte extent and 34 relocations; operand positions and instructions differ |

The JSON target definitions preserve complete original extents, embedded tables,
alignment, constant widths, symbol addends and calls. These pending specifications
do not extend engine/target.json or claim accepted matches. No bytes are excluded.

## Focused commands

From the repository root, using the existing private compiler configuration:

    python engine/research/surface-emission/probe.py compare --reference C:/PrivateGame/WMAIN.EXE
    python engine/research/surface-emission/probe.py behavior

The compare command compiles all five candidates with the original VC5 compiler
and the strict comparator. It returns 1 for nonmatching candidates, 2 for
prerequisite/build failures, and 0 only if all complete spans agree. The behavior
command builds four separate synthetic executables: emission with a controlled
culling stub and the matched bucket helper; the actual culling candidate with a
controlled projection callback; and the collector with a controlled affine
callee; and the legacy collector with controlled bucket, culling and emission
callees. It requires the locked Docker runner and does not read or execute the game.

Each run records commands, compiler/runtime fingerprints, candidate and verifier
source hashes, object/comparison results, VC5 /FAcs assembly listings and fixture
hashes in a fresh private build directory. Schema 3 records object, complete raw
function-section, listing and private byte/relocation-inventory hashes even for
unresolved comparisons. The pinned Docker runtime emits truncated .cod files;
each is explicitly flagged by missing function/file terminators. Complete object
inventories are checked against the COFF section, including tables and padding.
Completion requires source/toolchain freshness and unchanged artifact hashes.
An optional native diagnostic produces terminated listings only after checking
identical function bytes and relocations against the Docker-built object. See
[listing integrity](../../docs/listing-integrity-20260912.md).
No command creates an accepted full-build history record. The
[artifact checkpoint](../../docs/surface-artifacts-20260912.md) explains the
listing/report format, with its historical hash receipt. The
[latest collector receipt](../../docs/static-cell-ordering-20260912.json) records
the five-byte mismatch and strengthened fixture.
The [collector receipt](../../docs/static-cell-batch-20260912.json) and
[earlier receipt](../../docs/surface-batch-20260912.json) retain the preceding
four- and three-candidate snapshots.

## Focused fixture coverage

All four fixtures passed with VC5 RTM on 2026-09-12. These are unit fixtures with
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

- Legacy collection passes 1,195 checks covering linked/discovered groups,
  capacity boundaries, callback-visible reloads, slab selection, raw float
  arguments, inline records, seen references and null-reference visit counting.

## Remaining compiler work

The newer emitter still needs the original extra callee-saved register and cached
membership/mask schedule. C/C++ language, local names, register hints, packed-byte
caches, signedness and optimization probes have not reproduced it.

The legacy emitter retains converted height on x87 and uses separate calls for
conditional culling extents. That reaches the required 672-byte section, but
relocation inventory and instruction scheduling still disagree.

The culler initializes its loop counter before copying the base point. A float
array with inline memcpy restores the original EBP/EBX retention of base Y/Z.
Its six remaining bytes encode the plane-index load and base-Y store ordering.
No compiler profile or comparison exception was accepted. Before registry
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

The legacy collector retains both duplicated group walks and the three traversal
phases. C/C++ frontend, loop-index ordering, array indexing and scan-position
copy probes retain an 816-byte section with 34 relocations but do not reproduce
their original locations. It remains outside the accepted registry. See
[legacy-cell evidence](../../docs/legacy-cell-evidence.md) and the
[five-candidate checkpoint](../../docs/legacy-cell-batch-20260912.json).
