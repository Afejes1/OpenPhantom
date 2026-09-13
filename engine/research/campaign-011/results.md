# Campaign 011 results

**10 complete exact function matches covering 467 bytes.**
One research candidate remains; there are no new near matches.
all 209 focused assertions pass. Exact counts require every byte
of the complete span after verified address relocation. All assigned addresses
are separately confirmed team-reported not done.

| Function | Address | Full bytes | Status | Focused checks |
| --- | --- | ---: | --- | ---: |
| [light_disable_world](../campaign-011-light-disable-world/README.md) | 0x00406EED | 83 | Exact | 49 |
| [view_force_flags](../campaign-011-view-force-flags/README.md) | 0x00419400 | 32 | Exact | 12 |
| [view_clear_flags](../campaign-011-view-clear-flags/README.md) | 0x00419420 | 32 | Exact | 12 |
| [view_clear_visits](../campaign-011-view-clear-visits/README.md) | 0x00419440 | 80 | Research: 30 differing bytes | 23 |
| [bgl_identity](../campaign-011-bgl-identity/README.md) | 0x0041F700 | 27 | Exact | 5 |
| [bgl_push](../campaign-011-bgl-push/README.md) | 0x0041F71B | 85 | Exact | 31 |
| [bgl_pop](../campaign-011-bgl-pop/README.md) | 0x0041F770 | 44 | Exact | 15 |
| [bgl_load_matrix](../campaign-011-bgl-load-matrix/README.md) | 0x0041F910 | 25 | Exact | 9 |
| [bgl_get_matrix](../campaign-011-bgl-get-matrix/README.md) | 0x0041F929 | 25 | Exact | 9 |
| [bgl_get_translation](../campaign-011-bgl-get-translation/README.md) | 0x0041FA37 | 49 | Exact | 7 |
| [bgl_get_translation_xyz](../campaign-011-bgl-get-translation-xyz/README.md) | 0x0041FA68 | 65 | Exact | 37 |

The rendering flag functions include their original 13 and 11 NOP alignment
bytes. The vertex-visit function retains its full 80-byte target, including ten
alignment bytes: the better of two natural variants differs in 30 bytes (37.5%).
The worse second variant is retained in its log; the first source was restored
and independently compiled again. No target was shortened and no percentage was
estimated from different lengths. The vertex fixture covers nulls, reversed
ranges, an interior range, index 0x8000, index 65535 and the entire 65536-entry
range with complete storage preservation oracles.

Lighting deactivation preserves signed guards and delegates only active records;
it does not clear state after a callback that declines deactivation. The parent
strengthened its full-world/full-record callback mutation oracle, including
neighbor records. The three color routines from initial triage were deferred
without compiler experiments: their incoming-rounding FISTP and register pattern
suggest hand-written assembly. This remains an inference; ordinary C casts would
change rounding behavior and were not represented as correct replacements.

The transform helpers preserve 48-byte affine representations as opaque words.
Copies retain all bits, including floating-point payloads, without arithmetic
conversion. Push captures stack[old depth] separately from the current pointer;
pop preserves the original signed lower guard and absence of an upper guard.
Translation outputs follow original order and live reads when valid outputs
alias matrix words. Per-case review and limitations describe the tested domains.
No original identity table or other extracted data is committed; fixture values
are independently authored.

All sources are C. Rendering uses pinned VC5 /O2 /MT and the remaining helpers
use /Od /MT; common strict warning flags and toolchain lock are unchanged.
15 worker candidate compiles are recorded.
Parent final verification used 22 fresh candidate
compiles and 11 authored fixture builds/executions.
Eight fixtures were strengthened during parent review; there were no failed
parent builds and no candidate-source corrections. Push/load/get explicitly use
the standard memcpy intrinsic; disjoint-copy tests follow its C non-overlap
contract. All worker experiments and review refinements remain recorded.

11 final Docker cases used the existing explicit
native-listing fallback after source/toolchain and full code/fixup agreement.
Own listings without terminators are recorded separately; complete COFF inventory
remains checked. These provenance limits do not exclude comparison bytes.
No original executable was run or patched. Only authored fixtures executed in
locked Docker; native original-toolchain operation compiled only.

Independent review precedes exact or near classification. Hashes of final source
snapshots and reports are checked against exact staged Git blobs. Shared tooling
is unchanged, full regression is deferred, and historical accepted evidence stays
at fourteen functions and nineteen records. These focused results await deliberate
batch acceptance and do not establish byte-identical final executable layout.

Nine generated Ghidra names and eleven comments were updated and saved; two
trusted names were preserved and six affected callers re-decompiled. The last
worker candidate report was written at 07:11:49 UTC; final receipt aggregation
completed at 07:19:22. The rounded shared-account meter moved from 33% to 34%.
Full details remain in [review](review.json) and [metrics](metrics.json). Worker timestamp bases are
explicit; this workload is not a controlled model-speed or exact cost benchmark.
Only authored source, fixtures, metadata, documentation and hash receipts enter
Git. Original content, compiler binaries and private artifacts stay outside it.
No upstream submission or develop merge occurred.

Continue the single-worker strategy, retaining parked candidates without resetting
caps. The overnight window still stops new implementation at 08:38 UTC and ends
at 08:48 UTC.
