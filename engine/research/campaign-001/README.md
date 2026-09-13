# Campaign 001: bounded reconstruction pilot

This campaign follows two team-reported unfinished functions in `bp/bapdraw.c`.
The starting commit is `385b27fbef1069fa88c35bbe1c95064ca06226e8` with fourteen
historically accepted functions. Research outputs do not change that baseline.

The parent owns the campaign runner, review, tracking and publication. Sol at
medium reasoning owns frame setup at `0x00404180`; Terra at high reasoning owns
mover-cell gathering at `0x00405CF0`. Each receives a separate research folder,
a 20-compiler-experiment cap and a common deadline. Neither may change shared
source, verification rules, the accepted registry or Ghidra metadata. The parent
checks the resulting source independently before retaining its claims.

[The campaign definition](campaign.json) records the actual start, stop budget,
assignments and account-wide usage reading. [Review findings](review.json)
record material issues identified independently and their dispositions. [Results](results.md) separate useful candidates, passing focused checks,
strict complete matches and accepted checkpoints. Different workloads and
reasoning settings prevent treating this pilot as a controlled model comparison.

## Verification

Run from the repository root, replacing CASE with the research directory:

```text
python engine/research/campaign-001/verify.py --case CASE --runner native
python engine/research/campaign-001/verify.py --case CASE --runner docker --behavior --expect-report NATIVE_REPORT
```

Native operation compiles authored source with the pinned original VC5 and never
links or executes generated code. Docker behavior operation compiles, links and
executes only the authored fixture with the existing locked runner. It uses the
same compiler, no network, read-only root/toolchain/source mounts, and only a
fresh private build directory is writable. No original game executes.

When a complete target binding inventory exists, add `--reference` with the
private original executable path. The reference hash, full target extent,
relocation operands and constants must pass the existing comparator. A missing
target inventory yields `not-attempted`; it cannot produce an exact-match claim.
A strict mismatch returns exit code1 even when the focused fixture passes.
Successful compilation without a requested comparison returns0; it does not
certify byte equality. Prerequisite/build failures return2.

`--expect-report` verifies matching source/tool fingerprints and complete
function bytes/relocations across native and Docker output. When the authored
translation unit contains uninlined helpers, the runner instead verifies all
code sections, symbols and relocations and records that broader scope explicitly;
it makes no per-function extent or exact-match claim. Every run records
command timings, source hashes and full private artifact evidence. Truncated
compiler listings are identified; complete object inventories remain checked.

Focused runner checks:

```text
python -m unittest discover -s engine/research/campaign-001 -p test_verify.py -v
python -m unittest discover -s engine/tests -p test_research_artifacts.py -v
```

A near-match is a scheduling category, not a semantic or byte-equivalence
certificate. Unexplained differences, absent bindings and failed tests stay
visible. No byte exclusions, manufactured stack frames or raw-byte insertion
are permitted. No full regression/recovery cycle is part of this pilot.

A case may retain a complete address/body/padding description as
`reference_layout` while leaving `target` null. This records the hash of the
full original span without feeding an incomplete binding inventory to the
strict matcher. Both pilot cases currently use this mode.

## Empty Docker assembly listing

`--native-listing-fallback` is an explicit option for a zero-byte Docker listing.
It requires `--expect-report` from a completed native compile of identical
source and locked tools. Before executing a fixture, the runner validates the
native artifacts, requires complete listing terminators, and compares complete
COFF function bytes and relocations. It preserves the empty Docker listing and
records a separately named copy of the native listing with its provenance.
Default behavior still rejects an empty listing; byte matching is unchanged.
Run focused boundary tests from the repository root:

```powershell
python -m unittest discover -s engine/research/campaign-001 -p "test_*.py" -v
```

These small synthetic runner tests also run in feature-push CI. The full
regression and supplemental behavior job retains its existing batch cadence.
