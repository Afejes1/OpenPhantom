# conf_close_write

If output handle is nonzero, capture it and call service close at offset0x34; ignore result. Then clear output handle, copy127 bytes with strncpy semantics from the fixed closed-name pointer, and explicitly set output-name byte127 to zero. Close/copy callbacks can mutate globals, and the final store ordering is retained. Zero handle touches nothing.

Service offsets and cdecl argument order were cross-checked against installer00495fcc and close/read/write adapters0048695b/0048696e/00486992. The68-byte service view is partial; unused fields remain opaque. All strings, buffers, handles and service callbacks are authored and owned; no actual stream or original function executes. Full spans include every original instruction and no alignment tails. No byte exclusions or warning waivers.
