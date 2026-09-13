# Ready assignment: scan-line reset

Coordinator static review, 2026-09-13. Target `0x004084B8`, next function
`0x00408648`. Full interval and body both 400 bytes; RET is at `0x00408647`.
There are no trailing bytes and no PE absolute relocation operands in this span.
The only direct call is at `0x004084C6`; its operand is function+15 and targets
`0x00495290`, the tracked-allocation wrapper. This is not a CRT malloc binding.
The two incoming references are frame-setup calls at `0x0040444C` and
`0x0040459F`. Frame setup always supplies its existing global line record.

The team calls this `bapline_reset`, reports `not_done`, and has no known file
assignment (manifest line 1323). This is a dependency of the active frame-setup
work, not an assertion that the team's module is bp/bapdraw.c.

## Observed interface and state

Return the record pointer. Arguments are record pointer, start X, start Y,
end X, end Y, all five passed on the stack with caller cleanup. If the incoming
record is null, request exactly 80 bytes from the allocator. A failed allocation
returns null without record writes. An existing record must not allocate.

All listed fields are signed 32-bit integers, at these byte offsets:

| Offset | Meaning |
| --- | --- |
| 00 | Major distance plus one (count) |
| 04 | Index, reset to zero |
| 08, 0C | Current X/Y, copied from start |
| 10, 14 | Current step X/Y, reset to zero |
| 18, 1C, 20, 24 | Start X/Y, end X/Y |
| 28, 2C | Major and minor absolute coordinate differences |
| 30 | Twice minor minus major (current error) |
| 34, 38 | Diagonal X/Y signs, initially +1; -1 when corresponding end < start |
| 3C | Twice minor minus twice major (diagonal error increment) |
| 40, 44 | Straight step: X sign and zero if abs X >= abs Y, otherwise zero and Y sign |
| 48 | Twice minor (straight error increment) |
| 4C | Unwritten word; preserve it |

Absolute X/Y differences are computed from the stored endpoint fields and retained
as two local integers. Ties choose X as the major axis. The observed prologue
reserves eight bytes for locals and otherwise uses the ordinary unoptimized stack
frame. The source should express these operations in readable C, using /Od /MT;
no raw opcodes, inline-assembly reconstruction or artificial frame structures.
Two local names/declaration order may affect unoptimized VC5 allocation; any
experiments must have explicit hypotheses and remain within the cap.

## Required focused fixture

Check provided-record, allocation-success and allocation-failure paths; exact
allocation size/count and returned pointer; every written field and untouched
word; all direction quadrants, X/Y major axes, tie and zero-length lines.
Use expected values independent of the candidate body and return nonzero on
failure. Select endpoint differences that fit signed integers for the primary
contract; explicitly document overflow/INT_MIN cases if they are not tested.
The fixture allocator is an authored stub. Do not execute the original allocator
or game. Native compilation only; final fixture executes in locked Docker.

This evidence packet is authored prose and metadata, not copied decompiler output.
The parent independently verifies final source and objects against the full target.
