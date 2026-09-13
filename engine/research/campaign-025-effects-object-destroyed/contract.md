# Static contract

Two teardown callers. Nonnull object is passed to halo detach, then its live+100 slot is read and passed to shield stop, then the same object goes to zap detach. Ignore shield return. Preserve callback changes and owned object guards.

Full 47-byte span at `0x00438EE5`, no trailing gap. Team not_done and no local duplicate; full static instructions, call/data operands and caller references reviewed. Original game is never executed.
