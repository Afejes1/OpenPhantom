# Campaign059 results

Six untouched platform helpers match182 complete original bytes and13 operands on their first compile. All54,810 authored checks pass with native/Docker function and relocation parity. Independent review verifies84 source inputs and12 final reports. No byte exclusions, original-VC5 warning waivers or original execution.

| Function | Full bytes | Operands | Checks |
| --- | ---: | ---: | ---: |
| [platform_alloc_debug](../campaign-059-platform-alloc-debug/result.json) | 17 | 1 | 2,916 |
| [platform_free_debug](../campaign-059-platform-free-debug/result.json) | 17 | 1 | 2,430 |
| [platform_lock_identity](../campaign-059-platform-lock-identity/result.json) | 8 | 0 | 1,458 |
| [platform_realloc_debug](../campaign-059-platform-realloc-debug/result.json) | 21 | 1 | 3,402 |
| [platform_seconds](../campaign-059-platform-seconds/result.json) | 68 | 5 | 22,302 |
| [platform_ticks](../campaign-059-platform-ticks/result.json) | 51 | 5 | 22,302 |

The three heap wrappers preserve argument words and returned pointer identity; lock is a pointer identity function. Observers exercise owned/null pointers and unsigned sizes without actual allocation. Both timers choose a branch before callback invocation; QPC writes signed64 and its return is ignored. The scale is read after callback mutation. Tick fallback preserves unsigned time bits in an int result; seconds widens the unsigned time then divides by the exact1000.0f literal. Bounded finite vectors and caller-stored binary32 expectations avoid unproved nonfinite or extended-precision claims. Some fallback cases repeat over irrelevant high-resolution vectors, so check count is not unique case count.

The preliminary reviewer packet had two arithmetic/offset mistakes; independent parent checking resolved both before source experiments. Realloc uses relocation+12 and cohort total13. All candidates were prepared within the15-minute cap. Shared acceptance will follow with the next reviewed cohort; whole-executable identity remains pending.
