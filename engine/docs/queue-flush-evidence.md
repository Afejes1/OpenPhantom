# Queue flush reconstruction evidence

The team manifest marks 0x00402155, bapdraw_flushQueue in bp/bapdraw.c, not done.
Ghidra already calls it draw_model_material_buckets; that descriptive name is
retained. The source is [candidate.c](../research/queue-flush/candidate.c).
Behavioral identification has HIGH confidence from the complete static body,
five callers, five direct callees, queue writers and the shared field layouts.
This is an independently authored reconstruction, not recovered original text.

## Match scope

VC5 RTM 11.00.7022 with /Od /MT /W4 /WX /Zi reproduces all 1,112 bytes from
0x00402155 through RET at 0x004025AC. All 17 DIR32 operands (68 bytes) and
seven REL32 calls (28 bytes) verify symbols, addends, constants and original
destinations. The other 1,016 body bytes agree exactly. Reference and resolved
body SHA-256 are both:

a461436170687debfe6cbb3a8fba88f631bb194389e1eea3a591ecb7dbead1a5

Three INT3 alignment bytes occupy 0x004025AD..0x004025AF before the next
function at 0x004025B0. They are present in the original span and absent from
the standalone /Od object. The pending target stays **1,115 bytes**, so its
strict full-span comparison remains a size mismatch. The separate 1,112-byte
body comparison grants no acceptance and makes no linked-placement claim.
There are no ignored bytes, inserted padding, warning waivers or comparator
changes. Full-span acceptance must wait for verified layout evidence.

## Call graph and ABI

Observed incoming calls are 0x0043EA50 in run_game_frame, 0x004111EA and
0x0041180C in render_bap_objects_and_advance_animations, and 0x0045C220 /
0x0045C3C1 in the corresponding enclosing routines. The flush takes no
arguments and returns no used value. The small caller at 0x0045C214 invokes it
before backend flush/finalization calls when its frame-state flag equals one.
The function references no strings or imported APIs directly.

| Callee | Calls in flush | Observed contract |
|---|---|---|
| 0x00488210 | 3 | Set render-state flags; returns the prior state, ignored here |
| 0x0047B9BD | 1 | Prepare material/texture at mip 0; zero means fallback, nonzero means ready |
| 0x00488510 | 1 | Bind the descriptor at texture+0x2C or a null descriptor |
| 0x00487260 | 1 | Immediate draw of 32-byte vertices, full-width count/mode |
| 0x00487C50 | 1 | Build/enqueue a draw item; returns a queue-item pointer, ignored here |

The last callee forwards five arguments to 0x00487D20 and submits its result
through 0x00487C90. The immediate draw eventually passes the vertex buffer to a
backend interface. These are controlled stubs in the focused fixture, not
newly accepted implementations of the callees.

## Data views and traversal

The existing accepted bucket layout is 12 bytes: material, head and tail.
The flush returns immediately for a signed bucket count <= 0, preserving all
queue state. Otherwise it sets initial frame flags, visits each bucket, takes
its material's first texture at +0xB0 and requests mip 0. A failed preparation
binds null and uses both UV scale factors as 1. A ready texture binds the address
at +0x2C; its +0x1C bitmap pointer supplies unsigned width/height at +0x0C/+0x10.
The reciprocals are stored as float32. The header does not describe the complete
texture allocation or backend descriptor.

| Packet offset | Recovered field |
|---|---|
| +0x00 / +0x04 | Next packet / face pointer |
| +0x08 / +0x0C / +0x10 | Projected positions / pixel UVs / optional colors |
| +0x14..+0x17 | Count / mode / opacity / render tag, each one byte |
| +0x18 / +0x68 | Projected and UV index arrays |

Null-face packets are skipped. For each corner, the projected index selects
X/Y/RHW/Z from the 16-byte accepted projected-vertex view. Output is
X/Y/Z/RHW followed by diffuse, specular, U and V, totaling 32 bytes. A separate
face index from face+0x18 selects color and fog; it is not the projected index.
Pixel UVs are multiplied by the two stored reciprocal dimensions.

Absent colors produce opaque white even when packet opacity requests the
queued alpha path. Otherwise opacity != 255 replaces the color's high byte.
With vertex fog enabled, the fog index is the projected base's offset in the
global projected array plus the face index. With fog disabled, specular is zero.

Opacity 255 uses the immediate path. Other values, including zero, use the
queued path with 0x600, plus 0x40000 when face flags contain 0x10. A nonzero
render tag clears frame flag 0x40 on either path. The mode byte is promoted to
a full-width integer argument; the original explicitly clears the upper bits.
After each bucket, all three fields are cleared. After all buckets, bucket
count, last-bucket pointer, face-packet count and projected-vertex count reset.
Both the packet next pointer and the outer bucket count are reloaded after
callbacks as observed in the instructions.

## Reconstruction and focused checks

The initial ordinary C/C++ versions compiled to 1,180 bytes with the correct
0x860-byte stack frame. Private compiler diagnostics isolated local-symbol
ordering from frame size. Readable semantic identifiers and reversed declarations
recover the original variable placement. Correcting byte backend-mode parameters
to integer parameters restores two missing zero-extensions. Every local,
including the 64-entry vertex workspace, serves the reconstructed algorithm;
there is no fabricated frame, inline assembly or raw-byte insertion.

The final Docker fixture passes **5,480 checks** without warnings. Coverage
includes 0..20 corners, independent projected/UV/face indices, unsigned high-bit
dimensions, signed-zero and NaN bit-preserving copies, successful/failed texture
preparation, six opacity values across both color paths, three render tags,
face-state bits, enabled/disabled fog, zero-count and null-face packets,
nonpositive bucket counts, exact cleanup, and controlled callback changes to
packet links and bucket count. A zero-count packet can have null unused inputs.
These owned test bounds do not prove the original's allocation limits.

Native compilation reproduces identical body bytes and relocation metadata and
provides a complete listing. Docker listing truncation remains explicitly
flagged; complete private object inventories are independently checked.
No original game or generated native program executes. No full regression,
accepted-function rebuild or recovery cycle was repeated.

The same work session tested 160 source variants of the existing emitter,
culler and static collector. None improved their strict match status; those
retained candidates are unchanged. Their report hashes are recorded alongside
the new [flush receipt](queue-flush-body-20260912.json), so later work can avoid
repeating those expression, scalar and fade-lifetime experiments.
