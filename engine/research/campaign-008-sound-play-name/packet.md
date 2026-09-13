# Prepared target: sound_play_name

Team-not-done bp/bapsound.c. Complete interval0x00416787..0x004167C3, 60 bytes, no alignment exclusions. Full assembly/body/boundary and caller/callee context reviewed; all operands and complete PE relocation inventory independently validated. Pinned VC5 C /Od /MT.

int op_sound_play_name(const char *name,unsigned flags); extern int op_sound_initialized,op_sound_mode; int op_sound_play_descriptor_name(int slot,const char *name,int *handle,float *position,unsigned flags);
When initialized==0 return -1 and touch nothing. Otherwise mode=1 then return local saved result of play_descriptor_name(-1,name,0,0,flags). No later mode reset/init re-gate. Full caller00463031 pushes flags1 then name and pops8; callee004171A1 consumes exactly five stack arguments, copies64-byte default descriptor, applies name/flags then calls known start_sound_from_descriptor. Ignore old phantom ECX/thiscall metadata. Fixture no-op guard with null name, positive/negative init, flags all bits including existing0x800, argument pointer identity (stub need not dereference), signed return forwarding and callback mutation preservation; no assumptions about name lifetime beyond call.

Worker owns candidate.c/api.h/behavior.c/README.md/worker-log.json only. Same15min/10candidate compile/five nonimprovements cap. Native compile-only; authored fixture execution only locked Docker. No target/tool/Git/Ghidra changes, raw instructions, warnings or exclusions. Freeze and hand off; workerstop05:43:20 UTC, publication05:53:20 UTC.
