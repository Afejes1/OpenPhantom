# Ready assignment: screen-space polyline strip

The team reports bapline_drawPolyline, not_done, no original file, source line
1325. Existing trusted descriptive name is submit_screen_space_polyline_strip.
Original entry 0x00408735, RET 0x00408912, next function 0x00408913: complete
478-byte body/span with no alignment. The entire disassembly was reviewed.
There are four direct calls and three absolute operands, all in target.json.
All five callers were inspected: green diagnostic 0x408913, depth-four helper
0x43D5F5, and named object-linked/opaque/blended projectile zapline routines.
They supply world XYZ arrays, count, color, material and screen-space width.

## Interface and dependencies

void op_submit_polyline(const float *points, int count, unsigned int color,
                        void *material, float width);
Use the existing baseline.h definitions for projected vertices, face packets,
peek functions, op_transform_project and op_camera. Do not modify baseline.h.
The backend callback at 0x487C50 accepts material, unsigned flags, pointer to
32-byte vertices, integer vertex count and integer final mode; its integer
return is ignored. It wraps 0x487D20 and 0x487C90. Name it op_submit_polyline_quad.
Every callback in a fixture must be an authored stub, not original code.

The projected input layout is four floats x,y,reciprocal_depth,mapped_depth.
This naming is confirmed by the projection callee's reciprocal and depth-mapping
writes. An older Ghidra comment calling the tested component camera depth is
imprecise: the original tests mapped_depth at byte offset 12.
The output vertex is a natural 32-byte struct: float x,y,z,rhw; unsigned int
color,specular; float u,v. Here z receives mapped_depth and rhw receives
reciprocal_depth. Four contiguous output vertices are a real array, not artificial
frame storage. Original locals consist of that array plus projected pointer,
face-packet pointer, and loop index (140 bytes total), with ESI/EDI saved.

## Observed control and writes

1. Call op_peek_projected_vertices(count), then op_peek_face_packet() even if
   the first call returns null. If either result is null, return. The packet
   pointer is only an availability gate; it is not passed to the backend or
   otherwise touched. Do not remove this call or change its ordering.
2. Call op_transform_project(points, (float*)projected, NULL, NULL, count,
   (const float*)((const unsigned char*)op_camera + 8)). The global camera
   pointer is loaded at 0x408768 and advanced eight bytes. No extra null guard.
3. For i from zero while i < count-1, fill output[0] x,y,rhw,z from projected[i]
   in that assignment order (rhw before z). If output[0].z <= 0.0f, return from
   the entire function, preserving earlier submissions. Then set its color from
   the argument, specular=0x00FFFFFF, u=0, v=0.
4. Fill output[1] from projected[i+1] in the same x,y,rhw,z order and perform
   the same early return. Set color, specular, u=1, v=0.
5. Copy the full output[1] struct into output[2], add width to output[2].x,
   set its u=1,v=1. Copy output[0] into output[3], add width to output[3].x,
   set u=0,v=1. These are ordinary aggregate assignments: original copies use
   eight-word REP MOVSD. Do not copy opcode sequences or fabricate frame slots.
6. Submit (material,0x48E00,output,4,1), ignore its return, then continue.

Both depth comparisons use x87 FCOMP against the verified float zero at
0x4A809C and test AH mask 0x41. Negative/zero and unordered values take the
return branch. Preserve original compiler behavior; do not add sanitization.
Widths may be negative or zero; the original does not clamp them. No temporary
buffer count or packet count is advanced here. Count zero or one still performs
both peeks and the projection callback when buffers exist, then no submission.
Avoid INT_MIN count and overflow in focused fixtures.

## Focused checks and limits

Use authored stubs with explicit call order and argument recording, including
both null-buffer permutations, both-null, count zero/one, one valid segment,
multiple segments, early return at first/second endpoint, and later bad depth
after a successful submission. Check all four output records (all eight fields),
color/material/flags/count/mode, matrix pointer +8, null indices/codes and exact
projection arguments. Preserve input arrays; do not claim original backend
integration. Include negative/zero width, zero/negative/positive mapped depth,
and a quiet-NaN depth built safely by memcpy if practical under the pinned CRT.

Keep the complete target immutable. Use original VC5 C /Od /MT, native compile
only, locked Docker fixture execution. No raw byte insertion, frame packing,
warning waivers or byte exclusions. Ten worker compiler invocations or 15 minutes,
whichever comes first, with five-no-improvement stop; final coordinator review
is separate. Record every hypothesis, hash and report path. If stuck, preserve
the best honest candidate and explain the blocker instead of weakening matching.
