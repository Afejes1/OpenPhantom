# Campaign025 results

Six untouched team-not_done functions match all 871 complete bytes. All 7,829 focused checks pass. Every case matched on its first implementation compile.

| Function | Bytes | Checks |
|---|---:|---:|
| effects_object_destroyed | 47 | 165 |
| fade_reset | 40 | 18 |
| fog_start | 193 | 577 |
| fog_tick | 162 | 732 |
| shield_destroy | 333 | 5976 |
| tint_start_full | 96 | 361 |

Independent review covered full spans, every relocation operand, calling conventions, callback order, guarded state, and x87 rounding. Final native and Docker code/fixup inventories agree for all six functions. The compiler image, source snapshots, report hashes, and verified native listing fallback are recorded per case.

The shield destructor exercises all eight helper calls, including NULL arguments, post-callback clears and live count updates. Fog initialization preserves full-word integer RGB input followed by masks and late world access. Fog interpolation uses independent dyadic oracles; arithmetic-NaN payload identity is outside the behavior-fixture claim. The tint fixture verifies caller-side binary32 rounding; original clock producer source precision remains unresolved. Fade reset uses the actual reconstructed tint-stop helper, but its lifecycle provenance remains unresolved because no direct callers were found.

Existing Ghidra names and comments were retained. The fog initializer RGB signature was corrected to int, its caller re-decompiled, and the project saved. No original game was executed or modified. No byte exclusions or warning waivers.

No full regression suite ran for this research commit. Registered acceptance remains 43 functions/29 events. These six exact functions await canonical integration and final linked placement.
