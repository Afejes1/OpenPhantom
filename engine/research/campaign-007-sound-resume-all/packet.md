# Prepared target: sound_resume_all

Team-not-done bp/bapsound.c. Complete interval0x004174EF..0x00417567: 120 body/full bytes, no alignment. Complete body/instructions, next boundary, callers and import/global context reviewed. All PE operands independently verified. Pinned VC5 C /Od /MT; immutable target.

Channel array op_sound_channels[12] at5BAEA0, each128 bytes. Partial struct fields: void *sample at0, void *sample3d at4, int is3d at8, int active at0xC, unsigned flags at0x10, opaque bytes until vec3 position at0x54 (three floats), then tail through0x80. Define only needed views with padding. Assert exact offsets/stride in fixtures. No actual handle execution.

void op_sound_resume_all(void); typedef void (__stdcall *op_sound_resume_fn)(void *handle); extern op_sound_resume_fn op_sound_resume_2d,op_sound_resume_3d;
Signed index0..11; local pointer each slot. If flags&0x40000, clear only0x40000 BEFORE callback. Choose2D when is3d==0 else3D. Actual IAT8C1748 AIL_resume_sample@4 /8C174C AIL_resume_3D_sample@4. No init/disabled/null-handle gate. Full dispatcher event9, shutdown and remove-level callers inspected. No postcallback flag stores, so current mutations retained and later slot changes observed. Fixture mixed skipped/type states, boundaries, flags/order/nullhandles, arbitrary nonzero type, callback changes to current and later slots, second call naturally skips unless callback restored bit. Bounded captures.

Worker owns candidate.c/api.h/behavior.c/README.md/worker-log.json only; readable multiline fixtures. No target/shared-tool/metadata/Git changes. Same15min/10candidate-compile/five nonimprovements cap. Freeze/handoff each case promptly. Native compile only; authored fixtures only in locked Docker. Original never executed. No exclusions, fabricated frames, raw instructions or waivers. Worker stop05:07:17 UTC; publication05:17:17 UTC. Parent reviews and verifies final pairs.
