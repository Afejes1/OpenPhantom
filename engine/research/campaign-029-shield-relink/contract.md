# shield_relink contract

- Address: `0x0043CC28`; full span200, body197 bytes.
- ABI: `void __cdecl(void)`.
- Team manifest: `not_done`; no prior candidate at this address.

Open and capture an iterator from the object-pool global, scan32 live active shields, capture each saved actor ordinal, reset the same iterator, and traverse until NULL or an actor+0x100 match. On match write slot to actor+0x100, actor pointer to shield+0x10 and -1 to shield+0xB0. Close captured iterator after all rows. Requires bounded authored iterator sequences. Full200-byte target has a three-byte alignment tail;197-byte candidate remains research.

Only authored C, tests, semantic layout notes, addresses and digest evidence are public. Original executable, resource strings, compiler binaries and raw analysis remain private. External callbacks are authored boundaries until canonical integration. Exact classification requires zero differences across the full span after all validated address fixups, native/Docker code and relocation equality, and the isolated fixture. No padding insertion, byte exclusions or warning waivers.
