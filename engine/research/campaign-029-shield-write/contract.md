# shield_write contract

- Address: `0x0043C9B1`; full span331, body331 bytes.
- ABI: `int __cdecl(void)`.
- Team manifest: `not_done`; no prior candidate at this address.

Count32 active&&!stopping rows, copy count to a distinct serialized word, emit4 bytes, and return0 if the original count<=0. Otherwise scan32 live rows again, additionally excluding no_save. Emit52-byte stack snapshots: slot, visible word, elapsed bits, first31 name bytes plus NUL, radius bits, ARGB. Original count/emission eligibility discrepancy is retained. Callback mutation of the serialized count cannot change the captured early gate. All callback-written fields remain live for later rows.

Only authored C, tests, semantic layout notes, addresses and digest evidence are public. Original executable, resource strings, compiler binaries and raw analysis remain private. External callbacks are authored boundaries until canonical integration. Exact classification requires zero differences across the full span after all validated address fixups, native/Docker code and relocation equality, and the isolated fixture. No padding insertion, byte exclusions or warning waivers.
