# Acceptance003 results

The three world readers passed the clean registered VC5 build, raising accepted
functions from 23 to 26. Immutable event 000023 records the source revision,
toolchain, inputs, artifacts and complete comparisons. All previous 23 target
specifications were unchanged and all 26 passed again.

| Newly accepted function | Full bytes | Verified call operands | Differences |
| --- | ---: | ---: | ---: |
| world_read_emitters | 94 | 1 | 0 |
| world_read_sprite_refs | 108 | 2 | 0 |
| world_read_palettes | 90 | 1 | 0 |

The canonical partial header/world views have compile-time offsets and widths.
Signed stream size/count were recovered from the actual callee's stack accesses
and signed division. Unsigned payload arithmetic remains modulo 2^32; explicit
int conversion under VC5 preserves the argument word. Each reader's high-bit
fixture observes INT_MIN with the original 0x80000000 bits, and each compiled
function still matches its complete original span after address binding.

The combined fixture passed 189 checks with zero failures, including sequential
calls on one world, nested palette reading during sprite allocation, allocator
and reader mutations, mismatch retention and guarded storage. Existing BGL45
and colormap321 checks also passed. The 100 tooling tests and full CI passed.
No compile/test failure, new warning waiver, byte exclusion or matcher change.

Source: `5e54b23df927bcc2746ec985b64a96ab9494417d`.
Full CI: https://github.com/Afejes1/OpenPhantom/actions/runs/34758436828
Receipt details and hashes: [result.json](result.json).

These are complete function matches after verified address binding, not proof of
whole-executable identity or final linked placement. Synthetic callbacks perform
no real stream I/O; the original executable was never run. Frozen Campaign017
research bodies and evidence remain preserved separately.
