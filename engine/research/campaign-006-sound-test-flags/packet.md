# Prepared target: sound_test_flags

Team-not-done bp/bapsound.c. Full interval0x00417876..0x004178A3: 45 body/full bytes, no alignment. Coordinator inspected complete body/instructions, next boundary, XREFs and relevant caller/callee ABI. Complete PE relocation inventory and direct targets independently verified against owned original. Pinned VC5 C /Od /MT; immutable target.

Observed cdecl interface:
`unsigned op_sound_test_flags(int index, unsigned mask); extern op_sound_world_state *op_sound_world;`

Load descriptor-array pointer from world+0xCC8 into a local pointer, advance by index records of0x40 bytes, and return unsigned flags at record+0x18 AND mask unchanged (not normalized bool). No world/null/index bounds guard. World partial type opaque[0xCC8] then descriptor pointer. Descriptor partial type opaque[0x18], unsigned flags, opaque[0x24], total64. Local pointer assignment/advance are ordinary source operations witnessed by the original local stores, not fabricated padding. Sole AI caller0x435022 uses mask0x10 to select ownership; no direct callees. Fixture asserts ABI offsets/stride, several valid records and masks incl highbit, zero and allbits, complete no-mutation checks, changed global world between calls. Only valid array pointers/indices; an interior base with negative index is allowed only when it stays within the authored array. No invalid pointer tests or original world content.

Worker owns candidate.c/api.h/behavior.c/README.md/worker-log.json only. No target, shared tooling, metadata or Git changes. Existing15min/10candidate attempts/five nonimprovement cap per function; freeze immediately on exact result and report all attempt hashes. Native compile only; authored fixtures execute only in locked Docker. No warnings, exclusions, fabricated frames, raw instructions, original data or original execution. Overall worker deadline04:40:17; final publication04:50:17. Parent independently reviews and runs final verification pairs.
