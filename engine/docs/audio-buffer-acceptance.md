# Audio buffer wrapper acceptance

Three functions add 137 complete bytes, with one verified global address operand and two literal raw matches. [Acceptance 056](../research/acceptance-056/results.md) records the successful shared checkpoint in immutable event 79.

The recovered vtable exposes play at offset 0x30 and unlock at 0x4C. All prior member offsets remain asserted, including cursor 0x34, frequency 0x44 and stop 0x48. Source bodies are include-only copies of frozen independently verified candidates; previous 302 registry and target entries stay unchanged.

All 42,620 callback assertions are retained in shared tests. Expected snapshots are captured before candidate calls, callbacks verify their incoming state and arguments, and final checks preserve deliberate changes to table pointers, payloads and the audio gate. Pause NULL is tested only with gate zero. Unlock pointer/count patterns are inspected as opaque metadata without accessing arbitrary ranges. Original program execution and whole-executable identity remain outside this function checkpoint.
