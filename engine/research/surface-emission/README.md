# Surface-emission and scan-culling research batch

All three functions are reported not done in bp/bapdraw.c. None is accepted by
the matching registry. The existing fourteen-function baseline remains intact.

| Address | Team name | Candidate | Current comparison |
|---|---|---|---|
| 0x00406300 | bapdraw_emitFace | [C++ source](candidate.cpp) | Correct 432-byte extent; relocation positions and register allocation differ |
| 0x00405A50 | bapdrawOld_emitFace | [C source](legacy_candidate.c) | Reduced from 688 to the required 672 bytes; relocation inventory and instructions still differ |
| 0x00403FA0 | bapdrawOld_cullAgainstPlane | [C source](culling_candidate.c) | Correct 480-byte extent and all 22 relocations; ten instruction bytes differ |

The JSON target definitions preserve complete original extents, embedded tables,
alignment, constant widths, symbol addends and calls. These pending specifications
do not extend engine/target.json or claim accepted matches. No bytes are excluded.

## Focused commands

From the repository root, using the existing private compiler configuration:

    python engine/research/surface-emission/probe.py compare --reference C:/PrivateGame/WMAIN.EXE
    python engine/research/surface-emission/probe.py behavior

The compare command compiles this three-function batch with the original VC5
compiler and the strict comparator. It returns 1 for nonmatching candidates,
2 for prerequisites/build failures, and 0 only if all complete spans agree.
The behavior command builds two separate synthetic executables: emission with a
controlled culling stub and the matched bucket helper, and the real culling
candidate with a controlled projection callback. It requires the locked Docker
runner and does not read or execute the game.

Each run records commands, compiler/runtime fingerprints, candidate and verifier
source hashes, object/comparison results and fixture hashes in a fresh private
build directory. No command creates an accepted full-build history record.
See the [public batch receipt](../../docs/surface-batch-20260912.json).

Both focused fixtures passed with the original compiler on 2026-09-12. Emission
covers all 256 packed-opacity values, room masks, gates and duplicate marking,
new-path fade clamping, legacy sixteen-step integer fades, height updates and
culling-call interactions. Culling adds 3,038 checks covering all four planes,
all sixteen selections of two corners, nonzero flag values, input/state retention,
early returns, inclusive thresholds, signed zero and NaNs at three x87 precisions.
The projection callback is controlled test code. These are separate unit fixtures,
not an integrated emitter-to-culler test or a full game-behavior claim.

## Remaining compiler work

The newer emitter still needs the original extra callee-saved register and cached
membership/mask schedule. C/C++ language, local names, register hints, packed-byte
caches, signedness and optimization probes have not reproduced it.

The legacy emitter now retains converted height on x87 and uses separate calls
for conditional culling extents. That reaches the required 672-byte section but
still has 44 relocations against the original 43, plus register/scheduling
mismatches. The previous extra sixteen bytes are no longer the blocker.

The culler now initializes its loop counter before copying the base point.
Its remaining ten bytes encode early instruction scheduling and swapped EBX/EBP
retention of base Y/Z. Moving pair setup, arithmetic promotion, local names,
declaration order, explicit scalar copies and vector-pointer/memcpy forms did not
resolve this. Keep the full switch table and all alignment in the comparison.
Before registry promotion, report embedded table bytes separately from alignment
in aggregate coverage accounting; current accepted functions have no such table.

See [emission evidence](../../docs/surface-emission-evidence.md) and
[culling evidence](../../docs/scan-culling-evidence.md). Continue this batch before
a full original-toolchain acceptance checkpoint. A behavior pass cannot resolve
a byte mismatch.
