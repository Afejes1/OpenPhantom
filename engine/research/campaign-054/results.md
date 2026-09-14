# Campaign054 results

Six untouched configuration-stream helpers match all 336 original bytes with 21 verified address operands. Five matched on the first compile; the string writer matched on the second after enabling the observed VC5 strlen intrinsic. Native/Docker code and relocation parity is confirmed, with 442,608 authored checks and zero final failures, byte exclusions or VC5 warning waivers.

| Function | Full bytes | Checks | Status |
| --- | ---: | ---: | --- |
| [conf_close_write](../campaign-054-conf-close-write/result.json) | 69 | 140832 | Exact |
| [conf_get_file](../campaign-054-conf-get-file/result.json) | 10 | 1296 | Exact |
| [conf_open_read](../campaign-054-conf-open-read/result.json) | 22 | 29376 | Exact |
| [conf_read_bytes](../campaign-054-conf-read-bytes/result.json) | 67 | 100608 | Exact |
| [conf_write_bytes](../campaign-054-conf-write-bytes/result.json) | 73 | 85248 | Exact |
| [conf_write_string](../campaign-054-conf-write-string/result.json) | 95 | 85248 | Exact |

Caller/callee evidence resolved stale decompiler signatures: open_read preserves its integer return, and write_bytes has two stack parameters with a three-argument cdecl service call. Guarded authored service tables cover disabled paths, signed handle words, zero/high-bit counts, exact byte-count equality, captured string lengths and callback-visible mutations. Closing clears the handle before copying 127 bytes and forcing the last name byte to zero. The reader deliberately retains the original lack of a null-data guard; those cases forward only to safe authored observers.

The 84 current snapshot inputs and 12 selected final reports are retained alongside all 16 verification invocations. Two fixture-only compile failures are preserved: a ternary int-to-char setup warning and an unused common getter helper. Both were corrected without candidate changes or warning waivers. External mode/default-name contents are authored substitutes and are not mutated by callbacks. The service structure is a partial view, no real I/O runs, and shared acceptance039 remains separate from whole-executable identity.
