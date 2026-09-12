# Render queue reconstruction evidence

This group follows the matched projection and mesh render-state functions.
Names and partial C layouts describe observed behavior, not original symbols.

## Projected-vertex span lookup

0x00402130 (FUN_00402130) has a complete 37-byte body ending at 0x00402154;
the next function begins at 0x00402155. It reads the count at 0x004DD6EC,
adds the requested count with 32-bit wraparound, compares the result unsigned
against 8192, and returns null if larger. Otherwise it returns the buffer at
0x004BD6C0 plus count times 16. It does not modify the count or buffer.
Both count operands and the buffer operand are explicit DIR32 bindings.

Four incoming calls were inspected through XREFs: queue_model_mesh_faces
(0x0040F588), submit_screen_space_polyline_strip (0x00408744),
draw_sprite_quad (0x004279CA), and render_linked_sphere_effect (0x0043C19C).
The mesh and polyline callers pass the returned float4 buffer to the matched
transform/project routine. The mesh caller later invokes 0x00402046 to advance
the count and prepare fog values. Initialization at 0x00401D30 and the material
flush at 0x00402155 reset this counter.

The original upper-bound check is not overflow-safe validation. In particular,
count one plus request UINT_MAX wraps to zero and returns slot one. The candidate
retains that arithmetic. A valid backing-buffer counter is a caller invariant;
arbitrary corrupted counts are not claimed to yield valid C pointers.

## Face packet lookup

0x00402021 (FUN_00402021) has a complete 37-byte body ending at 0x00402045;
0x00402046 begins immediately afterward. It compares the count at 0x004DD6E0
as signed against 4096, returning null at or above the limit. Otherwise it
returns 0x004DD6F0 plus count times 184. No count or packet bytes are changed.
There is no lower-bound check; a nonnegative valid counter is a caller invariant.

The two incoming calls are in mesh submission (0x0040F6F6) and the screen-space
polyline helper (0x0040874F). Mesh submission fills the returned packet and calls
0x00401EB8. This separates availability, initialization, and successful commit.

## Face submission by material

0x00401EB8 (FUN_00401eb8) occupies 361 bytes through 0x00402020, with no padding
before the following lookup helper. It contains no calls, imported APIs or
strings. All 17 absolute operands bind the observed queue globals.

The argument is a packet whose next pointer is at +0 and model-face pointer at
+4. Mesh submission stores its RD_MODEL_FACE_TPM pointer there. The face's +0x20
field supplies the material key; it is not the packet's material field. Partial
structures declare only the accessed fields and the observed packet stride.
The complete original face type and other packet fields remain separate work.

The routine's observable order is:

1. Initialize a stack index; return zero immediately for a null packet.
2. Copy the low byte at 0x008BFBA0 into packet +0x17.
3. Return zero for a null face or null face material, keeping that tag write.
4. Reuse the cached bucket at 0x004DD6C0 when its material key matches; otherwise
   search existing 12-byte buckets at 0x005956F8 in ascending index order.
5. If no key matches, select the next bucket and increment the signed count at
   0x005959F8. A count above 64 is decremented again before returning zero.
   The rejected packet's next pointer, cache, and packet count remain unchanged.
6. Clear packet->next. An empty bucket receives the material, head and tail.
   Otherwise append to the previous tail's next pointer and advance the tail.
7. Publish the bucket cache, increment the packet count at 0x004DD6E0, return one.

An existing material can still accept packets when all 64 buckets are occupied.
The routine performs no independent 4096-packet capacity check. Its caller must
use the availability helper. Queue ownership, valid initialized bucket storage,
and non-overlapping live packets are caller invariants.

The sole incoming call is 0x0040FB4D in queue_model_mesh_faces, after filling
indices, projected-buffer pointers, clipping, and opacity. The flush consumer
0x00402155 walks bucket heads and packet next links, obtains textures from each
bucket's material key, uses the packet tag at +0x17, and clears the queues.
Those independent producer/consumer observations support HIGH confidence.

## Candidate and validation scope

The three independently compiled candidates match all 435 code-section bytes
in focused probes after 23 verified address relocations (92 bytes). No padding
occurs between these original function boundaries. Full clean-build records
remain the acceptance evidence for the committed source.

VC5 assigned different stack slots when descriptive local identifiers changed.
All 24 declaration-order permutations retained the initial mismatch. The names
current_bucket, search_index, material_key, and pFace reproduce the observed
slots while leaving the code's operations unchanged. The original source names
remain unknown; these are reconstructed identifiers retained for code generation.

The reconstructed C candidates use /Od /MT and no inline assembly or diagnostic
waivers. All complete section bytes and every relocation are acceptance inputs.
The current generated progress report and chained records identify which builds
have actually passed; an unrecorded candidate is not an accepted match.

Synthetic queue tests cover buffer boundaries, unsigned request wraparound,
non-reserving repeated lookups, partial failure writes, null packet/face/material,
cached and searched material hits, FIFO links, all 64 buckets, rejection of a
sixty-fifth distinct material, and continued existing-material submissions at
capacity, and committing the final available packet before subsequent lookup
rejection. Tests do not dereference pointers produced from corrupted counters.

## Remaining module work

The vertex commit/fog routine at 0x00402046 has been inspected but is not yet
reconstructed. It requires verified relative-call handling for the CRT conversion
helper and a double constant. Initialization, material flushing, the full mesh
submitter, and original translation-unit/link boundaries remain unfinished.
