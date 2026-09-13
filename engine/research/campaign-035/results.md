# Campaign035 results

Nine new exact functions, 422 complete bytes, all raw code matches with zero relocation operands. Every candidate required one implementation compiler attempt. All ten final native/Docker code pairs agree and all owned fixtures pass, totaling 5,382 assertions across the ten focused fixtures. The independent reviewer found no material defect.

| Function | Full / candidate bytes | Result | Checks |
|---|---:|---|---:|
| list_add_after | 57 / 57 | exact-function-match | 12 |
| list_advance | 40 / 40 | exact-function-match | 3780 |
| list_append | 61 / 61 | exact-function-match | 12 |
| list_count | 44 / 44 | exact-function-match | 504 |
| list_cut_before | 34 / 34 | exact-function-match | 18 |
| list_first | 42 / 36 | research-candidate | 504 |
| list_init | 24 / 24 | exact-function-match | 12 |
| list_insert_before | 56 / 56 | exact-function-match | 12 |
| list_last | 38 / 38 | exact-function-match | 504 |
| list_unlink | 68 / 68 | exact-function-match | 24 |

`list_first` remains research. Its separately verified36-byte body is exact, but the six required alignment bytes are unresolved, so the full42-byte comparison fails and no mismatch percentage is reported. No padding insertion, target shortening, warning waiver or byte exclusion was used. Further source variants are deferred until there is new layout evidence.

Fixtures use detached distinct insertion nodes, literal graph transitions, guarded payloads and finite acyclic traversal with signed step boundaries. Cycles and arbitrary insertion aliases are outside the fixture contract. Original game code was not executed. The nine exact functions await one shared acceptance checkpoint; whole-executable identity remains pending.
