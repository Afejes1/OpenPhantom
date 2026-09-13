# Campaign023 results

Six previously untouched team-not_done shield lifecycle functions match all 486 bytes, with eight verified address operands. All 4094 focused checks pass with zero failures. Five functions matched on the first compile; save size matched on attempt four after meaningful local names reproduced the original stack slots.

| Function | Complete bytes | Checks |
|---|---:|---:|
| shield_calloc | 56 | 128 |
| shield_destroy_all | 46 | 197 |
| shield_free | 23 | 12 |
| shield_save_size | 110 | 489 |
| shield_set_texture | 148 | 2740 |
| shield_stop | 103 | 528 |

The parent and independent Sol reviewer confirmed full spans, ABIs, call/data bindings, callback order, state preservation and bounded fixture domains. Final native/Docker code and relocation equality was verified after correcting the provisional owner-pointer field to an integer active flag. Initial verification receipts remain private; current source hashes are pinned in each final receipt.

Three high-confidence Ghidra names and six comments were applied, affected callers re-decompiled, and the project saved. The original executable was not executed or modified.

No full regression suite ran for this research commit. Registered acceptance remains 43 functions and 29 immutable events; these six await canonical shield integration. No near matches, byte exclusions or new warning waivers. Final linked placement and whole-executable identity remain pending.
