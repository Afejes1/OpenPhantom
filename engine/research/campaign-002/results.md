# Campaign 002 results

Two new functions now have independently verified complete-span matches under
the pinned original VC5 toolchain: **637 bytes total**. Zero functions were added
to accepted history; its fourteen functions and nineteen evidence records remain
unchanged until the existing batch checkpoint.

| Work item | Complete comparison | Focused Docker checks |
| --- | --- | --- |
| Scan-line reset, `0x004084B8` | All 400 bytes agree after resolving one independently reviewed allocator call | 238 passed |
| Segment line-side test, `0x00408648` | All 237 raw bytes agree; no relocations | 6,601 passed |
| Existing frame setup, `0x00404180` | All 153 original operands reviewed; candidate extent remains 1,376 versus 1,456 bytes | 6,548 passed |

The frame candidate receives no similarity percentage: its full extent differs,
so the strict comparator does not validate candidate operand locations. Its
original operand inventory is now complete, replacing the previous preparation
blocker with a concrete compilation mismatch. Candidate source was unchanged.

The [5% scheduling rule](../../docs/campaign-rules.md) never changes exact
acceptance. Revalidated historical evidence places the existing culler at 6/480
differing bytes (1.25%) and static collector at 5/896 (about 0.558%). These are
research priorities, not new matches or new fixture runs. The existing exact
816-byte legacy collector also remains pending batch acceptance. See [triage](triage.json).

## What the execution showed

One Sol worker at medium reasoning completed three sequential assignments:
the frame operand inventory, scan-line reset, then the segment predicate. Both
implementation handoffs needed no semantic source rewrite. Independent review
corrected one frame table base/addend mapping and requested broader scan-line
fixture coverage. The [review log](review.json) records these findings.

The scan-line helper matched on its second source variant by enabling VC5's
ordinary `abs` intrinsic. The segment predicate matched on its fifth variant
using conventional mathematical identifiers for its ten meaningful locals.
Changing declarations alone did not help; the identifier variant changed VC5's
local allocation. These observations do not prove the original source text or
a general compiler allocation algorithm. No artificial stack objects, opcode
insertion, warning waivers, excluded bytes or shortened spans were used.

The segment fixture preserves the original predicate's surprising behavior:
disjoint collinear segments and distinct point segments return true. Its 6,601
checks include 40 explicit/reversal/swap assertions and all 6,561 coordinate
tuples over -1, 0 and 1 against a determinant formulation. The scan-line fixture
checks direction, major-axis/tie choices, each initialized field, allocation
success/failure and preserved state. Counts are assertions, not distinct game
scenarios. Signed-overflow inputs and original-game integration remain untested.

## Time, compiler overhead and usage

The campaign started at 01:16:02 UTC on 2026-09-13. Final independent candidate
verification finished at 01:44:59 UTC, about 29 minutes later. Publication follows
within the 45-minute window; no subsequent campaign starts automatically. Workers
stopped within their assignment limits. They used thirteen compiler invocations
across seven source variants, including fixture verification repeats; the
coordinator ran six final native/Docker compilations across the three cases.
One unchanged-input worker compile was unnecessary and is retained in the log.

| Final verification | Native candidate compile | Docker candidate compile | Complete Docker run |
| --- | ---: | ---: | ---: |
| frame_setup | 0.082 s | 1.183 s | 5.630 s |
| reset_scan_line | 0.059 s | 1.134 s | 5.603 s |
| segments_intersect | 0.067 s | 1.146 s | 5.683 s |

These single-run measurements again put Docker overhead in seconds; analysis,
source experiments and review dominate elapsed time. The weekly account meter
moved from 21% to 22% used. It is rounded, shared and may lag, so this is not a
precise campaign cost. Separate coordinator review minutes were not timed.

## Publication and next step

Source, fixtures, prepared packets, rules and [hash-based receipts](results.json)
are published together in the personal fork. Native operation compiled only;
authored fixture execution stayed inside the locked Docker runner. The original
game was analyzed statically. Ghidra semantic names/comments were saved.
Compiler binaries, the original executable, generated executables and private
disassembly/build artifacts are not included in Git.

Focused verification passed; the lightweight registry, work-order and annotation
checks apply to this feature push. The strict matcher and shared runner were
unchanged, and no full regression was run for these candidate experiments.

Keep this one-worker workflow for another small-function batch, with prepared
interfaces and independent review. The two implementation handoffs are promising,
but these smaller workloads do not establish a model or concurrency winner.
Before promoting exact research candidates, perform the existing batch acceptance
checkpoint once; do not silently increase accepted counts based on this campaign.
