# Campaign017 results

**Six complete functions match with zero differing bytes after all call operands
are resolved:517 bytes,11 verified calls and281 passing focused checks.**
All six were reported not_done in the team's bp/bapworld.c manifest. Each matched
on the worker's first native candidate compile; no candidate bodies were rewritten.

| Function | Complete bytes | Verified calls | Focused checks |
| --- | ---: | ---: | ---: |
| [world_skip_chunk](../campaign-017-world-skip-chunk/candidate.c) | 93 | 4 | 55 |
| [world_read_emitters](../campaign-017-world-read-emitters/candidate.c) | 94 | 1 | 35 |
| [world_read_sprite_refs](../campaign-017-world-read-sprite-refs/candidate.c) | 108 | 2 | 75 |
| [world_read_palettes](../campaign-017-world-read-palettes/candidate.c) | 90 | 1 | 39 |
| [world_free_scripts](../campaign-017-world-free-scripts/candidate.c) | 74 | 1 | 55 |
| [world_restart_clock](../campaign-017-world-restart-clock/candidate.c) | 58 | 2 | 22 |

Each final source received independent static/fixture review and a fresh native
compile plus locked Docker compile/behavior run. All code and relocations agreed.
Original intervals extend through the next function entry and contain no omitted
padding or tables. The tested native listing fallback records the empty Docker
listing and verifies full object/fixup equality before using its peer listing.
[Metrics](metrics.json) and [review](review.json) preserve every attempt and receipt.

The first worker compile runs totaled8.50 seconds. Six successful final verification
pairs totaled42.70 seconds. One additional pair reached a rejected fixture compile:
two script-pointer array declarations lacked their own asterisks. Parent review
fixed that test code and retained the failed receipt. A private wrapper initially
passed a case file instead of the directory and was rejected before any compiler
ran. All14 parent candidate compiles, seven fixture compile attempts and the zero-
compile wrapper failure are accounted for. There were no warning waivers.

Review strengthened guarded whole-object expectations, callback-visible state,
nonzero wrapped size products and explicit expected payload sizes. The chunk-skip
fixture's integer pointer sentinels were replaced with owned addresses. Those
changes affect fixtures only. The source/fixture timestamps in worker logs start
at source creation; they are not measurements of all model reasoning time.
Preparation ran12:02-12:15:37 UTC alongside the preceding acceptance work.
Campaign implementation and final verification ran12:15:37-12:27:20 UTC, with
publication afterward inside the13:00:37 UTC deadline.

Three generated Ghidra function names now describe chunk skipping, script-data
cleanup and clock restart. The existing reader names were retained. Caller
re-decompilation confirmed their dispatch, destructor and level-loader roles;
Ghidra changes were saved. The bridge retained a stale thiscall convention on
chunk-skip despite accepting the plain prototype, and rejected explicit cdecl.
Its saved comment warns about the spurious this argument. The actual cdecl ABI
comes from the two stack arguments, real callees and matching source, not that
remaining decompiler artifact.

These are focused research results; accepted registered functions remain23 with
22 immutable build records. Shared world/header types, actual backend integration
and final executable layout remain pending. The stream fixture observes32-bit
argument words and does no actual reading for zero, NULL or huge-size inputs.
Those tests prove wrapper forwarding, not safety of such inputs in the real
compressed reader. Reconcile its signed-division parameter interpretation before
shared integration. No full-game or whole-executable identity is claimed.

No full regression was repeated for this research batch. The preceding colormap
acceptance checkpoint already passed full CI. This push uses the existing focused
runner, tracking and annotation checks. Shared weekly usage read40% near completion
versus39% at the start of the4.5-hour window; this is rounded account-wide usage,
not an isolated cost measurement.

Published source checkpoint:9ef616508ba406c3e697f546d6092cec12ce4bd0.
[Focused CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34757543252) passed.
