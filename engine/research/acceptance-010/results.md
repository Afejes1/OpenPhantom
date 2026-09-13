# Acceptance010 results

Ten fog and tint functions (773 complete bytes) are promoted. All 62 registered functions pass the full original-toolchain comparison. All 3,647 new effects checks and prior behavior suites pass. The full supplemental modern CI checkpoint passed with warnings treated as errors; 100 tooling tests and 19 focused runner tests passed.

The integrated fixture uses actual restoration -> world fog application, opaque tint -> initializer, and reset -> tint stop. It verifies full state, callback order, live world retargeting, captured RGB versus live ranges/flags, color-byte preservation, timing and terminal paths. Original x87 unordered and midpoint cases remain explicitly separate from modern supplemental behavior.

The first build exposed an older active-world type and a fixture narrowing diagnostic. The second passed behavior but the broad world-header inclusion changed three previously accepted VC5 functions. The final forward declaration/compatibility alias and localized complete definition restored those exact matches, verified first with four focused compile-only probes and then with the complete checkpoint. All failure receipts remain preserved. No source warning was suppressed and no comparison byte was excluded.

Registered history now contains 62 functions and 31 immutable events. Event `000031-813d65433fd48837.json` records source `1c1ac435183dbd7015675953c5e1a79c0b59805e`. All 113 input hashes were verified against current files and committed Git blobs. Original game execution and final executable identity are not claimed. Clock producer source precision and fade-reset lifecycle provenance remain explicitly unresolved.
