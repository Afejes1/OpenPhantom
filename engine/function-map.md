# Function metadata recovered during matching

Names below describe observed behavior and are not claimed to be original symbols.
The original executable is never modified. Only Ghidra analysis metadata changes.

Address | Original Name | New Name | Confidence | Purpose | Evidence
--- | --- | --- | --- | --- | ---
0x0040F170 | FUN_0040f170 | combine_face_vertex_clip_codes | HIGH | Return union of indexed vertex clip codes plus intersection in bits 8-15 | Complete disassembly; unsigned count at face+0x14 and index pointer at +0x18; mesh caller at 0x0040F5EF tests 0xFF80; independent VC5 /O2 /MT candidate matches all 64 bytes including six padding bytes; synthetic zero/common/disjoint/behind-camera tests pass

See [view/projection evidence](docs/view-projection-evidence.md) for scope and
remaining questions. Other existing Ghidra names are preserved.
