# Campaign 076

Three complete first-attempt matches: skills new game51 bytes, easier27, and harder27. Total105 bytes and11 verified operands. All final native/Docker pairs agree and each matrix passes41,472 assertions (124,416 total). Shared checkpoint completed in [Acceptance054](../acceptance-054/result.json), grouped with the three C075 table matches.

The state has216 bytes with difficulty at offset192. The original initialization zeroes the whole object, sets difficulty4 and clears both selection indexes. Easier/harder retain their signed predicates and one-step behavior for malformed values; they do not clamp. Full object snapshots and independent globals cover both int extrema safely.

Base-plus-addend bindings preserve the shared state alias. No byte exclusions, raw-byte insertion or warning waivers. Incidental return registers are not claimed as an API. The original executable was not run.
