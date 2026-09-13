# Campaign 005 results

**Ten complete function matches, 413 bytes, 205 focused assertions passing.**
All matches are relocation-adjusted: 44 verified global/call operands are resolved
to their original addresses, and then every byte of each complete span matches.
None is a raw unadjusted object-byte match or proof of a complete rebuilt executable.
No bytes, alignment, operand locations or warnings were excluded.

| Function | Original address | Full bytes | Verified operands | Focused checks |
| --- | --- | ---: | ---: | ---: |
| [Stored music volume](../campaign-005-music-volume/README.md) | 0x0041073F | 11 | 1 | 18 |
| [Music attachment status](../campaign-005-music-detached/README.md) | 0x00410592 | 17 | 1 | 12 |
| [Requested music state](../campaign-005-music-state/README.md) | 0x004107B5 | 26 | 2 | 8 |
| [Requested music sequence](../campaign-005-music-sequence/README.md) | 0x004107CF | 26 | 2 | 8 |
| [Disable music](../campaign-005-music-disable/README.md) | 0x00410527 | 47 | 7 | 18 |
| [Enable music](../campaign-005-music-enable/README.md) | 0x00410556 | 60 | 7 | 30 |
| [Save cached music IDs](../campaign-005-music-save/README.md) | 0x0041074A | 61 | 6 | 21 |
| [Read cached music IDs](../campaign-005-music-restore/README.md) | 0x00410787 | 46 | 2 | 33 |
| [Periodic music service](../campaign-005-music-periodic/README.md) | 0x004104EC | 59 | 8 | 21 |
| [Pause music](../campaign-005-music-pause/README.md) | 0x004107E9 | 60 | 8 | 36 |

All ten final native/Docker pairs agree on function bytes and relocation inventories.
Each Docker compile produced an empty listing; the existing explicit fallback used
the independently verified native listing after complete input/code/fixup equality.
Both the empty artifact and provenance hashes are retained in each receipt. The
pinned VC5 compiler, flags, strict comparer and full target intervals were unchanged.

Nine worker candidates matched on the first compile. The 61-byte save helper needed
two: compact boolean normalization emitted 54 bytes, while a natural explicit
conditional return matched the original branch structure. The worker used eleven
candidate compiler invocations; the coordinator used twenty final candidate builds
and ten authored fixture builds. Final verification finished at 03:55:51 UTC after
the 03:37:17 start. These small wrappers are less difficult than Campaign 004's
hierarchy and collision loops; this is improved throughput for this queue, not a
controlled model benchmark or evidence that the remaining engine will match as quickly.

Independent review corrected fixture-only signed overflow in the enable helper's
boundary-value setup before execution. No candidate required a semantic correction.
Tests cover requested-state/sequence distinction, version and sentinel boundaries,
callback ABI/order, ignored failures, counter visibility, callback-mutated globals,
and pause-flag timing. They use authored data and callbacks, not game assets or real
DLL/device/file execution. The [review](review.json) records focused coverage limits.

No shared verification tooling changed, so no additional broad tool-test run was
needed. Lightweight registry, tracking and feature CI checks accompany publication;
full regression and accepted-history promotion remain deferred. Historical accepted
evidence stays at fourteen functions and nineteen records. These ten new focused
matches remain separate from that historical registry.

The shared weekly account meter moved from 26% to 27% during this campaign. Rounded,
account-wide readings cannot measure exact campaign cost. Keep one Sol/medium worker:
review kept pace, but the fixture correction still demonstrates the value of an
independent coordinator. Next prepare remaining music state/volume/detach helpers
and then related sound helpers from the saved shortlist. Preserve the resume
wrapper's trailing alignment if selected; do not shorten its interval to improve
counts. Do not repeat these ten completed assignments.

The original executable was never run or patched. Only authored source, fixtures,
analysis notes, target metadata and hash receipts enter Git. No original content,
compiler binaries, private build outputs, develop merge or upstream submission.
