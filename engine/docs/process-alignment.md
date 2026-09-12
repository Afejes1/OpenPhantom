# Process alignment and reproducibility

Checkpoint audit: 2026-09-12, source revision
`1e238219f96979319bc203071522f4259382c8f3`. Read the generated
[progress report](progress.md) for later function counts and evidence.

## Alignment with OpenPhantom

The current upstream `CONTRIBUTING.md` and `engine/engine-identification.md`
were checked against this fork and were identical. Their upstream Git blob IDs
were `17922f16e2024e9173b810b9b9c01281cae34508` and
`f9258fbcc95262b2a0fba1c70a07bcb61dddcfc1`, respectively.

The matching build uses actual Visual C++ 5.0 RTM `cl.exe` 11.00.7022 and
`link.exe` 5.00.7022 through wibo, as the upstream compiler research describes.
`/MT` and per-source `/Od` or `/O2` choices are recorded. Ghidra supplies static
analysis evidence; the modern TPM engine is a separate project and is not a
certificate of equivalent original code generation.

Reconstructed source, evidence, tests, and tooling stay in `engine/`. The
original game is read from the user's separate local copy. Original executables,
assets, extracted data, and raw decompiler output are not added to Git. The
contributor has confirmed direct discussion with the authors; this branch
remains an unpublished proposal/experiment. Following the published process
does not imply upstream acceptance of every implementation choice.

One scoped compiler diagnostic waiver, C4725 in `transform_project.c`, preserves
the original x87 divide instructions. Warning level 4 is restored afterward;
other diagnostics remain errors. The registry records the waiver and its
[evidence](x87-projection-evidence.md). It is not an excluded byte or an
upstream-approved exemption. Inline assembly is identified explicitly, and the
original historical source text is not claimed to have been recovered.

## What byte matching proves

At this checkpoint seven complete function code sections total 1,121 bytes,
including 35 bytes of alignment padding. Two sections match directly. Five match
after resolving 120 address-operand bytes using explicit symbol/addend or
constant bindings, checked against the original executable's relocation data.
The resolved full-span hashes agree. No comparison bytes are excluded.

These results establish matching function code generation under the recorded
conditions. They do not establish original link placement or a byte-identical
retail executable. The original CRT startup/heap library revision, translation
unit boundaries, full function inventory, link order, data, resources, and PE
metadata remain unresolved. The complete-file verifier excludes nothing and
correctly rejects the current console test fixture as a retail replacement.

## Where the reproducible pieces live

| Item | Committed to the fork? | Recovery source |
|---|---|---|
| Reconstructed source, tests, verifier, registry and build commands | Yes | Git history and private Git bundle |
| Dockerfile, pinned Debian/wibo identifiers and setup instructions | Yes | Git history |
| Compiler/header/library fingerprints and runtime image identity | Yes, in sanitized build records | Verification history |
| VC5 compiler, headers and libraries | No | Private recovery bundle |
| Saved Docker runtime image and downloaded wibo binary | No | Private recovery bundle/image |
| Original retail executable and assets | No | User's separately retained game copy |
| Windows, Python, Git and Docker host installation | No | Separately installed prerequisites |

The container contains Debian and wibo. It mounts the private VC5 tree read-only
and runs the real compiler and linker. Each build has a fresh writable output
directory, no network, and no access to the retail reference or Docker socket.
The Windows Python controller reads the retail reference for comparison. Only
the newly compiled synthetic behavior fixture is executed, inside the container.

A Git clone alone is therefore not a complete offline toolchain recovery.
The private checkpoint adds the compiler inventory, exact saved runtime image,
source history, relative configuration, provenance, and checksums. The compiler
copy came from the documented community preservation archive; it has not been
authenticated against original Microsoft installation media.

The [seven-function recovery proof](recovery-20260912-seven-functions.json)
records a fresh-checkout restore with all 53 tooling tests, the VC5 behavior
fixture, and all seven byte comparisons passing. It includes the checkpoint's
manifest digest separately from the private payload. This was tested on the same
physical host. Off-machine backup and recovery on a second host remain unverified.

## Function tracking and regression process

`registry.json` records each function's purpose, analysis confidence, research
module, source/evidence/test locations, quirks, exceptions, and diagnostic
waivers. `target.json` owns its original address, complete extent, symbol,
compiler flags, and relocation bindings. Chained build records preserve the
source commit and hashes, compiler/header/library hashes, runtime image,
commands, object hashes, behavior results, and complete byte accounting.

For each new group: analyze the original, reconstruct source, add focused tests,
compile with VC5, compare the full code sections, and resolve every discrepancy.
Commit the source, build/test/record from that clean checkpoint, then repeat from
a fresh build before continuing. The history validator rejects silently changed
accepted definitions; changed source fingerprints make old results historical.
No manually entered status is sufficient to accept a function.

## GitHub Actions and costs

Checked on 2026-09-12: `Afejes1/OpenPhantom` is public and the engine workflow uses
standard `windows-latest`. It runs the synthetic verifier tests, registry/history
checks, source annotation lint, and a supplemental modern x86 behavior fixture.
It does not perform VC5/retail matching and configures no artifact uploads or
cache. A green run is evidence for those stated checks only.

Under [GitHub's current billing rules](https://docs.github.com/en/billing/concepts/product-billing/github-actions),
standard hosted-runner execution in public repositories is free. This workflow
therefore does not consume the private-repository monthly minute allowance.
Changing repository visibility, using larger runners, or adding billable storage
can change costs. This is a configuration assessment, not an audit of the user's
whole GitHub bill. The local Docker builds are outside GitHub Actions and use the
local computer's resources.
