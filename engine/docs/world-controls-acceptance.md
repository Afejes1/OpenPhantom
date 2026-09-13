# World controls acceptance

Three Campaign020 functions use the shared OP_B3D_WORLD view: clock, ambient light and fog. The previously verified offsets and CD4 extent stay unchanged. The new fields have compile-time offset and width assertions. Source statements retain the proven candidate bodies with canonical include and type substitutions only.

The shared fixture retains focused cases and adds clock-to-ambient-to-fog execution on one guarded world, with every fog backend observing the complete expected state. Backend callbacks are authored stubs. Finite clock conversion is tested in both compiler environments. VC5-only edge cases use the existing OP_VC5_BEHAVIOR fixture guard and save/set/restore the masked,64-bit precision,round-to-nearest x87 environment. Nonfinite or unrepresentable float-to-int conversion is outside the fixture domain. This is not a byte exclusion or whole-game FPU-state claim.

All original spans and verified operands must pass the clean original-toolchain shared build before promotion. Modern MSVC smoke is supplementary. See research/acceptance-005/result.json for the eventual immutable event and source revision.
