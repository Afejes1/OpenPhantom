# Campaign 009 results

**Six complete relocation-adjusted matches covering 1,025 bytes.** All 583
focused assertions pass. Every byte of each complete span matches after resolving
48 independently verified address operands. No bytes, alignment or warnings
were excluded. All six addresses are team-reported not done in bp/bapsound.c.

| Function | Address | Full bytes | Status | Focused checks |
| --- | --- | ---: | --- | ---: |
| [sound_init_channels](../campaign-009-sound-init-channels/README.md) | 0x0041609E | 145 | Exact | 315 |
| [sound_start_sample](../campaign-009-sound-start-sample/README.md) | 0x00417659 | 138 | Exact | 26 |
| [sound_load_ref](../campaign-009-sound-load-ref/README.md) | 0x0041618B | 167 | Exact | 52 |
| [sound_free_extra_refs](../campaign-009-sound-free-extra-refs/README.md) | 0x00416589 | 171 | Exact | 75 |
| [sound_set_master_volume](../campaign-009-sound-set-master-volume/README.md) | 0x00417379 | 224 | Exact | 56 |
| [sound_play_voice](../campaign-009-sound-play-voice/README.md) | 0x004172B4 | 180 | Exact | 59 |

Worker compiler attempts were 2, 1, 2, 4, 2 and 1: twelve in total. Final parent
verification used twelve fresh candidate compiles and six authored fixture
builds/executions. An earlier initializer fixture failed to compile under VC5
because its static const zero oracle lacked an explicit initializer. That failed
pair adds two candidate compiles and one failed fixture compile, with no fixture
execution. The corrected fixture passed without a warning waiver. All attempts
and their hashes remain in [metrics](metrics.json) and [review](review.json).

All sources are C compiled with the pinned VC5 RTM toolchain and /Od /MT.
The initializer and resource loader explicitly request the standard memset and
strcpy compiler intrinsics to reproduce the observed inline operations. These
source directives are recorded; the compiler version and common flags are
unchanged. Cleanup reached a match using a natural cursor/alias traversal after
three experiments differed in stack-slot selection. No raw instruction bytes or
fabricated stack layouts were introduced.

Channel initialization preserves all twelve complete record clears, callback
order, null allocation behavior and final flag/slot stores. Sample start fixes the
2D/3D branch before callbacks while reloading the live handle and flags afterward.
Resource loading uses an independent stack name copy and preserves the signed
failure-counter threshold. Extra-reference cleanup captures next before custom
free, preserving live resource, registry and counter changes made by callbacks.

The master-volume clamp uses inclusive comparisons. The initial strict-comparison
suggestion was equivalent only for finite inputs; the final grouping also
preserves the original NaN-to-zero path. Independent fixtures assert actual gain
bits at signed input boundaries, finite clamp results, quiet NaN, skipped channel
types and callback changes to gain, later channel type and driver. Voice playback
preserves prior-handle release, mode changes, positional field setup and input-lock
selection, without restoring state after the final playback callback.

Parent review strengthened five fixtures before final verification: full record
clears and final bytes, stack-copy independence, owned-pointer bounds and next
capture, numeric/state oracles, and mode/handle observations at callbacks. These
corrections changed authored tests, not the frozen candidate source. All six
receipts also received a final worker read-only review.

Five final Docker runs required the existing explicit native-listing fallback,
used only after complete native/Docker code, input and fixup equality. The
master-volume Docker run produced its own valid listing and needed no fallback.
Each receipt retains the applicable provenance. Full regression remains deferred;
shared tooling is unchanged. Historical accepted evidence remains fourteen
functions and nineteen records. These focused matches await deliberate batch
acceptance and do not establish a byte-identical linked executable.

Four generated Ghidra names and six comments were updated and saved; two trusted
names were preserved. Affected callers were re-decompiled. Some older inferred
calling conventions can still introduce phantom ECX/this parameters in decompiler
text; source ABI is independently established from caller/callee instructions.
The known metadata bridge limitation is recorded, not treated as a byte exclusion.

Worker logs identify filesystem/report timestamp bases instead of claiming full
measured task durations. The coordinator observed all final receipts by 06:01:05
UTC and revalidated their hashes during aggregation. This prepared batch is not
a controlled model-speed benchmark. The rounded shared-account meter moved from
30% near batch start to 31% near publication; exact campaign cost is unavailable.

Only authored sources, tests, metadata, notes and hash receipts are committed.
Original game content, compiler binaries and private build artifacts remain
outside Git. The original game was inspected statically; authored fixtures ran
only in locked Docker. No develop merge or upstream submission occurred.

Continue the same one-worker strategy with untouched sound lifecycle helpers.
The overnight hard stop remains 08:48 UTC; no new implementation after 08:38 UTC.
