# Acceptance checkpoint002

**Both colormap functions are promoted through the existing acceptance recorder.
The registered total is23, up from21, with22 immutable build records.**

The complete73-byte loader and68-byte free loop still match after integration:
141 bytes, ten explicitly verified address operands and zero differing bytes.
The frozen bodies changed only their shared-header include. All prior21 function
specifications remained unchanged and their comparisons passed in the same build.

The combined fixture passed321 checks using the actual reconstructed load and free
functions together. It covers repeated loads, failure, last-slot insertion and
capacity overflow behavior, ordered freeing, callback-mutated live counts,
retained slots, nonpositive counts and final reset. Only synthetic backend
callbacks and authored fixture code execute inside locked, networkless Docker.

A clean source commitc1146a9a580d517ca5bdb84eaf28568f0942e868 produced the registered
VC5 build. [Record22](../../evidence/runs/000022-66c3fa81cf8b217f.json) contains the
source snapshot, compiler lock, original target identity and every result.
[Result metadata](result.json) records private artifact hashes and build location.
Reproduce using engine/verify.py build, test and record with the owned original
reference and locked private toolchain at that source revision.

The100 synthetic tests and19 focused runner tests passed locally.
[Full integration CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34756468991)
also passed, including the modern x86 supplemental fixture and annotations.
The early99-pass/one-missing-fixture preparation run is retained in the review;
the completed fixture subsequently passed the complete suite. No warning waiver,
byte exclusion, new matcher rule or source rewrite was needed.

These are exact function comparisons after verified address resolution. Original
translation-unit boundaries, final linked addresses and whole-executable identity
remain pending. This checkpoint promotes existing exact research; it does not
count those two functions again as new reconstruction output.
