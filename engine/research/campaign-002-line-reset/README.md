# Scan-line reset reconstruction

This candidate reconstructs the complete 400-byte function at `0x004084B8` in
readable C. It initializes an existing scan-line record or requests one 80-byte
record through the tracked allocator, preserves the unused word at offset
`0x4C`, derives signed diagonal and straight steps, and initializes the error
terms and current position.

The focused authored fixture covers an existing record, allocation success and
failure, all direction quadrants, X-major and Y-major lines, ties, a zero-length
line, every written field and preservation of the unused word. Test coordinate
differences stay within signed `int`; overflow and `INT_MIN` absolute-value
behavior are outside this bounded fixture.

Native runs compile only. Fixture execution is restricted to the locked Docker
runner. The original executable is read statically for strict comparison and is
never executed.

## Exact comparison result

The final VC5 `/Od /MT` candidate reproduces the complete 400-byte span after resolving the single reviewed allocator call relocation. All 396 non-relocation bytes agree, with no exclusions, inserted bytes, warning waivers or artificial frame construction.

`#pragma intrinsic(abs)` is required because an ordinary unoptimized library call produced a 406-byte first candidate. The intrinsic keeps the signed-magnitude operations in readable C and makes VC5 emit the observed branchless integer magnitude sequence. It does not insert opcodes or relax comparison.

The final locked-Docker fixture passes 238 checks with zero failures. It includes pure horizontal and vertical lines in both directions and verifies all 80 allocation-stub bytes remain untouched on failure. Native and Docker builds independently retain the exact 400-byte function and one allocator relocation. Integrated game behavior and signed-overflow or `INT_MIN` inputs remain outside this focused verification.

The coordinator independently rebuilt the final inputs natively and in Docker.
The [public receipt](result.json) pins those reports, source hashes, toolchain,
complete-span comparison and focused result. This remains research pending the
existing batch acceptance checkpoint; the historical accepted count is unchanged.

Reproduce from the repository root with the privately provisioned original
toolchain and reference executable:

```powershell
python engine/research/campaign-001/verify.py --case engine/research/campaign-002-line-reset --runner native --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE
python engine/research/campaign-001/verify.py --case engine/research/campaign-002-line-reset --runner docker --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE --behavior --expect-report <native-report-path>
```
