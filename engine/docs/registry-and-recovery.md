# Function registry and private recovery

This fork remains an unpublished experiment. The contributor confirmed direct
discussion with the authors and authorized continuing this work on 2026-09-12.

## What is tracked

[registry.json](../registry.json) assigns each registered function to a research
module and records purpose, confidence, evidence, behavioral test location,
compiler quirks, and exceptions. Module assignments are research groupings until
original translation-unit boundaries are independently established.
[target.json](../target.json) owns exact addresses, source paths, symbols,
compiler flags, complete extents, and relocation bindings.

The generated [progress report](progress.md) derives status from public
[evidence/runs](../evidence/runs). Those JSON records are created only after the
private verifier rechecks source and tool fingerprints, object hashes, the
original reference, all comparison bytes, and the VC5 behavioral result.
Commands use portable ENGINE/BUILD/TOOLCHAIN placeholders. Tool inventories
use relative paths and retain every selected compiler, header, and library hash.

A record is one verified build, not one manually entered claim. It includes
the source commit and full source snapshot, the runtime image ID, commands,
per-function specifications and results, body/padding/address byte accounting,
and the preceding record's digest. Files are created exclusively and never
overwritten by the recorder. Git provides reviewable history; these hashes
detect drift, not malicious fabrication by someone authorized to rewrite Git.
Private raw binaries, full local logs, and original bytes are not published.

The chain permits additional functions but rejects deletion or changes to the
accepted functions' definitions, comparison spans, flags, or bindings. Toolchain
changes need a separately reviewed calibration series. The existing check/accept
commands remain stricter: they require the identical target manifest and private
lock. An intentional definition correction requires an explicit evidence-backed
migration, not resetting history or silently changing the comparison denominator.

Quirks describe source choices that still match every compared byte. Exceptions
describe unresolved exclusions and are never counted as matching by the recorder.
Raw code equality, equality after verified relocation resolution, behavioral
evidence, and whole-file equality remain distinct. No current record certifies
original linked placement. A stale source snapshot is labelled historical.

## Record and review a build

Commit source, target, registry, and tool changes before building so each public
record identifies a clean source checkpoint. Then, from the repository root:

    python engine/verify.py build --reference C:/PrivateGame/WMAIN.EXE
    python engine/verify.py test --reference C:/PrivateGame/WMAIN.EXE --build engine/build/BUILD_ID
    python engine/verify.py record --reference C:/PrivateGame/WMAIN.EXE --build engine/build/BUILD_ID
    python engine/progress.py
    python engine/progress.py --check

Review and commit the new JSON record and progress report. A second build from
that clean commit can append another record. Evidence/report files are excluded
from the compiled-source snapshot, avoiding a self-referential hash cycle.
CI validates the public registry/history/report and synthetic failure tests.
Actual matching still requires the private compiler and original reference.

## Create a private recovery checkpoint

The host requires Windows, Python 3.13+, Git, and a working Docker Linux engine.
Their installers and the retail game are not included. The checkpoint contains:

- A Git bundle of the clean source commit and its reachable history.
- Every file in the configured compiler/include/library fingerprint inventory.
- A Docker save of the exact immutable runtime image.
- Relative configuration, acquisition provenance, and checksums for every file.

The output is a new directory under engine/private, excluded from Git. Do not
publish it or add it to GitHub artifacts. It contains the private compiler.

    python engine/restore.py pack --destination engine/private/recovery/checkpoint-UNIQUE

Retain the printed manifest SHA-256 separately from the bundle. No existing
checkpoint, source directory, compiler installation, or Docker image is deleted.
Interrupted output remains an incomplete checkpoint; choose a new name when
retrying. The final manifest is written only after all payload files exist.

## Restore into a fresh checkout

Use the retained manifest SHA-256, not one recomputed from an untrusted bundle:

    python engine/restore.py inspect --bundle engine/private/recovery/checkpoint-UNIQUE --manifest-sha256 RETAINED_SHA256
    python engine/restore.py restore --bundle engine/private/recovery/checkpoint-UNIQUE --manifest-sha256 RETAINED_SHA256 --destination engine/private/restores/restore-UNIQUE --docker "C:/Program Files/Docker/Docker/resources/bin/docker.exe"

Restore validates every file, the complete inventory, relative paths, source
commit, and runtime identity. It rejects traversal, Windows alternate streams,
reserved names, links, unlisted files, corruption, and an existing destination.
It creates a detached source checkout and a private configuration with rebased
paths. Loading the saved image preserves its identity and requires no download.

Enter the restored repository and run configure to make a new local path lock,
then build/test/record with your separately supplied retail executable. The
portable toolchain fingerprint must match the existing history despite the new
absolute paths. A restore receipt verifies extraction only; it explicitly says
rebuild_verified=false until an actual build is separately demonstrated.

A successful same-machine fresh-checkout restore is useful recovery evidence.
It is not proof of recovery on another physical machine, an off-machine backup,
or independence from the installed host prerequisites. Copy the completed
private bundle and separately retained checksum to your chosen backup medium
before treating it as protection against loss of this computer.
