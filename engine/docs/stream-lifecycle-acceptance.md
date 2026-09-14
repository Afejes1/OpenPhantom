# Stream lifecycle acceptance

Three functions contribute 291 complete bytes and four verified call operands. Canonical source differs from focused candidates only by header include. The previous 290 target and registry entries remain unchanged. Shared acceptance pending.

The plain wrapper allocates and zeros 0x306c bytes; packed open allocates and zeros 0xf06c bytes and initializes packing/window state. Both retain the original file argument through allocator callbacks. Packed-open allocation failure closes the captured file; plain-open failure leaves it open. Stream close captures the file, releases the wrapper and returns the captured value. A null input returns null without callbacks.

Three retained guarded matrices pass 17,472 focused assertions. Allocator and close observers check pre-initialization snapshots. Release mutation tests distinguish captured return values from reloaded fields. The synthetic harness routes the existing allocator, release and diagnostic-close symbols to these observers while active.

No byte exclusions, padding, raw-byte insertion, ignored operands or warning waivers. These are complete function matches with resolved calls; original final executable placement and whole-binary identity remain unfinished.
