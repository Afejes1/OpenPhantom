# Prepared music callback helper: music_restore_cached_ids

Team-not-done bp/bapmusic.c at 0x00410787. Full body and interval to next entry 0x004107B5: 46 bytes, no alignment gap. Coordinator reviewed every instruction, decompiled behavior, callers and callees. Complete PE DIR32 inventory, all direct-call targets and original identity independently verified before source experiments. Original VC5 C /Od /MT; no target changes.

Observed cdecl interface and callback contracts:
`int op_music_restore_cached_ids(int version); extern int op_music_cached_ids[2]; int op_music_read_bytes(void *data, int size);`

Reject every version except 2 by returning1 without reading or changing cache. For2 call read_bytes(cache,8); return 1 if read result zero else 0. Full wrapper0x451CB0 forwards pointer[8],size[C] to0x451FB0; latter passes pointer,size,1,file to fread-like0x49B6F0 and both preserve returned EAX. Do not infer void from incorrect Ghidra prototype. Sole dispatcher event0B caller forwards version; event18 separately replays cached IDs. Test rejection versions inclnegative/boundaries, exact pointer/size, zero/nonzero/negative return normalization, callback modifications retained even on failure, no request-state mutation or music callbacks. This is cached-ID read only, no playback or real I/O.

Read campaign rules. Worker owns only candidate.c, api.h, behavior.c, README.md and worker-log.json here. Retain conventional readable C; no raw instructions, artificial frames, warning waivers, exclusions, or original execution. Fixture contains authored data and stubs only. Native compile only; execute authored fixture in locked Docker. Per function 15min/10invocations/five successive nonimprovements; exact candidate freezes immediately. Handoff reports every experiment, hashes, failures and unresolved limits. Coordinator handles fresh final verification, metadata and Git.
