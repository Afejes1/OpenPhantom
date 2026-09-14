# conf_write_string

Reject zero output handle or null text. Capture unsigned strlen once, call service write(handle,text,length) at offset0x40, and return1 iff exact returned byte count equals captured length. Empty strings with live handles still invoke write with zero length; callback mutation cannot change the captured expected count.

Service offsets and cdecl argument order were cross-checked against installer00495fcc and close/read/write adapters0048695b/0048696e/00486992. The68-byte service view is partial; unused fields remain opaque. All strings, buffers, handles and service callbacks are authored and owned; no actual stream or original function executes. Full spans include every original instruction and no alignment tails. No byte exclusions or warning waivers.

Original SCASB loop is reproduced with the VC5 strlen intrinsic directive, following existing intrinsic conventions. Initial non-intrinsic library-call attempt remains recorded.
