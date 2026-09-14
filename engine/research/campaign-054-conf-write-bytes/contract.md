# conf_write_bytes

Reject zero output handle or null data. Call cdecl service write(handle,data,count) and compare its returned unsigned bit pattern with count; exact equality returns1. Original has two stack arguments and no ECX parameter despite generated thiscall metadata.

Service offsets and cdecl argument order were cross-checked against installer00495fcc and close/read/write adapters0048695b/0048696e/00486992. The68-byte service view is partial; unused fields remain opaque. All strings, buffers, handles and service callbacks are authored and owned; no actual stream or original function executes. Full spans include every original instruction and no alignment tails. No byte exclusions or warning waivers.
