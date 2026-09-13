# Cached nearby collision surfaces

The authored C implementation covers the complete 555-byte function at
`0x0040D03E`. Native and locked-Docker output have the same bytes and relocation
inventory; all **81 focused assertions pass**. It remains a **research candidate**:
the strict comparison rejects different operand locations, with no percentage
assigned. Of 25 operands, the first thirteen and final operand align, ten are one
byte early, and one is two bytes early. All ordered symbol, kind, addend and
constant identities were checked independently against the verified original
inventory. This diagnostic does not relax exact location requirements.

The candidate preserves radius thresholds, returning assertion behavior, the
unused incoming world argument, active-world reloads, first-cell-only cache,
exact 255-byte visited clear, cumulative append offsets, cached records and
post-append capacity assertion. The ordinary memset uses VC5's intrinsic pragma;
there is no injected instruction data, byte exclusion or warning waiver.

Independent caller/callee review corrected the square enumerator to
`(cells, position, radius, capacity)`. The worker's tenth invocation caught a
fixture stub still using the old order; its corrected handoff was independently
compiled and tested by the coordinator. The strengthened fixture checks callback
argument bits, all cache misses, maximum 25 cells, all 255 cleared bytes plus a
sentinel, callback-time assertion state, active-world changes and generation
mutation. Valid authored fixtures do not establish game-wide collision behavior.

The ten attempts remain in [worker-log.json](worker-log.json), including the
incorrect intermediate claim that all twelve later operands were one byte early.
The parent correction and [final receipt](result.json) give measured positions.
No cap was reset and no original game code was executed.

Reproduce with the pinned compiler and owned reference; mismatch exit code 1 is
expected even when the fixture passes:

```powershell
python engine/research/campaign-001/verify.py --case engine/research/campaign-004-nearby-surfaces --runner native --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE
python engine/research/campaign-001/verify.py --case engine/research/campaign-004-nearby-surfaces --runner docker --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE --behavior --expect-report <native-report-path>
```
