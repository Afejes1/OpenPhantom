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

| 0x00401D30 | FUN_00401d30 | initialize_model_frame_state | HIGH | Reset queue/projection state, select frame flags and initialize software fog | Complete body; three callers, four callees, world fields and downstream flag reader; docs/frame-state-evidence.md |

| 0x004059F0 | FUN_004059f0 | link_surface_draw_entry | HIGH | Expand missing opacity and prepend the current entry to its texture/special bucket | Full body and padding; both callers own allocation; world.texture_count supplies special bucket; docs/surface-entry-evidence.md |

| 0x00403EF0 | FUN_00403ef0 | step_camera_space_grid_position | HIGH | Advance a camera-space triplet by signed unit X/Y grid steps | Complete body/padding, three incoming calls, transform copy and affine/culler consumers; docs/scan-step-evidence.md |

| 0x00406300 | FUN_00406300 | queue_visible_surface | HIGH behavior; compilation unverified | Apply current visibility/fade rules and prepend a surface entry | Full body, two mover callers, shared frame-mask writers and bucket consumer; docs/surface-emission-evidence.md |
| 0x00405A50 | FUN_00405a50 | queue_legacy_scan_surface | HIGH behavior; compilation unverified | Apply legacy integer fade, update scan height, cull and queue a surface | Full body, both legacy cell callers, camera/basis state and three call sites; docs/surface-emission-evidence.md |

| 0x00403FA0 | FUN_00403fa0 | cull_camera_space_scan_plane | HIGH behavior; compilation unverified | Test two selected camera-space corners through a scalar projection callback and reject against one scan boundary | Complete body/table/alignment, seven incoming calls, camera callback selector and four scalar callees, setup/basis/edge writers; docs/scan-culling-evidence.md |

| 0x004064B0 | FUN_004064b0 | gather_visible_static_cell_surfaces | HIGH behavior; compilation unverified | Traverse static cell surface groups, apply room/frustum skips and queue visible surfaces | Complete body and alignment, sole world-traversal caller, affine callee, bound/counter writers and consumers; 896-byte extent/all40 operands verified but five instruction bytes differ; docs/static-cell-evidence.md |

| 0x004056C0 | FUN_004056c0 | gather_legacy_cell_surfaces | HIGH; focused byte match awaiting batch acceptance | Gather linked/discovered group surfaces, perform slab culling and emit inline/reference records | Complete 809-byte body and seven NOPs, two world traversal callers, bucket/culler/emitter callees and group/queue/seen state; all 34 operands and 816-byte span match under VC5 C++; docs/legacy-cell-evidence.md |

| 0x00402155 | draw_model_material_buckets (existing descriptive name) | unchanged | HIGH behavior; body match, alignment pending | Flush material packet buckets into normalized backend vertices and immediate/alpha draws | Five callers, five direct callees, complete body and partial resource views; 1,112 body bytes and 24 operands agree, three trailing alignment bytes unresolved; docs/queue-flush-evidence.md |

| 0x00404180 | FUN_00404180 | initialize_legacy_scan_frame | HIGH purpose; extent mismatch | Copy active camera state, derive directional scan limits, build offsets and reset scan sentinels | Complete static body and seven padding bytes; world-render caller; trigonometric/line/scan-step callees; camera and scan globals; 153 original operands independently reviewed; strict extent mismatch 1,376 vs 1,456 bytes; VC5 C candidate passes 6,548 focused assertions; research/campaign-002/frame-binding-evidence.md |
| 0x00405CF0 | FUN_00405cf0 | gather_mover_cell_surfaces | HIGH purpose; byte comparison pending | Gather seed/mover cell surfaces with phase-specific visitation, fade/bucket rules and animation/direct mover tails | Complete static body and fifteen padding bytes; world-render caller at 0x004054C3; __ftol and queue_visible_surface callees; shared visit and queue globals; corrected VC5 C++ candidate passes 36 focused assertions; research/campaign-001-mover-gather/README.md |

| 0x004084B8 | FUN_004084b8 | reset_grid_scan_line | HIGH; focused full-span match awaiting batch acceptance | Allocate or reset an 80-byte integer grid-line traversal record from four endpoint coordinates | Complete 400-byte body; two frame-initializer calls; tracked allocator call at 0x00495290; field writes and error/step formulas; original VC5 C reproduces all 400 resolved bytes and passes 238 focused checks; research/campaign-002-line-reset/result.json |

| 0x00408648 | FUN_00408648 | test_segment_pair_line_sides | HIGH; focused raw-byte match awaiting batch acceptance | Reject endpoint pairs lying strictly on the same side of either supporting line | Complete 237-byte body; eight signed coordinate arguments; determinant-derived coefficients, two signed XOR rejection tests, no calls/globals; disjoint collinear and distinct point pairs return true; VC5 C matches all 237 raw bytes with 6,601 focused checks; research/campaign-002-segment-test/result.json |
