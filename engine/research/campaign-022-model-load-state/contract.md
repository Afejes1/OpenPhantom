# Static contract

cdecl int(const OP_MODEL*), no NULL guard. Complete14-byte span0045C47B..0045C488 before nextentry0045C489. Reads exact dword+0xD0, returns unchanged bits. Callers pass object stored at owner+0x34 and test zero/nonzero. Signedness of nonzero state is not established by those callers; source int preserves all32 bits. No calls/globals/relocations. Partial record extent0xD4 covers the read, not a claim of full model size.
