# Campaign060 results

Seven untouched math wrappers match223 complete bytes and16 operands, all on their first compile. Their1,339 focused checks pass. Sine remains a25-byte research candidate against27 original bytes after five natural source experiments; its66 checks pass, but differing extents have no valid mismatch percentage. All16 final reports and112 source snapshot inputs were independently rehashed.

| Function | Target / candidate bytes | Status | Checks |
| --- | ---: | --- | ---: |
| [acos_degrees_copy2](../campaign-060-acos-degrees-copy2/result.json) | 25 / 25 | exact-function-match | 39 |
| [acos_degrees_copy3](../campaign-060-acos-degrees-copy3/result.json) | 25 / 25 | exact-function-match | 39 |
| [asine_degrees](../campaign-060-asine-degrees/result.json) | 25 / 25 | exact-function-match | 38 |
| [cosine_degrees](../campaign-060-cosine-degrees/result.json) | 25 / 25 | exact-function-match | 68 |
| [scalar_angle](../campaign-060-scalar-angle/result.json) | 25 / 25 | exact-function-match | 39 |
| [sincos_degrees](../campaign-060-sincos-degrees/result.json) | 58 / 58 | exact-function-match | 506 |
| [sine_degrees](../campaign-060-sine-degrees/result.json) | 27 / 25 | research-candidate | 66 |
| [tangent_degrees](../campaign-060-tangent-degrees/result.json) | 40 / 40 | exact-function-match | 610 |

The original degree scale is57.296326654336454, slightly different from mathematical180/pi; the exact observed constant is preserved. Three arccosine wrappers are distinct original addresses with identical semantics. Sincos captures one binary32 radian value and honors both NULL outputs and exact alias (cosine last). Tangent preserves the live x87 path after its wrap callback and retains callback state mutations. Finite, in-domain, away-from-pole numeric tables allow4 ULP for stored nonzero floats and exact signed zero; byte comparison still requires zero differences. No internal extended-precision equivalence is claimed by numeric fixtures.

Automatic approval review initially blocked the sine freeze for model capacity; the same direct route later recovered. It separately rejected the reviewer's optional Python table recomputation twice under the older arbitrary-script restriction, despite a retry quoting the user's prior authorization. That extra computation remains uncompleted and no workaround was used. Independent manual source/oracle review and all source/report hash checks completed. No exclusions, original-VC5 warning waivers or original-game execution. Whole-executable identity remains pending.
