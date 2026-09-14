# Campaign055 results

Three untouched rectangle functions match all 407 original bytes on their first compile. These are raw byte matches with no address operands or relocation adjustment. All four authored utility fixtures pass 103,064 checks, and native/Docker function and relocation parity is confirmed. No byte exclusions or VC5 warning waivers.

| Function | Full bytes | Candidate bytes | Checks | Status |
| --- | ---: | ---: | ---: | --- |
| [rect_overlap_inclusive](../campaign-055-rect-overlap-inclusive/result.json) | 102 | 102 | 9600 | Raw exact |
| [rect_overlap_strict](../campaign-055-rect-overlap-strict/result.json) | 102 | 102 | 9600 | Raw exact |
| [rect_union](../campaign-055-rect-union/result.json) | 203 | 203 | 4800 | Raw exact |
| [rotating_checksum](../campaign-055-rotating-checksum/result.json) | 88 | 75 | 79064 | Body exact; layout pending |

The rectangle predicates distinguish touching edges. All three retain signed comparisons after explicit32-bit wrapping, including negative/zero extents and overflow cases. A separate widened-arithmetic oracle checks all pairs of20 edge examples plus2000 deterministic full-word profiles, disjoint storage, exact self-alias and complete object guards. Union store order remains width then x, height then y, followed by possible extent growth. No partial-alias guarantee is claimed.

The checksum body is75 raw bytes equal to the original in both final builds; its unchanged88-byte target includes13 missing alignment bytes, so it remains research with no full-span percentage. The [separate diagnostic](../campaign-055-rotating-checksum/body-diagnostic.json) is parent-verified. The reviewer completed all56 source and8 report checks, but capacity prevented its last read of that optional diagnostic. Three exact functions will join the next utility cohort for acceptance040, reducing full regression frequency. Whole-executable identity remains pending.
