# Campaign039 results

Five new exact functions cover344 complete bytes with15 verified external operands. All paired native/Docker function and relocation checks agree, and all authored fixtures pass (1,392 checks). No byte exclusions, warning waivers, synthetic stack padding or near-match promotions were used.

Ten implementation compiles were bounded across five functions. The loader matched on its second source variant using an explicit common failure cleanup label. FreeEntry matched on its fifth attempt when the traversal pointer was declared inside the block where it is used, leaving the unsigned index in the outer scope. Pointer naming and declaration-order trials alone did not change its ten stack-slot differences. All final code is ordinary typed C.

| Function | Full bytes | Checks |
| --- | ---: | ---: |
| [keyframe_free](../campaign-039-keyframe-free/result.json) | 59 | 84 |
| [keyframe_free_entry](../campaign-039-keyframe-free-entry/result.json) | 127 | 804 |
| [keyframe_load](../campaign-039-keyframe-load/result.json) | 102 | 324 |
| [material_set_load_hook](../campaign-039-material-set-load-hook/result.json) | 28 | 90 |
| [material_set_unload_hook](../campaign-039-material-set-unload-hook/result.json) | 28 | 90 |

The cleanup fixture explicitly distinguishes captured array traversal from live loop count and final base-pointer reload. Literal mutation profiles shrink or expand the count, retarget the final base, replace current/next payloads, and clear the next payload. Full guarded snapshots check prior clears and final callback replacement overwritten byNULL. Load/free fixtures verify hook dispatch, allocation/load failure, free(NULL), nonzero success, cleanup order and callback hook mutation. Material hook tests preserve distinct globals and never invoke installed callbacks.

These are focused reconstructions; canonical shared acceptance and whole-executable identity remain separate. Original executable and resource data were not executed or published.
