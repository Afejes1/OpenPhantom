# Prepared target: sound_enable

Team-not-done bp/bapsound.c. Full interval0x004178D1..0x004178FA: 41 body/full bytes, no alignment. Coordinator inspected complete body/instructions, next boundary, XREFs and relevant caller/callee ABI. Complete PE relocation inventory and direct targets independently verified against owned original. Pinned VC5 C /Od /MT; immutable target.

Observed cdecl interface:
`void op_sound_enable(void); extern int op_sound_disabled; extern const char op_sound_disabled_key[]; int op_sound_write_option(const char *key, int value);`

If disabled==0 return unchanged with no callback. Any nonzero value calls write_option(key,0), ignores result, then sets disabled=0 after callback. No sound init/start callback. Config writer's caller/callee ABI is key thenint, cdecl BOOL result. Menu toggle and sound initializer0x4159F0 callers corroborate role. Fixture exact key identity/zero, old flag visible to callback, callback mutation then final zero override, positive/zero/negative returns and non-1 flag values. Key is authored placeholder string.

Worker owns candidate.c/api.h/behavior.c/README.md/worker-log.json only. No target, shared tooling, metadata or Git changes. Existing15min/10candidate attempts/five nonimprovement cap per function; freeze immediately on exact result and report all attempt hashes. Native compile only; authored fixtures execute only in locked Docker. No warnings, exclusions, fabricated frames, raw instructions, original data or original execution. Overall worker deadline04:40:17; final publication04:50:17. Parent independently reviews and runs final verification pairs.
