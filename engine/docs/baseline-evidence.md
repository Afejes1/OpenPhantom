# Four-function baseline evidence

Research checkpoint: 2026-09-12. Reference MD5
`7c5af8428c19b17cca09ae3a49bd10ef`, SHA-256
`02babee8d64bc0bf47451aaae9af630f4a028cf63f9996bda6df497caba34f11`.
The local file is 829,952 bytes, PE32 x86, linker header 5.0, image base
0x00400000, entry RVA 0x0009CB80, timestamp 0x371D4700. It was inspected
statically, not executed. The active Ghidra program was TPM:/WMAIN.EXE.

These candidates were written from behavior and layout observations. They are
not pasted decompiler output, recovered original source text, or verified VC5
matches. Existing modern TPM source served as research context; it is not a
dependency of this component. The address metadata below is independently
checked against the contributor's hash-pinned reference by `verify.py reference`.

| Address | Analysis name | Candidate symbol | Behavior confidence | Extent | Status |
|---|---|---|---|---|---|
| 0x0040E840 | FUN_0040e840 | op_material_mode | HIGH for return mapping; material interpretation tentative | [0x0040E840, 0x0040E869), 41 bytes | VC5 unverified |
| 0x00406E22 | FUN_00406e22 | op_grid_cell | HIGH for address arithmetic | [0x00406E22, 0x00406E3A), 24 bytes | VC5 unverified |
| 0x0040DCEE | solve_b3d_plane_coordinate | op_plane_coordinate | HIGH for axes 0/1/2 and observed stores | [0x0040DCEE, 0x0040DE6E), 384 bytes | VC5 unverified |
| 0x0040EF40 | FUN_0040ef40 | op_update_projection | HIGH for field/global transfers; original type names unknown | [0x0040EF40, 0x0040EFC1), 129 bytes | VC5 unverified |

Confidence in observed behavior is distinct from confidence in a candidate's
machine-code match. No Ghidra names, types, comments, or executable bytes were
changed for this work.

## Observations and uncertainties

- The switch utility returns zero only for input 1, otherwise two. The stack
  local and switch-shaped branch are retained to seek the documented /Od output.
- The grid lookup uses base 0x0059DFC0 with byte strides 128 and 32768. No bounds
  checks appear in this routine. Caller domains and the allocation extent are
  not established by this function alone; the test fixture exercises only valid
  indices inside its synthetic allocation. It does not establish real grid size.
- The plane solver returns the vertex coordinate without a point write when the
  selected normal component compares equal to zero or positive one. The x87
  status checks at 0x0040DD03 and 0x0040DD27 inspect C3 without C2: masked NaNs
  take that same early return. Arithmetic is stored to the point after every
  step. Five incoming call sites were inspected through Ghydra XREFs; they include
  height, overhead-clearance, and ledge-related queries. Invalid axes are outside
  this batch's caller contract. Modern ordered equality cannot establish the
  unordered behavior of this VC5 source candidate.
- Projection setup reads a camera pointer at 0x006F83E4. Its viewport pointer is
  at +4, focal field at +0x3C, and frustum pointer at +0x48. The viewport supplies
  raw 32-bit fields at +8/+0xC and signed integer edges at +0x18/+0x1C/+0x20/+0x24.
  The frustum supplies the near and far fields at +4/+8. Full disassembly shows
  twelve absolute address operands, totalling 48 bytes. The remaining 81 bytes
  are code bytes compared without adjustment. No claim is made about the opaque
  fields in the partial structures.

The plane solver's two constant-address operands account for 8 bytes: 384 full
bytes, 376 compared without adjustment. The grid has 24 full bytes and one
4-byte operand. The full first-batch denominator is 578 code bytes; it is not a
percentage of the game. This clarifies the mixed counting conventions in the
earlier engine-identification table without changing its historical findings.

## Matching constraints

The first three candidates start with /Od /MT, projection with /O2 /MT, based on
the upstream compilation account in engine-identification.md section 1a. The
build additionally requests /Zi for diagnostic symbols, /Gy- so each one-function
object has a complete code section, and /W4 /WX. These options are recorded and
remain subject to actual VC5 calibration; none has been proved by this fork yet.

The strict comparator accepts only ordinary x86 COFF, one entire function per
code section, and DIR32 relocations whose positions agree with the original PE
HIGHLOW relocation table. It checks external symbol names and addends, and reads
local constant definitions to verify their values. Unexpected padding, extra
functions, alternate relocation forms, or unknown symbols remain mismatches or
unresolved results. Add support only with an evidence-backed test, never by
silently masking another byte range.

Object code equality does not prove linked placement. `raw-code-match` is thus
deliberately not `literal_function_match`. The diagnostic console fixture includes
test code and synthetic globals and cannot be the retail link. The full-file
comparator excludes nothing, including timestamp, headers, padding and data.
It currently exposes the whole-image gap; it does not solve original link order,
resource construction, CRT selection, or historical link timestamps.

After calibration, use each complete original module as the unit of expansion.
Preserve existing matches before changing the target manifest; record any
manifest migration explicitly. Matching a module establishes a useful behavioral
reference for separate modern-engine comparisons, not blanket correctness of
the modern reconstruction.
