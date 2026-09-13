# Prepared target: sound_pause_all

Team-not-done bp/bapsound.c. Complete interval0x00417477..0x004174EF: 120 body/full bytes, no alignment. Complete body/instructions, next boundary, callers and import/global context reviewed. All PE operands independently verified. Pinned VC5 C /Od /MT; immutable target.

Channel array op_sound_channels[12] at5BAEA0, each128 bytes. Partial struct fields: void *sample at0, void *sample3d at4, int is3d at8, int active at0xC, unsigned flags at0x10, opaque bytes until vec3 position at0x54 (three floats), then tail through0x80. Define only needed views with padding. Assert exact offsets/stride in fixtures. No actual handle execution.

void op_sound_pause_all(void); typedef void (__stdcall *op_sound_pause_fn)(void *handle); extern op_sound_pause_fn op_sound_pause_2d,op_sound_pause_3d;
Signed index0..11; local pointer set to each current channel. If flags&0x20000 nonzero, set flags|=0x40000 BEFORE callback. Choose 2D when is3d==0, otherwise3D. IAT8C1750 AIL_stop_sample@4 /8C1754 AIL_stop_3D_sample@4. No initialized/disabled gate or null-handle guard; handles exactforwarded. Dispatcher004158F0 event8 caller inspected. Callback mutations to current flags/handles/type persist (no postcall stores); changes to later slot flags/type affect later iterations. Test mixed skipped/2D/3D, first/last slot, exact call order, flag visibility and preservation, null handles, callback mutation of later slots, repeated calls. Bounded captures avoid fixture overflow.

Worker owns candidate.c/api.h/behavior.c/README.md/worker-log.json only; readable multiline fixtures. No target/shared-tool/metadata/Git changes. Same15min/10candidate-compile/five nonimprovements cap. Freeze/handoff each case promptly. Native compile only; authored fixtures only in locked Docker. Original never executed. No exclusions, fabricated frames, raw instructions or waivers. Worker stop05:07:17 UTC; publication05:17:17 UTC. Parent reviews and verifies final pairs.
