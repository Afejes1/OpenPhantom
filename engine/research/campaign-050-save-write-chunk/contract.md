# save_write_chunk

Call save_header(context,bytes,low16 kind). Return1 immediately only if header returns exactly1. Otherwise call save_write(data,bytes), returning1 on zero and0 on every nonzero result. The context is a numeric resource identifier, not a C string. Header callback mutations remain visible to the write callback. Upper16 bits of the push carrying kind have no defined meaning.

Fixtures use three guarded owned buffers, complete expected snapshots, owned/null stream tokens, five signed return words, and callback order checks. Counts0/1/8/63/UINT_MAX are forwarding observations; authored backends do not read or write by those sizes and no real file I/O occurs. Callback buffer mutation is bounded and independent of the count. All strings and storage are authored. No original game execution, copied game content, byte exclusions or VC5 warning waivers. Full-span function identity is separate from shared acceptance and whole-executable identity.
