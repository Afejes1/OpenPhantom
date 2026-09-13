# Prepared target: bgl_translate_negative

Team not done bp/bgl.c. Full unpadded span0x0041F85F..0x0041F895: 54bytes, 1 operands. Every instruction, PE relocation and call boundary verified against owned original before source work. Pinned VC5 C /Od /MT.

void(const OP_VEC3 *input). Local XYZ independently unary-negated with x87, then translate_vector(&local). Test signed finite/zeros/quietNaN sign as pinned; complete input captured before callback mutates input, no postwrites. Use natural unary minus, no bitwise instruction emulation.

All APIs cdecl. OP_VEC3 holds float x,y,z. OP_MATRIX holds nine float basis values followed by OP_VEC3 translation (48bytes); do not substitute integer parameter ABI. External op_bgl_current is OP_MATRIX*, identity is const OP_MATRIX. Core callees fully inspected: void op_build_rotation(OP_MATRIX*out,const OP_VEC3*axis,float angle); void op_build_translation(OP_MATRIX*out,const OP_VEC3*input); void op_multiply(OP_MATRIX*left,const OP_MATRIX*right); void op_rotate_basis(OP_VEC3*out,const OP_VEC3*input,const OP_MATRIX*matrix). Local wrapper callee prototypes follow this queue. Original full callee bodies prove no extra args or returned values.

Callers inspected: effect00439AB6, emitter spawn, projectile transform00456CBB, ripple projection; sibling wrappers corroborate ABI. Scale-vector and rotate-point have no Ghidra incoming XREF; purpose still corroborated by complete bodies/core callees, sibling layout and parameters. No strings/imports directly referenced.

Worker owns candidate/api/behavior/README/worker-log only. Parent owns target/case/packet/sharedfiles/Git.15min/10compiler invocations/five consecutive nonimprovements per case; early park permitted. Worker hard stop08:02:19UTC. Native compileonly, authored fixtures run by parent in locked Docker. Record every experiment sourcehash/report/hypothesis/time including failed compiles; no rawbytes/inlineassembly/forcedpadding/fakeframes/warningwaivers/exclusions.
