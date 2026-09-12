# Legacy cell surface collection

The team manifest marks 0x004056C0, bapdrawOld_gatherCell in bp/bapdraw.c,
not done. The descriptive name gather_legacy_cell_surfaces has HIGH behavioral
confidence from the complete static body, two incoming world-traversal calls,
three callees and their shared state. It is not claimed as an original symbol.
The authored [C++ reconstruction](../research/surface-emission/legacy_collection_candidate.cpp)
now passes focused strict byte comparison. It awaits the shared batch acceptance
checkpoint and has not been added to the accepted registry.

## Callers, ABI and complete extent

World traversal 0x00404730 calls this function at 0x00404B33 and 0x00404C94
after selecting a non-null, nonempty grid cell and checking a left/right scan
boundary. The collector receives the cell pointer and a camera-space float
triplet at 0x008BEA1C. Saving the two-argument prototype and re-decompiling
the caller recovers both arguments at both call sites. Its body ends at RET 0x004059E8; seven NOPs reach the next function
at 0x004059F0. The complete comparison span is 816 bytes, including all 809
body bytes and all seven alignment bytes.

The pending target specifies all 28 absolute operands and six near-call operands.
Calls at 0x00405759 and 0x00405877 use the bucket helper 0x004059F0;
0x0040590C and 0x00405947 use scan-plane culling 0x00403FA0;
0x004059AC and 0x004059C9 use legacy emission 0x00405A50.
The full body has no string or imported-API reference. Every declared original
operand was checked against the pinned retail PE, including its relocation
inventory. No original instruction bytes or binary artifacts are committed.

## Group traversal before slab rejection

A nonzero byte at cell+0x0C enables exactly three group indexes at +0x0D..0x0F.
It is an enable flag, not the loop count. Zero indexes are skipped in this phase.
World+0x620 supplies the unsigned group-count bound; pointers begin at +0x624.
Only state-zero groups are processed, and state becomes 2 before dereferencing
the group. This first phase has no null-group or active-group guard.

When cell+1 has mask 0x01 set, the second phase walks count-byte cell[0] records,
starting at cell+0x10 with 64-byte stride. Non-0xFFFF tags with flags &0x0C
can discover a group from record+0x2A. Group zero is allowed here. State changes
from 0 to 1 before the pointer/active test. Null or inactive groups remain 1;
an active group receives field+0x64 = 1 and state = 2 before its parts are read.

Group+0x24 is a signed part count; parts start at +0xD0 and have stride 0x9C.
Each part has a signed surface count at +0 and a surface-pointer array at +0x4C.
Both phases walk those arrays, check the cached queue count against 8192 before
reading the next surface, and queue only flags &4 with no selected-room overlap.
After the bucket call they reload the global count and the surface-array element,
write the entry surface and increment the global count. The capacity return
ends the entire function, preserving preceding state writes and queued entries.

These are partial prefix views only. The header does not infer a full world/group
allocation or original array capacity. Test allocations are independently owned.

## Slab selection and scan state

| Scan side | Inverted height | First cull | Second cull |
|---|---|---|---|
| Zero | Zero | plane 2, bound B | plane 2, bound A |
| Zero | Nonzero | plane 2, bound A | plane 2, bound B |
| Nonzero | Either | plane 3, bound A | plane 3, bound B |

Bounds A/B are raw float values at cell+4/+8. Any nonzero first result returns
without touching scan position, cached height or the previous cull-enabled value;
the earlier group work remains. The side/inversion globals are read again for
the second call. Its full integer result becomes cull-enabled. Then the input
triplet is copied to scan position and cached height is set to positive zero.

## Inline records and reference visits

Ordinary records are passed to legacy emission. A 0xFFFF record instead uses
its byte+2 count to walk references at +8 with stride 12. Each entry holds a
surface pointer and a 32-bit seen index at +4. A nonzero seen byte skips the
entry. Otherwise a non-null surface is emitted and the integer counter at
0x0059DF4C is incremented, even for a null pointer. This function does not mark
seen bytes itself. The reference count and outer record count are reloaded
after calls. No independent queue-capacity check is added to this phase.

The bounded fixture uses four references within each owned record and a seen
index of 65536 to verify the 32-bit access. This does not establish an original
maximum reference count or seen-buffer allocation.

## Focused validation and remaining work

The original VC5 RTM C++ frontend, /O2 /MT /W4 /WX /Zi, now produces the
complete matching 816-byte span. All 28 absolute operands (112 bytes) and six
near-call operands (24 bytes) verify their exact symbols, addends and original
destinations. The other 680 bytes, including every padding byte, agree exactly.
Reference and resolved-candidate SHA-256 are both
2bce02537cc0ccb8d3a916cc5269ab50ed310bd2a35da52255c79d9e91c76bc0.
There are no exclusions or warning waivers. Linked placement and the complete
retail executable are still unverified.

Guarding nonempty traversal before initializing cursors restores the original
register lifetime and load schedule. Referencing the global queue counter
lets VC5 retain it in ECX across the relevant loops. Part iteration uses the
surface-array field at part+0x4C, discovered records use flags at +0x28, and
reference records use the count at +2; the cursor increments retain the original
0x9C/64/12-byte strides. Those fields and offsets are independently documented
above and checked by the synthetic fixture. No stack-frame or raw-byte assembly
was introduced. A greater-than-zero byte test emits the original JBE, whereas
an equivalent nonzero test emits JZ. The C frontend leaves one SIB byte different;
C++ emits the exact original base/index ordering. This establishes the frontend
needed by this reconstruction, not the original source text or file extension.

The actual candidate linked to controlled bucket/culler/emitter stubs passes
1,195 assertions in locked Docker. Native compilation with the same compiler
also reproduces identical function bytes and relocations and supplies a complete
listing; no generated executable runs on the host. Cases include all three linked slots,
duplicate/zero/out-of-range indexes, active/null groups, signed counts,
part/surface strides, room filtering, state-before-capacity returns, both queue
boundaries, post-callback count/pointer/count-limit reloads, both slab calls,
raw signed-zero/NaN/infinity payloads, scan initialization, normal records,
seen/null/duplicate references, count wrap and a 255-record traversal.

Stub mutations establish source behavior at call boundaries, not a claim that
real callees perform those mutations. This fixture does not establish integrated
renderer or whole-game equivalence. The original executable was read statically
only. No full regression or recovery cycle was repeated. See the
[focused exact-match receipt](legacy-cell-match-20260912.json).

Per the [batch cadence](verification-cadence.md), the full original-toolchain
registry build and immutable record will follow when several functions are
ready. The accepted baseline currently remains fourteen historical functions.
