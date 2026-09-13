# sprite_release contract

- Address: `0x004295F4`; full span71, body71 bytes.
- ABI: `int __cdecl(void** slot)`.
- Team manifest: `not_done`; no prior candidate at this address.

NULL slot or NULL handle returns0 without callbacks. Capture the handle, release it, zero the local and then caller slot, return1. A callback replacement of the slot is intentionally overwritten; changes to other slots remain. Tests include owned alias handles and exact callback order.

Only authored C, tests, semantic layout notes, addresses and digest evidence are public. Original executable, resource strings, compiler binaries and raw analysis remain private. External callbacks are authored boundaries until canonical integration. Exact classification requires zero differences across the full span after all validated address fixups, native/Docker code and relocation equality, and the isolated fixture. No padding insertion, byte exclusions or warning waivers.
