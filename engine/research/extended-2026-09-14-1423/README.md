# Two-hour reconstruction checkpoint

Authorized window: September 14, 2026, 14:23:45 to 16:23:45 UTC (10:23:45 a.m. to 12:23:45 p.m. Eastern). New implementation stopped before the 16:08:45 cutoff.

**273 to 290 accepted functions: 17 additions.** Sixteen were newly matched in this window; module_start_all was a reviewed carry-in that completed shared acceptance here. Each count is a unique original address.

| Acceptance | Added functions | Complete bytes | Verified operands | Retained focused assertions |
| --- | ---: | ---: | ---: | ---: |
| [acceptance-048](../acceptance-048/results.md) | 4 | 715 | 17 | 2,296,800 |
| [acceptance-049](../acceptance-049/results.md) | 2 | 351 | 7 | 1,411,200 |
| [acceptance-050](../acceptance-050/results.md) | 4 | 390 | 3 | 454,567 |
| [acceptance-051](../acceptance-051/results.md) | 4 | 544 | 11 | 1,251,840 |
| [acceptance-052](../acceptance-052/results.md) | 3 | 370 | 6 | 6,111,472 |

Total: **2,370 additional bytes**, 44 verified address operands and 11,525,879 retained focused assertions. A050 also adds 7,515 shared routing assertions. Tests and compiler attempts are not function counts; repeated validation is not another implementation. The unaccepted separator fixture has a separate 156,240 assertions and contributes zero accepted functions.

## Absolute remaining work

The latest contributor manifest lists 2,241 application functions: 1,298 team-done and 943 team-not-done. Our 290 accepted functions cover 282 of their not-done entries and overlap eight team-done entries. **661 remain** if all team-done claims are correct. Those team claims have not been independently accepted by our verifier. [Address-by-address comparison](manifest-comparison.json) and [17-function delta](accepted-function-delta.csv) retain the exact mapping.

## What matching proves

Each source is compiled with the pinned original VC5 toolchain. The verifier compares the complete original function span, including required tables/alignment, rather than a decompiler listing or visual resemblance. One new function is raw-byte equal. For the other 16, the verifier checks every recorded relocation operand and resolves it to its original target address before comparing every byte; the resulting span hash must equal the original span hash. Address bytes are resolved, not ignored. All 17 have zero differences and no added exclusions or warning waivers.

This remains function-level evidence. Original linked placement, the final complete executable, and game-wide behavioral identity remain unfinished. Local original-toolchain authored fixtures execute only in the locked Docker environment; native VC5 peers compile without executing fixtures. Supplemental modern-compiler fixtures run in GitHub Actions. The original executable is used statically and is never executed. Canonical target sources now comprise 289 C files and one C++ file.

## Quality and throughput observations

Related cohorts reused verified structures, helper contracts and fixture patterns. Independent review ran alongside one implementation lane. The 16 new matches used 31 bounded source compiler attempts, plus one attempt for the unaccepted separator candidate: 32 attempts total, excluding the carried-in C067 work. Four routines needed source-shape iteration, while most matched quickly. This is a workload observation, not a controlled model comparison. The cohort approach remains preferable to enlarging mismatch tolerances or retrying parked cases without new evidence.

All five shared acceptance checkpoints passed the original-toolchain fixture/comparison gates and same-source supplemental CI. The final cohort exposed modern-compiler C4702 only in three test observers; the minimal correction retained failed assertions, archived prior receipts and repeated affected focused pairs plus the shared build. No warning waiver or candidate change was used. A050 fixture escaping and namespace corrections and the VC5 C4127/C4706 diagnostics also remain documented.

## Handoff

[Next queue](next-queue.md) records nearby layout/ABI blockers and preserves capped attempts. The separator remains unaccepted at 42 compiled versus 56 target bytes; module send-shutdown remains parked. Ghidra evidence comments were saved; two attempted calling-convention signature corrections were rejected by the bridge and remain explicitly unresolved in [metadata notes](ghidra-metadata.json).

[Final acceptance](../acceptance-052/results.md) pins the original-toolchain build, all source hashes and CI. No upstream/develop merge or original-binary modification occurred.
