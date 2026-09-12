# Reconstruction progress

Registered reconstruction work only; not a whole-program census.

Generated from registry.json, target.json and chained verification records. No game bytes are stored here.

Whole executable: **incomplete**. Original linked placement: **unverified**.

| Module | Function / source | Original address | VC5 frontend | Last verified result | Bytes / address bytes |
|---|---|---|---|---|---|
| material | [material_mode](../src/material_mode.c) | 0x0040E840 | c | raw-code-match | 41 / 0 |
| spatial_grid | [grid_cell](../src/grid_cell.c) | 0x00406E22 | c | relocation-adjusted-match | 24 / 4 |
| map_geometry | [plane_coordinate](../src/plane_coordinate.c) | 0x0040DCEE | c | relocation-adjusted-match | 384 / 8 |
| view_projection | [update_projection](../src/update_projection.c) | 0x0040EF40 | c | relocation-adjusted-match | 144 / 48 |
| view_projection | [face_clip_flags](../src/face_clip_flags.c) | 0x0040F170 | c | raw-code-match | 64 / 0 |
| view_projection | [transform_project](../src/transform_project.c) | 0x0040EFD0 | c | relocation-adjusted-match | 416 / 44 |
| mesh_submission | [set_mesh_render_thing](../src/set_mesh_render_thing.c) | 0x0040F1B0 | c | relocation-adjusted-match | 48 / 16 |
| render_queue | [peek_projected_vertices](../src/peek_projected_vertices.c) | 0x00402130 | c | relocation-adjusted-match | 37 / 12 |
| render_queue | [peek_face_packet](../src/peek_face_packet.c) | 0x00402021 | c | relocation-adjusted-match | 37 / 12 |
| render_queue | [queue_face_packet](../src/queue_face_packet.c) | 0x00401EB8 | c | relocation-adjusted-match | 361 / 68 |
| render_queue | [submit_gathered_surfaces](../src/submit_gathered_surfaces.c) | 0x00406830 | c | relocation-adjusted-match | 48 / 20 |
| render_queue | [set_frame_state](../src/set_frame_state.c) | 0x00401D30 | c | relocation-adjusted-match | 392 / 172 |
| render_queue | [push_surface_draw_entry](../src/push_surface_draw_entry.cpp) | 0x004059F0 | c++ | relocation-adjusted-match | 96 / 24 |

Registered functions: **13**. Last recorded matches with unchanged definitions: **13**.
Current source fingerprint agrees with the latest run: **yes**.
This count is not whole-program coverage and does not certify the modern TPM reconstruction.

Latest build: '20260912-141544-c0201220', source commit '906474e89850a5b4ec915f814e8cbed9ae772745'.
History contains 14 verified build records.

## Workarounds and exceptions

- **material_mode**: Retain switch-shaped stack-local code generation under /Od. Exceptions: none.
- **grid_cell**: Evaluate Y before X to reproduce EAX/ECX allocation; Caller bounds and full grid extent remain unestablished. Exceptions: none.
- **plane_coordinate**: Y/Z divisions use explicit double intermediates and float stores; VC5 x87 C3-only comparisons take the early return on unordered input. Exceptions: none.
- **update_projection**: Local float retains the original reciprocal load/divide order; Compare all 15 trailing NOP alignment bytes. Exceptions: none.
- **face_clip_flags**: Unsigned vertex count; zero count returns 0xFF00 without array reads; Compare all six trailing NOP alignment bytes. Exceptions: none.
- **transform_project**: Reconstructed inline x87 kernel with compiler-generated C frame and alignment; original source text remains unknown; Raw positive-zero depth check distinguishes negative zero; unordered comparisons retain original condition flags; Retain reciprocal in x87 precision while storing its rounded float value; Zero selection entries leave output and clip slots unchanged; selected indices are one-based; Compare all nine trailing NOP bytes. Exceptions: none.
  Compiler diagnostic waiver C4725: Preserve original x87 FDIV instructions despite the advisory about defective early Pentium processors. Scope: src/transform_project.c function; warning level 4 explicitly restored afterward. No byte exclusion.
- **set_mesh_render_thing**: No opacity clamping; preserve out-of-range and nonfinite multiplication; Compare all five trailing NOP alignment bytes. Exceptions: none.
- **peek_projected_vertices**: Unsigned count addition wraps before the bound check; the helper does not reserve slots. Exceptions: none.
- **peek_face_packet**: Signed upper-bound check only; counter validity is a caller invariant; does not reserve a packet. Exceptions: none.
- **queue_face_packet**: Write render tag before face/material validation; Increment then roll back the bucket count on capacity failure; Successful submission appends in FIFO order and advances the packet count; no independent packet-capacity check; Retain descriptive local names because VC5 stack-slot assignment changes with identifiers. Exceptions: none.
- **submit_gathered_surfaces**: Zero count leaves the previous result untouched and does not call the renderer; Preserve camera-plus-eight and count arguments although the reviewed callee does not read them; Compare all eight trailing NOP alignment bytes. Exceptions: none.
- **set_frame_state**: Initial frame flags are written before being replaced by backend flags; Software-fog values remain stale when setup is skipped; Reciprocals and range are written before rejecting negative or VC5-unordered ranges; zero range stays enabled; An auxiliary frame flag at 0x0059DA00 has no reads in the returned direct-XREF set; its purpose remains unresolved. Exceptions: none.
- **push_surface_draw_entry**: VC5 RTM C++ frontend is required for the matching five-bit opacity instruction order in this reconstruction; Nonzero packed flags select the special bucket even when the low five bits expand to zero; Preserve existing nonzero opacity and all entry fields except next; caller advances count and writes surface; Compare both trailing NOP bytes; caller owns record and bucket bounds. Exceptions: none.

## Unresolved program work

- Original CRT startup/heap library revision
- Complete source-module inventory and original translation-unit boundaries
- Retail link order, placements, resources, data, and historical PE metadata
