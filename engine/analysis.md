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

## Campaign 004: hierarchy and collision-query continuation

The first overnight batch preserves the model-hierarchy and surface-below query
as research candidates. Hierarchy has a complete 144-byte span and all five
operands verified, but eight register-selection bytes differ (5.556%). Its 17
focused checks cover traversal order, root versus child amputation, hidden meshes,
ABI offsets and callback changes. The surface query passes 53 focused checks but
has a 229-byte defined implementation versus the 227-byte original, so no mismatch
percentage is assigned. Static comparison with the adjacent reported-done above
query resolves the selected function's alias. Neither result is accepted.

Direct recursion now has a narrow verified COFF path: the declared call must
resolve to the exact defined symbol and original entry of the function currently
compared. Complete spans, all fixups and every other byte remain mandatory.
Synthetic failure tests and an independent review found no material issues.
See research/campaign-004 for final batch receipts and remaining work.

The nearby collector at `0x0040D03E` now has a defined 555-byte C candidate and
81 passing focused assertions. Its original span is also 555 bytes, but eleven
of 25 operand positions differ. Fourteen align, ten are one byte early and one
is two bytes early; no percentage is assigned. Independent caller/callee review
corrected the square enumerator ABI before final verification. All three cases
remain research candidates, with no new exact match or accepted-history entry.

## Campaign 005: small music helpers

Ten independently authored VC5 C helpers now match all 413 bytes of their complete
original intervals after verifying every address operand. There are no alignment
exclusions. All 205 focused assertions pass, with native/Docker function bytes and
fixups agreeing for every case. These are function-level research matches awaiting
the accepted-history batch checkpoint; final linked executable identity is not
established. See [the campaign results](research/campaign-005/results.md).

The getters distinguish requested state (sentinel1000) from requested sequence
(sentinel2000), resolving a duplicated team alias. Disable copies requested IDs
after writing the option; enable reloads cached IDs after earlier callbacks and
ignores callback failures. Save/read handle only the paired cached IDs, not full
playback state. Periodic service gates on initialized exactly equal to1, whereas
pause is unconditional and writes its flag after unlocking. Fixtures exercise
these callback-visible timing details with authored stubs; no original executable
or imported backend was executed.

## Campaign 006: music lifecycle and sound helpers

Ten additional C functions match603 complete bytes after all52 address operands
are verified, with258 focused assertions passing. State caching occurs before
lock while volume caching and detach initialization clearing occur after unlock.
The sound enable/disable guards distinguish zero from exactly one. Placement
records use a60-byte stride, whereas indexed sound descriptors use64 bytes;
stop-all releases twelve slots. These complete matches remain separate from
accepted history and final executable layout. One fixture snapshot overread was
corrected before execution; no candidate correction was needed. See
[the campaign results](research/campaign-006/results.md).

## Campaign 007: channels and placement lifecycle

Nine additional C functions match716 complete bytes after34 verified address
operands, and the70-byte position-copy candidate remains research with13 differing
register-setup bytes. All164 focused checks pass across the ten cases. Release,
pause and resume preserve callback-visible state timing. The placement-reset loop
intentionally never advances its first-record pointer. Activation retains the
pinned x87 equal/unordered behavior and late reloads after random generation.
Three fixture-only review corrections preceded execution; no candidate semantic
correction was needed. A Ghidra auto-this convention correction remains pending
for activation despite independently established one-stack-argument source ABI.
See [campaign results](research/campaign-007/results.md).

## Campaign 008: sound accessors and lifecycle

Eight team-not-done functions reproduce900 complete bytes after69 verified address
fixups, including48 total generated switch-table bytes. All454 authored
fixture checks pass on final frozen native/Docker pairs. See
[results](research/campaign-008/results.md) and [review](research/campaign-008/review.json).
No byte exclusions, warning waivers, target changes or accepted-history promotion.
Metadata naming/comments are saved; previously inferred ECX/this parameters may
remain in decompiler output because of the known calling-convention bridge limit.
Worker timestamp audit distinguishes observed file/report times from unmeasured
task duration. Original game static only; full regression deferred.

## Campaign 009: initialization, resources and voice

