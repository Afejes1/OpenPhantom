# Campaign040 results

Three new exact functions cover239 complete bytes with7 verified external operands. All four paired native/Docker function and relocation checks agree, and all authored fixtures pass (4,821 checks). Each candidate required one implementation compile. No byte exclusions, warning waivers or near-match promotions were used.

The color packer remains research: its natural170-byte body matches raw in a [separate diagnostic](../campaign-040-colour-pack/body-diagnostic.json), but the unchanged full target is184 bytes and its14-byte tail remains unresolved. That diagnostic is not accepted as another exact function, and no mismatch percentage is inferred from the extent difference.

| Function | Full bytes | Status | Checks |
| --- | ---: | --- | ---: |
| [colour_pack](../campaign-040-colour-pack/result.json) | 184 | research-candidate | 608 |
| [material_destroy](../campaign-040-material-destroy/result.json) | 53 | exact-function-match | 96 |
| [material_install_palette](../campaign-040-material-install-palette/result.json) | 42 | exact-function-match | 20 |
| [material_invalidate_cache](../campaign-040-material-invalidate-cache/result.json) | 144 | exact-function-match | 4097 |

The material destructor checks override/default callback timing; palette installation publishes the original embedded+48 address after callback mutations; cache invalidation clears only observed words in bounded guarded rows. Color cases use literal unsigned packed-word oracles with portable shift counts0..31 and immutable guarded formats. Original executable and resource data were not executed or published.
