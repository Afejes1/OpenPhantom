# Matching reconstruction analysis

The authoritative analysis program is TPM:/WMAIN.EXE in the open Ghidra project.
The original is the hash-pinned 829,952-byte PE32 x86 retail image in target.json,
image base 0x00400000, entry 0x0049CB80. It is inspected statically, never executed
by the matching workflow. The built WMAIN.EXE in each private build directory
is a synthetic console test fixture, not a reconstructed retail application.

The compiler baseline is VC5 RTM 11.00.7022 with linker 5.00.7022. Per-function
optimization follows observed compilation, with /MT and strict warning checks.
The original runtime startup/heap library revision remains unresolved.

## Current subsystem focus

The baseline covers a switch helper, spatial-grid address arithmetic, a plane
coordinate solver, and camera projection setup. View/projection expansion follows
the call graph: mesh submission prepares vertex indices, transforms/projects
vertices, reduces per-vertex clip codes per face, rejects trivially clipped
faces, and passes surviving faces to downstream clipping/queue work.

The face clip reducer has no imported calls, globals, or strings. Its behavior
is supported by the body, its mesh caller, synthetic behavior cases, and complete
VC5 code-section comparison. [function-map.md](function-map.md) records the
analysis metadata change; [module evidence](docs/view-projection-evidence.md)
records observed layouts and unresolved module boundaries.

The mesh-submission group now includes the matched active-Thing/opacity setup
helper. Its projectile caller and ordinary actor sibling identify the shared
render context used by mesh submission. See [mesh evidence](docs/mesh-submission-evidence.md).

The render-queue group connects projected vertices to material-bucket submission.
Availability helpers do not reserve storage; successful face submission advances
the packet counter, while a separate fog/vertex commit advances the vertex count.
Material submission preserves cached lookup, FIFO ordering, and partial writes on
failure. See [render queue evidence](docs/render-queue-evidence.md).

## Outstanding architecture work

The transform/project kernel now matches with documented inline assembly and
a C4725 compiler-advisory waiver. Follow its downstream clip/queue consumers. Recover original translation-unit boundaries, source/link order,
resources, runtime initialization, imports, data construction, and historical
PE metadata before claiming the complete original binary. Registered function
counts do not establish total application coverage or equivalence of the modern
TPM engine. The generated [progress report](docs/progress.md) is authoritative
for recorded matching results.

## Gathered surface wrapper

The 0x00406830 wrapper submits nonempty surface buckets through 0x0041B070 and
retains its return value. It preserves a previous result on an empty queue.
See [evidence](docs/gathered-submission-evidence.md). The team manifest lists
this in bp/bapdraw.c as not done; their labels guide scheduling separately from
our own comparison records.

## Model frame initialization

The 0x00401D30 initializer connects the matched projection and queue routines
to frame flags and software fog. It writes reciprocals and range before rejecting
negative or unordered ranges; zero range stays enabled. The extra word at
0x004DD6C8 is a render-flag mask, confirmed by its downstream OR operation.
See [frame evidence](docs/frame-state-evidence.md).

## Legacy surface bucket insertion

The 0x004059F0 helper chooses the ordinary texture bucket or the extra bucket
indexed by world.texture_count, preserving or expanding opacity before prepending
the current entry. Callers fill and reserve entries. Its matching reconstruction
uses the original VC5 C++ frontend, recorded separately from historical-language
claims. See [surface-entry evidence](docs/surface-entry-evidence.md).

## Legacy camera-space scan

The 0x00403EF0 helper advances the transformed scan position by the camera
first two basis vectors. Only exact signed unit directions act, with separate
float stores between axes. Setup copies the camera transform and the world
renderer combines affine initialization with these incremental steps.
See [scan-step evidence](docs/scan-step-evidence.md).

## Surface emission paths under reconstruction

The two emitters share authored opacity and bucket storage but retain distinct
fade and uniqueness-marking rules. Current emission uses a float time fraction;
legacy scan emission uses a sixteen-step integer counter and updates camera-space
height before culling. Both candidates are unverified for byte equality and are
excluded from accepted counts. See [evidence](docs/surface-emission-evidence.md).

## Legacy scan-plane culling under reconstruction

The legacy scan culler at 0x00403FA0 tests two table-selected corners after a
height/basis offset, rejects nonpositive or unordered camera depth, and invokes
a scalar projection callback through camera+0x4C. Four boundary comparisons
short-circuit on a passing corner. All four scalar callback variants were
reviewed to establish the ABI and projected depth coordinate. The candidate's
480-byte extent and checked internal switch-table destinations agree, but ten
instruction bytes differ. Its focused fixture passes; it remains outside the
accepted registry. See [scan-culling evidence](docs/scan-culling-evidence.md).

## Static-cell collection under reconstruction

The ordinary cell collector at 0x004064B0 processes 64-byte records after a
16-byte cell header, using extra-record room/frustum skips and a last-height
cache before current-path emission. Capacity is checked only at function entry.
The four-candidate research batch now includes this C++ reconstruction: its
896-byte extent is correct, but relocation positions still differ. Its focused
VC5 fixture passes 4,184 checks. See [static-cell evidence](docs/static-cell-evidence.md).
The culler's additional processor/optimizer, arithmetic, inline-vector and
indexing probes retained its ten-byte mismatch; no new compiler profile was
accepted. The fourteen-function history remains unchanged.
