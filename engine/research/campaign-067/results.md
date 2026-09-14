# Campaign 067 results

One additional exact function is ready for the next shared acceptance batch.
The accepted baseline remains 273 functions at event 70.

| Function | Complete target / candidate bytes | Result | Focused checks |
| --- | ---: | --- | ---: |
| [module_start_all](../campaign-067-module-start-all/result.json) | 193 / 193 | Exact after two verified address operands; acceptance pending | 343,200 |
| [module_send_shutdown](../campaign-067-module-send-shutdown/result.json) | 198 / 193 | Research: complete extent differs | 343,200 |

Each had one original-VC5 implementation compile. Final native/Docker verification
pairs agree on authored code and relocations, and both isolated fixtures pass.
The tests link the real accepted finder and check guarded lists, signed and
duplicate IDs, eligibility, callback returns, live-link mutations and status
updates after callbacks. The start helper sets bit 1 after event 3 succeeds.
Shutdown clears bit 0 after event 2 succeeds.

Shutdown's original broadcast branch stores a callback-result copy at stack -12.
The natural source does not reproduce that extra store or its associated layout.
The five-byte length difference is not a count of mismatched bytes or a similarity
percentage. No dead local, artificial stack frame, padding, excluded bytes or
shortened target was introduced. Establish the source/compiler origin of that
store before another attempt; retain this attempt and its original cap.

Both addresses are marked not_done in the latest contributor snapshot. Only
start is a focused exact match; neither has been added to the accepted registry.
This preserves one shared full regression checkpoint for the one-hour window.