Six team-not-done C functions reproduce 1,025 complete bytes after 48 verified
address fixups; 583 authored checks pass. The source records VC5 memset/strcpy
intrinsic directives. The master-volume clamp preserves unordered NaN-to-zero;
cleanup captures next before free and preserves callback-visible live state.
Five fixtures were strengthened before final verification; one strict VC5 fixture
compile failure was corrected without a waiver and retained in the experiment
ledger. Five final Docker runs used the verified listing fallback; one emitted
its own valid listing. Four names and six comments are saved in Ghidra, with
existing inferred convention limitations documented. See
[results](research/campaign-009/results.md) and [review](research/campaign-009/review.json).
No byte exclusions or accepted-history promotion; original game static only.

## Campaign 010: sound lifecycle checkpoint

Three full function matches (671 bytes), two reviewed near matches and one
research candidate; all 436 focused checks pass. Five generated names and six
comments were saved in Ghidra, preserving the existing channel-release name.
Independent review corrected the register-level preload gate before final
verification. See [complete results](research/campaign-010/results.md) for
source ABI, callback state, listing provenance and unresolved layout differences.

## Campaign 011: lighting and transform-state checkpoint

10 complete exact functions (467 bytes), 0 near and 1 research;
209 focused assertions pass. Full rendering alignment is retained.
See [results](research/campaign-011/results.md) for independent source review,
copy/alias behavior, Ghidra metadata and remaining limitations.

## Campaign 012: transform operation wrappers

10 complete exact functions (520 bytes), 0 near and 1 research;
257 focused assertions pass. Original full spans are retained.
See [results](research/campaign-012/results.md) for independent source review,
copy and floating-point behavior, Ghidra metadata and remaining limits.

## Campaign 013: body layout and final candidates

Three verified bodies total130 bytes but retain35 unresolved original INT3 padding
bytes in their full targets; no new full exact matches. Sequence is8/190bytes
different, reference-axis61/245;526 focused checks and10 evidence checks pass.
See [results](research/campaign-013/results.md) for unchanged full targets,
independent body reproduction, callback/unsigned/floating behavior and limits.

## Acceptance002 and Campaign017

The shared colormap load/free integration raised accepted registered functions to23
through immutable record22, after the full original-toolchain build and321 shared
checks. Full CI passed. Campaign017 then reconstructed six unfinished world
reader/cleanup functions,517 full bytes and281 focused checks, with no source
rewrites. See [acceptance](research/acceptance-002/results.md) and
[campaign results](research/campaign-017/results.md). World/header integration and
signed stream callback types remain pending; the bridge's stale chunk-skip
thiscall metadata is explicitly documented and is not the source ABI oracle.

## Acceptance003 and Campaign018

Three canonical world readers raised registered acceptance to26 functions through
immutable event23, with189 shared checks and full tooling CI. Campaign018 adds
six exact cleanup/wrapper functions,639 complete bytes and241 focused checks.
Their original bodies needed no corrections; callback oracles were strengthened.
See [acceptance003](research/acceptance-003/results.md) and
[Campaign018](research/campaign-018/results.md). Table620 capacity and final
canonical cleanup storage remain explicit integration questions.

## Campaign019

Two exact vertex readers add301 focused bytes; three readers have six remaining
stack-slot differences each, and dialog has eight (above5%). All627 focused
checks pass. Existing names were retained and Ghidra comments saved.
[Results](research/campaign-019/results.md) preserve categories, failed fixture
compilation and every final source/toolchain receipt. Acceptance004 will cover
only the two exact readers, with the original local mismatch-success behavior.

## Acceptance004

The exact global/local vertex readers now share canonical types with the accepted
world readers. All28 registered spans remain exact, and488 combined world-reader
checks pass with immutable record24 and successful full CI. The original local
payload-mismatch success return is preserved. See
[acceptance004](research/acceptance-004/results.md).

## Campaign020

Six world lifecycle/control functions add856 exact focused bytes and2058 authored
checks. Typed allocation fields preserve exact code; callback order and x87 edge
fixtures have independent review. [Results](research/campaign-020/results.md)
include the failed fixture compile and source-domain limits. Registered acceptance
remains28 functions until the next canonical world-controls checkpoint.

## Campaign021

Five functions match745 complete bytes; the material-run function remains9/248 differing bytes, unaccepted. All354 focused checks pass. Static analysis identified temporary-only actor-name normalization and grid/cell preprocessing. See research/campaign-021/results.md. Shared acceptance remains31 functions/25 records.

## Acceptance008 and Campaign023

