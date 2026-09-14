# Five-hour continuation results

**57 newly accepted functions: 290 to 347.** The latest contributor manifest has 943 unfinished entries; 339 now have local accepted evidence, leaving **604** if the team's 1,298 completion claims are correct. Eight local functions overlap their completed list. The immutable checkpoint is [event 86](../../evidence/runs/000086-9f7b907124b79ba9.json).

The new functions cover 3,496 complete code bytes and 211 verified address operands. Seven match raw bytes directly; fifty match exactly after their declared COFF address relocations are resolved to the verified original addresses. No bytes are ignored. These are complete function-span results, not a claim that a final linked executable matches the entire 1999 PE.

[Address-based function list](new-functions.csv) records each source path, size, category, first exact attempt, acceptance event, source commit, build and focused assertion count. [Machine-readable results](results.json) also retain the span hashes, aggregate metrics and parked research.

| Shared checkpoint | New functions | Complete bytes | Retained focused assertions |
| --- | ---: | ---: | ---: |
| [acceptance-053](../acceptance-053/result.json) | 3 | 291 | 17,472 |
| [acceptance-054](../acceptance-054/result.json) | 6 | 221 | 340,296 |
| [acceptance-055](../acceptance-055/result.json) | 3 | 245 | 25,664 |
| [acceptance-056](../acceptance-056/result.json) | 6 | 351 | 85,260 |
| [acceptance-057](../acceptance-057/result.json) | 8 | 162 | 73,736 |
| [acceptance-058](../acceptance-058/result.json) | 11 | 278 | 105,744 |
| [acceptance-059](../acceptance-059/result.json) | 6 | 441 | 239,924 |
| [acceptance-060](../acceptance-060/result.json) | 6 | 368 | 271,552 |
| [acceptance-061](../acceptance-061/result.json) | 4 | 459 | 511,048 |
| [acceptance-062](../acceptance-062/result.json) | 2 | 274 | 112,392 |
| [acceptance-063](../acceptance-063/result.json) | 2 | 406 | 1,696,848 |

All 347 final original-toolchain comparisons and the authored shared fixture pass; the same-source supplemental CI also passes. The final build uses 578 Git-identical source inputs. The callback type recovery includes a strict recheck of previously accepted registration bytes, and all earlier task matrix counts are retained.

The retained focused matrices contain 3,479,936 assertions; that is not a count of functions or distinct test scenarios. Small accessors and an original five-byte no-op count as functions, so function throughput cannot be extrapolated into a reliable completion date.

First exact worker attempt distribution: 1: 46 functions, 2: 7 functions, 4: 1 function, 6: 2 functions, 7: 1 function. Median selected focused native verification: 1.41 seconds; Docker verification with authored execution: 5.60 seconds. This measures verification receipts, not authoring time or total batch-build time.

Useful partial work remains capped:

- `stream_bits`: 9/209 differing bytes; near-match. It is excluded from accepted totals.
- `stream_read`: 11/244 differing bytes; near-match. It is excluded from accepted totals.
- `random_table_initialize`: 7/82 differing bytes; research-candidate. It is excluded from accepted totals.
- `resource_backend_handle`: different original/candidate extents 31/30; research-candidate. It is excluded from accepted totals.
- `skills_effective`: 13/156 differing bytes; research-candidate. It is excluded from accepted totals.

Observed issues were handled without relaxing acceptance: source/API typing and compiler stack placement, a modern-only float expectation difference with the original VC5 oracle unchanged, and one host memory allocation failure that required a complete unchanged verification retry. No original program was executed, no warning waiver or byte exclusion was added, and no parked function had its cap reset.

The next prepared task is the [302-byte fixed-step routine](../fixed-step-preparation-001/README.md). Recover its shared floating-point storage and the explicitly unused forwarded scheduler parameter first, then use bounded sequential fixtures with actual accepted callees. The two remaining semantic names are deliberately unresolved. No source attempt was made for that routine.

All work remains on the personal fork branch; no upstream/develop merge was performed. Public changes contain authored source, tests, metadata and documentation, with no original game or compiler binaries.
