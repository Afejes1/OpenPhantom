# Campaign022 results

Six functions from the team's not-done list match exactly: 319 complete bytes. Two small emitter wrappers also match (29 bytes), but the manifest lists them as done; they are explicitly excluded from the priority score and from promotions. Two cell helpers remain near matches: 9/258 and 6/249 bytes, all verified as one-byte EBP displacement differences. These are not accepted matches.

All ten packets passed independent read-only source/API/fixture review and final native/Docker equality. Authored fixtures passed 3,952 checks, zero failures. The eight exact cases matched on their first implementation compile. Cell experiments stopped at the nine-attempt cap after about three minutes. There were 26 implementation candidate compilations, 22 final candidate compilations, and 11 fixture compilations; one fixture compile failed on two byte conversions, was corrected with explicit casts, and passed without weakening /W4 /WX.

The table-based shield tests cover all32 slots, signed bounds, active/inactive records, arbitrary full-word setters, color byte limits, and finite/signed-zero/quiet-NaN radius returns under an explicit x87 control word. The no-save flag also bypasses visibility classification; the stable visibility-bypass research ID is retained with both meanings documented.

No shared registry or acceptance history changed. No full regression suite was rerun for this focused packet. Original game execution, whole-executable parity and final linked placement are not claimed.

| Function | Bytes | Result | Fixture checks | Team status |
|---|---:|---|---:|---|
| cell_last_lod_flags | 249 | near-match | 22 | not_done |
| cell_lod_runs | 258 | near-match | 26 | not_done |
| emitter_clear_event | 12 | exact-function-match | 20 | done |
| emitter_shutdown | 17 | exact-function-match | 28 | done |
| get_system_font | 10 | exact-function-match | 24 | not_done |
| model_load_state | 14 | exact-function-match | 24 | not_done |
| shield_radius | 67 | exact-function-match | 1032 | not_done |
| shield_set_colour | 94 | exact-function-match | 1096 | not_done |
| shield_set_visibility_bypass | 67 | exact-function-match | 840 | not_done |
| shield_set_visible | 67 | exact-function-match | 840 | not_done |

See [metrics](metrics.json), [review](review.json), [rules](rules.md), and [diagnostics](diagnostics.json).
