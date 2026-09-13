# Material-bucket queue flush

The team reports **bapdraw_flushQueue**, 0x00402155 in bp/bapdraw.c, not done.
The authored [C source](candidate.c) matches all **1,112 body bytes** with
VC5 RTM /Od /MT and 24 strictly verified address operands. This is a body-only
research result. The full **1,115-byte** target still fails: three trailing
inter-function alignment bytes have not been reproduced in a verified layout.
No accepted target, registry entry or history record is created.

From the repository root with the existing private locked toolchain:

    python engine/research/queue-flush/verify.py compare --reference C:/PrivateGame/WMAIN.EXE
    python engine/research/queue-flush/verify.py behavior
    python engine/research/queue-flush/verify.py native-listing --docker-report engine/build/<comparison-build>/research-result.json

Compare returns **1** for the unresolved full span even when its separate body
comparison matches. It returns 2 for prerequisites/build failures and 0 only
when the full target matches. The body diagnostic never changes target.json.
All original address bindings are also validated against the retail PE's
relocation inventory. The original is read statically only.

Behavior compiles the candidate and [controlled backend fixture](behavior.c)
with the original toolchain and runs only the authored fixture inside locked
Docker. All 5,480 checks pass. Native-listing performs compilation only, requires
a completed current-source Docker report, and verifies identical full function
bytes and relocation metadata. It does not execute a generated program.

Each fresh private build retains source/verifier fingerprints, commands, logs,
objects, compiler listings, complete object inventories and fixture hashes.
Docker listings are explicitly flagged when their terminators are missing;
complete byte/relocation inventories remain checked against the actual object.
Native compilation provides a terminated listing for the same function.

Local names and declaration order affect VC5's unoptimized allocation. The
retained names remain descriptive, and every local has an algorithmic use.
Backend draw modes are full-width integers despite their byte packet storage;
a byte parameter declaration omits the original caller zero-extension.

The partial resource views in [flush.h](flush.h) support this call boundary.
The texture member at +0x2C marks the start of a backend descriptor, whose full
layout is outside this header. The fixture does not execute real rendering or
certify integrated backend/game behavior.

See [static and behavioral evidence](../../docs/queue-flush-evidence.md) and the
[body-match receipt](../../docs/queue-flush-body-20260912.json). Continue the
alignment/link-layout work without shortening the full target or inserting
padding bytes. The accepted baseline remains fourteen historical functions.
