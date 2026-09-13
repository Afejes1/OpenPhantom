# Shared world chunk reader acceptance

The emitter, sprite-reference and palette readers use one partial OP_B3D_HEADER,
OP_B3D_WORLD and stream prefix with compile-time width/offset checks. Header fields
at20,2D0,2E8,2EC,800 and world fields64,68,6C,CB4,CB8,CCC,CD0 retain their verified
positions. Names distinguish the unrelated count fields. Unknown bytes remain
opaque; these are not claims of complete original structures.

The original four-stack-argument stream backend uses signed division at47080A and
470871; its shared declaration takes int size/count. Reader products stay unsigned
and the size call argument explicitly converts to int, preserving the low32 bits
with VC5. Allocator495290 takes one32-bit size argument and returns storage. The
three function bodies retain original control flow and reload order; full byte
comparison is required after these type/name conversions.

The combined authored fixture runs the actual three readers against shared guarded
world/header storage and explicit allocator/read callbacks. It checks sequential
reader state, mismatch retention, callback-time values, allocation failure, live
header reload, wrapped payload sizes and signed-boundary argument words. The real
stream backend and game are never executed. Zero/NULL/huge I/O cases establish
forwarding only; final whole-executable placement remains pending.

See ../research/acceptance-003/results.md for completed evidence when available.

## Vertex-reader extension

Acceptance004 adds global and local vertex records at header48/4C and50/54,
world94/9C andA4/AC, with opaque32-byte and28-byte records. Existing fields and
structure extents are unchanged. The global payload mismatch returns0; the local
payload mismatch returns1. Both capture the starting record pointer before the
loop, reload signed count and read size across callbacks, and copy the final live
count only after success. The shared fixture includes both actual readers in
sequence, empty and high-bit counts, signed size words, callback mutations,
guarded original/alternate buffers and partial failures. See
../research/acceptance-004/results.md for the eventual acceptance receipt.
