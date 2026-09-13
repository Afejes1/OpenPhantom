cmake_minimum_required(VERSION 3.20)
project(openphantom_supplemental_behavior C CXX)
if(NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(FATAL_ERROR "This supplemental smoke test requires modern MSVC x86. Use verify.py for VC5.")
endif()
add_executable(behavior_smoke behavior.c ../src/material_mode.c ../src/grid_cell.c
  ../src/plane_coordinate.c ../src/update_projection.c ../src/face_clip_flags.c ../src/transform_project.c ../src/set_mesh_render_thing.c
  ../src/peek_projected_vertices.c ../src/peek_face_packet.c ../src/queue_face_packet.c ../src/submit_gathered_surfaces.c ../src/set_frame_state.c ../src/push_surface_draw_entry.cpp ../src/step_scan.c
  ../src/bgl_identity.c ../src/bgl_push.c ../src/bgl_pop.c ../src/bgl_load_matrix.c ../src/bgl_get_matrix.c ../src/bgl_get_translation.c ../src/bgl_get_translation_xyz.c
  ../src/extended_load_colormap.c ../src/extended_free_colormaps.c
  ../src/world_read_emitters.c ../src/world_read_sprite_refs.c ../src/world_read_palettes.c
  ../src/world_read_global_vertices.c ../src/world_read_local_vertices.c
  ../src/world_set_clock.c ../src/world_set_ambient.c ../src/world_apply_fog.c
  ../src/world_read_animation_names.c ../src/world_mangle_names.c ../src/world_restart_clock.c
  ../src/world_read_chunk_header.c ../src/world_skip_chunk.c ../src/world_load_via_skip.c
  ../src/model_load_state.c ../src/get_system_font.c ../src/shield_radius.c ../src/shield_set_colour.c ../src/shield_set_visible.c ../src/shield_set_visibility_bypass.c
  ../src/shield_free.c ../src/shield_destroy_all.c ../src/shield_stop.c ../src/shield_save_size.c ../src/shield_calloc.c ../src/shield_set_texture.c ../src/shield_draw_released.c ../src/shield_destroy.c ../src/effects_object_destroyed.c)

target_compile_options(behavior_smoke PRIVATE /W4 /WX /Od /fp:precise)
enable_testing()
add_test(NAME supplemental_behavior COMMAND behavior_smoke)
