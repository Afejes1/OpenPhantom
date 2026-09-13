# Campaign 004 results

Three authored C implementations were preserved with independent original-toolchain
verification. **No new exact match, near match, body-only match or accepted-history
entry** was earned in this batch. This is useful partial work, with each remaining
mismatch recorded rather than waived.

| Function | Original / candidate bytes | Comparison | Focused checks |
| --- | ---: | --- | ---: |
| [Model hierarchy](../campaign-004-model-hierarchy/README.md) | 144 / 144 | 8 bytes differ (5.556%); all 5 operands verified | 17 |
| [Surface at or below](../campaign-004-surface-below/README.md) | 227 / 229 | Complete extent differs; no percentage | 53 |
| [Nearby collector](../campaign-004-nearby-surfaces/README.md) | 555 / 555 | 11 of 25 operand locations differ; no percentage | 81 |

All 151 focused assertions pass. Each candidate has a fresh native/Docker pair
with equal emitted function bytes and fixups. The hierarchy's empty Docker listing
uses the existing verified native-peer fallback. The surface has seven separately
verified operand contracts; the collector has independently verified ordered
operand identities, which do not substitute for exact positions. The pinned VC5
compiler, all target spans and every exclusion policy remain unchanged.

A narrow matcher extension now supports direct recursion through the exact
function's defined COFF symbol. All 43 affected tooling tests pass (16 relative-call,
20 matching, 7 internal-label tests); the feature workflow adds the relative-call
suite. Full regression was not run. Historical accepted evidence remains fourteen
functions and nineteen records.

Independent review caught and corrected a material enumerator calling-convention
error that a fixture sharing the same wrong header would miss. It also corrected
an overstated worker offset diagnostic. The review records the surface fixture's
limited callback-order assertion. There are no known remaining material review
findings, but these focused checks do not prove whole-game behavior.

The worker made 23 candidate compiler invocations across three capped assignments;
the coordinator made six final candidate verification builds. Final verification
finished at 03:23:04 UTC, after the 02:55:18 campaign start. Final native runs took
1.396-1.428 seconds and Docker runs including fixtures took 5.600-6.027 seconds.
This batch's exact-match throughput was lower than Campaign 003; analysis, review,
and compiler-layout work dominate, not these measured container runs. Different
function complexity makes this unsuitable as a model benchmark. The rounded shared
weekly meter moved from 24% at overnight setup to an observed 26%; that is not exact
campaign usage attribution.

Next: retain one Sol/medium implementation worker and use the
[small music-helper shortlist](../overnight-2026-09-13/next-shortlist.json).
Prepare and verify each exact span and ABI before dispatch, then batch related
helpers within the same per-function caps. Do not revisit these capped candidates
automatically or trade stricter acceptance for a higher completion count.

The original executable was never run or patched. Public files contain authored
source, fixtures, target metadata and hash receipts; licensed tools, game content,
raw disassembly and generated binaries stay private. No develop merge or upstream
submission was made.
