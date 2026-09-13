# Surface-at-or-below query

This C implementation reconstructs the team-not-done function at 0x0040C9F8.
Static comparison with the adjacent above-query resolves the manifest's alias:
the selected function skips surfaces whose vertices are all strictly above the
query, then requires XY containment. It does not interpolate plane height.

**Research candidate: 229 bytes versus the complete 227-byte target.** No byte
mismatch percentage is assigned when extents differ. The explicit, defined
zero return adds two bytes compared with the original exit that reuses the
iterator's zero result. All seven candidate operand offsets, external symbols,
kinds and zero addends were independently checked against the original; this
separate inventory check does not pass the full-span comparison.

The [public receipt](result.json) records fresh native/Docker agreement and
**53 focused assertions passing**. Coverage includes empty iteration, masks,
all-above rejection, equality, mixed heights, zero vertices, subsequent XY
matches, nonzero return normalization, NaN vertex/query behavior, world reload,
geometry argument preservation and observed field offsets. Callbacks are authored
stubs; no original game code or collision helper is executed.

The worker used eight compiler invocations, including readable-fixture rebuilds.
The coordinator added a source annotation and strengthened the final fixture,
then independently recompiled it with both runners. No semantic candidate rewrite,
target change, byte exclusion or warning waiver was made. See [attempts](worker-log.json).

Reproduce with the pinned compiler and owned reference. Both commands return 1
because complete-span comparison fails even when behavioral checks pass:

```powershell
python engine/research/campaign-001/verify.py --case engine/research/campaign-004-surface-below --runner native --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE
python engine/research/campaign-001/verify.py --case engine/research/campaign-004-surface-below --runner docker --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE --behavior --expect-report <native-report-path>
```

Final native report: engine/build/campaign-001-has_surface_at_or_below-d3fab8c852be/research-result.json.
Final Docker report: engine/build/campaign-001-has_surface_at_or_below-2c6b54271354/research-result.json.
