# Campaign057 results

Three untouched utilities match all765 original bytes and nine address operands. Callback sorting matched411 bytes on its first compile. Token and quoted-string extraction matched on their fourth compile after token-aware renaming of their existing meaningful locals reproduced VC5 stack allocation. No fabricated locals, stack padding, byte exclusions or VC5 warning waivers.

| Function | Full bytes | Operands | Checks |
| --- | ---: | ---: | ---: |
| [util_quoted_string](../campaign-057-util-quoted-string/result.json) | 176 | 2 | 454,656 |
| [util_sort_range](../campaign-057-util-sort-range/result.json) | 411 | 4 | 1,085,920 |
| [util_tokenize](../campaign-057-util-tokenize/result.json) | 178 | 3 | 326,656 |

All1,867,232 authored checks pass with native/Docker function and relocation parity. The independent reviewer verified42 source inputs and six final reports. Lexical fixtures cover returned delimiter versus closing-quote-plus-one, absent quotes, NULL output, bounded truncation, zero-size unsigned clamp wrapping, live callback mutations and full guards. Sort uses independent final ordering, live callback snapshots and a literal seven-event schedule for [3,1,2], plus signed extrema and duplicate values in both directions.

The three matches will join the next window-state cohort for acceptance041. External lexical callbacks are authored observers, and no original executable runs. Whole-executable identity remains pending.
