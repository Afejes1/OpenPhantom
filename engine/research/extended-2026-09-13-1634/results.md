# Six-hour reconstruction window results

The window authorized for 2026-09-13 16:34:23-22:34:23 UTC produced **69 new exact function matches covering 6,996 complete bytes**. The accepted shared baseline grew from **40 to 105 functions**, or 4,469 to 10,714 complete bytes. Final clean acceptance, authored behavioral fixtures and supplemental CI pass.

| Measure | Result |
| --- | ---: |
| New focused campaigns | 12 (023-034) |
| New exact functions | 69 |
| Unique promotions into shared acceptance | 65 |
| Promotions created in this window | 62 |
| Promotions from the previous exact queue | 3 |
| New exact functions still awaiting integration | 7 / 988 bytes |
| Parked near matches | 1 |
| Other parked research candidates | 6 |
| Overlap with team-reported done functions | 0 |
| Immutable acceptance records | 28 to 38 |

Nine deliberate acceptance checkpoints (008-016) and one additional final-source replay produced those ten records. Promotions and research matches are separate measures: adding 69 and 65 would double-count work.

## Final accepted evidence

Build `20260913-180706-ca1f0e3a` at source `39af9501bbaf0c05728dbead492509a5109a78b8` passes all 105 complete comparisons. [Immutable event38](../../evidence/runs/000038-c2e76bb9869da217.json) records 212 source input hashes. [Full supplemental CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34785872027) passes on the same source. The final batch adds the connected puppet initializer, track removal, playback and both fades.

Exact means complete function spans, including alignment/tables where present, with every address operand verified and zero differing bytes after relocation handling. This is **not yet a byte-identical linked WMAIN.EXE**: final layout, data and whole-executable identity remain pending. No byte exclusions, warning waivers or artificial padding were introduced. The strict matcher is unchanged.

The compiler remains Visual C++ 5.0 RTM 11.00.7022, with the locked toolchain and recorded per-function options. Native operation compiles only. Authored fixtures execute in the pinned Docker image with networking disabled and the established sandbox. The original executable remains read-only static evidence and was not run. Original content, compiler binaries and private generated artifacts are excluded from Git; source, tooling, hashes and reproducible build instructions are committed.

## What was reconstructed

| Campaign | Exact functions | Complete bytes | Focus |
| --- | ---: | ---: | --- |
| [Campaign023](../campaign-023/results.md) | 6 | 486 | Shield allocation, release and save sizing |
| [Campaign024](../campaign-024/results.md) | 7 | 416 | Fog/fade/shield helpers |
| [Campaign025](../campaign-025/results.md) | 6 | 871 | Shield destruction, timed fog and tint |
| [Campaign026](../campaign-026/results.md) | 7 | 455 | Letterbox, halo resources and overlay state |
| [Campaign027](../campaign-027/results.md) | 6 | 461 | Zap lifecycle and halo cleanup |
| [Campaign028](../campaign-028/results.md) | 7 | 658 | Effects loading, ripple materials and render wrappers |
| [Campaign029](../campaign-029/results.md) | 7 | 1,324 | Effects save, attachment and sprite resources |
| [Campaign030](../campaign-030/results.md) | 4 | 1,020 | Shield persistence, ripple spawn and zap geometry |
| [Campaign031](../campaign-031/results.md) | 4 | 414 | Projection helpers, halo flicker and zap subdivision |
| [Campaign032](../campaign-032/results.md) | 5 | 304 | Projected-overlay expiration, removal, clearing and sorting |
| [Campaign033](../campaign-033/results.md) | 5 | 187 | Application/frame/shutdown helpers |
| [Campaign034](../campaign-034/results.md) | 5 | 400 | Puppet initialization, track removal, playback and fades |

## Verification and effort

The [historical audit](audit.json) verifies all 76 final case receipts, 1,065 recorded historical source inputs against 489 unique Git blobs, and 152 private native/Docker report hashes. A final integrity check also verifies 520 private object, listing and inventory artifact hashes. All 123 retained implementation attempt reports and their private candidate source hashes also verify. An [independent read-only review](review.json) reconciled counts, eligibility, the remaining queue and content boundaries.

There were 121 actual candidate compiler invocations in those 123 implementation experiments: four failed compiles and two preflight attempts that never reached the compiler. All failures remain recorded. Compiler command time for those experiments totaled 7.51 seconds; their complete runner reports totaled 166.10 seconds. The final independent verification pairs totaled 105.60 seconds native and 428.24 seconds Docker. These measurements exclude shared acceptance rebuilds, fixture-only iterations, analysis and review; they are not a model benchmark.

The focused fixtures report 17,674,503 checks, dominated by repeated pool/lifecycle combinations. That number is an audit checksum, not a measure of unique behavior or game completion. More useful evidence includes actual connected cleanup and save/load chains, live callback mutation checks, full-state preservation, counter wrapping, capacity boundaries, stable ties and track lifecycle transitions.

The new exact functions have a median span of 70 bytes; the largest is 350 bytes. This is successful coverage of small and medium dependencies, not a basis for extrapolating time to complete the entire game. Seven difficult cases were parked under the existing caps instead of relaxing acceptance.

Account-wide weekly usage moved from 47% to 57% consumed at the last check (22:10:55 UTC). This is rounded shared-account usage and may include other tasks; it is not a campaign-specific token bill. No reset credit was consumed.

## Recommendation and handoff

Keep one implementation writer with a read-only independent reviewer. Batch cohesive functions, use focused checks while reconstructing, and reserve full regression for shared integration checkpoints. The measured compiler times do not justify replacing Docker; the important work remains ABI/extent analysis, useful fixture construction and reconciliation of shared types and callbacks.

Next, integrate the exact zap geometry trio, then the sprite resource trio after resolving shared symbol/type collisions, and finally `frame_prepare`. The [next-work queue](next-queue.md) records the contracts and seven parked cases. Preserve the five-percent near-match category for scheduling only. No nonzero mismatch is accepted.

Before proposing upstream integration, reconcile the team's forthcoming source and agree contribution scope as required by CONTRIBUTING.md. This work is published only to the personal fork branch; no upstream/develop merge or external issue/message was made.

Final Ghidra evidence notes for the ten application/frame/track functions were saved to TPM:/WMAIN.EXE, preserving existing comments. No executable bytes were modified.

The fixed-window status and stop time are recorded in [status.json](status.json). All earlier run windows remain completed history; the broader goal remains separately paused.

The continuation heartbeat is paused. Reconstruction and audits concluded at 22:22:55 UTC after 348.53 minutes; final Git publication follows this recorded closure. The six-hour deadline was not extended.
