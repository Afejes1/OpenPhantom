# Static contract

Signed slot bounds and nonzero active word gate three raw float word stores at50/54/58. Observed EAX is null on failure and record pointer after successful stores; no direct callers found, so no ordinary caller is claimed.

Complete 80-byte span at `0x0043AF45`, no trailing gap. Team not_done and no local duplicate. Ghidra full instructions, caller/callee references and all address operands reviewed. Fixtures use authored data only. Final linked placement remains a separate gate.

Prototype uncertainty: the original successful exit leaves its last-store base in EAX. With zero direct callers, intended return use is unproved. This candidate explicitly returns the record pointer and is83 bytes rather than the original80, so it is research-only and not eligible for acceptance. Do not force equivalence using missing-return behavior or a warning waiver. The fixture checks the authored candidate contract and field effects, not independently executed original behavior.
