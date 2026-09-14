# Campaign058 results

Six untouched window-state helpers match all99 original bytes and nine data operands on their first compile. All19,440 authored checks pass with native/Docker function and relocation parity. No byte exclusions, original-VC5 warning waivers or original execution.

| Function | Full bytes | Operands | Checks |
| --- | ---: | ---: | ---: |
| [window_get_handle](../campaign-058-window-get-handle/result.json) | 10 | 1 | 2,160 |
| [window_get_instance](../campaign-058-window-get-instance/result.json) | 10 | 1 | 2,160 |
| [window_get_rect](../campaign-058-window-get-rect/result.json) | 10 | 1 | 3,024 |
| [window_set_handle](../campaign-058-window-set-handle/result.json) | 13 | 1 | 5,184 |
| [window_set_instance](../campaign-058-window-set-instance/result.json) | 13 | 1 | 5,184 |
| [window_set_rect](../campaign-058-window-set-rect/result.json) | 43 | 4 | 1,728 |

Independent review verified84 source inputs and12 final reports. Caller evidence identifies the window and instance globals; explicit32-bit words preserve all bits without host-width pointer assumptions. The rectangle is a neutral16-byte record because no direct callers establish coordinate signedness. Fixtures cover216 initial states, all documented global preservation, guarded input, exact self-alias and live getter mutation; intermediate store order is established statically, not claimed from snapshots.

Temporary approval-service capacity failures delayed preparation, but the same direct routes recovered and the scaffold was ready within its original preparation limit. These six matches will join the three C057 parser/sort matches for acceptance041. Whole-executable identity remains pending.
