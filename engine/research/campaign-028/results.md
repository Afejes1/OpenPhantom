# Campaign028 results

Seven untouched functions match658 complete bytes. All41,267 focused checks pass across eight candidates, with native/Docker code and fixup inventories equal. Ripple reset remains136/138-byte research because of its two-byte alignment tail.

| Function | Original / candidate bytes | Checks | Evidence |
|---|---:|---:|---|
| effects_load | 152 / 152 | 5436 | exact-function-match |
| effects_object_created | 17 / 17 | 21 | exact-function-match |
| effects_object_visibility | 40 / 40 | 578 | exact-function-match |
| halo_draw_actor | 98 / 98 | 651 | exact-function-match |
| ripple_material | 163 / 163 | 6664 | exact-function-match |
| ripple_reset | 138 / 136 | 8989 | research-candidate |
| zap_draw_default | 83 / 83 | 112 | exact-function-match |
| zap_draw_segment | 105 / 105 | 18816 | exact-function-match |

Independent review covers all49 bindings, ABIs, callback order, exact word/float forwarding and full owned-state snapshots. Ripple material must return its captured result even when release callbacks change the cache. Effects loading publishes six fog words only after the96-byte read succeeds and preserves later callback changes. Halo drawing checks flags only on entry.

Sixteen implementation attempts include one rejected preflight: the halo draw call operand is85, not86. Fifteen actual candidate compiles ran, with no compiler failures. Descriptive local names reproduce the observed VC5 stack slots: color/opacity/material for ripple material, and material/num_points for both zap wrappers. No behavior or flags changed during that tuning.

Zap negative depths are outside the source contract because negative C shifts are undefined. Ripple material indices are limited to0..7. NULL external backend results are authored fixture-domain cases. Default zap-wrapper caller provenance remains unresolved. Original resource text, game execution, byte exclusions, forced padding and warning waivers are absent.

Registered acceptance remains69 functions/32 events; the next combined checkpoint will integrate these seven and the six exact Campaign027 functions. Final executable layout is not yet proved.
