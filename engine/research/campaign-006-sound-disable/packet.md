# Prepared target: sound_disable

Team-not-done bp/bapsound.c. Full interval0x004178A3..0x004178D1: 46 body/full bytes, no alignment. Coordinator inspected complete body/instructions, next boundary, XREFs and relevant caller/callee ABI. Complete PE relocation inventory and direct targets independently verified against owned original. Pinned VC5 C /Od /MT; immutable target.

Observed cdecl interface:
`void op_sound_disable(void); extern int op_sound_disabled; extern const char op_sound_disabled_key[]; int op_sound_write_option(const char *key, int value); void op_sound_stop_all(void);`

Only disabled==1 returns unchanged; other values (including nonzero negative/2) enter. Call write_option(key,1), stop_all(), then disabled=1. Ignore writer result and do not recheck flag changed by callbacks. Both callbacks see flag before final write, subject to their own changes. Stop-all0x4176E3 takes noargs, iterates release slots0..11. Full config ABI/callee and menu/sound-initializer callers reviewed. Fixture gate distinction, exact key/value, call order, ignored returns and callback mutations finally overwritten by1. No real options or sample release execution.

Worker owns candidate.c/api.h/behavior.c/README.md/worker-log.json only. No target, shared tooling, metadata or Git changes. Existing15min/10candidate attempts/five nonimprovement cap per function; freeze immediately on exact result and report all attempt hashes. Native compile only; authored fixtures execute only in locked Docker. No warnings, exclusions, fabricated frames, raw instructions, original data or original execution. Overall worker deadline04:40:17; final publication04:50:17. Parent independently reviews and runs final verification pairs.
