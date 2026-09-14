# Campaign038 results

All six new functions match on their first implementation compile:327 complete bytes with10 verified external operands. Final paired native/Docker function and relocation checks agree, and all authored fixtures pass (1,346 checks). One function is a raw-code match; five are strict relocation-adjusted matches. There are no exclusions, warning waivers or near-match promotions.

Two strict fixture compiler failures exposed unused authored helpers. The hook tests now vary both opposite callbacks to check global independence; unrelated face scaffolding was removed from the keyframe header fixture. Candidate source and comparison criteria did not change. [All verification attempts](verification-attempts.json) preserve the superseded failed pairs alongside final successful receipts.

| Function | Full bytes | Checks |
| --- | ---: | ---: |
| [face_free](../campaign-038-face-free/result.json) | 35 | 26 |
| [face_free_arrays](../campaign-038-face-free-arrays/result.json) | 53 | 480 |
| [face_init](../campaign-038-face-init/result.json) | 138 | 24 |
| [keyframe_init_header](../campaign-038-keyframe-init-header/result.json) | 45 | 600 |
| [keyframe_set_load_hook](../campaign-038-keyframe-set-load-hook/result.json) | 28 | 108 |
| [keyframe_set_unload_hook](../campaign-038-keyframe-set-unload-hook/result.json) | 28 | 108 |

Face fixtures preserve unknown fields and guards, check cleanup order, and exercise live second-array replacement, activation and clearing after the first callback. Keyframe tests establish typed hook exchange without invocation and132-byte header clear-before-copy plus final NUL. The manifest duplicates the load-hook name at the unloader address; static consumers establish the distinction. Default-name contents are authored. Original game execution and whole-executable identity are not claimed.
