# Campaign032 results

Five exact functions, 304 complete bytes, 12 verified operands and 8906553 focused checks. All were marked not_done by the team manifest. Final native and Docker code/relocations agree, with verified native listing fallback where the Docker listing is empty. No byte exclusions, warning waivers or artificial padding.

| Function | Full bytes | Checks |
| --- | ---: | ---: |
| projection_expire | 96 | 6881177 |
| projection_remove | 64 | 1179631 |
| projection_clear | 32 | 832416 |
| projection_sort | 48 | 1324 |
| projection_compare | 64 | 12005 |

Seven implementation compiler invocations covered eight recorded attempts. One sort binding preflight failed before compilation and was corrected from rel32 to the runner-supported rel32-call; no compiler waiver. Counted typed-pointer traversal reproduces the expiry/removal register loops. The comparator uses unsigned address words and sequence keys. Its Ghidra location is still a label because function creation failed; static instructions establish the contract.

Fixtures cover every pool slot, complete record clearing/preservation, signed token/dirty boundaries, strict finite expiry boundaries, callback mutation and unsigned sort order. The sort fixture observes qsort; libc integration belongs to the batch checkpoint. This is function evidence, not whole-executable identity.
