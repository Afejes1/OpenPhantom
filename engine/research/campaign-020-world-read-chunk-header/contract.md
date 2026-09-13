# world_read_chunk_header

Read one 12-byte local record containing an eight-byte tag followed by an unsigned
payload size. Initialize the output payload to zero before the read. A zero read returns
-1 and retains callback changes to the output. A nonzero read scans up to 23 live table
entries with an eight-byte bounded comparison, copies the local payload to the output,
and returns the matching index or 23. Use a natural struct plus signed index local, with
size and offset assertions. The table fixture contains authored strings; no original
tags are needed. Exercise every index, no match, negative-success and zero-failure
returns, table mutation, output mutation and exact callback arguments.

The complete 112-byte original interval and all address operands were independently
checked against the pinned reference. The team manifest reports bapworld_readChunkTag as
not done. The actual calling conventions, callbacks and live field ordering are recorded
in the prepared static review. Synthetic callbacks establish forwarding and ordering,
not real backend execution. No byte exclusions, raw byte insertion, assembly frames or
new warning waivers are permitted.
