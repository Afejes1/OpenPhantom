# Prepared target: sound_stop_all

Team-not-done bp/bapsound.c. Full interval0x004176E3..0x00417711: 46 body/full bytes, no alignment. Coordinator inspected complete body/instructions, next boundary, XREFs and relevant caller/callee ABI. Complete PE relocation inventory and direct targets independently verified against owned original. Pinned VC5 C /Od /MT; immutable target.

Observed cdecl interface:
`void op_sound_stop_all(void); void op_sound_release_slot(unsigned slot);`

Unconditionally iterate local signed int i from0 while i<12, incrementing by1, and call release_slot(i). Exactly twelve ascending calls0..11, no initialized or disabled gate. Callee independently takes unsigned index and rejects>=12; wrapper range is observed signedCMP/JGE12. Full body and next boundary reviewed. Fixture record order, count, exact endpoints, no out-of-range calls and repeat invocation starts at0; stub stores bounded authored records and must not write out of range if candidate calls too many times.

Worker owns candidate.c/api.h/behavior.c/README.md/worker-log.json only. No target, shared tooling, metadata or Git changes. Existing15min/10candidate attempts/five nonimprovement cap per function; freeze immediately on exact result and report all attempt hashes. Native compile only; authored fixtures execute only in locked Docker. No warnings, exclusions, fabricated frames, raw instructions, original data or original execution. Overall worker deadline04:40:17; final publication04:50:17. Parent independently reviews and runs final verification pairs.
