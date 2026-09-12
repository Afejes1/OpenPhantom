# View/projection reconstruction evidence

Reference: the SHA-256-pinned retail WMAIN target in target.json. Ghydra session
on 2026-09-12 confirmed TPM:/WMAIN.EXE, x86 Windows, base 0x00400000.
Original bytes were read statically; the original game was not executed.

This is a research grouping, not a claim that original source-file boundaries
have been recovered or that the complete module is finished.

## Scope and call relationships

- 0x0040EF40 updates projection globals from the active camera. Already matched.
- 0x0040EFD0 transforms/project vertices and produces per-vertex clip codes.
  Eight incoming calls were found, including mesh submission, sprites,
  screen-space polylines, particles and linked sphere effects. Reconstruction
  uses a reconstructed inline x87 kernel; see [kernel evidence](x87-projection-evidence.md).
- 0x0040F170 combines indexed per-vertex clip codes for a face. Its incoming call
  at 0x0040F5EF is in queue_model_mesh_faces, after the projection call.

The mesh caller tests the reducer's result against 0xFF80. It rejects a face if
any behind-camera bit is set or if all vertices share a clipping plane bit.
Otherwise it retains the low-byte union for later clipping. The initial Ghidra
signature omitted an argument. Stack disassembly established both arguments;
the recovered signature now exposes the clip-code table at 0x005B6038 in the
caller. The reducer was renamed combine_face_vertex_clip_codes, its evidence
comment was added, and the analysis program was saved.

## Face clip-code reducer

Address: 0x0040F170. Original analysis name: FUN_0040f170.
Current name: combine_face_vertex_clip_codes.
Reconstructed symbol: op_face_clip_flags. Confidence: HIGH for observed behavior.

The first argument points to a partial face layout: unsigned count at +0x14 and
32-bit vertex-index pointer at +0x18. The second is a per-vertex byte-code array.
The function starts with union zero and intersection 0xFF, reads each indexed
byte, updates both accumulators, and returns intersection shifted by eight OR
union. A zero count returns 0xFF00 without dereferencing either array. Valid
indices and count/array bounds are caller obligations; this routine adds no
bounds checks.

The loop setup tests count using an unsigned branch; a signed negative-count
interpretation from decompiler output is not used. The loop has no calls, imports,
referenced strings, or absolute-address operands. The observed instruction body
is 58 bytes, ending at 0x0040F1AA. Six NOP alignment bytes follow through
0x0040F1AF, before the next function starts. The candidate comparison covers all
64 bytes, with no relocation exclusions and no stripped padding.

Synthetic behavior tests cover zero count, one vertex, reordered indices,
duplicate indices, disjoint plane bits, a common plane bit, and the behind-camera
bit used by the caller. Compiler matching and behavior results are recorded
separately in the generated progress report and immutable build history.

## Remaining work

The update, transform/project, and face-code reduction routines now have matching
candidates. Inspect original module boundaries and downstream clip/queue consumers
before claiming a complete source module. The [x87 evidence](x87-projection-evidence.md)
records the source-form inference, compiler advisory, precision, signed-zero,
selection, and clipping contracts.
