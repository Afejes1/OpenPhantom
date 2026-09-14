# Campaign 063 results

Nine untouched window/kernel functions match 520 complete bytes and 34 operands on their first actual compiler invocation. All570,560 checks pass, with18 selected reports and126 snapshot inputs independently rehashed.

| Function | Full bytes | Checks |
| --- | ---: | ---: |
| [kernel_add_dialog](../campaign-063-kernel-add-dialog/result.json) | 53 | 112640 |
| [kernel_add_hook](../campaign-063-kernel-add-hook/result.json) | 96 | 90112 |
| [kernel_get_window](../campaign-063-kernel-get-window/result.json) | 10 | 10752 |
| [kernel_hide](../campaign-063-kernel-hide/result.json) | 19 | 23552 |
| [kernel_remove_dialog](../campaign-063-kernel-remove-dialog/result.json) | 150 | 203456 |
| [kernel_remove_hook](../campaign-063-kernel-remove-hook/result.json) | 122 | 95744 |
| [kernel_set_callback](../campaign-063-kernel-set-callback/result.json) | 33 | 5632 |
| [kernel_set_field](../campaign-063-kernel-set-field/result.json) | 13 | 5120 |
| [kernel_show](../campaign-063-kernel-show/result.json) | 24 | 23552 |

The ten runner attempts include one preflight-only inventory rejection before any candidate compiler invocation: add-dialog had a transcribed operand offset28 instead of29. The original instruction immediate at498E76 relative498E59 proves29; correcting target metadata required no source change. All9 candidate compiles and18 final native/Docker verification calls succeeded.

Hook insertion retains the original free-slot precondition; the invalid full-table case would write beyond16 entries and is not executed. Removal scans identities regardless of active flags after its signed count gate. Dialog insertion uses unsigned capacity checks; removal preserves stable order and removes the first duplicate. Tests preserve full tables and scalars without assuming global guard adjacency. Show/hide use authored stdcall observers with captured arguments and persistent mutation. Kernel window8620F0 is distinct from prior window-state handle8620A4. No exclusions, new warning waivers, OS or original-game execution. Whole-executable identity remains pending.