The chunk-header/skip integration raises registered acceptance to 43 functions with immutable event 29 and successful full integration CI. The new shared fixture passes 7,066 checks. Campaign023 adds six exact shield lifecycle research functions, 486 complete bytes and 4,094 focused checks. The active word at record+0 is now distinguished from the attached actor pointer at+0x10; older accepted shield views will be reconciled at a canonical integration checkpoint. See [acceptance008](research/acceptance-008/results.md) and [Campaign023](research/campaign-023/results.md).

## Campaign024

Seven exact fog/fade/shield helpers add416 bytes and the whole focused batch passes1,357 checks. The offset setter remains an83-versus80-byte research candidate with unresolved return intent. Existing Ghidra context is preserved; original execution remains prohibited. See [Campaign024](research/campaign-024/results.md).

## Campaign025 effects lifecycle

Six additional team-not_done functions match 871 full function bytes under the locked VC5 toolchain, with all relocation operands resolved and 7,829 authored checks passing. Shield destruction preserves ordered callbacks and subsequent clears. Timed fog and tint retain observed x87 and callback ordering. Fog RGB arguments are full integers followed by masks; the stale Ghidra byte-parameter signature was corrected. Fade-reset lifecycle provenance remains unresolved. See research/campaign-025/results.md. This is function-level evidence, not final executable identity.

## Campaign026 letterbox and halo lifecycle

Seven untouched functions match 455 complete bytes; 9,954 focused checks cover all eight candidates. Halo resource setup, shutdown and per-object detach have evidence-backed identities. Serializer fixtures check the full 28-byte block and callback mutations; object rendering reloads flags and the signed shield slot after halo drawing. Overlay restore remains research because its eight-byte alignment tail is unresolved. See [Campaign026](research/campaign-026/results.md).

## Campaign027 zapline and halo cleanup

Six untouched functions match461 complete bytes with23,949 focused checks. The active/mode endpoint query and active endpoint detach share a64-record,120-byte pool; resource lifecycle and full reset contracts are independently reviewed. Halo free-all body behavior is verified while its caller provenance remains unresolved. See [Campaign027](research/campaign-027/results.md). Registered acceptance remains69 functions until the next combined checkpoint.

## Campaign028 load and render helpers

Seven functions match658 complete bytes; all41,267 focused checks pass. The batch covers staged effects loading, object halo drawing, ripple material construction, two zapline wrappers and object lifecycle/ambient bridges. Ripple reset remains research due to its two-byte alignment tail. See [Campaign028](research/campaign-028/results.md). All13 exact C27/C28 candidates await the next combined acceptance checkpoint.

## Campaign029 save and attachment dependencies

Seven functions match1,324 complete bytes;50,485 focused checks cover all nine candidates. The save writer retains the authentic shield count/emission discrepancy and live callback behavior. Halo attachment uses authored external-name fixtures. Six identities and nine signatures were recovered from static call relationships and typed C. Restore/relink remain research due to full-span alignment tails. See [Campaign029](research/campaign-029/results.md).

## Six-hour window ending 2026-09-13 22:34 UTC

Campaigns023-034 produced69 new exact functions covering6,996 complete bytes. Acceptance008-016 and the final replay raise the shared baseline from40 to105 functions, with38 immutable records. All105 full spans and shared authored fixtures pass at39af9501bbaf0c05728dbead492509a5109a78b8. Seven exact functions await canonical integration; one near match and six other candidates remain research. The matcher, zero-difference threshold and public-content boundary are unchanged. Whole-executable identity remains pending. See [window results](research/extended-2026-09-13-1634/results.md) for the audit and next-work queue.

## Overnight continuation through acceptance032

The ten-hour window beginning2026-09-14 01:40:41 UTC has added19 newly exact functions covering1,203 complete bytes and accepted three exact material functions carried into the window. The baseline is172 functions at event55, with all full spans, shared authored behavior and same-source supplemental CI passing. Campaigns041-044 connect colormap lifecycle, lights, canvas/face allocation and affine matrix helpers. Sixteen newly exact addresses are team-not_done; three are team-reported_done. The canvas-inner tail and two palette-table builders remain research. No function is accepted with a byte mismatch; whole-executable identity remains pending. See [current window tracking](research/extended-2026-09-14-0140/status.json), [matrix acceptance](research/acceptance-032/results.md), and [palette research](research/campaign-045/results.md).

## Colormap reader and geometry checkpoint

