# Campaign026 results

Seven untouched team-not_done functions match all 455 complete bytes. All 9,954 focused checks pass across eight candidates. Overlay restore remains research: its 51-byte emitted body does not cover the complete 59-byte original span, which includes an eight-byte alignment tail. No tail is excluded or synthesized.

| Function | Original / candidate bytes | Checks | Evidence |
|---|---:|---:|---|
| detach_halo | 103 / 103 | 648 | exact-function-match |
| effects_draw_object | 69 / 69 | 4656 | exact-function-match |
| halo_shutdown | 44 / 44 | 280 | exact-function-match |
| halo_startup | 59 / 59 | 315 | exact-function-match |
| letterbox_set_enabled | 81 / 81 | 3750 | exact-function-match |
| overlay_read_state | 33 / 33 | 110 | exact-function-match |
| overlay_restore_state | 59 / 51 | 85 | research-candidate |
| overlay_save_state | 66 / 66 | 110 | exact-function-match |

Independent review covers all 50 operand bindings, callbacks, pointer identity, live state reloads and signed inputs. Final native and Docker code/fixup inventories agree for every case. Per-case receipts preserve source snapshots, report hashes, toolchain identity and the verified native listing fallback.

Nine implementation compiles were used. The first object-draw compile diagnosed an unused argument; a standard void cast documented that unused argument, and the second compile matched without a warning waiver. Six other exact cases matched on their first compile. The overlay restore was parked after its first extent mismatch.

Halo resource initialization, shutdown and per-object detach now have evidence-backed semantic Ghidra names. Existing comments are preserved, affected callers are re-decompiled, and the project is saved. No original game execution, original resource strings, byte exclusions, forced padding or warning waivers.

No full regression suite ran for this research commit. Registered acceptance remains 62 functions / 31 events. Seven exact candidates await canonical integration; no whole-executable identity is claimed.
