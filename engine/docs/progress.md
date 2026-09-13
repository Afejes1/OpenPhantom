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
| legacy_scan | [step_scan](../src/step_scan.c) | 0x00403EF0 | c | relocation-adjusted-match | 176 / 48 |
| bgl_matrix_state | [bgl_identity](../src/bgl_identity.c) | 0x0041F700 | c | relocation-adjusted-match | 27 / 8 |
| bgl_matrix_state | [bgl_push](../src/bgl_push.c) | 0x0041F71B | c | relocation-adjusted-match | 85 / 32 |
| bgl_matrix_state | [bgl_pop](../src/bgl_pop.c) | 0x0041F770 | c | relocation-adjusted-match | 44 / 20 |
| bgl_matrix_state | [bgl_load_matrix](../src/bgl_load_matrix.c) | 0x0041F910 | c | relocation-adjusted-match | 25 / 4 |
| bgl_matrix_state | [bgl_get_matrix](../src/bgl_get_matrix.c) | 0x0041F929 | c | relocation-adjusted-match | 25 / 4 |
| bgl_matrix_state | [bgl_get_translation](../src/bgl_get_translation.c) | 0x0041FA37 | c | relocation-adjusted-match | 49 / 12 |
| bgl_matrix_state | [bgl_get_translation_xyz](../src/bgl_get_translation_xyz.c) | 0x0041FA68 | c | relocation-adjusted-match | 65 / 12 |
| extended_colormaps | [extended_load_colormap](../src/extended_load_colormap.c) | 0x00428844 | c | relocation-adjusted-match | 73 / 24 |
| extended_colormaps | [extended_free_colormaps](../src/extended_free_colormaps.c) | 0x0042888D | c | relocation-adjusted-match | 68 / 16 |
| b3d_chunk_readers | [world_read_emitters](../src/world_read_emitters.c) | 0x0041D302 | c | relocation-adjusted-match | 94 / 4 |
| b3d_chunk_readers | [world_read_sprite_refs](../src/world_read_sprite_refs.c) | 0x0041D360 | c | relocation-adjusted-match | 108 / 8 |
| b3d_chunk_readers | [world_read_palettes](../src/world_read_palettes.c) | 0x0041D5A3 | c | relocation-adjusted-match | 90 / 4 |
| b3d_chunk_readers | [world_read_global_vertices](../src/world_read_global_vertices.c) | 0x0041D8EE | c | relocation-adjusted-match | 149 / 4 |
| b3d_chunk_readers | [world_read_local_vertices](../src/world_read_local_vertices.c) | 0x0041D983 | c | relocation-adjusted-match | 152 / 4 |
| b3d_world_controls | [world_set_clock](../src/world_set_clock.c) | 0x0041F0C9 | c | relocation-adjusted-match | 67 / 8 |
| b3d_world_controls | [world_set_ambient](../src/world_set_ambient.c) | 0x0041F10C | c | relocation-adjusted-match | 62 / 8 |
| b3d_world_controls | [world_apply_fog](../src/world_apply_fog.c) | 0x0041F14A | c | relocation-adjusted-match | 230 / 40 |
| b3d_world_names | [world_read_animation_names](../src/world_read_animation_names.c) | 0x0041E3BD | c | relocation-adjusted-match | 253 / 8 |
| b3d_world_names | [world_mangle_names](../src/world_mangle_names.c) | 0x0041EABC | c | raw-code-match | 158 / 0 |
| b3d_world_names | [world_restart_clock](../src/world_restart_clock.c) | 0x0041EB5A | c | relocation-adjusted-match | 58 / 8 |
| focused_accessors | [model_load_state](../src/model_load_state.c) | 0x0045C47B | c | raw-code-match | 14 / 0 |
| focused_accessors | [get_system_font](../src/get_system_font.c) | 0x0046B754 | c | relocation-adjusted-match | 10 / 4 |
| focused_accessors | [shield_radius](../src/shield_radius.c) | 0x0043AFD3 | c | relocation-adjusted-match | 67 / 12 |
| focused_accessors | [shield_set_colour](../src/shield_set_colour.c) | 0x0043AEE7 | c | relocation-adjusted-match | 94 / 4 |
| focused_accessors | [shield_set_visible](../src/shield_set_visible.c) | 0x0043B016 | c | relocation-adjusted-match | 67 / 4 |
| focused_accessors | [shield_set_visibility_bypass](../src/shield_set_visibility_bypass.c) | 0x0043B059 | c | relocation-adjusted-match | 67 / 4 |
| b3d_chunk_stream | [world_read_chunk_header](../src/world_read_chunk_header.c) | 0x0041CD80 | c | relocation-adjusted-match | 112 / 12 |
| b3d_chunk_stream | [world_skip_chunk](../src/world_skip_chunk.c) | 0x0041CDF0 | c | relocation-adjusted-match | 93 / 16 |
| b3d_chunk_stream | [world_load_via_skip](../src/world_load_via_skip.c) | 0x0041E4BA | c | relocation-adjusted-match | 32 / 4 |
| shield_lifecycle | [shield_free](../src/shield_free.c) | 0x0043B1E9 | c | relocation-adjusted-match | 23 / 4 |
| shield_lifecycle | [shield_destroy_all](../src/shield_destroy_all.c) | 0x0043B64D | c | relocation-adjusted-match | 46 / 4 |
| shield_lifecycle | [shield_stop](../src/shield_stop.c) | 0x0043B5E6 | c | relocation-adjusted-match | 103 / 4 |
| shield_lifecycle | [shield_save_size](../src/shield_save_size.c) | 0x0043C943 | c | relocation-adjusted-match | 110 / 4 |
| shield_lifecycle | [shield_calloc](../src/shield_calloc.c) | 0x0043AE1B | c | relocation-adjusted-match | 56 / 4 |
| shield_lifecycle | [shield_set_texture](../src/shield_set_texture.c) | 0x0043AE53 | c | relocation-adjusted-match | 148 / 12 |
| shield_lifecycle | [shield_draw_released](../src/shield_draw_released.c) | 0x0043B67B | c | relocation-adjusted-match | 134 / 28 |
| shield_lifecycle | [shield_destroy](../src/shield_destroy.c) | 0x0043B09C | c | relocation-adjusted-match | 333 / 48 |
| shield_lifecycle | [effects_object_destroyed](../src/effects_object_destroyed.c) | 0x00438EE5 | c | relocation-adjusted-match | 47 / 12 |
| effects_state | [fog_snapshot](../src/fog_snapshot.c) | 0x00438F14 | c | relocation-adjusted-match | 35 / 12 |
| effects_state | [fog_restore](../src/fog_restore.c) | 0x00438F37 | c | relocation-adjusted-match | 74 / 28 |
| effects_state | [fog_set_start](../src/fog_set_start.c) | 0x00439023 | c | relocation-adjusted-match | 42 / 12 |
| effects_state | [fog_set_end](../src/fog_set_end.c) | 0x0043904D | c | relocation-adjusted-match | 33 / 8 |
| effects_state | [tint_start_opaque](../src/tint_start_opaque.c) | 0x004393D0 | c | relocation-adjusted-match | 42 / 4 |
| effects_state | [tint_stop](../src/tint_stop.c) | 0x004393FA | c | relocation-adjusted-match | 56 / 20 |
| effects_state | [fog_start](../src/fog_start.c) | 0x0043906E | c | relocation-adjusted-match | 193 / 60 |
| effects_state | [fog_tick](../src/fog_tick.c) | 0x00438F81 | c | relocation-adjusted-match | 162 / 76 |
| effects_state | [tint_start_full](../src/tint_start_full.c) | 0x00439370 | c | relocation-adjusted-match | 96 / 44 |
| effects_state | [fade_reset](../src/fade_reset.c) | 0x00439720 | c | relocation-adjusted-match | 40 / 16 |
| halo_overlay | [letterbox_set_enabled](../src/letterbox_set_enabled.c) | 0x004396CF | c | relocation-adjusted-match | 81 / 28 |
| halo_overlay | [overlay_save_state](../src/overlay_save_state.c) | 0x00439752 | c | relocation-adjusted-match | 66 / 40 |
| halo_overlay | [overlay_read_state](../src/overlay_read_state.c) | 0x00439794 | c | relocation-adjusted-match | 33 / 8 |
| halo_overlay | [halo_startup](../src/halo_startup.c) | 0x004397F0 | c | relocation-adjusted-match | 59 / 36 |
| halo_overlay | [halo_shutdown](../src/halo_shutdown.c) | 0x0043982B | c | relocation-adjusted-match | 44 / 24 |
| halo_overlay | [effects_draw_object](../src/effects_draw_object.c) | 0x00438E78 | c | relocation-adjusted-match | 69 / 8 |
| halo_overlay | [detach_halo](../src/detach_halo.c) | 0x0043A154 | c | relocation-adjusted-match | 103 / 24 |
| zap_effects | [detach_zap](../src/detach_zap.c) | 0x0043D4DF | c | relocation-adjusted-match | 116 / 4 |
| zap_effects | [zap_startup](../src/zap_startup.c) | 0x0043CCF0 | c | relocation-adjusted-match | 50 / 20 |
| zap_effects | [zap_shutdown](../src/zap_shutdown.c) | 0x0043CD22 | c | relocation-adjusted-match | 23 / 8 |
| zap_effects | [zap_reset](../src/zap_reset.c) | 0x0043D553 | c | relocation-adjusted-match | 31 / 8 |
| zap_effects | [zap_has_actor](../src/zap_has_actor.c) | 0x0043D572 | c | relocation-adjusted-match | 131 / 12 |
| halo_overlay | [halo_free_all](../src/halo_free_all.c) | 0x0043A1BB | c | relocation-adjusted-match | 110 / 28 |
| effects_save | [effects_load](../src/effects_load.c) | 0x00439235 | c | relocation-adjusted-match | 152 / 64 |
| halo_overlay | [halo_draw_actor](../src/halo_draw_actor.c) | 0x00439A54 | c | relocation-adjusted-match | 98 / 12 |
| ripple_effects | [ripple_material](../src/ripple_material.c) | 0x0043AAC3 | c | relocation-adjusted-match | 163 / 44 |
| zap_effects | [zap_draw_segment](../src/zap_draw_segment.c) | 0x0043D648 | c | relocation-adjusted-match | 105 / 20 |
| zap_effects | [zap_draw_default](../src/zap_draw_default.c) | 0x0043D5F5 | c | relocation-adjusted-match | 83 / 20 |
| halo_overlay | [effects_object_visibility](../src/effects_object_visibility.c) | 0x00438EBD | c | relocation-adjusted-match | 40 / 8 |
| halo_overlay | [effects_object_created](../src/effects_object_created.c) | 0x00438E67 | c | relocation-adjusted-match | 17 / 4 |
| halo_overlay | [halo_add](../src/halo_add.c) | 0x00439857 | c | relocation-adjusted-match | 231 / 24 |
| halo_overlay | [halo_attach](../src/halo_attach.c) | 0x0043993E | c | relocation-adjusted-match | 278 / 64 |
| effects_save | [effects_write](../src/effects_write.c) | 0x0043912F | c | relocation-adjusted-match | 262 / 132 |
| effects_save | [shield_write](../src/shield_write.c) | 0x0043C9B1 | c | relocation-adjusted-match | 331 / 16 |
| effects_save | [shield_load](../src/shield_load.c) | 0x0043CAFC | c | relocation-adjusted-match | 300 / 20 |
| ripple_spawn | [ripple_spawn](../src/ripple_spawn.c) | 0x0043A2F0 | c | relocation-adjusted-match | 350 / 28 |
| projected_overlay | [projection_expire](../src/projection_expire.c) | 0x0041C8B0 | c | relocation-adjusted-match | 96 / 16 |
| projected_overlay | [projection_remove](../src/projection_remove.c) | 0x0041C910 | c | relocation-adjusted-match | 64 / 8 |
| projected_overlay | [projection_clear](../src/projection_clear.c) | 0x0041C950 | c | relocation-adjusted-match | 32 / 4 |
| projected_overlay | [projection_sort](../src/projection_sort.c) | 0x0041C970 | c | relocation-adjusted-match | 48 / 20 |
| projected_overlay | [projection_compare](../src/projection_compare.c) | 0x0041C9A0 | c | raw-code-match | 64 / 0 |
| application_helpers | [application_run](../src/application_run.c) | 0x0043E5E0 | c | relocation-adjusted-match | 51 / 24 |
| application_helpers | [graphics_close](../src/graphics_close.c) | 0x0043F5C0 | c | relocation-adjusted-match | 25 / 16 |
| application_helpers | [diagnostics_close](../src/diagnostics_close.c) | 0x0043EFD1 | c | relocation-adjusted-match | 28 / 12 |
| application_helpers | [simulation_set_halted](../src/simulation_set_halted.c) | 0x0043FAA8 | c | relocation-adjusted-match | 13 / 4 |
| application_helpers | [halo_flicker](../src/halo_flicker.c) | 0x0043A116 | c | relocation-adjusted-match | 62 / 24 |
| application_helpers | [projection_token](../src/projection_token.c) | 0x0041C4B0 | c | relocation-adjusted-match | 16 / 8 |
| application_helpers | [overlay_size](../src/overlay_size.c) | 0x00439748 | c | raw-code-match | 10 / 0 |
| puppet_tracks | [puppet_init](../src/puppet_init.c) | 0x00483670 | c | relocation-adjusted-match | 64 / 4 |
| puppet_tracks | [track_remove](../src/track_remove.c) | 0x004837A0 | c | raw-code-match | 96 / 0 |
| puppet_tracks | [track_play](../src/track_play.c) | 0x00483850 | c | raw-code-match | 48 / 0 |
| puppet_tracks | [track_fade_in](../src/track_fade_in.c) | 0x00483880 | c | relocation-adjusted-match | 96 / 8 |
| puppet_tracks | [track_fade_out](../src/track_fade_out.c) | 0x004838E0 | c | relocation-adjusted-match | 96 / 8 |
| zap_geometry | [zap_build](../src/zap_build.c) | 0x0043D6B1 | c | relocation-adjusted-match | 211 / 36 |
| zap_geometry | [zap_subdivide](../src/zap_subdivide.c) | 0x0043D784 | c | relocation-adjusted-match | 326 / 96 |
| zap_geometry | [zap_draw_opaque](../src/zap_draw_opaque.c) | 0x0043D8CA | c | relocation-adjusted-match | 159 / 20 |
| sprite_resources | [sprite_frame](../src/sprite_frame.c) | 0x0042959D | c | relocation-adjusted-match | 87 / 4 |

