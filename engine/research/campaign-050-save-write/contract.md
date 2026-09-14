# save_write

Forward the original data pointer and unsigned byte count to save_write_raw and preserve its full EAX return word. The stale void decompiler signature is contradicted by callers testing EAX.

Fixtures use three guarded owned buffers, complete expected snapshots, owned/null stream tokens, five signed return words, and callback order checks. Counts0/1/8/63/UINT_MAX are forwarding observations; authored backends do not read or write by those sizes and no real file I/O occurs. Callback buffer mutation is bounded and independent of the count. All strings and storage are authored. No original game execution, copied game content, byte exclusions or VC5 warning waivers. Full-span function identity is separate from shared acceptance and whole-executable identity.
