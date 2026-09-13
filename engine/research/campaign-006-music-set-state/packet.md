# Prepared target: requested music state setter

Team-not-done bp/bapmusic.c, 0x004105A3..0x0041060E: complete107-byte body/span,
no alignment. Coordinator inspected full body/instructions, next entry, eight
callers and backend thunk. PE identity, complete DIR32 inventory and direct call
target independently verified. Immutable target.json; VC5 C /Od /MT.

API int op_music_set_state(int requested), cdecl one stack argument. Ghidra's
extra thiscall argument is an inference error: body reads [EBP+8], and verified
enable/dispatcher callers push one int and clean4 bytes. Globals int
op_music_initialized (0x5BAB8C), op_music_state (0x4AA41C), op_music_lock_depth
(0x5BAB90). Model actual import slots as typedef void (__stdcall
*op_music_lock_fn)(void); extern op_music_lock_fn op_music_lock, op_music_unlock.
Direct callback int op_music_backend_set_state(int state) maps to0x49A2FC.
No original imports execute; fixtures assign authored lock/unlock stubs.

Return0 without callbacks/mutation when initialized==0, or when requested equals
current state. Any nonzero initialized value enters. Otherwise write requested
state BEFORE lock(); increment the current depth AFTER lock; invoke backend with
CURRENT state reloaded after lock; preserve its returned int across unlock();
decrement current depth after unlock; return the preserved backend result unchanged.
No return normalization, rollback, playback-success interpretation, extra gates or
restoration of callback-mutated globals. One ordinary saved-return local is needed.

Fixture: both gates incl non-1 initialized, negative/boundary requested IDs without
overflow, cached state visible to lock, backend argument reload after lock changes
state, counter visibility/order, callback-mutated depth/state/initialized, ignored
gate changes after entry and preserved positive/zero/negative backend results across
unlock. Exclude signed counter overflow and real backend integration explicitly.

Worker owns candidate.c/api.h/behavior.c/README.md/worker-log.json only. No targets,
shared tooling, metadata or Git edits. Per function15min/10candidate invocations or
five successive nonimprovements. Log every attempt/hash/report; freeze immediately
on exact native match. Parent performs final native/Docker pair and receipt review.
Native compile only; authored fixture execution only in locked Docker. No raw bytes,
fabricated frame storage, warning waivers, exclusions or original execution.
