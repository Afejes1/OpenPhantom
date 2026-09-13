# Campaign 008 results

**Eight complete relocation-adjusted matches covering 900 bytes.**
All 454 focused assertions pass. Every byte of each complete span
matches after resolving 69 independently verified address operands. The getter
and setter include their entire generated switch tables: 24 bytes each.
No code, tables, alignment or warnings were excluded.

| Function | Address | Full bytes | Status | Focused checks |
| --- | --- | ---: | --- | ---: |
| [sound_get_field](../campaign-008-sound-get-field/README.md) | 0x0041668F | 128 | Exact | 17 |
| [sound_set_field](../campaign-008-sound-set-field/README.md) | 0x0041670F | 120 | Exact | 44 |
| [sound_shutdown](../campaign-008-sound-shutdown/README.md) | 0x00415AB5 | 131 | Exact | 115 |
| [sound_play_name](../campaign-008-sound-play-name/README.md) | 0x00416787 | 60 | Exact | 20 |
| [sound_play_name_volume](../campaign-008-sound-play-name-volume/README.md) | 0x004167C3 | 92 | Exact | 38 |
| [sound_periodic](../campaign-008-sound-periodic/README.md) | 0x00415D1D | 109 | Exact | 144 |
| [sound_play_index](../campaign-008-sound-play-index/README.md) | 0x00417143 | 94 | Exact | 28 |
| [sound_poll_channel](../campaign-008-sound-poll-channel/README.md) | 0x00415D8A | 166 | Exact | 48 |

All eight matched on their first worker candidate compile. Parent review then
performed sixteen fresh candidate builds (one native and one Docker per case)
and eight authored fixture builds. Sources remain C compiled by the pinned
VC5 RTM toolchain with /Od /MT. No candidate needed a semantic rewrite.
This is a prepared workload, not a controlled model-speed benchmark.

The sound getter preserves floating field bits and converts the signed integer
field through x87. The setter uses the pinned CRT conversion for its integer
field; its numeric fixtures stay finite and inside the signed-int range.
Shutdown preserves the original callback order and clears initialized last.
Named playback preserves the return across the final volume reset. Periodic
service compares enable/pause flags exactly with 1, and does not re-test pause
inside its twelve-channel loop. Indexed playback applies category8 only when
the current category is negative. Polling reads record and flags again after
the status callback, clears the active bit before release, and preserves other bits.

Independent fixture review corrected signaling-NaN sentinels that would pass
through an x87 float-return helper, added explicit signed-rounding expectations,
tested initialized changes during shutdown/playback callbacks, and added a
category-at-callback oracle to indexed playback. Polling captures use actual
bounds guards and independent complete expected flags. These changes strengthen
the authored tests; they do not alter the reconstructed candidate source.

A timing audit found unsupported manually entered worker timestamps. Those were
replaced with recorded clock or filesystem/report times and an explicit basis
in every worker log. Compiler-report times are not presented as full task
durations; packet-reading start and review time are not individually measured.
The coordinator records the final handoff and verification observations separately.
See [metrics](metrics.json) and [review](review.json).

All eight native/Docker pairs agree on complete code and fixups. The existing
verified native-listing fallback was used for empty Docker listings, retaining
hashes and provenance. The matcher, toolchain, common flags and accepted history
are unchanged. Full regression remains deferred; feature CI checks tracking,
annotations and focused tooling boundaries. Historical accepted evidence is still
fourteen functions and nineteen records. These focused results await deliberate
batch acceptance and do not establish a matching linked executable.

Seven generated Ghidra names and eight comments were updated and saved; the
existing indexed-playback name was retained. Older inferred calling conventions
can still create phantom ECX/this parameters in decompiler text. The verified
source interfaces come from caller stack setup and callee accesses; the known
bridge convention-edit limitation remains documented rather than silently waived.

All eight addresses are team-reported not done in bp/bapsound.c. Only authored
source, fixtures, metadata, notes and hash receipts are committed. Original game
content, compiler binaries and private build artifacts remain outside Git. The
original game and real backends were never executed; authored fixtures ran only
in the locked Docker container. No develop merge or upstream submission occurred.

Continue with the same one-worker strategy and untouched sound lifecycle helpers.
The fixed overnight stop remains 08:48 UTC, with no new implementation after 08:38.

The rounded shared-account usage meter was29% near batch start and30% at05:35:22UTC. These observations do not isolate exact campaign usage.
