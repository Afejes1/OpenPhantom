# Campaign 012 results

**10 complete exact function matches covering 520 bytes.**
There are 0 reviewed near matches and 1 research candidates;
all 257 focused assertions pass. Exact counts require every byte
of the complete original span after verified address relocation. Every assigned
address is independently confirmed team-reported not done, with no earlier local
candidate. The original spans were frozen before source experiments.

| Function | Address | Full bytes | Status | Focused checks |
| --- | --- | ---: | --- | ---: |
| [bgl_rotate_axis](../campaign-012-bgl-rotate-axis/README.md) | 0x0041F79C | 67 | Exact | 73 |
| [bgl_rotate_xyz](../campaign-012-bgl-rotate-xyz/README.md) | 0x0041F7DF | 44 | Exact | 29 |
| [bgl_translate_vector](../campaign-012-bgl-translate-vector/README.md) | 0x0041F80B | 44 | Exact | 15 |
| [bgl_translate_xyz](../campaign-012-bgl-translate-xyz/README.md) | 0x0041F837 | 40 | Exact | 25 |
| [bgl_translate_negative](../campaign-012-bgl-translate-negative/README.md) | 0x0041F85F | 54 | Exact | 11 |
| [bgl_scale_xyz](../campaign-012-bgl-scale-xyz/README.md) | 0x0041F895 | 66 | Exact | 15 |
| [bgl_scale_vector](../campaign-012-bgl-scale-vector/README.md) | 0x0041F8D7 | 33 | Exact | 9 |
| [bgl_multiply](../campaign-012-bgl-multiply/README.md) | 0x0041F8F8 | 24 | Exact | 9 |
| [bgl_rotate_point](../campaign-012-bgl-rotate-point/README.md) | 0x0041F942 | 52 | Exact | 6 |
| [bgl_transform_point](../campaign-012-bgl-transform-point/README.md) | 0x0041F976 | 96 | Exact | 28 |
| [bgl_transform_xyz](../campaign-012-bgl-transform-xyz/README.md) | 0x0041F9D6 | 97 | Research: 19 differing bytes | 37 |

This queue reconstructs the transform wrappers around the 48-byte affine matrix
and three-component floating-point vectors. Static review covered complete core
callee implementations as well as wrapper instructions, caller arguments and
external data layouts. Each packet records the full interface and behavior.
Scale-vector and rotate-point have no incoming Ghidra XREF; their semantics are
corroborated by complete bodies, core callees and sibling data layout instead.
No original identity table or other extracted data is committed.

The independent review checks borrowed pointers versus local copies, argument
order, callback mutations, current-transform reloads, output ordering and valid
alias/null cases. Finite arithmetic, signed zero and floating-point payload cases
are distinguished from bit-preserving copies. Pinned VC5 x87 behavior defines
floating-point edge cases; these fixtures are not a claim about other compilers
or floating-point environments. Per-case reviews state their precise limits.

All candidate sources are C with pinned VC5 /Od /MT. Common strict warning flags
and the original-toolchain lock are unchanged. There are
12 recorded worker candidate compiles;
parent final verification used 22 fresh candidate
compiles and 11 authored fixture builds/executions.
Every worker experiment, including worse variants, is retained. Failures and
parent corrections are separately recorded in [review](review.json).

11 final Docker cases used the existing explicit
native-listing fallback after complete source/toolchain/code/fixup agreement.
Own listings without terminators are recorded separately; complete COFF inventory
remains checked. No comparison bytes, operands or warnings are excluded.
Only authored fixtures execute in locked Docker; native toolchain operation
compiles only. The original game was never run or patched.

Source snapshots and receipts are verified against the exact staged Git blobs.
Shared tooling is unchanged, full regression is deferred, and historical accepted
evidence remains fourteen functions and nineteen records. These reviewed focused
results still await deliberate batch acceptance and final executable linking;
they do not establish a byte-identical executable.

Ghidra metadata changes, actual timestamps, compiler counts and account readings
are retained in [review](review.json) and [metrics](metrics.json). Rounded account
usage is shared across tasks and is not an exact cost or model-speed benchmark.
Source, fixtures, semantic metadata and hash receipts enter the personal fork;
original content, licensed compiler binaries and private artifacts stay outside Git.
No upstream submission or develop merge occurred.

Music resume and sound muted were inventoried read-only, then deferred without
compiler attempts to keep this queue bounded. Preserve parked candidates and
all per-function caps. The fixed overnight authorization stops new implementation
at08:38 UTC and ends at08:48 UTC.
