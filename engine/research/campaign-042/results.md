# Campaign042 results

Six new exact functions cover 414 complete bytes with nine verified external operands. All matched on their first implementation compile. Final native/Docker function bytes and relocations agree; authored fixtures pass 1,967 checks. No byte exclusions or warning waivers.

| Function | Full bytes | Checks |
| --- | ---: | ---: |
| [colormap_free_entry](../campaign-042-colormap-free-entry/result.json) | 104 | 1456 |
| [colormap_load](../campaign-042-colormap-load/result.json) | 85 | 232 |
| [light_free](../campaign-042-light-free/result.json) | 35 | 26 |
| [light_free_inner](../campaign-042-light-free-inner/result.json) | 5 | 5 |
| [light_init](../campaign-042-light-init/result.json) | 139 | 24 |
| [light_new](../campaign-042-light-new/result.json) | 46 | 224 |

Colormap tests cover full844-byte clearing, captured allocation, common cleanup, live flag/secondary-handle changes during release, and clearing each current handle after its callback. Allocation failure forwards NULL into cleanup in the original; focused execution observes that call without dereference. Connected integration must use valid roots for the actual cleanup implementation.

Light tests use guarded64-byte roots, literal initialization bit patterns, preserved uninitialized fields, captured allocation, initializer return independence, and cleanup-before-root-release order. The empty inner cleanup is included because it is an actual called dependency.

One incorrect review suggestion removed a required fixture loop index. Its resulting compilation failure is retained in the attempt history; the declaration was restored and all final hashes/receipts were independently checked. Function candidates did not change.

These are function-level matches. Whole-executable identity and final linker layout remain pending.