Acceptance033 connects the real colormap reader and writer plus point-plane distance and vector angle, reaching176 accepted functions at event56. All176 complete spans and shared authored tests pass on clean original-toolchain source, with same-source supplemental CI. The current overnight window has23 newly exact functions (20 team-not_done,3 team-reported_done),2,199 complete bytes and26 promotions including3 carried-in cases. The real reader returns success after gray fallback even when file open fails; this behavior is now reflected in the connected lifecycle and extended-list fixtures. Authored static-pool tests do not establish real file/heap behavior or whole-executable identity. See [acceptance033](research/acceptance-033/results.md).

## Palette resource and face-normal checkpoint

Acceptance034 reaches182 accepted functions at event57 with six complete-span matches and real palette dependency-chain fixtures. All182 original-toolchain comparisons and shared authored tests pass; same-source supplemental CI passes with its historical-CRT compatibility setting documented. This overnight window has29 newly exact functions (26 team-not_done,3 team-reported_done),2,645 complete bytes and32 promotions including3 carried-in cases. The Euler transform and palette release alignment tail remain research. Whole-executable identity remains pending. See [acceptance034](research/acceptance-034/results.md).

## Save stream checkpoint

Acceptance035 reaches188 accepted functions at event58. All188 original-toolchain comparisons and shared authored tests pass with same-source supplemental CI. Six new save functions connect actual effects/overlay read/write chains and pass640,808 stream checks, including2,485 legacy reads and684 writes. Existing182 definitions remain unchanged after accurate save ABI declarations and explicit distinct CRT reader/writer symbols. This overnight window has35 newly exact functions,2,909 bytes and38 accepted promotions including3 carried in. Save-slot functions are next; whole-executable identity remains pending. See [acceptance035](research/acceptance-035/results.md).

## Save slot checkpoint

Acceptance036 reaches193 accepted functions at event59. All193 comparisons and shared authored tests pass with same-source supplemental CI. Five exact save-slot helpers preserve prior188 definitions and pass17,850 connected path checks plus2,028 guarded label-cleanup checks. This overnight window has40 newly exact functions,3,133 bytes and43 accepted promotions including3 carried in. Manifest-name conflicts are documented by numeric address. Disk-space checking remains research; directory enumeration is next. Whole-executable identity remains pending. See [acceptance036](research/acceptance-036/results.md).

## Directory enumeration checkpoint

Acceptance037 reaches 197 accepted functions at event60. All 197 comparisons and shared authored tests pass with same-source supplemental CI. Four exact directory helpers preserve prior 193 definitions and pass 77,430 focused checks plus 558 connected lifecycle checks. The overnight window now has 44 newly exact functions, 3,640 bytes and 47 accepted promotions including three carried in. One-shot enumeration remains layout research; the next module cohort has matched first compile and awaits focused verification. Whole-executable identity remains pending. See [acceptance037](research/acceptance-037/results.md).

## Module lookup and removal checkpoint

Acceptance038 reaches 203 accepted functions at event61. All 203 comparisons and shared authored tests pass with same-source supplemental CI. Six exact module helpers preserve prior 197 definitions and pass 560,080 connected checks plus 85,312 retained focused checks. The overnight window has 50 newly exact functions, 4,125 bytes and 53 accepted promotions including three carried in. Ghidra module comments and two configuration ABI corrections were saved. Configuration-stream reconstruction is next; whole-executable identity remains pending. See [acceptance038](research/acceptance-038/results.md).

## Configuration stream checkpoint

Acceptance039 reaches 209 accepted functions at event62. All 209 comparisons and shared authored tests pass with same-source supplemental CI. Six exact configuration helpers preserve prior 203 definitions and pass 442,608 focused checks plus 20,400 connected checks. The overnight window has 56 newly exact functions, 4,461 bytes and 59 accepted promotions including three carried in. Integer rectangle utilities and checksum layout research are next; whole-executable identity remains pending. See [acceptance039](research/acceptance-039/results.md).

## Utility checkpoint

Acceptance040 reaches216 accepted functions at event63. All216 comparisons and shared tests pass, including247,920 retained focused checks and 3296 connected checks, with same-source supplemental CI. Seven utilities add657 complete bytes with six verified operands. The overnight window has63 newly exact functions across5,118 bytes and66 accepted promotions including three carried in. See [acceptance040](research/acceptance-040/results.md). Whole-executable identity remains pending.

## Lexical and window checkpoint

