cmake_minimum_required(VERSION 3.20)
project(openphantom_supplemental_behavior C)
if(NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(FATAL_ERROR "This supplemental smoke test requires modern MSVC x86. Use verify.py for VC5.")
endif()
add_executable(behavior_smoke behavior.c ../src/material_mode.c ../src/grid_cell.c
  ../src/plane_coordinate.c ../src/update_projection.c ../src/face_clip_flags.c ../src/transform_project.c ../src/set_mesh_render_thing.c
  ../src/peek_projected_vertices.c ../src/peek_face_packet.c ../src/queue_face_packet.c)
target_compile_options(behavior_smoke PRIVATE /W4 /WX /Od /fp:precise)
enable_testing()
add_test(NAME supplemental_behavior COMMAND behavior_smoke)
