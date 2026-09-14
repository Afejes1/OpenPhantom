# Campaign056 results

Four untouched utility functions match all 250 original bytes with all six address operands verified. Three matched on their first compile; the lowercase loop matched on its second compile after expressing the observed while-loop structure. All 223,920 authored fixture checks pass, with native/Docker function and relocation parity. No byte exclusions or original-VC5 warning waivers.

| Function | Full bytes | Address operands | Checks | Status |
| --- | ---: | ---: | ---: | --- |
| [util_file_exists](../campaign-056-util-file-exists/result.json) | 62 | 3 | 2448 | Exact |
| [util_snprintf](../campaign-056-util-snprintf/result.json) | 58 | 1 | 1008 | Exact |
| [util_strdup](../campaign-056-util-strdup/result.json) | 84 | 1 | 9216 | Exact |
| [util_strlwr](../campaign-056-util-strlwr/result.json) | 46 | 1 | 211248 | Exact |

The duplication fixture proves allocation size is captured before an allocator callback while copied source bytes are reread afterward. The variadic wrapper preserves argument forwarding and its final terminator; tests use positive sizes because the original has no zero-size guard. Lowercase tests cover all256 initial byte values, signed argument extension, returned low-byte stores, and live future-byte changes. File-existence tests prove a captured handle is closed through a reloaded service-table pointer after callback retargeting. All callbacks and data are authored.

The independent reviewer approved source contracts and rehashed56 source inputs plus8 final reports. Transient approval-service capacity failures briefly blocked parent commands and one optional Ghidra comment; they did not change the evidence, and direct access later recovered. No original game execution occurred. These four functions join the three C055 rectangles for acceptance040, preserving batched shared regression. Whole-executable identity remains pending.