Acceptance041 reaches225 accepted functions at event64. All225 comparisons and shared tests pass, including1,886,672 retained focused checks and34,992 connected window checks, with successful same-source supplemental CI. Nine functions add864 complete bytes with18 verified operands. The overnight window has72 newly exact functions across5,982 bytes and75 accepted promotions including three carried in. See [acceptance041](research/acceptance-041/results.md). Whole-executable identity remains pending.

## Platform and math checkpoint

Acceptance042 reaches238 accepted functions at event65, with13 promotions matching405 complete bytes and29 operands. All238 comparisons and shared tests pass, including56,149 retained focused and16,214 connected checks; same-source supplemental CI succeeds after a documented modern cosine-oracle correction. The window has85 newly exact functions across6,387 bytes and88 accepted promotions including three carried in. See [acceptance042](research/acceptance-042/results.md). Whole-executable identity remains pending.

## Length and audio checkpoint

Acceptance043 reaches252 accepted functions at event66, with14 promotions matching1062 complete bytes and24 operands. All252 comparisons and shared tests pass, including402,348 retained focused checks and 22956 connected checks; same-source supplemental CI succeeds. This window has99 newly exact functions across7,449 bytes and102 promotions including three carried in. See [acceptance043](research/acceptance-043/results.md). Whole-executable identity remains pending.

## Kernel window checkpoint

Acceptance044 reaches261 accepted functions at event67, with9 promotions matching520 complete bytes and34 operands. All261 comparisons and shared tests pass, including570,560 retained focused and 24448 connected checks; same-source supplemental CI succeeds. This window has108 newly exact functions across7,969 bytes and111 promotions including three carried in. See [acceptance044](research/acceptance-044/results.md). Whole-executable identity remains pending.

## Video lifecycle checkpoint

Acceptance 045 reaches 264 accepted functions at event 68, with three promotions matching 322 complete bytes and 16 operands. All 264 comparisons and shared tests pass, including 69,936 retained focused and 100224 connected checks; same-source supplemental CI succeeds. This window has 111 newly exact functions across 8,291 bytes and 114 promotions including three carried in. See [acceptance 045](research/acceptance-045/results.md). Whole-executable identity remains pending.

## Final debug and provenance checkpoint

Acceptance 046 reaches 267 accepted functions at event 69. Three new debug wrappers match 203 complete bytes and 19 operands. All 267 comparisons and shared tests pass, including 62,694 focused and 93312 connected debug checks; same-source supplemental CI succeeds. All 461 source inputs match their recorded Git blobs after correcting two historical local line-ending discrepancies. This ten-hour window has 114 newly exact functions across 8,494 bytes and 117 promotions including three carried in. See [acceptance 046](research/acceptance-046/results.md). Whole-executable identity remains pending.

## Next-session save-header ABI note

Static follow-up on451BC2 found that its name helper46F639 looks up a module ID, then strncpy-copies node+20 into the caller buffer. It takes three stack arguments; entry PUSH ECX is local allocation, and the caller cleans12 bytes. Ghidra previously displayed a misleading four-argument thiscall form. The bridge rejected an explicit cdecl prototype but accepted the corrected three-argument signature; re-decompilation now shows the right argument flow with an unresolved calling-convention warning. An evidence comment is saved, no function was renamed, and no candidate was compiled. Missing-module lookup leaves the name region untouched, while the header writer ignores the helper return; future fixture domains must account for that. See [next-work](research/extended-2026-09-14-0140/next-work.json).

Re-decompiling the save-header caller still injects an extra receiver-like argument despite the corrected helper body. This metadata inconsistency remains unresolved and is explicit in next-work.json; the observed three-stack-argument contract is the authority. No source acceptance or test result depends on this preliminary metadata.

## September 14 one-hour continuation

Acceptance 047 reaches 273 accepted functions at event 70: six module dispatchers
add 900 complete bytes and eleven verified operands. All shared comparisons and
authored tests pass, and the same-source supplemental CI succeeds. One additional
193-byte start helper is focused-exact and awaits the next batch; shutdown remains
research at 193 candidate bytes versus a 198-byte full target. The latest contributor
snapshot now has 265 of its 943 unfinished functions locally accepted, leaving 678
assuming the reported-done entries are correct. See the [window report](research/extended-2026-09-14-1325/results.md)
and [module queue](research/extended-2026-09-14-1325/module-queue.json). No original
game execution or whole-executable identity is claimed.
