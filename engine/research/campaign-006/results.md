# Campaign 006 results

**Ten complete function matches, 603 bytes, 258 focused assertions passing.**
All matches are relocation-adjusted: all 52 global/call operands are independently
verified and resolved to their original addresses before comparing every byte of
the complete spans. No bytes or alignment are excluded. These are function-level
matches, not a rebuilt executable or accepted-history promotion.

| Function | Address | Full bytes | Verified operands | Focused checks |
| --- | --- | ---: | ---: | ---: |
| [music_set_state](../campaign-006-music-set-state/README.md) | 0x004105A3 | 107 | 11 | 45 |
| [music_set_volume](../campaign-006-music-set-volume/README.md) | 0x004106CC | 115 | 11 | 42 |
| [music_detach](../campaign-006-music-detach/README.md) | 0x0041046C | 128 | 15 | 58 |
| [sound_stop_channel](../campaign-006-sound-stop-channel/README.md) | 0x00417368 | 17 | 1 | 10 |
| [sound_find_record](../campaign-006-sound-find-record/README.md) | 0x00417043 | 24 | 2 | 15 |
| [sound_deactivate_place](../campaign-006-sound-deactivate-place/README.md) | 0x0041778C | 34 | 1 | 10 |
| [sound_test_flags](../campaign-006-sound-test-flags/README.md) | 0x00417876 | 45 | 1 | 13 |
| [sound_enable](../campaign-006-sound-enable/README.md) | 0x004178D1 | 41 | 4 | 14 |
| [sound_disable](../campaign-006-sound-disable/README.md) | 0x004178A3 | 46 | 5 | 23 |
| [sound_stop_all](../campaign-006-sound-stop-all/README.md) | 0x004176E3 | 46 | 1 | 28 |

All ten worker candidates matched on their first compile. The coordinator reviewed
each frozen source and fixture, then performed one fresh native/Docker pair per
case. Code and fixup inventories agreed before authored fixtures ran. This used
ten worker candidate compiles, twenty final candidate compiles and ten fixture
compiles. Final verification finished at 04:24:09 UTC after the 04:05:17 start.
The helper-heavy queue supports good throughput; this is not a controlled model
benchmark or a prediction for larger engine functions.

The state setter caches before locking and reloads state for the backend; its
return is not evidence of successful playback. Volume caches only after unlock,
and the pinned VC5 x87 equality path skips unordered quiet NaNs as well as equal
values. Detach optionally resumes, resets sentinel IDs and clears initialized last.
Sound enable skips zero, while disable skips exactly one. The sound flags helper
returns masked bits, not normalized truth. Sound placement records are 60 bytes;
sound descriptors are 64 bytes. Stop-all visits exactly twelve slots in order.

Parent review caught and corrected one fixture-only out-of-bounds snapshot before
execution: the sound flags test used a valid interior pointer for negative indexing
but copied a full array from that interior address. The corrected fixture copies
both full owned arrays from their true bases. No candidate required a correction.
A final independent worker review covered all ten sources, fixtures and receipts
with no material remaining findings. See [review.json](review.json).

The existing explicit native-listing fallback was used for all ten Docker builds:
Docker emitted empty listings, while complete input/code/fixup agreement allowed
use of each native peer listing. Empty artifact and provenance hashes remain in
receipts. No compiler flags, strict matcher, warnings or target extents changed.
No shared tooling changed, so no broad tool-test rerun was needed. Lightweight
registry/tracking checks and feature CI accompany publication; full regression
and accepted-history promotion remain deferred. Historical acceptance stays at
fourteen functions and nineteen records.

The shared weekly usage meter read 27% both at campaign start and after verification.
Rounded account-wide readings do not imply zero usage or exact campaign cost.
Continue one Sol/medium worker with independently prepared packets and parent review.
Next select untouched team-not-done sound helpers or the next documented module;
preserve full alignment for any selected resume/status wrapper. Do not repeat
these ten completed assignments or reset exhausted earlier attempt caps.

Only authored C source, fixtures, analysis metadata and hash receipts enter Git.
The original executable and real game backends were never executed or patched.
Compiler binaries, original content and private artifacts remain outside Git.
No develop merge or upstream submission was made.
