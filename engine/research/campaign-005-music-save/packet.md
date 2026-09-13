# Prepared music callback helper: music_save_cached_ids

Team-not-done bp/bapmusic.c at 0x0041074A. Full body and interval to next entry 0x00410787: 61 bytes, no alignment gap. Coordinator reviewed every instruction, decompiled behavior, callers and callees. Complete PE DIR32 inventory, all direct-call targets and original identity independently verified before source experiments. Original VC5 C /Od /MT; no target changes.

Observed cdecl interface and callback contracts:
`int op_music_save_cached_ids(int tag); extern int op_music_state, op_music_sequence; extern int op_music_cached_ids[2]; int op_music_write_frame(int tag, const void *data, int size, unsigned short version);`

Copy requested state and sequence to the contiguous two-int cache. Call write_frame(tag,cache,8,2), return 1 if callback result nonzero else 0. Full callee0x451C6D reads tag[8],data[C],size[10],version WORD[14], with caller's right-to-left pushes independently checked. Sole dispatcher event0A caller forwards tag. Fixtures check both copied before callback, exact pointer/tag/size/version, repeated values, boundary IDs, normalized negative/nonzero failures, and callback changes to cache/requests retained after returning. This wrapper saves only two cached IDs, not full music state. No actual file I/O in fixture.

Read campaign rules. Worker owns only candidate.c, api.h, behavior.c, README.md and worker-log.json here. Retain conventional readable C; no raw instructions, artificial frames, warning waivers, exclusions, or original execution. Fixture contains authored data and stubs only. Native compile only; execute authored fixture in locked Docker. Per function 15min/10invocations/five successive nonimprovements; exact candidate freezes immediately. Handoff reports every experiment, hashes, failures and unresolved limits. Coordinator handles fresh final verification, metadata and Git.
