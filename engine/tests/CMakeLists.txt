cmake_minimum_required(VERSION 3.20)
project(openphantom_supplemental_behavior C CXX)
if(NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(FATAL_ERROR "This supplemental smoke test requires modern MSVC x86. Use verify.py for VC5.")
endif()
add_executable(behavior_smoke behavior.c ../src/material_mode.c ../src/grid_cell.c
  ../src/plane_coordinate.c ../src/update_projection.c ../src/face_clip_flags.c ../src/transform_project.c ../src/set_mesh_render_thing.c
  ../src/peek_projected_vertices.c ../src/peek_face_packet.c ../src/queue_face_packet.c ../src/submit_gathered_surfaces.c ../src/set_frame_state.c ../src/push_surface_draw_entry.cpp ../src/step_scan.c
  ../src/bgl_identity.c ../src/bgl_push.c ../src/bgl_pop.c ../src/bgl_load_matrix.c ../src/bgl_get_matrix.c ../src/bgl_get_translation.c ../src/bgl_get_translation_xyz.c)
target_compile_options(behavior_smoke PRIVATE /W4 /WX /Od /fp:precise)
enable_testing()
add_test(NAME supplemental_behavior COMMAND behavior_smoke)
