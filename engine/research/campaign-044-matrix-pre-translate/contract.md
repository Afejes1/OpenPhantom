# matrix_pre_translate

Cdecl direct signed-float component addition to translation offsets36/40/44, with sequential stores and no calls. Full58-byte raw target. Literal binary32 oracles under saved/restored masked x87,53-bit precision,round-nearest include signed zeros,cancellation,ties-to-even,subnormals,overflow and infinities. Test disjoint input and exact translation self-alias. Arbitrary partial overlap is outside the authored fixture domain because earlier stores may change later inputs.

Team done. Status is checked by numeric address in the supplied manifest. Parent independently confirmed every complete span, operand and ABI against static disassembly/callee access. Original VC5 /Od /MT; no exclusions or warning waivers. Fifteen-minute/ten-attempt per-function cap, five non-improving attempt stop. Native compilation only; authored fixtures execute in locked Docker. Original game never executed.
