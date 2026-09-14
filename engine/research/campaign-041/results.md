# Campaign041 results

Three new exact functions cover132 complete bytes with10 verified external operands. All matched on their first implementation compile. Final native/Docker code and relocations agree and authored fixtures pass (1,338 checks). No exclusions, warning waivers or near-match promotions were used.

| Function | Full bytes | Checks |
| --- | ---: | ---: |
| [colormap_free](../campaign-041-colormap-free/result.json) | 29 | 28 |
| [colormap_set_current](../campaign-041-colormap-set-current/result.json) | 50 | 840 |
| [colormap_set_hardware](../campaign-041-colormap-set-hardware/result.json) | 53 | 470 |

Current/hardware setters preserve observed store-before-callback timing and retained callback mutations. Signed device counts distinguish positive installation from nonpositive storage-only behavior; equal NULL remains a no-op, while changed NULL plus positive count is excluded from authored execution. The free wrapper uses the existing canonical symbol for later connection to extended-colormap cleanup and always releases the original captured root after contents cleanup. All root storage is authored and guarded.

Preparation began before the user extended the run; its original campaign cutoff was preserved. These three new matches are counted in the ten-hour extension because implementation/verification occurred after that extension began. Whole-executable identity remains pending.
