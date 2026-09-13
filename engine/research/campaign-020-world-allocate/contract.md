# world_allocate

Allocate exactly 0xCDC bytes. Failure returns NULL without calling the timer. Success
zeroes the whole allocation, calls the timer, and writes the clock, capacity, ambient
and fog defaults in the observed order. The timer sees zeroed storage; its writes to
unrelated fields survive, and subsequent default stores overwrite only their own fields.
The standard memset intrinsic is supported by existing source policy. Use guarded,
aligned, owned fixture storage and verify the whole resulting object. Never call the
original allocator.

The complete 193-byte original interval and all address operands were independently
checked against the pinned reference. The team manifest reports bapworld_alloc as not
done. The actual calling conventions, callbacks and live field ordering are recorded in
the prepared static review. Synthetic callbacks establish forwarding and ordering, not
real backend execution. No byte exclusions, raw byte insertion, assembly frames or new
warning waivers are permitted.
