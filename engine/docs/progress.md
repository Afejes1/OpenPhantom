# Reconstruction progress

Registered reconstruction work only; not a whole-program census.

Generated from registry.json, target.json and chained verification records. No game bytes are stored here.

Whole executable: **incomplete**. Original linked placement: **unverified**.

| Module | Function / source | Original address | VC5 frontend | Last verified result | Bytes / address bytes |
|---|---|---|---|---|---|
| material | [material_mode](../src/material_mode.c) | 0x0040E840 | c | raw-code-match (historical; current source unverified) | 41 / 0 |
| spatial_grid | [grid_cell](../src/grid_cell.c) | 0x00406E22 | c | relocation-adjusted-match (historical; current source unverified) | 24 / 4 |
| map_geometry | [plane_coordinate](../src/plane_coordinate.c) | 0x0040DCEE | c | relocation-adjusted-match (historical; current source unverified) | 384 / 8 |
| view_projection | [update_projection](../src/update_projection.c) | 0x0040EF40 | c | relocation-adjusted-match (historical; current source unverified) | 144 / 48 |
| view_projection | [face_clip_flags](../src/face_clip_flags.c) | 0x0040F170 | c | raw-code-match (historical; current source unverified) | 64 / 0 |
| view_projection | [transform_project](../src/transform_project.c) | 0x0040EFD0 | c | relocation-adjusted-match (historical; current source unverified) | 416 / 44 |
| mesh_submission | [set_mesh_render_thing](../src/set_mesh_render_thing.c) | 0x0040F1B0 | c | relocation-adjusted-match (historical; current source unverified) | 48 / 16 |
| render_queue | [peek_projected_vertices](../src/peek_projected_vertices.c) | 0x00402130 | c | relocation-adjusted-match (historical; current source unverified) | 37 / 12 |
| render_queue | [peek_face_packet](../src/peek_face_packet.c) | 0x00402021 | c | relocation-adjusted-match (historical; current source unverified) | 37 / 12 |
| render_queue | [queue_face_packet](../src/queue_face_packet.c) | 0x00401EB8 | c | relocation-adjusted-match (historical; current source unverified) | 361 / 68 |
| render_queue | [submit_gathered_surfaces](../src/submit_gathered_surfaces.c) | 0x00406830 | c | relocation-adjusted-match (historical; current source unverified) | 48 / 20 |
| render_queue | [set_frame_state](../src/set_frame_state.c) | 0x00401D30 | c | relocation-adjusted-match (historical; current source unverified) | 392 / 172 |
| render_queue | [push_surface_draw_entry](../src/push_surface_draw_entry.cpp) | 0x004059F0 | c++ | relocation-adjusted-match (historical; current source unverified) | 96 / 24 |
| legacy_scan | [step_scan](../src/step_scan.c) | 0x00403EF0 | c | relocation-adjusted-match (historical; current source unverified) | 176 / 48 |
| bgl_matrix_state | [bgl_identity](../src/bgl_identity.c) | 0x0041F700 | c | relocation-adjusted-match (historical; current source unverified) | 27 / 8 |
| bgl_matrix_state | [bgl_push](../src/bgl_push.c) | 0x0041F71B | c | relocation-adjusted-match (historical; current source unverified) | 85 / 32 |
| bgl_matrix_state | [bgl_pop](../src/bgl_pop.c) | 0x0041F770 | c | relocation-adjusted-match (historical; current source unverified) | 44 / 20 |
| bgl_matrix_state | [bgl_load_matrix](../src/bgl_load_matrix.c) | 0x0041F910 | c | relocation-adjusted-match (historical; current source unverified) | 25 / 4 |
| bgl_matrix_state | [bgl_get_matrix](../src/bgl_get_matrix.c) | 0x0041F929 | c | relocation-adjusted-match (historical; current source unverified) | 25 / 4 |
| bgl_matrix_state | [bgl_get_translation](../src/bgl_get_translation.c) | 0x0041FA37 | c | relocation-adjusted-match (historical; current source unverified) | 49 / 12 |
| bgl_matrix_state | [bgl_get_translation_xyz](../src/bgl_get_translation_xyz.c) | 0x0041FA68 | c | relocation-adjusted-match (historical; current source unverified) | 65 / 12 |
| extended_colormaps | [extended_load_colormap](../src/extended_load_colormap.c) | 0x00428844 | c | relocation-adjusted-match (historical; current source unverified) | 73 / 24 |
| extended_colormaps | [extended_free_colormaps](../src/extended_free_colormaps.c) | 0x0042888D | c | relocation-adjusted-match (historical; current source unverified) | 68 / 16 |
| b3d_chunk_readers | [world_read_emitters](../src/world_read_emitters.c) | 0x0041D302 | c | unverified | 94 / 4 |
| b3d_chunk_readers | [world_read_sprite_refs](../src/world_read_sprite_refs.c) | 0x0041D360 | c | unverified | 108 / 8 |
| b3d_chunk_readers | [world_read_palettes](../src/world_read_palettes.c) | 0x0041D5A3 | c | unverified | 90 / 4 |

