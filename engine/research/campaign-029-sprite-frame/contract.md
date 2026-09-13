# sprite_frame contract

- Address: `0x0042959D`; full span87, body87 bytes.
- ABI: `void* __cdecl(void* sprite)`.
- Team manifest: `not_done`; no prior candidate at this address.

NULL outer sprite returnsNULL. Nonnull sprite requires a valid material at+0x84. Capture material and frame at material+0xB0 before the preparation callback(material,frame,0). Zero callback result returnsNULL; every nonzero result returns the captured frame+0x2C even if callback retargets the source fields. Successful cases require owned frame storage through that offset; no inner NULL behavior is claimed.

Only authored C, tests, semantic layout notes, addresses and digest evidence are public. Original executable, resource strings, compiler binaries and raw analysis remain private. External callbacks are authored boundaries until canonical integration. Exact classification requires zero differences across the full span after all validated address fixups, native/Docker code and relocation equality, and the isolated fixture. No padding insertion, byte exclusions or warning waivers.
