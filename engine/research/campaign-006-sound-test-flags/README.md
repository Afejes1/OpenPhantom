# Sound descriptor flag test

The natural local pointer advance and mask match all 45 bytes with the world binding verified. The fixture checks the 64-byte stride, 0x18 flags offset, masks, high bits, negative in-array indexing, world changes, and no mutation. Worker report: `engine/build/campaign-001-sound_test_flags-7921812a1ab9/research-result.json`.

Final parent review and fresh native/Docker verification confirm the complete 45-byte relocation-adjusted match with 13 focused checks passing.
See [result.json](result.json) for source, toolchain, object, fixture and report hashes.
The verified native-listing fallback was used after code/fixup equality; no bytes
were excluded. Final executable layout and accepted-history promotion remain pending.
Parent review corrected a fixture-only snapshot overread before execution; the
candidate and target were unchanged. Both full owned arrays are now snapshotted
from their allocation bases, including the valid interior-base negative-index case.
