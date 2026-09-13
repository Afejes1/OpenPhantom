# world_destroy

NULL returns one; every nonnull world returns zero. A zero load-state skips all
releases, including the world itself. With a nonzero initial load-state, conditionally
release the actor block at 0x1F4 when count 0x1E8 is nonzero, then clear that pointer.
Call the five cleanup helpers in the recorded order. Release the live pointer at 0xA0 if
nonnull without clearing it, then release the world. Later callbacks cannot cancel the
already-entered load gate. The fixture records calls without freeing its owned storage.
Check callback changes, order, all conditional branches and whole-object retention.

The complete 192-byte original interval and all address operands were independently
checked against the pinned reference. The team manifest reports bapworld_free as not
done. The actual calling conventions, callbacks and live field ordering are recorded in
the prepared static review. Synthetic callbacks establish forwarding and ordering, not
real backend execution. No byte exclusions, raw byte insertion, assembly frames or new
warning waivers are permitted.
