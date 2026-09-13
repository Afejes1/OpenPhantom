# Campaign 010 results

**Three complete relocation-adjusted matches covering 671 bytes, two reviewed
near matches, and one research candidate.** All 436 focused assertions pass.
The six complete spans cover 1,421 bytes with 83 verified address operands;
the three exact spans account for 34 operands. No bytes, alignment or warnings
were excluded. These addresses are team-reported not done in bp/bapsound.c.

| Function | Address | Full bytes | Status | Focused checks |
| --- | --- | ---: | --- | ---: |
| [sound_remove_level](../campaign-010-sound-remove-level/README.md) | 0x00415C40 | 211 | Research: 12 differing bytes | 57 |
| [sound_module_init](../campaign-010-sound-module-init/README.md) | 0x004159F0 | 197 | Exact | 31 |
| [sound_register_level](../campaign-010-sound-register-level/README.md) | 0x00415B38 | 264 | Near: 12 differing bytes | 39 |
| [sound_add_extra_ref](../campaign-010-sound-add-extra-ref/README.md) | 0x0041705B | 232 | Exact | 214 |
| [sound_free_channel](../campaign-010-sound-free-channel/README.md) | 0x00417567 | 242 | Exact | 37 |
| [sound_play_by_name](../campaign-010-sound-play-by-name/README.md) | 0x004171A1 | 275 | Near: 13 differing bytes | 58 |

Register-level differs in 12/264 bytes (4.545%); named playback differs in
13/275 bytes (4.727%). Both have complete comparisons and passing independent
fixtures, so they enter the finish-later queue. Remove-level differs in 12/211
bytes (5.687%) and remains research. The 5% rule schedules follow-up work; it
never changes exact acceptance. Parent independently resolved the two level
loops and identified their twelve residual differences as exchanged -4/-8 local
stack displacements. Named playback's residual differences are recorded without
claiming an independently proven cause. None of these three is counted exact.

All sources are C, compiled with the pinned original VC5 RTM toolchain using
/Od /MT and unchanged strict common flags. Extra-reference insertion and named
playback explicitly request the standard strcpy intrinsic. Insertion reached an
exact match using natural early-return/for traversal. No raw instruction bytes,
fabricated stack storage, warning waivers or byte exclusions were introduced.

Fourteen worker candidate compiles cover all recorded experiments. Parent final
verification used twelve fresh candidate compiles and six authored fixture
builds/executions. An earlier insertion fixture pair adds two candidate compiles
and one failed fixture compile, with no execution: C character literals in a
ternary required an explicit bounded char conversion under strict VC5 warnings.
Its failed reports and resolution are preserved in [review](review.json).

Independent review caught a substantive defect in register-level: the first two
worker drafts inverted the preload flag gate, and their fixture shared that
inversion. The third source loads only when bit 0x2000 is clear. The final fixture
uses independently identified records and explicit skip/failure/success order.
Those superseded source attempts remain visible; their provisional results are
not acceptance evidence. This finding supports retaining independent review
before any near-match classification.

The parent strengthened all six fixtures: signed/live world traversal, final
initialization timing and partial failures, independent flag gates and full
record bytes, owned-pointer and bounded-copy checks, live channel/resource
reloads, and named playback's full descriptor/float-bit/category oracle. Tests
include callback mutations, quiet NaN and signed zero where applicable. All final
sources and receipts also received a separate read-only worker audit.

Docker produced its own listings for all six final runs; no native-listing
fallback was needed. Listings lacking terminators are explicitly recorded in
[review](review.json); the complete COFF inventory and independent native/Docker
code/fixup equality remain checked. This listing limitation is not a byte
exclusion. Original executable data was only inspected statically; only authored
fixtures ran in locked Docker. Invalid original paths such as allocation failure
or overlong names are documented per case and were not silently repaired.

Five generated Ghidra names and six comments were updated and saved; the trusted
channel-release name was preserved. Affected callers were re-decompiled. Existing
inferred calling-convention metadata can still add phantom ECX parameters; source
ABI follows reviewed caller/callee instructions. No executable bytes were edited.

Shared tooling is unchanged and full regression remains deferred to the batch
acceptance/integration checkpoint. Historical accepted evidence remains fourteen
functions and nineteen records. These are focused function results; final linked
executable identity and accepted-history promotion remain pending.

The last worker candidate report was written at 06:42:05 UTC, before the 06:49:21
worker stop. The coordinator handoff observation at 06:49:43 is an upper bound,
not additional implementation time. Receipt aggregation completed at 06:50:15.
The rounded shared-account meter moved from 31% to 33%; this is not a measured
campaign cost or controlled model-speed comparison.

Only authored sources, fixtures, metadata, notes and hash receipts are committed.
Original game content, compiler binaries and private build artifacts remain
outside Git. No develop merge or upstream submission occurred. Continue with one
worker and untouched small sound helpers; retain these three parked candidates
without resetting their attempt budgets. Stop new implementation at 08:38 UTC
and finish the overnight campaign at 08:48 UTC.
