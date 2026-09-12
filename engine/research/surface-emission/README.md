# Surface-emission research batch

Both functions are reported not done in bp/bapdraw.c. Neither is accepted by
the matching registry. The existing fourteen-function baseline remains intact.

| Address | Team name | Candidate | Current comparison |
|---|---|---|---|
| 0x00406300 | bapdraw_emitFace | [C++ source](candidate.cpp) | 432-byte section, but relocation positions and register allocation differ |
| 0x00405A50 | bapdrawOld_emitFace | [C source](legacy_candidate.c) | 688-byte section versus required 672 bytes; instruction differences remain |

The JSON target definitions preserve complete original extents, padding,
constant widths, symbol addends and calls. They are pending specifications;
they do not extend engine/target.json or claim accepted matches.

## Focused commands

From the repository root, using the existing private compiler configuration:

    python engine/research/surface-emission/probe.py compare --reference C:/PrivateGame/WMAIN.EXE
    python engine/research/surface-emission/probe.py behavior

The compare command compiles only these two candidates and uses the existing
strict comparator. It returns 1 for nonmatching candidates, 2 for prerequisites
or build failures, and 0 only if both complete spans agree. The behavior command
also compiles the previously matched bucket helper and the synthetic fixture.
It requires the locked Docker runner and does not read or execute the game.
All commands, source hashes and results stay in a fresh private build directory.
No command creates an accepted full-build history record.

The focused fixture passed with the original compiler on 2026-09-12. It covers
all 256 packed-opacity values, room-mask combinations, gates and duplicate
marking, new-path fade clamping, legacy sixteen-step integer fades, height
updates, and culling-call interactions. The culler is a fixture stub; its actual
implementation and complete game behavior are not certified. A behavior pass
does not resolve either byte mismatch.

## Remaining compiler work

For the newer emitter, plain C/C++, explicit byte caches, readable local-name
variants, register hints and /Ox probes have not reproduced the original extra
callee-saved register and cached membership/mask schedule. Keep the original
432-byte extent and all operands unchanged.

For the legacy emitter, height retention, explicit vector intermediates and
separate culling-extent assignments have not reduced the candidate to the
required complete 672-byte span. The original retains converted height on x87
across the update; the current candidate reloads it. The conditional float
argument and mask loads also differ. Do not use padding changes to hide this.

See [static evidence](../../docs/surface-emission-evidence.md). Continue this
batch before a full original-toolchain checkpoint. The preserved candidates
compile without warnings and pass only the stated focused behavior tests.
