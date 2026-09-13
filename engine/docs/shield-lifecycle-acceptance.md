# Shield lifecycle acceptance

Acceptance009 integrates nine exact functions (1,000 complete bytes) with the four previously accepted shield accessors. All share one 0xB4 record and a partial actor view whose shield slot is at 0x100.

The provisional field0 owner pointer is corrected to an integer active flag. The attached actor is at0x10. Existing accessor symbols remain stable; their fixtures now use explicit nonzero integer active values. Field0x0C is named no_save and also bypasses visibility-volume classification. The name field is a bounded extent view to the next record, not a recovered capacity promise.

The shared fixture retains the authored calloc, free, stop, save-size, texture and released-draw cases. It additionally links actual destroy_all -> destroy -> shield_free -> release, and object teardown -> shield_stop. NULL buffers reach shield_free but do not call the heap backend. Callback boundaries verify intervening clears, full-table state, guards, pointer identity, live counts, later-record mutation and no revisits of earlier records. An accessor/stop/save sequence verifies the unified field layout.

Existing allocator/release callbacks route to the shield fixture only while its explicit scope is active. No original game code or game assets are executed. External heap, sprite, halo, zapline and renderer callbacks remain authored fixtures. Original string names are not included. The unchecked source name copy is retained for byte identity; fixtures use bounded owned strings.

Admission requires a clean committed source build, all registered full-span comparisons, the isolated authored behavior fixture, tooling tests and the full supplemental CI checkpoint. Final executable placement, data and resources remain unverified. No new byte exclusions or warning waivers.

The modern supplemental build uses an equivalent explicit NUL-copy loop for the texture name. VC5 11.00 retains the original strcpy intrinsic branch verbatim. No secure-CRT capacity is invented and no warning is suppressed.
