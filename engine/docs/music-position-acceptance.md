# Music and playback-position acceptance

Three functions add 214 complete bytes and six verified address operands. Audio-play-position is a literal raw byte match. [Acceptance 056](../research/acceptance-056/results.md) records the successful shared checkpoint in immutable event 79.

Music uses a16-byte MCI status block plus a separate result local. An explicit VC5 memset intrinsic reproduces the observed initialization sequence; its status return is ignored. Only the callback-filled mode value determines stopped/playing. The position wrapper tests status bit1, reloads the vtable after its first callback and optionally requests the playback cursor.

All 42,640 focused assertions survive integration, including callbacks that mutate global music state or retarget an audio buffer vtable. Expected state is prepared before execution. The header exposes getters at0x10 and0x24 while preserving all previous offsets, and canonical bodies differ only by include. Prior305 target/registry entries remain unchanged. No original execution, byte exclusions or warning waivers.
