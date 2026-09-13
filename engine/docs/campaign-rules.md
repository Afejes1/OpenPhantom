# Bounded reconstruction campaigns

These are working rules for the personal fork. They do not change OpenPhantom's
contribution rules, the strict matcher or the immutable acceptance registry.
All source is independently authored; original game content and compiler binaries
stay outside public commits. The original executable is static evidence only.

## Evidence status

| Status | Required evidence |
| --- | --- |
| Exact function match | Complete original span including tables/alignment; all address operands verified; zero differing bytes; focused fixture passes and independent review is complete. Accepted history still requires the existing batch checkpoint. |
| Body exact, layout pending | Independently verified complete body equality, with unresolved alignment/placement explicitly recorded; not accepted as a full function. |
| Near match, finish later | Complete trustworthy comparison, at most 5% differing bytes, passing focused fixture and independent review with no known behavioral defect; remains research. |
| Research candidate | Compiling useful source whose comparison is incomplete, whose extent differs, whose mismatch exceeds 5%, or whose review has open defects. Preserve the best source and explicit blockers. |

Five percent is a scheduling threshold, never proof of correctness or permission
to count a function accepted. A one-byte branch or constant error can be material.
Store absolute mismatch count, full target and candidate lengths, binding counts,
comparison status and remaining reasons. The percentage is 100 * different_bytes /
full_target_bytes only after the existing strict comparer has verified the same
complete span and every operand. Until a separately validated comparison supports
other layouts, different extents or relocation locations receive no percentage.
Never estimate similarity from a length ratio. Report body and full-span results
separately; whole-executable identity remains its own final milestone.

## Assignment readiness and ownership

Start from existing candidates and small functions reported unfinished by the
team. Before source experiments, establish the full original interval, signature,
constants, external/global/call mappings and intended focused behavior cases.
Missing inventory is an explicit preparation task, not permission to guess values.
The coordinator independently verifies the inventory before relying on its claims.

Use one Sol worker at medium reasoning. A worker may receive a batch of related
prepared cases and process them sequentially; each function retains its own
cap, separate receipt and freeze/handoff. Increasing the queue does not increase
concurrent workers or permit shared-file edits.
The coordinator reviews ambiguous analysis, behavior and receipts independently.
Escalate reasoning only for a specific unresolved issue. Workers own named files;
only the coordinator edits shared verification/tracking files, Ghidra metadata,
accepted history or Git. The worker cannot change acceptance rules or targets to
fit generated output. Handoffs include failures, uncertainty and all experiments.
Use token-aware local renames; unrestricted text replacements can corrupt
public type and function names. Inspect generated source before spending a
compiler attempt, including checks for unintended literal newline escapes.

## Time and stopping rules

A campaign lasts at most 45 minutes including review and publication. Reserve
its final ten minutes for review, focused verification, receipts and publication.
An implementation assignment ends at 15 minutes or ten worker compiler
experiments, including worker verification builds, whichever comes first.
The coordinator records its final independent native/Docker verification pair
separately; those checks must also fit within the campaign deadline. Stop earlier after five consecutive experiments
without measurable improvement. Preparation assignments also have a 15-minute
cap. Do not reset a function's cap by relabeling the same work a new task. Preserve
useful partial work and report a concrete next question when a cap is reached.

Each compiler experiment records its hypothesis, input hash, outcome and elapsed
time. Campaign metrics may join worker logs to the existing private run receipts
for measured timings. Batch fixture edits before final verification; avoid
recompiling unchanged inputs after a failed edit that changed no files. No source variant is retained solely because a byte score improved if its
behavior or provenance is wrong. No raw-byte insertion, shortened spans, ignored
operands, fabricated stack frames, new warning waivers or byte exclusions.

## Verification and evaluation

Use pinned VC5 frontend/options. Native original-toolchain operation is compile
only. A zero-byte Docker assembly listing may use the explicit, tested native
peer listing fallback after identical source/toolchain provenance and complete
function/fixup agreement; retain the empty Docker artifact and identify the
listing source. This changes no byte-comparison requirement.
Execute authored synthetic fixtures only in the existing locked Docker
runner; never execute the original game. Reuse existing runners and matchers.
Check material branches/state transitions, not just a large assertion count.
For a changed final candidate, use independent review, a focused Docker fixture
and code/relocation cross-check as appropriate; verify source hashes against
staged Git blobs before publishing. Keep private artifacts with their hashes.

Run affected tooling tests when tooling changes; run lightweight registry,
tracking and annotation checks for feature pushes. Full regression/recovery is
reserved for the existing batch/integration or toolchain checkpoints, not every
candidate experiment. A historical accepted report is labeled historical.

Evaluate completed comparisons and exact matches separately, total elapsed time,
review minutes, material corrections, experiments and approximate account usage.
Different workloads are not a controlled model benchmark. Consider two simultaneous
implementation workers after at least two consecutive handoffs need no substantial
semantic rewrite and review is keeping pace. No automatic subsequent campaign
unless the contributor explicitly authorizes a bounded continuation window.
Such a window must record a fixed deadline, stop new implementation early enough
for publication, and preserve every per-function cap and evidence requirement.
See the [six-hour authorization](../research/overnight-2026-09-13/README.md).
