# Prepared target: sound_stop_channel

Team-not-done bp/bapsound.c. Full interval0x00417368..0x00417379: 17 body/full bytes, no alignment. Coordinator inspected complete body/instructions, next boundary, XREFs and relevant caller/callee ABI. Complete PE relocation inventory and direct targets independently verified against owned original. Pinned VC5 C /Od /MT; immutable target.

Observed cdecl interface:
`void op_sound_stop_channel(unsigned slot); void op_sound_release_slot(unsigned slot);`

Forward the complete unsigned slot argument once to release_slot and return void, without a local range/null guard. Callee0x417567 independently checks unsigned slot<12, but that guard belongs to the callee. Both dialogue callers pass one stack word. Fixtures verify one call and exact argument bits for0,11,12,large values and UINT_MAX; callback never indexes original state or executes real sound code.

Worker owns candidate.c/api.h/behavior.c/README.md/worker-log.json only. No target, shared tooling, metadata or Git changes. Existing15min/10candidate attempts/five nonimprovement cap per function; freeze immediately on exact result and report all attempt hashes. Native compile only; authored fixtures execute only in locked Docker. No warnings, exclusions, fabricated frames, raw instructions, original data or original execution. Overall worker deadline04:40:17; final publication04:50:17. Parent independently reviews and runs final verification pairs.
