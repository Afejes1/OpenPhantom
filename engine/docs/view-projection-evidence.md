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
  remains pending; a decompiler rendering is not accepted as implementation.
- 0x0040F170 combines indexed per-vertex clip codes for a face. Its incoming call
  at 0x0040F5EF is in queue_model_mesh_faces, after the projection call.

The mesh caller tests the reducer's result against 0xFF80. It rejects a face if
any behind-camera bit is set or if all vertices share a clipping plane bit.
Otherwise it retains the low-byte union for later clipping. The current Ghidra
signature for the caller's rendering omits an argument; stack disassembly, not
the inferred decompiler signature, establishes this reducer's two arguments.

## Face clip-code reducer

Address: 0x0040F170. Current analysis name: FUN_0040f170.
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

Match the complete transform/project routine at 0x0040EFD0, including selected
vertex indices, optional clip output, zero-depth behavior, depth-mode selection,
and the original x87 evaluation order. Then inspect module boundaries and the
downstream clip/queue consumers before claiming a complete original module.
