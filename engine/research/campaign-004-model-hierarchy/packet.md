# Prepared target: recursive model hierarchy submission

Team not-done entry: rdThing_drawNode, bp/bapmodel.c, 0x004100A0.
Existing Ghidra semantic name queue_model_hierarchy is retained.
Original VC5 optimized C candidate: /O2 /MT. Full target 144 bytes to 0x00410130;
RET at 0x00410126 ends a 135-byte body; nine following NOP alignment bytes are
part of the full target. Do not trim/pad source or add another function to force
extent. Body-only equality, if established separately, is layout pending.

Coordinator inspected full Ghidra body/disassembly, caller 0x0040FE70 and callee
0x0040F1E0, and independently verified the exact target with the pinned PE reader.
XREF query found the caller and self-recursion. All three PE relocations and both
near calls are in target.json. Its self-call is part of the required inventory.

Observed interface is void cdecl(node*). Partial node accesses: ordinal int at
0x44; mesh_index int at 0x4C; child_count unsigned at 0x54; first_child pointer at
0x58; next_sibling pointer at 0x5C. Unknown fields stay explicitly opaque.
The global current thing at 0x005B8E68 is a pointer with matrix-array pointer at
0x20, amputated-node int-array pointer at 0x28, hidden-mesh int-array pointer at
0x2C. Each matrix is 48 bytes (12 floats). Global selected geoset 0x005B5FE0 points
to a partial record whose mesh-array pointer is at 4; mesh stride is 112 bytes.
Caller chooses the geoset and root and builds joint matrices before traversal.
The mesh callback at 0x0040F1E0 receives mesh pointer followed by matrix pointer.

First submit this node's mesh if mesh_index != -1 and its hidden entry equals
zero. Then, when child_count is nonzero, traverse first_child and successive
next_sibling exactly child_count iterations. Skip recursion for a child whose
amputation entry (indexed by child ordinal) is nonzero. Hidden mesh does not
suppress descendants; amputated child does suppress its entire subtree. Root
amputation is not checked here. Callback order is root before children in sibling
chain order. Current thing is read again for child gates. The loop compares the
updated parent child_count after each iteration; do not freeze mutable state
without evidence. No null/cycle/invalid-index guard exists in this function.

Focused authored fixture should cover a leaf, -1 mesh sentinel, hidden mesh with
visible child, amputated subtree, sibling chain differing from array order,
mesh/matrix stride and ordinal selection, and valid callback-driven state/count
changes where useful. Exclude invalid indices/cycles/dangling pointers explicitly.
Use stub callback and authored arrays only; no original callback is executed.

Worker owns candidate.c, api.h, behavior.c, README.md, worker-log.json only.
No target/case/shared tooling/metadata/Git changes. 15 minutes or ten compiler
attempts, five consecutive nonimprovements; log source hashes and failures.
Use existing campaign-001/verify.py. Native compile only; final authored fixture
may use Docker and a native peer, with explicit listing fallback only if needed.
