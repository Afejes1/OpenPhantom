# Scan-line free wrapper reconstruction

This candidate reconstructs the complete 23-byte conditional release wrapper at
`0x004084A1`. A null record produces no call; a nonnull record is passed
unchanged to the tracked deallocator wrapper.

The authored fixture checks null call suppression, two distinct record
addresses, exact pointer identity, cumulative call counts and byte-for-byte
record preservation through a no-write release stub. Native verification
compiles only; fixture execution is limited to the locked Docker runner. The
original allocator, deallocator and executable are never executed.

## Comparison and fixture status

The first readable VC5 /Od /MT candidate matches the complete 23-byte span after resolving the single four-byte tracked-deallocator call. All 19 non-relocation bytes agree, with no padding, exclusions or warning waivers.

Two locked-Docker fixture attempts stopped before fixture compilation because the compiler produced a zero-byte candidate.cod and the unchanged runner correctly rejected the empty assembly listing. The native listing is 1,252 bytes and its candidate object matches exactly. At worker handoff the fixture had not executed; the source was preserved unchanged. The coordinator subsequently verified the fixture using the guarded fallback described below. The original failure reports and hashes remain recorded in worker-log.json.

## Final independent verification

The coordinator rebuilt the final inputs natively and in locked Docker. The
[public receipt](result.json) records complete-span equality and 9 passing
focused assertions. Accepted history remains unchanged pending batch acceptance.
The initial Docker listing failures remain in the worker log. The explicit
`--native-listing-fallback` now retains the empty Docker listing and copies a
complete native listing only after identical-source/toolchain provenance and
full function/fixup equality checks. No source or byte exception was introduced.

```powershell
python engine/research/campaign-001/verify.py --case engine/research/campaign-003-line-free --runner native --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE
python engine/research/campaign-001/verify.py --case engine/research/campaign-003-line-free --runner docker --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE --behavior --expect-report <native-report-path> --native-listing-fallback
```