Registered functions: **109**. Last recorded matches with unchanged definitions: **109**.
Current source fingerprint agrees with the latest run: **yes**.
This count is not whole-program coverage and does not certify the modern TPM reconstruction.

Latest build: '20260913-185153-a2cd920b', source commit 'd911f98989eef764f40c0eab201f099802ad9848'.
History contains 40 verified build records.

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
- **world_read_global_vertices**: Unsigned payload products preserve32-bit wrapping; explicit signed stream-size boundary preserves original argument words; World record pointer is captured before the loop; header count and record size remain live across callbacks. Exceptions: none.
- **world_read_local_vertices**: Payload mismatch returns one without callbacks or stores; World record pointer is captured before the loop; header count and record size remain live across callbacks. Exceptions: none.
- **world_set_clock**: Clock integer conversion fixture domain is finite and representable; original x87 0.01f produces9 ticks; Input seconds bits are retained; old ticks and seconds are read before their replacement. Exceptions: none.
- **world_set_ambient**: Masked x87 unordered comparison clamps quiet NaN to positive zero; negative zero is retained; Compiler-specific edge fixture uses an explicit saved/restored floating-point environment. Exceptions: none.
- **world_apply_fog**: RGB bytes are captured before callbacks while range and enabled flag stay live; Fog RGB uses full unsigned words; clear RGB and pixel callbacks use byte and short parameters. Exceptions: none.
- **world_read_animation_names**: Captured original cursor and live retargeted table remain distinct across the read callback; Allocation failure still returns success after count and size publication. Exceptions: none.
- **world_mangle_names**: Signed count interpretation is explicit on the shared unsigned count word; Temporary normalization has no external writeback; full byte equality proves the local work. Exceptions: none.
- **world_restart_clock**: Canonical signed tick fields preserve the original return-word bits; delta zero uses the shared float field; The linked helper is the reconstructed name routine, not a fixture stub. Exceptions: none.
- **model_load_state**: None. Exceptions: none.
- **get_system_font**: None. Exceptions: none.
- **shield_radius**: None. Exceptions: none.
- **shield_set_colour**: None. Exceptions: none.
- **shield_set_visible**: None. Exceptions: none.
- **shield_set_visibility_bypass**: Stable source ID for visibility bypass also represents the no-save/transient flag. Exceptions: none.
- **world_read_chunk_header**: A zero read returns minus one without a final payload store; any nonzero read scans bounded tags; The tag table remains live across compare callbacks; unknown tags return twenty-three. Exceptions: none.
- **world_skip_chunk**: Actual cdecl ABI has two arguments; seek and backend failures are ignored; The explicit unsigned-to-signed read count cast preserves the original 32-bit word; the allocated local is retained through callbacks. Exceptions: none.
- **world_load_via_skip**: The unused world pointer is not accessed; offset 0x800 is a distinct field; The integration fixture links the actual reconstructed skip helper. Exceptions: none.
- **shield_free**: Shared canonical record uses integer active at offset0 and attached actor at offset0x10; internal reconstructed callees are linked directly.. Exceptions: none.
- **shield_destroy_all**: Shared canonical record uses integer active at offset0 and attached actor at offset0x10; internal reconstructed callees are linked directly.. Exceptions: none.
- **shield_stop**: Shared canonical record uses integer active at offset0 and attached actor at offset0x10; internal reconstructed callees are linked directly.. Exceptions: none.
- **shield_save_size**: Shared canonical record uses integer active at offset0 and attached actor at offset0x10; internal reconstructed callees are linked directly.. Exceptions: none.
- **shield_calloc**: Shared canonical record uses integer active at offset0 and attached actor at offset0x10; internal reconstructed callees are linked directly.. Exceptions: none.
- **shield_set_texture**: Shared canonical record uses integer active at offset0 and attached actor at offset0x10; internal reconstructed callees are linked directly.. Exceptions: none.
- **shield_draw_released**: Shared canonical record uses integer active at offset0 and attached actor at offset0x10; internal reconstructed callees are linked directly.. Exceptions: none.
- **shield_destroy**: Shared canonical record uses integer active at offset0 and attached actor at offset0x10; internal reconstructed callees are linked directly.. Exceptions: none.
- **effects_object_destroyed**: Shared canonical record uses integer active at offset0 and attached actor at offset0x10; internal reconstructed callees are linked directly.. Exceptions: none.
- **fog_snapshot**: Original-toolchain floating behavior is tested separately from supplemental modern behavior.. Exceptions: none.
- **fog_restore**: Original-toolchain floating behavior is tested separately from supplemental modern behavior.. Exceptions: none.
- **fog_set_start**: Original-toolchain floating behavior is tested separately from supplemental modern behavior.. Exceptions: none.
- **fog_set_end**: Original-toolchain floating behavior is tested separately from supplemental modern behavior.. Exceptions: none.
- **tint_start_opaque**: Original-toolchain floating behavior is tested separately from supplemental modern behavior.. Exceptions: none.
- **tint_stop**: Original-toolchain floating behavior is tested separately from supplemental modern behavior.. Exceptions: none.
- **fog_start**: Original-toolchain floating behavior is tested separately from supplemental modern behavior.. Exceptions: none.
- **fog_tick**: Original-toolchain floating behavior is tested separately from supplemental modern behavior.. Exceptions: none.
- **tint_start_full**: Original-toolchain floating behavior is tested separately from supplemental modern behavior.. Exceptions: none.
- **fade_reset**: Original-toolchain floating behavior is tested separately from supplemental modern behavior.; No direct callers; lifecycle provenance remains unresolved.. Exceptions: none.
- **letterbox_set_enabled**: External resource, rendering and serializer callbacks are authored fixtures.; Full original extent and every operand binding are required; final executable placement remains pending.. Exceptions: none.
- **overlay_save_state**: External resource, rendering and serializer callbacks are authored fixtures.; Full original extent and every operand binding are required; final executable placement remains pending.. Exceptions: none.
- **overlay_read_state**: External resource, rendering and serializer callbacks are authored fixtures.; Full original extent and every operand binding are required; final executable placement remains pending.. Exceptions: none.
- **halo_startup**: External resource, rendering and serializer callbacks are authored fixtures.; Full original extent and every operand binding are required; final executable placement remains pending.. Exceptions: none.
- **halo_shutdown**: External resource, rendering and serializer callbacks are authored fixtures.; Full original extent and every operand binding are required; final executable placement remains pending.. Exceptions: none.
- **effects_draw_object**: External resource, rendering and serializer callbacks are authored fixtures.; Full original extent and every operand binding are required; final executable placement remains pending.. Exceptions: none.
- **detach_halo**: External resource, rendering and serializer callbacks are authored fixtures.; Full original extent and every operand binding are required; final executable placement remains pending.. Exceptions: none.
- **detach_zap**: Original complete span and every operand must match; external callbacks remain authored fixtures.. Exceptions: none.
- **zap_startup**: Original complete span and every operand must match; external callbacks remain authored fixtures.. Exceptions: none.
- **zap_shutdown**: Original complete span and every operand must match; external callbacks remain authored fixtures.. Exceptions: none.
- **zap_reset**: Original complete span and every operand must match; external callbacks remain authored fixtures.. Exceptions: none.
- **zap_has_actor**: Original complete span and every operand must match; external callbacks remain authored fixtures.. Exceptions: none.
- **halo_free_all**: Original complete span and every operand must match; external callbacks remain authored fixtures.; No direct callers; lifecycle provenance remains unresolved.. Exceptions: none.
- **effects_load**: Original complete span and every operand must match; external callbacks remain authored fixtures.. Exceptions: none.
- **halo_draw_actor**: Original complete span and every operand must match; external callbacks remain authored fixtures.. Exceptions: none.
- **ripple_material**: Original complete span and every operand must match; external callbacks remain authored fixtures.; Unchecked appearance domain0..7; VC5 evaluates the opacity material argument before color.. Exceptions: none.
- **zap_draw_segment**: Original complete span and every operand must match; external callbacks remain authored fixtures.; Operational depth domain is nonnegative; negative C shifts are outside the source contract.. Exceptions: none.
- **zap_draw_default**: Original complete span and every operand must match; external callbacks remain authored fixtures.; No direct callers; lifecycle provenance remains unresolved.. Exceptions: none.
- **effects_object_visibility**: Original complete span and every operand must match; external callbacks remain authored fixtures.. Exceptions: none.
- **effects_object_created**: Original complete span and every operand must match; external callbacks remain authored fixtures.. Exceptions: none.
- **halo_add**: Original complete span and every address operand must match. External backends remain authored fixtures.. Exceptions: none.
- **halo_attach**: Original complete span and every address operand must match. External backends remain authored fixtures.. Exceptions: none.
- **effects_write**: Original complete span and every address operand must match. External backends remain authored fixtures.. Exceptions: none.
- **shield_write**: Original complete span and every address operand must match. External backends remain authored fixtures.. Exceptions: none.
- **shield_load**: Original complete span and every address operand must match. External backends remain authored fixtures.. Exceptions: none.
- **ripple_spawn**: Original complete span and every address operand must match. External backends remain authored fixtures.. Exceptions: none.
- **projection_expire**: Full span includes compiler alignment. Surface and sequence are unsigned32 address words. External qsort uses the authored fixture runtime.. Exceptions: none.
- **projection_remove**: Full span includes compiler alignment. Surface and sequence are unsigned32 address words. External qsort uses the authored fixture runtime.. Exceptions: none.
- **projection_clear**: Full span includes compiler alignment. Surface and sequence are unsigned32 address words. External qsort uses the authored fixture runtime.. Exceptions: none.
- **projection_sort**: Full span includes compiler alignment. Surface and sequence are unsigned32 address words. External qsort uses the authored fixture runtime.. Exceptions: none.
- **projection_compare**: Full span includes compiler alignment. Surface and sequence are unsigned32 address words. External qsort uses the authored fixture runtime.. Exceptions: none.
- **application_run**: Exact complete span; callback effects and source signedness limits remain documented in focused contract.. Exceptions: none.
- **graphics_close**: Exact complete span; callback effects and source signedness limits remain documented in focused contract.. Exceptions: none.
- **diagnostics_close**: Exact complete span; callback effects and source signedness limits remain documented in focused contract.. Exceptions: none.
- **simulation_set_halted**: Exact complete span; callback effects and source signedness limits remain documented in focused contract.. Exceptions: none.
- **halo_flicker**: Exact complete span; callback effects and source signedness limits remain documented in focused contract.. Exceptions: none.
- **projection_token**: Exact complete span; callback effects and source signedness limits remain documented in focused contract.. Exceptions: none.
- **overlay_size**: Exact complete span; callback effects and source signedness limits remain documented in focused contract.. Exceptions: none.
- **puppet_init**: Valid indices0..3 only; all unresolved fields opaque; callback returns ignored; finite fade fixtures.. Exceptions: none.
- **track_remove**: Valid indices0..3 only; all unresolved fields opaque; callback returns ignored; finite fade fixtures.. Exceptions: none.
- **track_play**: Valid indices0..3 only; all unresolved fields opaque; callback returns ignored; finite fade fixtures.. Exceptions: none.
- **track_fade_in**: Valid indices0..3 only; all unresolved fields opaque; callback returns ignored; finite fade fixtures.. Exceptions: none.
- **track_fade_out**: Valid indices0..3 only; all unresolved fields opaque; callback returns ignored; finite fade fixtures.. Exceptions: none.
- **zap_build**: Proven wrapper prefix17 vectors; depths0..4 and disjoint/exact snapshot alias only. Real build/subdivide and wrapper-specific live callback order.. Exceptions: none.
- **zap_subdivide**: Proven wrapper prefix17 vectors; depths0..4 and disjoint/exact snapshot alias only. Real build/subdivide and wrapper-specific live callback order.. Exceptions: none.
- **zap_draw_opaque**: Proven wrapper prefix17 vectors; depths0..4 and disjoint/exact snapshot alias only. Real build/subdivide and wrapper-specific live callback order.. Exceptions: none.
- **sprite_frame**: Outer NULL returnsNULL without backend; valid inner material/frame required; return captured frame+44 despite callback retargeting.. Exceptions: none.

## Unresolved program work

- Original CRT startup/heap library revision
- Complete source-module inventory and original translation-unit boundaries
- Retail link order, placements, resources, data, and historical PE metadata
