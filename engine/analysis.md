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
480-byte extent and checked internal switch-table destinations agree, but six
instruction bytes differ. Its focused fixture passes; it remains outside the
accepted registry. See [scan-culling evidence](docs/scan-culling-evidence.md).

## Static-cell collection under reconstruction

The ordinary cell collector at 0x004064B0 processes 64-byte records after a
16-byte cell header, using extra-record room/frustum skips and a last-height
cache before current-path emission. Capacity is checked only at function entry.
The C++ candidate now has the correct 896-byte extent and all forty relocation
operands verified. Five instruction bytes still differ: stack cleanup and one
x87 comparison trade places. Its focused VC5 fixture passes 4,208 checks,
including opposite-infinity cases that exposed and corrected an unordered-bound
discrepancy. See [static-cell evidence](docs/static-cell-evidence.md).
The culler's additional processor/optimizer, arithmetic, inline-vector and
indexing probes did not resolve scheduling. A float-array base copy reduces
its mismatch to six bytes; no new compiler profile was accepted. The fourteen-function history remains unchanged.

## Legacy-cell collection under reconstruction

The legacy collector at 0x004056C0 first traverses three optional header group
indexes, then discovers active groups through eligible inline records. It reserves
entries after the bucket helper, preserving callback-visible count and pointer
reloads. Slab culling follows those group phases. Accepted cells publish scan
position and zero cached height, then emit normal records or unseen references.
A null unseen reference still increments an integer visit counter. The authored
C++ reconstruction matches the full 816-byte span with all 34 address operands
verified and no byte exclusions. Its focused fixture passes 1,195 checks; the
match awaits the shared batch acceptance checkpoint. See
[legacy-cell evidence](docs/legacy-cell-evidence.md). No new function is accepted.

## Material bucket flush

The no-argument flush at 0x00402155 consumes the accepted material/packet queues,
builds 32-byte vertices with normalized UVs and per-corner color/fog, then dispatches
immediate or queued alpha draws before clearing queue state. Its authored VC5 C
body matches all 1,112 bytes with 24 verified operands and passes 5,480 focused
checks. The full 1,115-byte span remains unmatched because three trailing
alignment bytes need verified layout reproduction. No accepted count changes.
See [queue-flush evidence](docs/queue-flush-evidence.md).

## Bounded parallel campaign: frame setup and mover gathering

Frame setup at 0x00404180 copies camera state, derives octant/scan limits and
fills the offset table. Mover gathering at 0x00405CF0 expands seed and mover
cells, applies opacity/bucket rules, then visits animation maps/direct movers.
Two worker candidates received independent coordinator review and focused VC5
Docker verification (6,548 and 36 assertions). Native/Docker output agrees at
the documented artifact scope. At that checkpoint, complete binding inventories
remained pending and no new exact match or accepted record was claimed. See the
[campaign results](research/campaign-001/results.md) for findings, timings and
source hashes. Whole-code regression was deferred under the batch cadence.

## Campaign 002 comparison readiness

The frame-setup original inventory now accounts for all 144 absolute operands
and nine direct calls. Independent review corrected a biased table pointer to
base 0x004AA080 plus addend 16. The enabled strict comparison confirms an extent
mismatch (1,376 candidate bytes versus 1,456 original bytes); it cannot yet score
candidate operands or byte similarity. The focused fixture still passes 6,548
assertions. See [frame evidence](research/campaign-002/frame-binding-evidence.md).

The fork's [campaign rules](docs/campaign-rules.md) separate exact functions,
body-only matches, measured near matches and research candidates. Historical
objects revalidated for scheduling show six differing culler bytes (1.25%) and
five static-collector bytes (0.558%); those remain research. The existing 816-byte
legacy-cell match still awaits a shared acceptance checkpoint. These are not
new matches produced by this campaign.

The scan-line reset at `0x004084B8` now has a reviewed C reconstruction: all 400 bytes match after resolving its single tracked-allocator call. The record is 80 bytes; existing records retain the unused final word, and allocation failure returns null without writes. All 238 focused assertions pass in locked Docker. This is a focused exact result pending the existing batch acceptance checkpoint, with no whole-executable or overflow-input claim. See [the receipt](research/campaign-002-line-reset/result.json).

The segment predicate at `0x00408648` rejects same-side endpoint pairs against each supporting line. It intentionally has no collinear bounding-box test: disjoint collinear segments and even distinct point segments return true. Readable VC5 C matches all 237 raw bytes without relocations, and 6,601 focused assertions pass against explicit cases and a bounded determinant oracle. See [the receipt](research/campaign-002-segment-test/result.json).

## Campaign 003 related helpers

The line advance at `0x004083D0` has a complete 209-byte raw match. It always advances the record and returns true only when the updated index is strictly greater than count. The 23-byte release wrapper at `0x004084A1` matches after resolving its tracked-deallocator call. Final focused fixtures pass 27 and 9 assertions respectively. An explicit verifier option handles an empty Docker listing using a complete native peer listing only after matching source/toolchain provenance and function bytes/fixups; the empty original artifact remains recorded. See the [campaign review](research/campaign-003/review.json).

The polyline strip at `0x00408735` now has a complete 478-byte comparison with all seven operands verified. Eight bytes differ in register-setup instruction ordering before the first aggregate copy (1.674%); 35 focused checks pass. It remains a near match for later work. The original tests mapped depth, not camera depth; projection-callee evidence corrected the older Ghidra comment.

The green diagnostic segment wrapper at `0x00408913` has a verified 76-byte body match after resolving its polyline call. One original INT3 alignment byte remains pending, so the full 77-byte target still fails. All 26 focused checks pass, including exact signed-zero/NaN bit copies and aliased endpoints. Body evidence remains separate from complete-function acceptance.
