# Campaign053 results

Six untouched module helpers match all 485 original bytes with 13 verified address operands. All six matched on their first compile. Native/Docker code and relocation parity is confirmed, with 374,612 authored checks and zero failures, byte exclusions or VC5 warning waivers.

| Function | Full bytes | Checks | Status |
| --- | ---: | ---: | --- |
| [module_find_by_id](../campaign-053-module-find-by-id/result.json) | 52 | 1080 | Exact |
| [module_find_by_name](../campaign-053-module-find-by-name/result.json) | 71 | 82072 | Exact |
| [module_get_flags](../campaign-053-module-get-flags/result.json) | 152 | 257040 | Exact |
| [module_initialize_once](../campaign-053-module-initialize-once/result.json) | 44 | 2160 | Exact |
| [module_remove](../campaign-053-module-remove/result.json) | 149 | 6340 | Exact |
| [module_remove_void](../campaign-053-module-remove-void/result.json) | 17 | 25920 | Exact |

Independent review rehashed 84 snapshot inputs and 12 final reports. The recovered 36-byte node includes links, identifier, status, an opaque callback word and a 16-byte name. Signed one-bit fields reproduce VC5 extraction; requested flag4 maps raw status bit3 into result bit2. Names and identifier lookups preserve first-match semantics. Removal patches neighboring links and global endpoints before release. Name comparison may alter the current identifier and next link, and tests check those live reads and termination. The one-time initializer retains the observed nonzero latch behavior.

Fixtures own all storage and model bounded acyclic graphs; no real heap release or original module callback runs. The isolated removal finder does not retarget head/tail, and shared acceptance will connect the actual side-effect-free finder. All six functions proceed to acceptance038; whole-executable identity remains pending.
