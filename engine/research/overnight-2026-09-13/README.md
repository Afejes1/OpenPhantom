# Six-hour reconstruction campaign: 2026-09-13

**Completed; heartbeat paused.** See the [morning report](results.md),
[aggregate metrics](metrics.json) and [final status](status.json).
The setup and operating instructions below are retained as campaign history.

The contributor authorized six hours of continued reconstruction using the tested
one-worker strategy. The fixed window is **02:48 to 08:48 UTC on 2026-09-13**
(22:48 September 12 to 04:48 September 13, America/New_York).
This explicitly authorizes chaining bounded campaigns inside this window; it
supersedes only the default rule against an automatic subsequent campaign.

## Resume and stop

Read [status.json](status.json), the latest numbered campaign and
[the campaign rules](../../docs/campaign-rules.md) before starting work.
The baseline is commit `3d7bbd03799a92f5b7f161361ea6c2599bb63ff3` on
`codex/vc5-matching-baseline` in the contributor's `Afejes1/OpenPhantom` fork.
Use the existing worktree at
`C:/Dev/Ghidra/release-verification/openphantom-vc5-baseline`.
The in-thread heartbeat is named **OpenPhantom six-hour campaign**
(id `openphantom-six-hour-campaign`). It checks for continuation every five
minutes; active work must be coordinated, never duplicated. The schedule expires
at the fixed deadline. No new Codex task or autonomous goal was created.

Check actual UTC time at each run and before dispatching work. One run can do one
campaign of at most 45 minutes, including ten minutes reserved for publication.
Stop new implementation by **08:38 UTC**. Finish the summary and pause the
heartbeat by **08:48 UTC**; do not start a function too late to verify it.
If fewer than ten minutes remain after a checkpoint, finish the morning report
and pause early. Do not compensate for downtime by extending the window.
If the computer sleeps, the app closes, a prerequisite disappears, or account
limits intervene, record the actual interruption rather than claiming six hours
of work occurred. Do not consume reset credits or purchase capacity.

## Work and quality

Reuse one Sol worker at medium reasoning with a queue of related prepared cases.
The coordinator independently prepares inventories and reviews final results.
Each function retains the 15-minute/ten-compile cap and five-attempt no-improvement
stop. Old exhausted caps are not renewed by this authorization. Prefer untouched
team-not-done functions, proceeding through the documented module work order as
practical. Preserve useful near matches for a separate finishing pass.

Use Ghydra MCP to confirm the authoritative TPM WMAIN.EXE at every session start.
The original executable remains static evidence only. Use the pinned VC5 frontend
and flags, complete target spans, all verified address operands and existing
strict matcher. Native execution compiles only; authored fixtures run only in
the existing locked Docker environment. Five percent remains a scheduling
threshold, never exact acceptance. No span trimming or byte exclusions.

Record each case's source, fixture, immutable target, failed and successful
experiments, independent review, native/Docker comparison, source hashes and
limitations. Check the final hashes against exact staged Git blobs before pushing.
No compiler binaries, original game content, decompiler output or private build
artifacts enter Git. Make no upstream submission or develop merge.
Run focused case checks and affected tooling tests, plus the existing lightweight
feature CI. Full regression and accepted-history promotion remain deliberate
batch checkpoints, not work performed after every function.

## Checkpoints and morning report

Update status.json with each numbered campaign, implementation state, reviewed
outcome, exact commit, CI result and concrete resume point. Count this window's
new results separately from historical work. Exact matches, body-only matches,
near matches and accepted history remain distinct. At final close, add results.md
with function/byte totals, meaningful tests, exceptions, pushes, elapsed active
work, interruptions and approximate shared account usage. Include open questions
without representing them as completed functions.

At setup, the shared weekly account meter reads 24% used. This rounded reading is
not a precise campaign cost. Historical accepted evidence contains fourteen
functions and nineteen chained records; campaigns 002/003 also contain focused
research matches awaiting a deliberate acceptance checkpoint.

The initial five-minute read-only worker triage will identify small untouched
functions for campaign 004. Its reply is delivered to this thread. Do not repeat
campaign 003's four assignments or polish its capped polyline by default.
