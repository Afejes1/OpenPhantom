# sprite_acquire contract

- Address: `0x00429540`; full span64, body64 bytes.
- ABI: `void* __cdecl(char* base_name)`.
- Team manifest: `not_done`; no prior candidate at this address.

The stack frame contains an80-byte formatting buffer and a separate4-byte result. Call the original formatter ABI with an external format symbol and base name, then acquire tag0x53505254 using that same buffer. Ignore formatter result. ReturnNULL for failed acquire, otherwise its captured result. Tests use bounded authored names/output; original format/resource strings are not included and arbitrary input lengths are not claimed safe.

Only authored C, tests, semantic layout notes, addresses and digest evidence are public. Original executable, resource strings, compiler binaries and raw analysis remain private. External callbacks are authored boundaries until canonical integration. Exact classification requires zero differences across the full span after all validated address fixups, native/Docker code and relocation equality, and the isolated fixture. No padding insertion, byte exclusions or warning waivers.
