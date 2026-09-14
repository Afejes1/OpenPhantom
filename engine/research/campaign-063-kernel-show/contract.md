# kernel_show

Void cdecl captures command and kernel window word, calls stdcall ShowWindow, ignores return. Callback mutates live state after observing captured arguments; mutation is preserved. Kernel window8620F0 is distinct from the prior window state8620A4.

All documented globals and all16 entries of both arrays are snapshotted; no physical guard adjacency around separately linked globals is assumed. ShowWindow is an authored observer. Independent Ghidra review confirmed full consecutive extents, operands, signedness, ABI and caller context; all nine are team not_done. No original game or OS execution, byte exclusion, warning waiver or whole-executable identity claim.
