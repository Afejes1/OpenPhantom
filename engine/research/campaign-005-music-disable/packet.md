# Prepared music callback helper: music_disable

Team-not-done bp/bapmusic.c at 0x00410527. Full body and interval to next entry 0x00410556: 47 bytes, no alignment gap. Coordinator reviewed every instruction, decompiled behavior, callers and callees. Complete PE DIR32 inventory, all direct-call targets and original identity independently verified before source experiments. Original VC5 C /Od /MT; no target changes.

Observed cdecl interface and callback contracts:
`void op_music_disable(void); extern const char op_music_disabled_key[]; int op_music_write_option(const char *key, int value); extern int op_music_state, op_music_sequence, op_music_resume_state, op_music_resume_sequence; void op_music_detach(void);`

Call option writer(key,1), then copy CURRENT requested state and sequence to resume globals, then detach. Ignore callback return values. No initialized guard. Option writer may mutate requested IDs: copies occur afterward. Detach sees both copies and may mutate globals; no later rewrite. Verify callback order and arguments, before/after snapshot timing, negative/boundary IDs and repeated disable. Config writer at 0x4648A1 takes key then int and returns BOOL; detach0x41046C takes no arguments. The menu caller at 0x4422B3 uses this for disabled toggle. Fixture key is an authored placeholder string, not extracted content.

Read campaign rules. Worker owns only candidate.c, api.h, behavior.c, README.md and worker-log.json here. Retain conventional readable C; no raw instructions, artificial frames, warning waivers, exclusions, or original execution. Fixture contains authored data and stubs only. Native compile only; execute authored fixture in locked Docker. Per function 15min/10invocations/five successive nonimprovements; exact candidate freezes immediately. Handoff reports every experiment, hashes, failures and unresolved limits. Coordinator handles fresh final verification, metadata and Git.
