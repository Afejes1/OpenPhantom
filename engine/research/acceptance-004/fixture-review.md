# Shared vertex-reader fixture review

The worker froze integration at2026-09-13T14:02:01.2574427Z, with no compiler
or fixture execution. The coordinator verified both source bodies by reversing
only canonical type/header/field substitutions, and verified all26 earlier target
and registry entries remain unchanged. The existing eight test-function bodies
retain identical non-whitespace text; all original test registrations remain.

The new types preserve independently observed offsets and32/28-byte record strides,
with original header804/worldCD4 extents unchanged. Both actual readers operate
on the same guarded world/header and typed owned record storage. Backend stubs
perform no I/O. Huge and zero sizes are forwarding evidence only.

Parent feedback added guarded original/alternate record arrays, first-read failure
with callback mutations for each function, both INT_MIN size words and both
zero-size positive-count paths. Final parent edits additionally check every new
read's count, stream and event, verify first-reader storage before recapturing it
for the second reader, and exercise ordinary empty input through both actual
readers. Shared sequence, payload mismatch asymmetry, signed high-bit bounds,
wrapped products, negative success, middle failure, live count/size changes and
captured-pointer behavior are retained. Candidate bodies were not changed.

Compilation, exact comparison and immutable acceptance remain pending.

Acceptance complete: final fixture SHA256eb4e40c6a6fe2db59bf75be995f9bf01480e89771a3f1a1e70020b541d756a38 passed488 checks,0 failures. All28 registered spans match; see result.json and immutable event24.
