# Matching reconstruction analysis

The authoritative analysis program is TPM:/WMAIN.EXE in the open Ghidra project.
The original is the hash-pinned 829,952-byte PE32 x86 retail image in target.json,
image base 0x00400000, entry 0x0049CB80. It is inspected statically, never executed
by the matching workflow. The built WMAIN.EXE in each private build directory
is a synthetic console test fixture, not a reconstructed retail application.

The compiler baseline is VC5 RTM 11.00.7022 with linker 5.00.7022. Per-function
optimization follows observed compilation, with /MT and strict warning checks.
The original runtime startup/heap library revision remains unresolved.

## Current subsystem focus

The baseline covers a switch helper, spatial-grid address arithmetic, a plane
coordinate solver, and camera projection setup. View/projection expansion follows
the call graph: mesh submission prepares vertex indices, transforms/projects
vertices, reduces per-vertex clip codes per face, rejects trivially clipped
faces, and passes surviving faces to downstream clipping/queue work.

The face clip reducer has no imported calls, globals, or strings. Its behavior
is supported by the body, its mesh caller, synthetic behavior cases, and complete
VC5 code-section comparison. [function-map.md](function-map.md) records the
analysis metadata change; [module evidence](docs/view-projection-evidence.md)
records observed layouts and unresolved module boundaries.

## Outstanding architecture work

The transform/project kernel now matches with documented inline assembly and
a C4725 compiler-advisory waiver. Follow its downstream clip/queue consumers. Recover original translation-unit boundaries, source/link order,
resources, runtime initialization, imports, data construction, and historical
PE metadata before claiming the complete original binary. Registered function
counts do not establish total application coverage or equivalence of the modern
TPM engine. The generated [progress report](docs/progress.md) is authoritative
for recorded matching results.
