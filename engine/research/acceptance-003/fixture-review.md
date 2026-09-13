# Shared world-reader fixture review

Started at `2026-09-13T12:43:00.1796467Z`. The fixture includes the canonical `b3d_chunk_readers.h`, exports `op_test_world_readers()`, has no `main`, and defines only the shared allocator and stream-reader callbacks required by that header. Every fixture-local symbol is prefixed `world_readers_` for safe textual inclusion.

All three real reconstructed readers operate on the same guarded canonical world, header, and stream types. Each case copies the complete guarded header and world before the target call and applies only explicit target or callback writes to the expected copy. Callback logs are bounds-checked and pointer values refer to owned fixture storage.

The cases cover all three mismatch exits without callbacks, zero sizes, nonzero wrapped products, `0x80000000` payloads observed as `INT_MIN` with unchanged bits, signed backend returns, null allocation forwarding, allocator-before-reader order, and reader-time mutation retention. The sprite allocator case changes the live header count after the outer byte count is captured. A cross-function case invokes the real palette reader from the sprite allocator, then proves that the outer sprite read retains its original 120-byte size while observing the new live sprite count of five.

The stream callback is synthetic and performs no I/O. These forwarding cases make no claim that null destinations or zero, wrapped, or huge sizes are valid with the real compressed-stream backend. Parent review and the integrated build remain pending.

Worker review finished at 2026-09-13T12:47:56.5634140Z. The reviewed fixture SHA-256 is e6a786ec747e1ad08a00e9ade7f4658fcfe0dac83aa48975b33e0098acd4b715. Both owned files contain LF-only ASCII text.

Parent review found that successful three-reader sequencing was not exercised,
and the independent allocator mutation mode was defined but unused. Added both
with pre-call expected state, plus exact callback counts, unchanged stream checks
and the palette high-bit boundary. All three now observe signed INT_MIN while
preserving the original0x80000000 argument word. No candidate, target or matcher
was changed during fixture review. Compilation and acceptance remain pending.
