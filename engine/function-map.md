# Function metadata recovered during matching

Names below describe observed behavior and are not claimed to be original symbols.
The original executable is never modified. Only Ghidra analysis metadata changes.

Address | Original Name | New Name | Confidence | Purpose | Evidence
--- | --- | --- | --- | --- | ---
0x0040F170 | FUN_0040f170 | combine_face_vertex_clip_codes | HIGH | Return union of indexed vertex clip codes plus intersection in bits 8-15 | Complete disassembly; unsigned count at face+0x14 and index pointer at +0x18; mesh caller at 0x0040F5EF tests 0xFF80; independent VC5 /O2 /MT candidate matches all 64 bytes including six padding bytes; synthetic zero/common/disjoint/behind-camera tests pass
0x0040EFD0 | FUN_0040efd0 | transform_project_selected_vertices | HIGH for behavior; inline-source history inferred | Apply affine transform, screen projection, optional clip codes, and reciprocal depth mapping | Complete body and nine padding bytes; eight incoming calls; C/inline-x87 candidate matches all 416 bytes after eleven checked address fixups; zero/negative-zero/NaN/selection/precision tests pass
0x0040F1B0 | FUN_0040f1b0 | set_mesh_render_thing | HIGH | Publish active render Thing and its opacity scaled by 255, with null reset | Complete 43-byte body and five padding bytes; projectile caller; ordinary actor setup identifies render_opacity at +0x150; mesh packet consumers; all 48 bytes match after four checked relocations; no-clamp and nonfinite behavior tests pass
0x00402130 | FUN_00402130 | peek_projected_vertex_span | HIGH | Return current float4 buffer span when unsigned wrapped count plus request is at most 8192; does not advance | Complete 37-byte body; four incoming XREFs; mesh/polyline projection consumers; separate commit and reset writers; VC5 /Od candidate matches all bytes after three verified address fixups
0x00402021 | FUN_00402021 | peek_model_face_packet | HIGH | Return current 184-byte model face packet while signed count is below 4096; does not advance | Complete 37-byte body; mesh/polyline callers; mesh fills fields then commits; counter writer and flush reset; VC5 /Od candidate matches after three verified fixups
0x00401EB8 | FUN_00401eb8 | queue_face_packet_by_material | HIGH | Append model face packet to material bucket with cache/search and 64-material limit | Complete 361-byte body; sole mesh caller; material flush follows bucket/head/next links; partial tag writes and count rollback; independent VC5 /Od source matches after 17 verified fixups; local identifiers preserve original stack slots

See [view/projection evidence](docs/view-projection-evidence.md) for scope and
remaining questions. Other existing Ghidra names are preserved.

| 0x00406830 | FUN_00406830 | submit_gathered_b3d_surfaces | HIGH | Submit nonempty gathered surfaces and store returned count | Full body and eight padding bytes; caller render_active_b3d_world; callee draw_b3d_surface_buckets; four global accesses and three cdecl arguments; docs/gathered-submission-evidence.md |
