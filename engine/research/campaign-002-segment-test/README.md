# Integer segment sidedness predicate

This readable C candidate reconstructs the 237-byte integer predicate at
`0x00408648`. It tests whether each segment's endpoints lie on opposing sides
of the other segment's supporting line and returns integer zero or one.

The authentic predicate has no bounding-box overlap test. Disjoint collinear
segments and two distinct zero-length segments therefore return one. The
fixture preserves those cases alongside crossings, parallel separation,
first- and second-pair rejection, endpoint contact, collinear overlap,
zero-length points, endpoint reversal and segment swapping. A bounded
small-coordinate pass compares against an independently grouped determinant
oracle.

Fixture coordinates avoid signed overflow. Machine-width overflow and
`INT_MIN` behavior are not tested, and the matching VC5 output is not a claim
of portable overflow semantics. Native verification compiles only; authored
fixture execution is restricted to the locked Docker runner. The original
executable is never executed.

## Exact comparison result

The final VC5 /Od /MT candidate is a raw-code match for all 237 bytes. The function has no relocations, calls, padding, exclusions or warning waivers. Five bounded source variants were compiled; the exact variant uses the conventional mathematical scalar names a1, b1, c1, a2, b2, c2 and r1 through r4. With VC5 debug code generation, those identifiers reproduce the observed meaningful-local stack placement without artificial arrays or padding variables.

The final locked-Docker fixture passes 6,601 checks with zero failures. Forty checks cover ten named behaviors and their reversal/swap forms; 6,561 repeated assertions form the bounded small-coordinate determinant pass. Native and Docker builds both reproduce the raw 237-byte target.

The coordinator independently rebuilt and checked the final source in both
runners. The [public receipt](result.json) records the final source hashes,
complete-span raw equality and 6,601 passing assertions. The accepted history
remains unchanged pending its existing batch checkpoint.

Reproduce from the repository root with privately provisioned prerequisites:

```powershell
python engine/research/campaign-001/verify.py --case engine/research/campaign-002-segment-test --runner native --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE
python engine/research/campaign-001/verify.py --case engine/research/campaign-002-segment-test --runner docker --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE --behavior --expect-report <native-report-path>
```
