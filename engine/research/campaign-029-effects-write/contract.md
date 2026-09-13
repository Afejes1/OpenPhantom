# effects_write contract

- Address: `0x0043912F`; full span262, body262 bytes.
- ABI: `int __cdecl(int opaque_context)`.
- Team manifest: `not_done`; no prior candidate at this address.

The caller supplies an opaque signed word. Zero all 96 saved bytes, snapshot six fog globals and world bit0, retrieve renderer RGB/range, store shield and overlay sizes, calculate the total using the separately accumulated local, and emit header, saved block, shields and overlay in order. Always return0. Renderer and serializer callbacks can change later-read state; already captured inputs/sizes remain captured. Addition inputs must stay within signed int range.

Only authored C, tests, semantic layout notes, addresses and digest evidence are public. Original executable, resource strings, compiler binaries and raw analysis remain private. External callbacks are authored boundaries until canonical integration. Exact classification requires zero differences across the full span after all validated address fixups, native/Docker code and relocation equality, and the isolated fixture. No padding insertion, byte exclusions or warning waivers.
