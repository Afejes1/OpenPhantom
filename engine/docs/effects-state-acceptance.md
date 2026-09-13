# Fog and tint state acceptance

Acceptance010 integrates ten complete functions (773 bytes) using the existing canonical B3D world. The timed initializer is named op_start_timed_fog_transition to avoid the existing op_fog_start renderer variable. Its source ID and original address remain stable; the function symbol is recompiled and compared.

The shared fixture links actual fog restoration to world_apply_fog, opaque tint to the full initializer, and reset to tint-stop. A transition sequence covers snapshot, direct start, mode0/mode1 initialization, finite interpolation and completion. Backend callbacks verify the pre-call stores, retained arguments, live state reloads, low-byte color masks, upper color-word preservation and full owned-state snapshots. Restoration's RGB callback retargets the active world, while world_apply_fog must continue using its original world argument; range/flags remain live and RGB locals remain captured.

All six existing world renderer callbacks route through the effects fixture only within its explicit scope. World-control and shield route flags/counters are checked on exit. External renderer and clock callbacks are authored fixtures. No original game is executed.

Original x87 unordered branches and midpoint rounding assertions are VC5-only. Modern supplemental coverage uses explicit floating-control APIs and finite dyadic cases, with no claim of original code generation. Arithmetic NaN is checked by class, not payload. The clock producer's original source precision remains unresolved; ST0 return and caller binary32 storage are verified. Fade-reset behavior is reconstructed, but lifecycle provenance remains unresolved because no direct callers were found.

Admission requires a clean committed source, full registered byte comparisons, authored isolated behavior, tooling tests and full supplemental CI. Final executable layout, data and resources remain pending. No byte exclusions or new warning waivers.

The first integrated fixture compile exposed an older op_world_fog view and active-world definition in frame setup. Frame setup now shares OP_B3D_WORLD; its used offsets210/218/21C and implementation remain unchanged. The existing frame fixture owns the sole active-world pointer. The failed build is preserved. An explicit byte cast was also added to the opaque-alpha fixture oracle, without a warning waiver.

A complete B3D header included through baseline.h also changed VC5 output for three older functions despite passing behavior. The final boundary uses a forward declaration and the compatibility alias op_world_fog for the same canonical tag, with the complete definition included only where fields are accessed. Four compile-only probes verify the three recovered matches and frame setup before the full checkpoint. This illustrates why accepted code is re-compared at shared integration checkpoints.
