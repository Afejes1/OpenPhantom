# kernel_add_dialog

Cdecl unsigned count<16 appends opaque dialog word, increments count and returns1; unsigned count>=16 returns0 without writes. Rejected counts include high-bit patterns.

All documented globals and all16 entries of both arrays are snapshotted; no physical guard adjacency around separately linked globals is assumed. ShowWindow is an authored observer. Independent Ghidra review confirmed full consecutive extents, operands, signedness, ABI and caller context; all nine are team not_done. No original game or OS execution, byte exclusion, warning waiver or whole-executable identity claim.

Initial preflight caught a data-operand offset typo: the count reload operand starts at +29, not +28. Corrected from original instruction 00498E74 (8B15, immediate at00498E76). Source unchanged; failed preflight retained.
