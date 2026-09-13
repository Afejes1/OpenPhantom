# Prepared target: sound_remove_level

Team not done, bp/bapsound.c; complete 0x00415C40..0x00415D13, 211 bytes and 14 bindings.

Interface: int op_sound_remove_level(int ignored). The module dispatcher supplies one zero stack argument and removes four bytes; the target never reads it. Use an explicit ignored parameter and ordinary void cast to avoid an unused-parameter warning. No claimed semantic meaning for that parameter.

Record is 60 bytes: unsigned flags at 0, char name[52] at 4, void *resource at 0x38. World prefix is opaque[0xCB4], signed int count at 0xCB4, record *records at 0xCB8. Globals: int op_sound_initialized, op_sound_ref_count, op_sound_level_registered; world *op_sound_world; void *op_sound_registry.

Callbacks: void op_sound_resume(void), op_sound_stop_all(void), op_sound_free_extra(void); int op_sound_mark_resource(void *resource,int flag); int op_sound_release_resource(void *resource); int op_sound_remove_name(void *registry,const char *name). Full callees confirm argument order and full EAX integer returns; results are ignored here.

Return 1 immediately if initialized is zero or world null. Otherwise resume, stop all, free extra, in that order. Only afterward capture current world's records. Use signed index 0 upward, reloading CURRENT world's signed count each iteration. For each captured record: mark(current resource,1); release(reloaded resource); clear resource; remove_name(current registry,record name); decrement CURRENT ref_count; clear resource AGAIN; advance record by 60 bytes. End sets level_registered=0 and returns 0. No second initialized gate, no world-null recheck inside traversal.

Fixture covers both guards, initialized noncanonical nonzero, zero/negative/1/3 counts, exact callback order, changed world/records during setup before capture, changed world/count during traversal while captured records continue, mark changes resource before release, release changes registry, remove changes count/resource and second clear overwrites it, callback changes initialized/registered with final registered zero. Use valid owned worlds/records throughout and avoid signed counter overflow. Assert ABI offsets and full record bytes outside intentional resource changes. No real resource/backend execution.

The coordinator verified every instruction against the owned reference PE and every absolute operand against its relocation inventory. All target operands and complete extents are immutable. Use pinned VC5 C /Od /MT and the existing campaign-001 verifier. All callbacks below are cdecl unless explicitly stdcall. Do not change ABI, targets, flags, tools or accepted history. No raw bytes, artificial frames, warning waivers or byte exclusions.

Worker owns candidate.c, api.h, behavior.c, README.md and worker-log.json only. Work sequentially; each case stops at 15 minutes, ten candidate compiles or five consecutive nonimprovements. Record actual clock observations, each hypothesis/hash/report, failures, and explicit timestamp bases. Native compile only; coordinator executes authored fixtures in locked Docker after review. Guard fixture array/pointer accesses and use independent expected states, not copies of the candidate algorithm. Worker stop 06:49:21 UTC; publication deadline 06:59:21 UTC.
