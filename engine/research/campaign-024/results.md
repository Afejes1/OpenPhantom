# Campaign024 results

Seven untouched team-not_done functions match all 416 complete bytes. All 1,357 focused checks pass, including the separately parked offset setter. The seven exact cases matched on their first implementation compile.

| Function | Original/candidate bytes | Status | Checks |
|---|---:|---|---:|
| fog_restore | 74/74 | exact | 234 |
| fog_set_end | 33/33 | exact | 62 |
| fog_set_start | 42/42 | exact | 62 |
| fog_snapshot | 35/35 | exact | 36 |
| shield_draw_released | 134/134 | exact | 357 |
| shield_set_offset | 80/83 | research | 396 |
| tint_start_opaque | 42/42 | exact | 160 |
| tint_stop | 56/56 | exact | 50 |

The offset setter is83 bytes versus80 because an explicit pointer return adds a load. With no direct callers, original return intent remains unproved. It has no valid full-span mismatch percentage and is not a near match or accepted function. No missing-return or warning-waiver workaround was used.

Independent review covered full spans, every operand, ABIs, x87 gates, callback order, full state and bounded data. Fog restoration includes all five active-world operands. Fog setter fixtures assert restored x87 control fields; released-draw callbacks change later records and counters to verify live traversal. Final native/Docker code and fixup inventories agree for all eight authored candidates.

Existing Ghidra names and analysis context were retained, comments updated and the project saved. No original game was run or modified. No byte exclusions or new warning waivers.

No full regression suite ran for this research commit. Registered acceptance remains43 functions/29 events. Seven exact cases await canonical integration and final linked placement.
