# Campaign 001 results

Two workers produced two reviewed, compiling candidates for functions the team
reported unfinished. **Zero new exact matches and zero accepted functions** were
added. The historical accepted baseline stays at fourteen functions; it was not
rerun as a whole during this pilot.

| Assignment | Worker | Original complete span | Final evidence |
| --- | --- | --- | --- |
| Frame setup, 0x00404180 | Sol, medium | 1,456 bytes | VC5 C; 6,548 focused assertions pass; native/Docker function code and relocations agree |
| Mover gathering, 0x00405CF0 | Terra, high | 1,552 bytes | VC5 C++; 36 focused assertions pass; native/Docker translation-unit code and relocations agree |

These spans total 3,008 original bytes investigated, not 3,008 matched bytes.
Frame setup emits a 1,376-byte section with 135 relocations. Mover gathering
emits 1,520 bytes across its authored code inventory, including helpers the
compiler did not inline. Neither number defines a similarity percentage.
Both complete original address spans are retained, including seven and fifteen
trailing alignment bytes respectively. Complete address/constant binding
inventories remain pending, so strict comparison is **not attempted**.

## Quality and effort

The campaign began at 00:22:54 UTC with a 45-minute cap. Workers stopped before
their assignment deadline. Final candidate verification finished at 00:52:19 UTC,
about 29 minutes after start; reporting, tracking and publication followed.
Publication normalized the mover case configuration from CRLF to repository LF
and refreshed its native/Docker receipts at 2026-09-13T00:59:04Z. The candidate
code and passing check count were unchanged; all input hashes now match Git blobs.
Workers reported eight candidate compilations for frame setup and four for mover
gathering, including failed builds. These are worker-reported iteration counts,
not an exhaustive command ledger. The coordinator then froze and independently
compiled each final input natively and in Docker.

Thirteen material review findings were addressed. They included incorrect
constants and offsets, a reversed ratio, conflated state tables, callback state,
phase-dependent visitation, negative fade handling and a fixture that always
returned success. The coordinator strengthened both fixtures after worker
handoff. [The review log](review.json) records the evidence and disposition.
Passing assertions cover selected state transitions; many frame assertions check
array entries. They are not thousands of distinct scenarios or proof of complete
behavioral equivalence. Trigonometry, line stepping and queue callbacks are
controlled authored fixtures. Original x87 precision boundaries and integrated
renderer behavior remain open.

Ten campaign runner boundary/inventory tests and eleven existing artifact tests
pass. No full regression or recovery suite was run. Generated fixtures executed
only in the existing locked Docker runner. The original executable was read
statically and never executed or modified. No warning waivers, byte masks or
accepted-history changes were introduced.

## Performance and usage

| Final run | Native candidate compile | Docker candidate compile | Complete Docker verification |
| --- | ---: | ---: | ---: |
| Frame setup | 0.064 s | 1.174 s | 5.752 s |
| Mover gathering | 0.068 s | 1.264 s | 5.524 s |

These are single observed runs, not a controlled performance benchmark. Docker
adds roughly a second to these tiny compilations. Analysis, source correction
and review dominate the elapsed pilot time. Native compile-only iteration is
useful; changing virtualization is not the next priority on this evidence.

The account-wide weekly usage meter read 20% used at start and 21% after final
verification. It is rounded, shared across account activity and may lag; this is
not a precise campaign cost or a billing estimate. Different function workloads
and reasoning settings also prevent declaring either worker model the winner.

## Recommendation

Keep two workers with coordinator review for another bounded batch. Give each
worker a smaller function or a precise address/relocation inventory, require an
early compile and representative fixture, and retain a tested candidate when
further scheduling experiments stop paying off. Do not enlarge the worker pool
until review effort falls. Measure both new tested candidates and new exact
matches per elapsed minute, with usage observations alongside them. This pilot
shows useful parallel drafting, not demonstrated faster exact matching.

The campaign stops here; no second campaign has started. Authored source, runner,
fixtures, tracking and [hash-based receipts](results.json) are committed together.
Compiler binaries, the original executable and private build artifacts are not
published. Existing toolchain acquisition/lock instructions remain prerequisites
for reproducing the commands in the campaign README.
