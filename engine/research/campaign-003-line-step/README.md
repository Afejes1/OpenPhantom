# Scan-line advance reconstruction

This candidate advances the reviewed 80-byte scan-line record using the
diagonal fields for nonnegative error and the straight fields for negative
error. It increments the index after every update and returns one only when the
new index is greater than the count.

The authored fixture checks negative, zero and positive errors, equality and
count-plus-one returns, custom signed steps, an already-over-count record,
untouched fields and a repeated trajectory. Inputs avoid signed overflow and
null pointers. Native verification compiles only; fixture execution is limited
to the locked Docker runner. The original executable is static evidence only.

## Exact comparison result

The first readable VC5 /Od /MT candidate is a raw-code match for all 209 bytes. The function has no calls, relocations, padding, exclusions or warning waivers. The coordinator-expanded locked-Docker fixture passes 27 focused checks with zero failures, and native/Docker function code agrees.

## Final independent verification

The coordinator rebuilt the final inputs natively and in locked Docker. The
[public receipt](result.json) records complete-span equality and 27 passing
focused assertions. Accepted history remains unchanged pending batch acceptance.
The fixture now also checks an updated index strictly below the count.

```powershell
python engine/research/campaign-001/verify.py --case engine/research/campaign-003-line-step --runner native --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE
python engine/research/campaign-001/verify.py --case engine/research/campaign-003-line-step --runner docker --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE --behavior --expect-report <native-report-path>
```