Registered functions: **26**. Last recorded matches with unchanged definitions: **23**.
Current source fingerprint agrees with the latest run: **no**.
This count is not whole-program coverage and does not certify the modern TPM reconstruction.

Latest build: '20260913-081301-a3349fa4', source commit 'c1146a9a580d517ca5bdb84eaf28568f0942e868'.
History contains 22 verified build records.

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
- **step_scan**: Only exact +1 and -1 directions act; other values do not dereference the position for that axis; Preserve separate float stores after X before applying Y; Explicit double promotions on the last two positive coordinates reproduce original VC5 x87 operand scheduling; All three trailing NOP bytes are compared; no byte exclusions. Exceptions: none.
- **bgl_identity**: C representation uses12 raw32-bit words for exact48-byte copy and alias behavior; Canonical shared header; candidate body unchanged from frozen Campaign011 snapshot. Exceptions: none.
- **bgl_push**: C representation uses12 raw32-bit words for exact48-byte copy and alias behavior; Canonical shared header; candidate body unchanged from frozen Campaign011 snapshot. Exceptions: none.
- **bgl_pop**: C representation uses12 raw32-bit words for exact48-byte copy and alias behavior; Canonical shared header; candidate body unchanged from frozen Campaign011 snapshot. Exceptions: none.
- **bgl_load_matrix**: C representation uses12 raw32-bit words for exact48-byte copy and alias behavior; Canonical shared header; candidate body unchanged from frozen Campaign011 snapshot. Exceptions: none.
- **bgl_get_matrix**: C representation uses12 raw32-bit words for exact48-byte copy and alias behavior; Canonical shared header; candidate body unchanged from frozen Campaign011 snapshot. Exceptions: none.
- **bgl_get_translation**: C representation uses12 raw32-bit words for exact48-byte copy and alias behavior; Canonical shared header; candidate body unchanged from frozen Campaign011 snapshot. Exceptions: none.
- **bgl_get_translation_xyz**: C representation uses12 raw32-bit words for exact48-byte copy and alias behavior; Canonical shared header; candidate body unchanged from frozen Campaign011 snapshot. Exceptions: none.
- **extended_load_colormap**: Signed shared count and live callback field reloads preserved; Array entries remain after free; capacity-full load still returns its resource; Common-header include is the only source text change from frozen Campaign015. Exceptions: none.
- **extended_free_colormaps**: Signed shared count and live callback field reloads preserved; Array entries remain after free; capacity-full load still returns its resource; Common-header include is the only source text change from frozen Campaign015. Exceptions: none.
- **world_read_emitters**: Unsigned payload products preserve32-bit wrapping; explicit signed stream-size boundary preserves original argument words; Shared offset-checked world/header views; callback mutations are observed in original order. Exceptions: none.
- **world_read_sprite_refs**: Unsigned payload products preserve32-bit wrapping; explicit signed stream-size boundary preserves original argument words; Shared offset-checked world/header views; callback mutations are observed in original order. Exceptions: none.
- **world_read_palettes**: Unsigned payload products preserve32-bit wrapping; explicit signed stream-size boundary preserves original argument words; Shared offset-checked world/header views; callback mutations are observed in original order. Exceptions: none.

## Unresolved program work

- Original CRT startup/heap library revision
- Complete source-module inventory and original translation-unit boundaries
- Retail link order, placements, resources, data, and historical PE metadata
