# Verification cadence for the reconstruction branch

The contributor requested batched reconstruction on 2026-09-12. Until integration
into develop, prioritize matching related functions over repeating complete
regression and recovery checks for every commit.

## During a batch

Aim for three to five related functions, adjusting the batch to function size
and unresolved dependencies. Inspect the original statically, reconstruct the
source, and compile each changing candidate with its locked VC5 frontend/options.
Compare the full function and padding with every relocation verified. A failed
candidate stays visibly unverified. Behavior checks focus on the affected
function and its immediate dependencies, including material edge cases.

Commit or push useful source/research checkpoints without rebuilding every
accepted function. Such checkpoints may retain a historical whole-source
fingerprint; the progress report must say so. Do not manufacture a full-build
record or relabel a candidate accepted merely to make that report current.

## Batch and integration checkpoints

After several functions are ready, run one full original-toolchain build,
comparison and behavioral fixture, then append one immutable build record.
Run broader synthetic tooling tests when verifier, runner, registry or recovery
code changes, and the modern behavior suite at meaningful batch checkpoints.
Repeat a build only for a concrete reproducibility question, a changed toolchain,
or a failure. Do not automatically perform two builds per batch.

Before merging into develop, run the full synthetic, modern and original-compiler
checks. A complete matching executable remains a separate objective. There are
no relaxed byte masks, changed spans or new exceptions in this cadence.

## CI and recovery

Ordinary feature-branch pushes validate the evidence chain, generated reports, team manifest
and source annotations. They do not run the complete regression suite.
Pull requests, workflow_dispatch, and pushes to develop/main run the full
synthetic/modern workflow.
For tooling changes, run the relevant local tests and a full checkpoint before
relying on new results; request a manual workflow run when it is available.

Recovery packaging and fresh-restore rebuilds belong at toolchain/recovery
changes or substantial milestones, not every function. The latest receipt
continues to certify only its named source checkpoint.

## Bounded campaign assignments

[Campaign rules](campaign-rules.md) define preparation, worker ownership, time
caps, independent review and the 5% near-match scheduling category. That category
does not change this document's exact acceptance or batch-checkpoint requirements.
