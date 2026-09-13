# Overnight results: September 13, 2026

**66 new full-function matches covering 5,315 original bytes**, plus three body-only
matches covering 130 bytes, three near matches and eight other research candidates.
All 80 candidates are independently authored **C (`.c`)**, with headers and focused C
fixtures. None of these overnight candidate implementations is C++.

The authorized window was 02:48-08:48 UTC (22:48 September 12 to 04:48 September 13
in America/New_York). Ten bounded campaigns, 004 through 013, used one reused Sol
worker at medium reasoning, with independent coordinator preparation and review.
The heartbeat was confirmed **PAUSED at 2026-09-13T08:38:40.097611+00:00**,
before the 08:48 UTC deadline. Final report publication follows that shutdown.
The machine-readable outcome is in [status.json](status.json).

## What the matches establish

All 66 exact results are **full original function spans with zero differing bytes
after individually verified relocation/address binding**. Native VC5 and Docker
VC5 outputs and operand inventories agree. This is function-level object evidence;
it is not a claim that the whole linked 1999 executable has been reproduced.
No mismatch percentage was accepted as exact and no bytes were excluded.

Three additional functions have exact executable bodies, but their natural object
code lacks 35 total bytes of original trailing INT3 padding. Their immutable full
targets and full-span mismatch receipts are preserved. They remain
`body-exact-layout-pending`, separate from the 66 complete matches. Campaign 013
adds a committed read-only body-evidence reproducer with ten focused checks.

The 5% threshold only identifies useful candidates to park for later work. The
three near matches are sound level registration (12/264 differing bytes), sound
play-by-name (13/275), and music sequence selection (8/190). None is accepted.

These are reviewed research results awaiting deliberate batch integration. The
accepted-history ledger remains **14 functions / 19 records**, with **zero overnight
promotions**. No develop merge or upstream submission occurred. Function count
alone does not measure engine completion; much of the yield is small helpers and
wrappers, with some larger sound routines.

## Checkpoints

All implementation checkpoints are pushed to `Afejes1/OpenPhantom`, branch
`codex/vc5-matching-baseline`. Exact commits and per-case source/result hashes are
in [metrics.json](metrics.json) and [status.json](status.json). Each linked campaign
contains its source locations, compiler experiments, focused behavior results,
independent review and unresolved questions.

| Campaign | Full exact | Exact bytes | Body only | Near | Research | Checks | Publication |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| [004](../campaign-004/results.md) | 0 | 0 | 0 | 0 | 3 | 151 | [CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34735785699) |
| [005](../campaign-005/results.md) | 10 | 413 | 0 | 0 | 0 | 205 | [CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34736926013) |
| [006](../campaign-006/results.md) | 10 | 603 | 0 | 0 | 0 | 258 | [CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34738009636) |
| [007](../campaign-007/results.md) | 9 | 716 | 0 | 0 | 1 | 164 | [CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34739403557) |
| [008](../campaign-008/results.md) | 8 | 900 | 0 | 0 | 0 | 454 | [CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34740736113) |
| [009](../campaign-009/results.md) | 6 | 1,025 | 0 | 0 | 0 | 583 | [CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34742113516) |
| [010](../campaign-010/results.md) | 3 | 671 | 0 | 2 | 1 | 436 | [CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34743873794) |
| [011](../campaign-011/results.md) | 10 | 467 | 0 | 0 | 1 | 209 | [CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34745149684) |
| [012](../campaign-012/results.md) | 10 | 520 | 0 | 0 | 1 | 257 | [CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34746492318) |
| [013](../campaign-013/results.md) | 0 | 0 | 3 | 1 | 1 | 526 | [CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34747844315) |
| **Total** | **66** | **5,315** | **3** | **3** | **8** | **3,243** | **All passed** |

The first campaign-013 push failed CI because its generated team work-order document
was not staged. Repair commit `2ec5d83ef8f910bd1506fe615afb506f1e5ed75f` adds that
publication correction; the candidate sources and compiler comparison receipts did
not change. The corrected CI passed. The initial failed run and successful repair
are both retained in the status checkpoint.

## Verification and reproduction

There were **3,243 passing focused behavioral assertions** across the 80 final
fixtures and **53 affected-tooling checks** (43 in campaign 004, ten in campaign
013). These assertions are bounded fixture evidence, not exhaustive behavior or
whole-game coverage. Callbacks and external routines are authored stubs where
specified in each case. Independent review strengthened state, callback ordering,
unsigned extremes, return-value and floating-point checks before final publication.
It also corrected a source-level flag interpretation in campaign 010 and a
preparation-only 90-degree constant misreading in campaign 013.

The record contains **129 worker candidate compiles**, **160 final coordinator
candidate compiles** (80 native/Docker pairs), and **80 passing final Docker fixture
executions**. Exploratory failures and extra builds remain in per-case logs and
campaign metrics; these final counts do not hide or count failed attempts as passed.
The lightweight feature CI validates runner boundaries, tracking and annotations.
Full regression was intentionally deferred to the deliberate acceptance checkpoint.

