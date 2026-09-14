# One-hour continuation results

Six new functions accepted: 267 to 273. This batch adds 900 complete function bytes
and eleven verified address operands. All six are relocation-adjusted matches:
after resolving each proven address operand, every byte matches, with no exclusions.
This does not establish an identical final executable.

| Count against the September 12 18:49 contributor snapshot | Functions |
| --- | ---: |
| Distinct application functions listed | 2,241 |
| Team reports done, not independently verified here | 1,298 |
| Team reports unfinished | 943 |
| Our accepted functions within those 943 | 265 |
| Our accepted functions overlapping their done list | 8 |
| Our total accepted functions | 273 |
| Still unfinished by either group, assuming their claims | 678 |

The remaining count fell from 684 to 678 in this window. These are unique original
addresses, not tests, compiler attempts or repeated verification runs. The old
snapshot used 969 unfinished and has been preserved for historical reports.
The [combined inventory](manifest-comparison.json) lists every original address,
both status columns, local source, match category and whether it remains open.

The cumulative accepted set contains 31 raw and 242 relocation-adjusted function
matches across 23,865 bytes. Address relocation is checked rather than ignored:
each external/global operand must point to its verified original destination;
opcodes, constants, branches, stack layout and complete spans must then compare
with zero differences. Final linked placement and whole-executable identity remain
separate unfinished milestones.

[Acceptance 047](../acceptance-047/results.md) records the one shared VC5 build,
all 273 passing comparisons, retained 1,613,760 focused assertions with the real
finder, 468 Git-identical source inputs and same-source supplemental CI.
Original source additions are six C files; the accepted set now has 272 C and one
C++ translation unit. Original compiler binaries and game contents stay private.

Reproduce the count report from the repository root:

```powershell
python engine/planning/compare_manifest.py --manifest engine/planning/team-manifest-20260912-1849.json --event engine/evidence/runs/000070-5617c8a530e377e3.json --output engine/private/current-manifest-comparison.json
```

The generator validates the immutable history and joins by numeric address. It
does not grant acceptance or assert the team's code is correct. The two known
historical snapshots were cross-checked against event 69 before generating this
event 70 report. Shared account usage observed during the window was 80 percent
used, 20 percent remaining; it is not a per-thread usage meter. No reset was used.

One additional function, [module_start_all](../campaign-067-module-start-all/result.json),
now has a focused exact 193-byte match and awaits the next batch acceptance. It
is still included in the 678 remaining count above. A second candidate,
module_send_shutdown, passes focused behavior but emits 193 bytes for a 198-byte
full target, so it remains research with no mismatch percentage. Across this
hour there were seven new focused exact matches, six promotions, and one research
candidate. The [eight-function ledger](functions.json) keeps these statuses separate.

The [next assignment](next-work.json) also documents the general lifecycle
dispatcher's verified ABI and complete span, its existing save-chain symbol, and
an unresolved source-layout question. No second shared regression was run.

The [final audit](audit.json) verifies all 468 current accepted inputs against
event 70 and Git, all 16 focused report hashes, 48 case-local input hashes
against Git, and the exact 2,241-address/678-remaining partition.

The [module queue](module-queue.json) covers all 20 entries in util/module.c:
12 accepted, one focused exact awaiting acceptance, one research candidate and
six otherwise unimplemented. Four further static assignment packets record
complete extents, operands, callback ordering, global writes and lifetime rules.
