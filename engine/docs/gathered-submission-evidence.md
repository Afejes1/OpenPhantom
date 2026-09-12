# Gathered surface submission

Function 0x00406830, originally FUN_00406830, is 40 body bytes followed by eight
NOP alignment bytes before the next function at 0x00406860. The complete
48-byte section is compared. Team manifest alias: bapdraw_submitGathered,
reported file bp/bapdraw.c, reported status not done on 2026-09-12.

The sole caller reported by Ghidra is render_active_b3d_world at 0x0043F5B9,
after world traversal and surface preparation. The wrapper reads the gathered
surface count at 0x0059DEBC. Zero skips the call and leaves the previous result
at 0x008BE924 unchanged. Any nonzero 32-bit value calls 0x0041B070 with three
cdecl arguments: world from 0x0059DF58, camera at 0x006F83E4 plus eight bytes,
and the count. It stores the returned EAX word as the last submitted count.
It neither clears nor increments the input count.

The existing trusted analysis name draw_b3d_surface_buckets describes the
callee. Its reviewed body consumes the world argument while leaving the other
two formal arguments unused. The wrapper retains all three ABI arguments;
we do not remove them because a later reconstruction might ignore them.
World writers in the traversal and the render caller corroborate the roles.
No strings or imported APIs occur in this wrapper. Purpose confidence: HIGH.

Under VC5 RTM /O2 /MT, reconstructed C produces the same full section after
verified resolution of four DIR32 operands and the single REL32 CALL operand
at offset 27. Its destination is pinned to 0x0041B070, not wildcarded. The
[relative-call gate](relative-call-verification.md) rejects a different symbol,
opcode, relocation kind, addend or destination. No byte exclusion, inline
assembly or compiler diagnostic waiver is used for this function.

The behavior fixture supplies a clearly identified stub for the unreconstructed
callee. It checks zero-count no-op behavior (including null inputs), argument
order and the exact eight-byte pointer offset, one call for nonzero counts,
UINT_MAX forwarding, preservation of input count, and positive/zero/negative
returned values. These are wrapper interaction tests, not evidence that the
renderer itself has been reconstructed. The original executable is only read
statically. Clean build records determine acceptance.
