# Campaign 007 results

**Nine complete matches covering 716 bytes, plus one research candidate.**
All 164 focused assertions pass. The nine exact results resolve 34 independently
verified address operands before every byte of their complete spans matches.
The research candidate adds one verified operand and 70 fully compared bytes.
No bytes or alignment were excluded. Final executable identity and accepted-history
promotion remain separate milestones.

| Function | Address | Full bytes | Status | Focused checks |
| --- | --- | ---: | --- | ---: |
| [sound_tick](../campaign-007-sound-tick/README.md) | 0x00415D13 | 10 | Exact | 7 |
| [sound_get_driver](../campaign-007-sound-get-driver/README.md) | 0x0041786C | 10 | Exact | 5 |
| [sound_get_master_volume](../campaign-007-sound-get-master-volume/README.md) | 0x00417459 | 30 | Exact | 19 |
| [sound_pin_channel](../campaign-007-sound-pin-channel/README.md) | 0x00417826 | 70 | Research: 13 differing bytes | 9 |
| [sound_release_handle](../campaign-007-sound-release-handle/README.md) | 0x0041612F | 92 | Exact | 25 |
| [sound_reset_places](../campaign-007-sound-reset-places/README.md) | 0x00416634 | 91 | Exact | 10 |
| [sound_pause_all](../campaign-007-sound-pause-all/README.md) | 0x00417477 | 120 | Exact | 29 |
| [sound_resume_all](../campaign-007-sound-resume-all/README.md) | 0x004174EF | 120 | Exact | 13 |
| [sound_control_place](../campaign-007-sound-control-place/README.md) | 0x004177AE | 120 | Exact | 15 |
| [sound_activate_place](../campaign-007-sound-activate-place/README.md) | 0x00417711 | 123 | Exact | 32 |

The channel-position helper has the correct 70-byte extent but 13 nonbinding bytes
still differ in aggregate-copy register setup: 18.571429%, above the 5% near-match
threshold. It passes nine focused checks and is parked as research after three
worker experiments. A memcpy variant was 66 bytes; removing const did not improve
the natural aggregate copy. No bytes were hidden and its comparison was not relaxed.
Future finishing work must retain these attempts and the original caps.

Six exact functions matched on the first worker compile. Reset, placement control
and activation each needed two; natural guard grouping changed the branch layout.
The worker used 15 candidate compiles in total. Parent review used 20 final candidate
builds and 10 authored fixture builds. Implementation finished at 04:53:43 UTC;
final independent verification and source-hash reconciliation completed by 04:55:58,
after the 04:32:17 start. This workload is not a controlled model benchmark.

The placement-reset loop intentionally keeps its first-record pointer fixed:
positive counts repeatedly restore only the first record's active field and clear
its deadline. Pause/resume mark flags before calling the selected 2D/3D backend and
observe changes to later slots. Release clears the selected handle only after its
callback. Activation copies world time before random adjustment, skips equal or
unordered spans under pinned VC5 x87 behavior, and reloads span/deadline after the
random callback. These observed details are retained rather than repaired.

Independent review made three fixture-only improvements before execution: an
explicit positive-zero bit test instead of NaN-sensitive equality; a real callback
bounds guard and repeated-pause check; and independent exact-rational expected
float bits instead of repeating the activation formula. No candidate required a
semantic correction. Final worker review found no material remaining issues.
See [review.json](review.json) and [metrics.json](metrics.json) for all attempts,
source hashes, counts and limitations.

All ten fresh native/Docker pairs agree on code and fixups. The existing verified
native-listing fallback was used for empty Docker listings, retaining their hashes
and provenance. No shared matcher, flags, targets or toolchain files changed;
therefore no extra broad tool-test rerun was needed. Lightweight registry/tracking
and feature CI accompany publication; full regression stays deferred. Historical
accepted evidence remains fourteen functions and nineteen records.

Eight generated Ghidra names and all ten comments were updated and saved. The
driver getter's pointer return was corrected. The activation calling convention
remains a metadata limitation: the MCP setter rejects explicit cdecl and Ghidra
retains an automatic this parameter. Full assembly and the caller's one push/pop4
independently establish the source's one-pointer cdecl contract; source and byte
evidence are unaffected. The pending correction is recorded, not reported applied.

The first account reading at 04:37:56 was 28% used and the final reading was 29%.
These rounded account-wide observations cannot determine exact campaign cost.
Continue one Sol/medium worker with prepared packets, parent review and the same
caps. Next prioritize untouched sound-field accessors, channel update/lifecycle
helpers or the next documented module. Do not repeat this batch or omit alignment
from the unattempted muted/resume wrappers to obtain an exact label.

Only authored C, fixtures, target metadata, research notes and hash receipts are
committed. The original executable and real game backends were never executed. Licensed
compiler binaries and private build artifacts remain outside Git. Authored
fixtures ran only in locked Docker.
No develop merge or upstream submission was made.
