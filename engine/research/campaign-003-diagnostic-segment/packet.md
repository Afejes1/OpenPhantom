# Ready assignment: diagnostic segment wrapper

Team bapline_drawSegment, not_done, source line 1326, no file recorded.
Original entry 0x00408913, RET 0x0040895E: 76-byte complete body. The next
function is 0x00408960, with one original INT3 alignment byte at 0x0040895F.
The strict target retains all 77 bytes. Do not insert a trap, dummy statement,
inline assembly, padding array or change target length to force this byte.
If only the 76-byte body compiles, retain an extent mismatch and stop after
confirming the fixture; parent may independently report body evidence separately.

Two cdecl pointers to XYZ triples, each three floats. Void return. Copy the
first triple into the first element of a local two-element vector array, then
copy the second triple into the second element. These copies are bit-preserving
12-byte aggregate assignments, not numeric conversions. Natural local storage
is 24 bytes. Then call op_submit_polyline with that local contiguous array,
count=2, color=0xFF00FF00, material=NULL, width=8.0f. Sole call instruction at
0x00408953, operand offset65, targets the reviewed polyline routine at 0x408735.
No globals or PE absolute operands. Incoming Ghidra XREF query returned none;
this is a limited query, not proof of no indirect use. Complete body disassembly
and the callee interface were independently reviewed by the coordinator.

Use a meaningful op_point3 struct with x/y/z and aggregate assignment, readable
C /Od /MT. Source names are hypotheses, not assertions of original names.
Fixture must capture the callback synchronously (stack input expires on return),
check exact six input bit patterns/order and all five callback arguments, and
verify original input data remains unchanged. Include distinct coordinates,
aliased endpoint pointers and signed-zero/quiet-NaN payload bit copies through
memcpy in the authored fixture. Never execute the original callback.

One worker, ten compiler invocations / 15 minutes / five nonimprovements, but
this is a small wrapper: do not spend iterations chasing the known alignment
boundary. Native compilation only; final fixture locked Docker with opposite
native peer. If an empty Docker listing occurs, the explicit
--native-listing-fallback may be used with a fresh completed --expect-report.
Parent owns target/case/packet/shared tooling/Git; worker owns only candidate.c,
api.h, behavior.c, README.md and worker-log.json. Record every attempt and freeze.
