# Prepared target: sound_pin_channel

Team-not-done bp/bapsound.c. Complete interval0x00417826..0x0041786C: 70 body/full bytes, no alignment. Complete body/instructions, next boundary, callers and import/global context reviewed. All PE operands independently verified. Pinned VC5 C /Od /MT; immutable target.

Channel array op_sound_channels[12] at5BAEA0, each128 bytes. Partial struct fields: void *sample at0, void *sample3d at4, int is3d at8, int active at0xC, unsigned flags at0x10, opaque bytes until vec3 position at0x54 (three floats), then tail through0x80. Define only needed views with padding. Assert exact offsets/stride in fixtures. No actual handle execution.

void op_sound_pin_channel(int index,const op_vec3 *position);
Use a natural local pointer to channels[index]. If active!=0, OR flags with0x20 first, then aggregate-copy three coordinate words into position+0x54. Do not add index/initialized/pointer guards. For inactive channel, null position is never read. No callee; three projectile callers pass slot and object XYZ, including initialize_tank_cannon_projectile_effects. Fixture valid indices0..11, arbitrary nonzero active, flags preservation, exact signed-zero/NaN bit copies, self-alias position, unchanged other bytes and neighbors. No invalid indices or partially overlapping typed objects.

Worker owns candidate.c/api.h/behavior.c/README.md/worker-log.json only; readable multiline fixtures. No target/shared-tool/metadata/Git changes. Same15min/10candidate-compile/five nonimprovements cap. Freeze/handoff each case promptly. Native compile only; authored fixtures only in locked Docker. Original never executed. No exclusions, fabricated frames, raw instructions or waivers. Worker stop05:07:17 UTC; publication05:17:17 UTC. Parent reviews and verifies final pairs.
