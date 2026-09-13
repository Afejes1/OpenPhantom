# Prepared target: surface-at-or-below presence query

Team not-done alias row at 0x0040C9F8 contains both above and below names. Static
comparison resolves the operational distinction: preceding function 0x0040C915
(reported done) counts vertices below the query; this function counts vertices
strictly above it. Reconstruct only 0x0040C9F8. This is a vertex-height gate plus
XY-containment query; it does not compute interpolated polygon height.

Coordinator reviewed complete body/disassembly, adjacent counterpart and called
iterator/geometry/XY helpers. XREF-to query returned none; no claim of global
unreachability. Original interval is 0x0040C9F8 through RET at 0x0040CADA inclusive:
227 bytes, no alignment before function 0x0040CADB. All two PE base relocations and
five call operands independently checked against the pinned original.

Observed cdecl interface: position float[3], flag mask in a 32-bit argument slot;
only low 16 bits can intersect the surface flags. Returns integer1 for qualifying
surface and0 when iteration ends. Global world pointer 0x008A0060 is passed first
to collect(world,position,0.0f), followed by reset(), then repeated next() calls.
next() returns pointer or null. No null position guard here.

Partial surface fields used: unsigned byte vertex_count at0x25, unsigned short
runtime_flags at0x3C. For each surface, skip unless flags & mask is nonzero.
Geometry helper receives(world,surface,vertices,normal), filling natural local
arrays of four XYZ vertices (48bytes) and one XYZ normal (12bytes). The original
72-byte local frame holds those arrays plus intindex, surfacepointer, unsigned
above-count. Preserve normal output even though the query does not read it.
The valid surface fixture domain is at most four vertices; do not turn this into
an input-validation change or permit buffer overflow in synthetic tests.

Count each vertex whose Z is strictly greater than position.Z. If above-count
== vertex_count, continue without calling XY containment (also rejects count0).
Otherwise call contains(position,surface,vertices); any nonzero return produces
integer1 immediately. Zero continues scanning. End-of-iterator produces0.
Equality permits containment. Mixed vertex heights may qualify even if actual
plane height is elsewhere. Original x87 test masks0x41: unordered, equal, and
less do NOT increment above-count. Exercise NaN carefully under pinned VC5.

Original locals observed relative to EBP: vertex array-0x30, index-0x34,
surfacepointer-0x38, unsignedabove-count-0x3C, normalarray-0x48. Use natural locals,
not a fabricated stackframe or rawinstruction insertion. The original null-exit
uses EAX from next() directly; do not add undefined nonvoid fall-through solely
to imitate it. If ordinary defined C cannot reproduce that shape within the cap,
preserve the candidate and report the exact blocker rather than adding waivers.

Fixture should cover empty iterator; mask rejection and upper bits; allabove;
equality; mixed heights; zerovertices; XY false then latertrue; nonzeroXYreturn
normalization; call order and argument identity; world reload when callbacks
change it; query/geometry preservation; NaN query or vertex; four-vertex boundary.
All callbacks are authored stubs. No original execution.

Worker owns candidate.c,api.h,behavior.c,README.md,worker-log.json only; coordinator
owns target,case,packet,sharedtools/Git. 15-minute/ten-compiler cap and no-improvement
stopping rule apply. Use campaign-001 runner, nativecompileonly, authoredDocker
fixtures. No shortening fullspan or ignoring changed operand positions.
