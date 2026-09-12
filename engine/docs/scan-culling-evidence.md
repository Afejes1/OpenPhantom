# Legacy camera-space scan-plane culling

The team manifest lists 0x00403FA0 as bapdrawOld_cullAgainstPlane in bp/bapdraw.c,
not done. Static evidence supports the descriptive name
cull_camera_space_scan_plane with HIGH confidence. It does not establish an
original source name or an accepted compiled match. The
[research candidate](../research/surface-emission/culling_candidate.c) remains
outside the accepted registry.

## Inputs, callers and callback

The complete body consumes a plane index, a float triplet and a float height.
Calls at 0x00404A4E, 0x00404B12 and 0x00404C73 come from world traversal
0x00404730; calls at 0x0040590C and 0x00405947 come from cell collection
0x004056C0; calls at 0x00405C81 and 0x00405C98 are in the legacy emitter.
The body has no imported API or string references and one indirect projection
callback. These inspected paths establish camera-space scan culling, rather
than a generic geometric plane-equation test.

The working triplet starts as a copy of position. Height times the third camera
basis at 0x008BEA04 updates Y and Z unconditionally; X is updated only when the
x87 height/zero comparison is nonzero and ordered. The selection table at
0x0059DEF8 contains four integers per plane. Each successive pair chooses whether
to add the first and second camera bases at 0x008BE9EC and 0x008BE9F8. The loop
examines two corners, not four; nonzero selector values add the whole basis.

Depth is the resulting Y. Nonpositive or unordered depth returns 1 before
projection. Otherwise the routine invokes camera pointer 0x006F83E4's callback
at +0x4C with cdecl arguments (output float3 pointer, input float3 pointer).
There is no implicit camera/this argument. Camera setup 0x00404180 copies the
transform from camera+8 and establishes the scan bases and edge values.

Projection selection at 0x00475E6D chooses scalar callbacks 0x00476442,
0x00476350, 0x00476623 or 0x0047651C according to projection type and aspect.
The reviewed callback bodies put input Y into output Z (depth). Orthographic
variants use camera+0x44 scale; perspective variants use camera+0x3C focal
length divided by depth. They apply canvas centers, and the non-unit-aspect
variants scale Y by camera+0x40. The prefix declaration in culling.h establishes
only the callback offset, not the complete original camera allocation.

## Return decisions

| Plane | Return 0 when a projected corner meets | Bound |
|---|---|---|
| 0 | X <= edge | 0x008BE9D4, canvas-derived +0x20 edge |
| 1 | X >= edge | 0x008BE9D0, canvas-derived +0x18 edge |
| 2 | Z >= threshold | float32 0.08 at 0x004A8030 |
| 3 | Y >= edge | 0x008BE9D8, canvas-derived +0x1C edge |

A passing first corner returns immediately. Otherwise the second is examined;
two failed projected comparisons return 1. The observed x87 <= branch in plane
0 also accepts unordered; the >= branches in planes 1-3 do not. Invalid plane
indices have no initial bounds check before table indexing. The candidate does
not invent one, and the fixture exercises the valid four-plane domain.

## Complete comparison span and checked internal fixups

The executable body is 449 bytes, through RET at 0x00404160. Three alignment
bytes precede the embedded sixteen-byte switch table at 0x00404164; twelve
trailing alignment bytes end at 0x0040417F. The next function begins at
0x00404180. All 480 bytes must agree. The three-byte gap is not uniformly NOP.

The 22 absolute operands comprise 17 external/constant bindings and five local
bindings. The latter identify the dispatch table at function+0x1C4 and its four
case destinations at +0x144, +0x157, +0x16A and +0x17D. Each has a corresponding
retail PE base relocation. The actual VC5 objects use C_LABEL (storage class 6)
symbols in the same code section with zero addends and exact symbol values.

The dir32-internal verifier requires those properties and the declared original
destination. It resolves addresses only in a temporary comparison buffer;
instructions, branch displacements, table layout and padding still compare
exactly. There are 88 checked address bytes, including 20 internal-label bytes,
and 392 remaining bytes. No object or original executable is patched.
Seven synthetic adversarial tests reject incorrect labels, sections, symbol
classes/types, targets, addends, relocation inventories, instructions and padding.

## Current evidence and limits

VC5 RTM 11.00.7022, /O2 /MT with /W4 /WX /Zi produces a 480-byte object section.
All 22 relocation locations and all five internal label targets agree, but six
instruction bytes differ starting at function+0x68. The original loads the plane
index before storing base Y and retains base Y/Z in EBP/EBX. Using a float
array for the copied base with inline memcpy restores the original retained registers; this candidate
still stores Y before loading the plane index. Neither matching
instruction count nor passing behavior is accepted as byte equality.

The original-compiler synthetic fixture passed 3,038 checks over corner
selection, preservation, all four thresholds, short-circuit/depth rejection,
signed zero and NaNs at x87 24/53/64-bit precision with nearest rounding and
masked exceptions. Its projection callback is a stub, so it does not certify
projection implementations, emitter/culler integration, other rounding modes,
unmasked exceptions or whole-game behavior. The original was statically read
only. See the [batch receipt](surface-batch-20260912.json).

A later metadata pass stored the plain C three-argument Ghidra prototype
(int plane, float *position, float height) and refreshed the legacy emitter
caller. Position is only read by the observed function; the authored source
retains const qualification. This metadata recovery does not establish a
compiled match or add an accepted function.

The later array-copy checkpoint reduces ten differing bytes to six. All 22
operands and the complete 480-byte extent still verify, and all 3,038 focused
checks pass. Source ordering, C/C++ frontend, header and multi-function context
probes did not remove the remaining instruction-order difference. Multi-function
objects were diagnostic only; no verifier or compiler-profile policy changed.
See [the five-candidate receipt](legacy-cell-batch-20260912.json).
