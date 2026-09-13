# Frame setup binding inventory evidence

This is a static comparison-preparation artifact for `WMAIN` at `0x00404180`.
No original code was executed, copied into this repository, or used to modify
Ghidra metadata.

The authoritative Ghydra instance was listed and confirmed as port 8192,
`TPM:/WMAIN.EXE`, x86 little-endian 32-bit, image base `0x00400000`. Ghydra
returned the complete 1,456-byte memory span, a 164-line decompilation and the
disassembly through the `RET` at body byte 1,448. The seven following alignment
bytes remain part of the target extent. The local file identity independently
agreed with SHA-256
`02babee8d64bc0bf47451aaae9af630f4a028cf63f9996bda6df497caba34f11`.

`frame-bindings.json` contains 153 bindings: all 144 PE DIR32 operands in the
span and all nine direct near-call operands. The DIR32 set comprises 128 named
global operands and 16 float32 constant operands. `verify_reference` accepted
the retail identity, exact relocation set, every original operand value, every
constant payload, every call opcode and every resolved call destination.

## Symbol mapping

The names follow the existing authored declarations in `frame.h`. Address and
addend assignments come from the original disassembly's access patterns and
the candidate's corresponding field use:

| Original base | Candidate symbol | Evidence |
| --- | --- | --- |
| `0x006F83E4` | `_op_frame_transform` | Loaded before the 12-value basis copy and viewport reads. |
| `0x008BE9EC` | `_op_scan_transform` | Destination of the 12-value basis copy. |
| `0x008BE9D0` | `_op_scan_bounds` | Four consecutive float stores use addends 0, 4, 8 and 12. |
| 0x008BE940, 0x004AA080, 0x0059DEF8 | _op_scan_octant, _op_scan_octant_table, _op_scan_axis_flags | Division by 45 selects an eight-int row. The original table operand is base addend 16 (0x004AA090) because the following first-element read uses [EDX-0x10]; the candidate indexes element zero from the declared array base. The loop writes the four flag groups, whose original operands use addends 16 and 32. |
| `0x008BE9C4`, `0x008BE9C8` | `_op_scan_direction_x`, `_op_scan_direction_y` | Each is multiplied by the verified 512.0 constant before `__ftol`. |
| `0x008BE978`, `0x008BE97C` | `_op_scan_direction_step_x`, `_op_scan_direction_step_y` | Stores of those two conversions and later scan-line call arguments. |
| `0x008BE98C` through `0x008BE998` | `_op_scan_x`, `_op_scan_y`, `_op_scan_anchor_x`, `_op_scan_anchor_y` | Paired anchor copies and later offset additions. |
| `0x008A011C` | `_op_frame_camera_state` | Pitch and eye-height field reads match the partial camera layout. |
| `0x008BE9A0`, `0x008BE9A8`, `0x008BE9AC`, `0x0059DF40` | `_op_scan_limit`, `_op_scan_bias`, `_op_scan_mode`, `_op_scan_mode_enabled` | Initialization, pitch-dependent updates and final limit clamps agree with candidate control flow. |
| `0x008BFA40` | `_op_frame_scan_line` | All operands from base through addend 72 match the 19-int scan-line layout and both initializer/advance sequences. |
| `0x008BEA1C` | `_op_scan_position` | Passed to the single scan-step call. |
| `0x008BE9C0` | `_op_scan_quadrant` | Written from the two octant ranges. |
| `0x008BEA3C` | `_op_scan_offsets` | Original indexed operands `0x008BF234` and `0x008BF238` are base addends 2,040 and 2,044, the two fields of row 255. |
| `0x008BE970` | `_op_scan_offset_index` | Initialized to one, loop-tested against 256 and incremented. |
| `0x008BE9E0`, `0x008BE9E4`, `0x008BE9E8` | `_op_scan_turn_x`, `_op_scan_turn_y`, `_op_scan_turn_z` | Consecutive zero stores in the final reset block. |
| `0x008BE948`, `0x008BE99C`, `0x008BE9B0`, `0x008BE9B8` | `_op_scan_pending`, `_op_scan_cursor`, `_op_scan_low_mark`, `_op_scan_high_mark` | Final reset block stores zero, 256, -1 and -1 respectively. |

The constant bindings cover the observed float32 values 0.0, 512.0, 0.5,
-360.0, 180.0, 360.0, -1.0, 90.0 and -0.8999999761581421 at their exact
original addresses. Duplicate operands remain separate bindings.

The nine reviewed `E8` operands bind four calls to `__ftol`, two calls to
`_op_sin_cos_degrees` at original target `0x00494238`, two calls to
`_op_init_scan_line` at `0x004084B8`, and one call to `_op_step_scan` at
`0x00403EF0`. Ghydra identifies the first target as the Visual Studio library
function and the last as the existing camera-space grid step function; the two
remaining mappings follow their argument sequences and the authored candidate.

## Validation boundary

The pinned native object has a 1,376-byte function section and 135 COFF
relocations. Strict comparison returns `complete function extent differs`
against the 1,456-byte original target, as expected. It stops before candidate
relocation comparison, so this inventory establishes original-side completeness
but does not claim that the current object has a matching relocation layout or
matching bytes. No mapping remains unresolved in the original span; independent
coordinator review is still required before enabling the target.

Coordinator review corrected one semantic base/addend pair: binding offset 110 initially named the biased operand value 0x004AA090 as the array base. The reviewed binding now names base 0x004AA080 with addend 16. Its resolved original operand remains unchanged, and all other independently reviewed symbol bases agreed.
