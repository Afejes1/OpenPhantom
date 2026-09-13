# Campaign 001: mover gather

This directory reconstructs the bounded WMAIN interval at `0x00405CF0` from
the supplied static Ghidra analysis. The target runs from `0x00405CF0` through
`0x004062FF` (1552 bytes): executable body `0x00405CF0..0x004062F0` inclusive
(1537 bytes), followed by fifteen `NOP` bytes. `queue_visible_surface` begins
at `0x00406300` and is outside the target.

Observed contract: the argument has a 16-byte header and 0x40-byte mover
records. It optionally expands three seed cell IDs, then eligible mover cell
IDs, using a partial world table at `world+0x624` indexed below
`world+0x620`. Each cell batch set has a count at `+0x24`; batches begin at
`+0xd0`, are 0x9c bytes apart, have their item count at `+0`, and surface list
at `+0x4c`. The two expansion loops inline the alpha/bucket operation whose
separate helper is at `0x00406300`; their cache/rejected-room details differ.
The tail visits animation-map triples for flagged `tag == -1` movers and queues
eligible direct movers.

Evidence: Ghidra reports one caller (`0x004054C3`, in `FUN_00404730`), two
callees (`__ftol` and `queue_visible_surface`), and the described offsets in
the decompiler/disassembly. The function uses 0x20 bytes of local stack and
saves EBX/EBP/ESI/EDI. Candidate source is readable partial C++ rather than a
byte-shaped transcription; it does not attempt an exact byte match.

`target.json` deliberately has no binding inventory. Relocation offsets are
not asserted without a separate static inventory; the address/body/padding
interval is complete. The synthetic fixture covers seed expansion, mover
expansion and its `+0x64` mark, animation-map queueing, and direct mover
queueing. Run it only with the locked Docker behavior runner. Native VC5 use,
if requested by the parent runner, is compile-only; no generated native code
is executed.

## Reviewed pilot checkpoint

The final coordinator fixture passes 36 assertions covering bucket links,
opacity/fade, shared visit state, seed/mover phase differences, rejected rooms,
animation-map limits and direct mover queueing. Native and Docker agree on
the full authored translation-unit code and relocations (1,520 code bytes).
Uninlined helpers prevent a valid single-function extent comparison. No
original-byte match or similarity percentage is claimed. [The receipt](result.json)
pins the final source and private report hashes. Complete binding inventory
and original inline layout reconstruction remain pending.
