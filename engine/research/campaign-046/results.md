# Campaign046 results

Two previously untouched team-not_done colormap serializers match852 complete bytes with25 verified address operands. Independent native/Docker builds agree on function bytes and relocations; authored reader and writer fixtures pass16,308 checks with zero failures. Neither function has byte exclusions or warning waivers. Shared acceptance remains separate.

| Function | Complete bytes | Operands | Fixture checks |
| --- | ---: | ---: | ---: |
| [colormap_load_entry](../campaign-046-colormap-load-entry/result.json) | 581 | 16 | 11412 |
| [colormap_write](../campaign-046-colormap-write/result.json) | 271 | 9 | 4896 |

Both functions matched on their fifth implementation compile. Earlier variants already had complete spans but15 local-stack differences. The retained source uses ordinary handle/file_header locals and the typed64-byte header; no artificial padding or unused stack variables. The interim16-word representation was not retained. All ten attempts and four final verification invocations are recorded.

Reader fixtures cover open failure, invalid marker, either allocation failure, aligned0/1/255 allocation residues, optional alpha after live flag changes, bounded basename copying and close/fallback order. Writer fixtures distinguish the header captured before open from later live palette/table/flag selections. Alternating service tables verify reload after every operation. All observed callback returns except open are ignored. Short read reports are tested with fully authored initialized destinations; actual truncated files are not covered.

The reviewer corrected an initial timing recommendation: reader open occurs before basename/name copying, while writer header capture occurs before open. Pointer-provenance guards were added before final verification to link each header local across callbacks. The original executable was never executed and no original strings or data are published. Whole-executable identity remains pending.
