# conf_read_bytes

Reject zero input-active latch or zero input handle. Otherwise pass data (including null) and unsigned count to service read(handle,data,count) at offset0x38 and return1 only for exact count equality. No data-null check or backend safety claim is added.

Service offsets and cdecl argument order were cross-checked against installer00495fcc and close/read/write adapters0048695b/0048696e/00486992. The68-byte service view is partial; unused fields remain opaque. All strings, buffers, handles and service callbacks are authored and owned; no actual stream or original function executes. Full spans include every original instruction and no alignment tails. No byte exclusions or warning waivers.
