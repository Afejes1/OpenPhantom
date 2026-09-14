# kernel_hide

Void cdecl calls stdcall ShowWindow with live kernel window and literal0, ignoring its result. Captured arguments and callback mutation preservation are tested.

All documented globals and all16 entries of both arrays are snapshotted; no physical guard adjacency around separately linked globals is assumed. ShowWindow is an authored observer. Independent Ghidra review confirmed full consecutive extents, operands, signedness, ABI and caller context; all nine are team not_done. No original game or OS execution, byte exclusion, warning waiver or whole-executable identity claim.
