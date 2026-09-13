# world_set_clock

A NULL world is unchanged. Otherwise copy current integer ticks at 0x50 to previous
ticks at 0x58; store the truncated low 32 bits of seconds multiplied by the float
constant 1000 at 0x50; store seconds minus the previous float time at 0x5C; then store
the input seconds at 0x54. Use the original compiler's natural cast and __ftol runtime.
The 0.01f input produces 9 milliseconds because the original x87 multiplication retains
extended precision. Test guarded storage and finite, representable conversion cases with
independent expected values. Nonfinite and out-of-range integer conversions remain
outside this C fixture domain.

The complete 67-byte original interval and all address operands were independently
checked against the pinned reference. The team manifest reports bapmap_setWorldClock as
not done. The actual calling conventions, callbacks and live field ordering are recorded
in the prepared static review. Synthetic callbacks establish forwarding and ordering,
not real backend execution. No byte exclusions, raw byte insertion, assembly frames or
new warning waivers are permitted.
