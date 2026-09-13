# Campaign 003 results

Four related functions received readable C implementations, independent review
and focused original-toolchain verification. **Two are new complete-span exact
matches, totaling 232 bytes.** One is a measured near match; one has an exact
body with alignment pending. Accepted history remains fourteen functions and
nineteen chained records, pending the existing batch acceptance checkpoint.

| Function | Full-span result | Focused Docker checks |
| --- | --- | --- |
| Line advance, `0x004083D0` | All 209 raw bytes match | 27 passed |
| Line release, `0x004084A1` | All 23 bytes match after resolving the tracked-deallocator call | 9 passed |
| Polyline strip, `0x00408735` | 8/478 bytes differ (1.674%); all seven operands verified | 35 passed |
| Diagnostic segment, `0x00408913` | Complete 76-byte body matches after resolving its call; one original INT3 alignment byte remains pending | 26 passed |

The diagnostic wrapper's unchanged full target is 77 bytes, so it still fails
strict comparison and receives no percentage. Its body diagnostic follows the
existing queue-flush method and cannot make the full-span result pass. The
polyline's eight differing bytes are the ordering of two register-setup
instructions before its first aggregate copy. It remains research under the
5% scheduling rule, not an exact or accepted function.

All four use the pinned original VC5 C frontend with `/Od /MT`, and all final
native/Docker function and relocation inventories agree. Native operation
compiled only. Authored fixtures executed only in the locked Docker runner;
the original game and its allocator/renderer callbacks were never executed.

## Quality and the larger work queue

One Sol worker at medium reasoning processed related cases sequentially. It
received the first two helpers as a batch while the coordinator prepared the
drawing functions, then received those two as separate follow-up assignments.
Per-function limits and named-file ownership remained in effect. No concurrent
implementation workers or automatic next campaign were started.

The two small helpers matched their first source variants. Independent review
added a below-count boundary case to the advance fixture, corrected an older
Ghidra comment to distinguish mapped depth from camera depth, and preserved the
original polyline behavior for NaN/nonpositive depth, signed widths and callback
ordering. The diagnostic fixture checks exact signed-zero/NaN payload copies
and aliased endpoints. Ninety-seven assertions are selected checks, not ninety-
seven game scenarios or proof of integrated renderer behavior.

The polyline reached its ten-compile cap. Two malformed editing attempts are
recorded rather than hidden; a proposed memcpy experiment never compiled.
The known alignment boundary on the diagnostic wrapper was left untouched.
No semantic source rewrite was required at final handoff. See [review](review.json)
and [experiment metrics](experiment-metrics.json) for findings and failed attempts.

## Empty Docker listings

The tiny release and diagnostic wrappers produced valid COFF objects but empty
Docker assembly listings. The existing default verifier rejected these runs.
A new explicit `--native-listing-fallback` requires a completed native report
with identical source and locked-toolchain provenance. Before fixture execution,
it checks complete function bytes and relocations and requires a complete native
listing. It retains the empty Docker listing and stores a separately named copy
of the native listing with hashes and clear provenance. Final integrity checks
cover the peer, copied listing, original empty artifact, objects and source.

This is an artifact-handling exception, not a byte exclusion or an original-
toolchain substitution. The underlying listing-flush cause was not established.
Nineteen focused runner tests and eleven existing artifact tests pass. The worker
independently reviewed the coordinator's change and found no material issues;
minor test-coverage limits are recorded in the review. The two real Docker
fixture runs also exercised the fallback successfully.

## Time, usage and publication

The campaign began at 02:06:18 UTC on 2026-09-13. All final candidate verification
finished at 02:36:05 UTC, about thirty minutes later. Reporting/publication follows
inside the 45-minute campaign window. The worker made eighteen compiler
invocations across eleven source variants, including failed attempts and
verification repeats. The coordinator made eight final native/Docker compiles.
A separate pre-compiler file-preparation failure is retained in the worker log.

| Final case | Native candidate compile | Docker candidate compile | Complete Docker run |
| --- | ---: | ---: | ---: |
| step_scan_line | 0.060 s | 1.115 s | 6.157 s |
| free_scan_line | 0.055 s | 1.117 s | 5.357 s |
| submit_polyline | 0.056 s | 1.132 s | 5.489 s |
| draw_diagnostic_segment | 0.059 s | 1.222 s | 5.705 s |

These are individual observations, with some independent verifications run
concurrently; they are not controlled model or virtualization benchmarks. Compiler
execution remains a small part of elapsed time. The weekly account meter moved
from 23% to 24% used; this rounded shared reading is not exact campaign billing.

Source, fixtures, target packets, rules, verifier/tests and [receipts](results.json)
are published together in the personal fork. Compiler binaries, game content,
original executable and private generated artifacts remain outside Git. Feature
CI now includes the small runner boundary suite alongside evidence/tracking and
annotation checks. Its full regression job remains skipped under the existing
cadence; no full regression was run locally.

Continue related-case batches with the same caps and independent review. Keep
near matches and alignment issues in a separate finishing queue. A longer run
can chain bounded checkpoints while preserving these status distinctions; the
present campaign stops after publication. Exact research results should enter
accepted history together at a deliberate batch acceptance checkpoint.
