# Campaign020 results

Six functions match all 856 original bytes after 27 independently verified address
operands. The locked-Docker authored fixtures pass 2,058 checks. Four functions
matched on the first worker compile; cleanup and the chunk-header reader each
needed a second natural control-flow form. The worker used eight candidate compiles.

| Function | Full bytes | Operands | Checks |
| --- | ---: | ---: | ---: |
| world_allocate | 193 | 2 | 43 |
| world_destroy | 192 | 8 | 246 |
| world_read_chunk_header | 112 | 3 | 1527 |
| world_set_clock | 67 | 2 | 16 |
| world_set_ambient | 62 | 2 | 28 |
| world_apply_fog | 230 | 10 | 198 |

Independent review replaced allocation byte-array casts with declared offset-checked
fields while retaining all193 emitted bytes. All other candidate bodies stayed
unchanged. Every fixture was reviewed and strengthened where needed. The table
reader covers every match position, unknown tags and embedded NUL semantics;
the mutation test now changes an actual table pointer. Cleanup tests verify the
complete release order and state visible during callbacks. Clock/ambient tests
use explicit x87 conditions and independently checked numeric expectations.

The parent corrected a stale destructor fixture member and a clock expected-value
typo before compilation. VC5 then rejected a parent-added pointer-array memcpy in
the chunk fixture. Explicit pointer assignments resolved its nested-qualification
warning without a waiver; the failed receipt remains in metrics.json. Final pairs
used12 candidate compiles and six successful fixture compiles, plus the failed
pair's two candidate compiles and one unsuccessful fixture compile.

Ghidra comments and three evidence-backed names were saved; affected callers were
re-decompiled. The allocation return-signature edit remains a bridge limitation,
separate from verified source ABI. The original executable and real backends were
not run. Clock tests exclude nonfinite/out-of-range integer conversions; floating
edge cases do not establish whole-game FPU state or arbitrary-compiler behavior.

This adds six focused exact results, not acceptance promotions. Registered history
remains28 functions/24 records. Next, integrate clock, ambient and fog with shared
world types and run one clean batch acceptance. The earlier three near readers and
dialog research case remain parked. Final whole-executable placement is pending.