The pinned compiler is Visual C++ 5.0 RTM, frontend 11.00.7022, with linker 5.00.7022.
Case manifests record exact flags; the source builds use strict warnings. The
native runner compiles only. Authored fixtures execute in the locked Docker runner,
with networking disabled and only their private build folder writable. The original
executable was neither executed nor patched. Ghidra was used as static evidence,
and evidence-backed metadata improvements were saved.

The Docker recipe, toolchain lock, build/matching scripts, independently authored
sources, fixtures and metadata are committed. Reproduction requires the user's
owned reference executable and licensed original compiler files; those binaries,
original content, decompiler output and private generated artifacts are outside Git.
Follow [the case runner instructions](../campaign-001/README.md) and each case's
manifest. Fresh reproductions produce new reports linked to the same pinned inputs.
Some Docker runs use the documented verified native-listing fallback; complete
COFF symbols/relocations and native/Docker object equality remain required. Each
campaign records that limitation rather than treating a missing listing as proof.

Pinned reference SHA-256:
`02babee8d64bc0bf47451aaae9af630f4a028cf63f9996bda6df497caba34f11`

Toolchain lock SHA-256:
`8a97faf8a20112d4a46da8154a4ebb03888f393eeeb1a0d2ddeacb1685f19333`

Docker image:
`sha256:1837b4cfa8c6d4b48ca11f14e58a248265c3ee16a4b0103b1bde143050d4fa7d`

## Throughput and usage

The ten recorded campaign intervals total **5.33 hours** from
campaign start to observed publication/CI confirmation. This includes preparation,
analysis, source work, review and CI waiting; it is not measured CPU or reasoning
time. Setup, gaps between campaigns and the morning report lie outside those
intervals. No prerequisite outage was recorded. The fixed six-hour authorization
was not extended to compensate for gaps.

For the 80 final pairs, the native runner's median receipt time was **1.39 seconds**
and Docker's was **5.55 seconds**. Their elapsed sums were 111.41 and
457.59 seconds respectively. These timings include runner overhead and Docker
fixture work and exclude worker experiments. They are not a controlled benchmark,
but show that these compiler invocations were a small portion of campaign wall time.
There is no measured reason here to replace Docker with a VM for throughput.

The rounded shared weekly account meter moved from **24% to 36% used** (64% remaining
at the final observation). That 12-point change can include other account work;
it is not an exact price, token count or isolated campaign charge. No reset credits
were consumed or extra capacity purchased. CI links show actual runs; dollar costs
were not estimated from these timings.

## Parked work

| Case | Category | Measured mismatch |
| --- | --- | --- |
| [queue_model_hierarchy](../campaign-004-model-hierarchy/README.md) | research | 5.5556% |
| [has_surface_at_or_below](../campaign-004-surface-below/README.md) | research | Full extent differs |
| [collect_nearby_surfaces](../campaign-004-nearby-surfaces/README.md) | research | Full extent differs |
| [sound_pin_channel](../campaign-007-sound-pin-channel/README.md) | research | 18.5714% |
| [sound_remove_level](../campaign-010-sound-remove-level/README.md) | research | 5.6872% |
| [sound_register_level](../campaign-010-sound-register-level/README.md) | near | 4.5455% |
| [sound_play_by_name](../campaign-010-sound-play-by-name/README.md) | near | 4.7273% |
| [view_clear_visits](../campaign-011-view-clear-visits/README.md) | research | 37.5000% |
| [bgl_transform_xyz](../campaign-012-bgl-transform-xyz/README.md) | research | 19.5876% |
| [music_resume](../campaign-013-music-resume/README.md) | body-exact-layout-pending | Full extent differs |
| [sound_is_muted](../campaign-013-sound-is-muted/README.md) | body-exact-layout-pending | Full extent differs |
| [music_set_sequence](../campaign-013-music-set-sequence/README.md) | near | 4.2105% |
| [bgl_perp_axis](../campaign-013-bgl-perp-axis/README.md) | research | 24.8980% |
| [bgl_random_vector](../campaign-013-bgl-random-vector/README.md) | body-exact-layout-pending | Full extent differs |

The body-only cases need linker/layout investigation for their original padding,
not extra source bytes inserted to manufacture a match. All other parked cases
retain their original attempt budgets. Preparation-only deferred work, including
the BGL color conversions and random-unit XYZ routine, is not counted among the 80.

## Recommended next checkpoint

1. Perform one deliberate acceptance pass over the 66 exact results: reconcile the
   upcoming upstream source drop and team manifest, review shared declarations and
   layouts, integrate selected sources and run the broader regression once.
2. Continue one Sol worker with prepared related queues and independent review.
   Keep the 15-minute/ten-compile cap and five-attempt no-improvement stop. The
   overnight results support this bounded process; they do not establish that a
   different model would be slower or more expensive on the same cases.
3. Schedule a separate short layout/padding investigation for the three body
   matches, then a targeted finishing pass for the three near matches. Preserve
   their mismatch evidence; keep zero bytes as the exact acceptance criterion.
4. Keep complex x87 and large traversal work explicitly time-boxed. Better ABI,
   constant and fixture preparation is more promising than adding implementation
   workers or removing independent review based on this run.

No further overnight implementation is queued. The campaign stops within its
original window; a later campaign needs a new user instruction.
