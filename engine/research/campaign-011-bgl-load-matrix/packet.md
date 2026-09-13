# Prepared target: bgl_load_matrix

Team not done bp/bgl.c; full0x0041F910..0x0041F929, 25bytes and1 verified operands. Pinned VC5 C /Od /MT.

All interfaces cdecl. OP_BGL_MATRIX is an opaque48-byte representation with unsigned int words[12]; words preserve affine float bits without arithmetic. extern OP_BGL_MATRIX *op_bgl_current; extern int op_bgl_depth; extern OP_BGL_MATRIX op_bgl_stack[64]; extern const OP_BGL_MATRIX op_bgl_identity_matrix. Declare only globals each case needs. Original identity contents independently verified as3x3 identity plus zero translation; original data is not committed. Original matrix stack base004AB2E0, currentpointer004ABEE0, signeddepth006BFA3C, identity004AB2B0. Callers include particle/emitter rendering00422130, linked sphere0043BF9E and transformed effect00439AB6; body stack setup and matrix copies corroborate no-argument/single-pointer/three-output ABI. Each target has no callees, imports or strings. Use natural C structure assignment/copy for full matrices; no fabricated assembly/frames. All targets complete and unpadded.

void op_bgl_load_matrix(const OP_BGL_MATRIX *source). Copy all48 bytes from supplied source to CURRENT *op_bgl_current; no gates, pointer/depth changes or other stores. Fixture distinct source/destination and alternate current, identical-object copy, fullbitpatterns including NaN/negativezero representations without FP operations, copy independence and neighbors. Do not test partially overlapping distinct struct objects: normal C aggregate assignment domain only; no nullactive pointers.

Worker owns candidate/api/behavior/README/worker-log only. Parent owns targets/flags/sharedfiles/Git/metadata. Every case15min/10compiles/five nonimprovements, freeze early useful results. Worker hardstop07:31:19UTC. Native compileonly; parent reviews and executes authored fixture in locked Docker. Record every attempt hash/hypothesis/report/clock plus timestampsbasis. No raw instruction bytes, inlineassembly, added spanpadding, fake stackframes, byte exclusions or warningwaivers.

Final review refinement: matching source uses the standard memcpy intrinsic.
The initial same-object fixture suggestion is superseded by disjoint-copy cases
that follow its C non-overlap contract. Full original code comparison is unchanged;
see result.json and independent review for precise evidence and limits.
