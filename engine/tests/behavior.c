/* Synthetic data only. Modern builds exercise finite behavior and x86 layouts;
 * the unordered x87 contract is asserted only in the original-toolchain build.
 */
#include "../src/baseline.h"
#include "../src/b3d_chunk_readers.h"
#include <float.h>
#include <stddef.h>
#include <stdio.h>

#ifdef OP_VC5_BEHAVIOR
#if !defined(_MSC_VER) || _MSC_VER != 1100
#error Original-toolchain behavioral verification requires VC5.
#endif
#endif

unsigned char op_grid[65536];
op_camera_state *op_camera;
unsigned int op_width_bits, op_height_bits;
float op_focal, op_depth_scale;
float op_edge_18, op_edge_1c, op_edge_20, op_edge_24;
float op_far, op_near;
int op_depth_mode;
op_render_thing *op_active_render_thing;
float op_mesh_opacity;

unsigned int op_projected_vertex_count;
op_projected_vertex op_projected_vertices[8192];
int op_face_packet_count, op_material_bucket_count;
op_face_packet op_face_packets[4096];
op_material_bucket op_material_buckets[64];
op_material_bucket *op_last_material_bucket;
unsigned char op_render_queue_tag;

static int failures;
static void check(int condition, const char *file, int line) { if (!condition) { ++failures; printf("Failed at %s:%d\n", file, line); } }
#define CHECK(test) check(!!(test), __FILE__, __LINE__)

static void plane_tests(void)
{
    int axis;
    float vertex[3] = {10.0f, 20.0f, 30.0f};
    float point[3], normal[3], result;
    for (axis = 0; axis < 3; ++axis) {
        point[0] = 1.0f; point[1] = 2.0f; point[2] = 3.0f;
        normal[0] = 2.0f; normal[1] = 2.0f; normal[2] = 2.0f;
        result = op_plane_coordinate(axis, vertex, normal, point);
        CHECK(result == 55.0f + (float)axis);
        CHECK(point[axis] == result);
        point[axis] = -17.0f;
        normal[axis] = 0.0f;
        CHECK(op_plane_coordinate(axis, vertex, normal, point) == vertex[axis]);
        CHECK(point[axis] == -17.0f);
        normal[axis] = -0.0f;
        CHECK(op_plane_coordinate(axis, vertex, normal, point) == vertex[axis]);
        CHECK(point[axis] == -17.0f);
        normal[axis] = 1.0f;
        CHECK(op_plane_coordinate(axis, vertex, normal, point) == vertex[axis]);
        CHECK(point[axis] == -17.0f);
    }
    point[0] = 1.0f; point[1] = 2.0f; point[2] = 3.0f;
    normal[0] = -2.0f; normal[1] = 2.0f; normal[2] = 2.0f;
    CHECK(op_plane_coordinate(0, vertex, normal, point) == -35.0f);
#ifdef OP_VC5_BEHAVIOR
    {
        union { unsigned int u; float f; } special;
        unsigned int before = _controlfp(0, 0);
        _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
        printf("x87 control word for nonfinite tests: %x\n", _controlfp(0, 0));
        for (axis = 0; axis < 3; ++axis) {
            special.u = 0x7fc00000u;
            normal[axis] = special.f;
            point[axis] = -17.0f;
            CHECK(op_plane_coordinate(axis, vertex, normal, point) == vertex[axis]);
            CHECK(point[axis] == -17.0f);
        }
        normal[0] = 2.0f; normal[1] = 2.0f;
        point[0] = 1.0f; point[1] = 2.0f;
        special.u = 0x7f800000u;
        normal[2] = special.f;
        CHECK(op_plane_coordinate(2, vertex, normal, point) == vertex[2]);
        _controlfp(before, _MCW_EM | _MCW_PC | _MCW_RC);
    }
#endif
}

static void clip_flag_tests(void)
{
    op_face_indices face;
    unsigned int indices[4] = {2, 0, 1, 0};
    unsigned char codes[3] = {1, 9, 128};
    CHECK(offsetof(op_face_indices, vertex_count) == 0x14);
    CHECK(offsetof(op_face_indices, vertex_indices) == 0x18);
    face.vertex_count = 0;
    face.vertex_indices = 0;
    CHECK(op_face_clip_flags(&face, 0) == 0xff00);
    face.vertex_indices = indices;
    face.vertex_count = 1;
    CHECK(op_face_clip_flags(&face, codes) == 0x8080);
    face.vertex_count = 3;
    CHECK(op_face_clip_flags(&face, codes) == 0x0089);
    face.vertex_indices = indices + 1;
    face.vertex_count = 2;
    CHECK(op_face_clip_flags(&face, codes) == 0x0109);
    codes[1] = 2;
    CHECK(op_face_clip_flags(&face, codes) == 0x0003);
    face.vertex_count = 3;
    CHECK(op_face_clip_flags(&face, codes) == 0x0003);
    codes[0] = 0;
    face.vertex_count = 1;
    CHECK(op_face_clip_flags(&face, codes) == 0);
}

#include "skills_state_behavior.c"
#include "random_table_behavior.c"
#include "gameplay_random_behavior.c"
#include "stream_lifecycle_behavior.c"
#include "projection_behavior.h"
#include "mesh_behavior.h"
#include "queue_behavior.h"
#include "gathered_behavior.h"
#include "frame_state_behavior.h"
#include "frame_prepare_behavior.h"
#include "surface_entry_behavior.h"
#include "scan_behavior.h"
#include "bgl_state_behavior.c"
#include "geometry_behavior.c"
#include "colormap_io_behavior.c"
#include "normal_behavior.c"
#include "save_stream_behavior.c"
#include "save_slots_behavior.c"
#include "module_behavior.c"
#include "module_dispatch_behavior.c"
#include "module_lifecycle_behavior.c"
#include "module_cleanup_behavior.c"
#include "conf_stream_behavior.c"
#include "utilities_behavior.c"
#include "path_index_behavior.c"
#include "path_copy_behavior.c"
#include "path_helpers_behavior.c"
#include "lexical_behavior.c"
#include "window_state_behavior.c"
#include "platform_behavior.c"
#include "trig_behavior.c"
#include "length_behavior.c"
#include "audio_behavior.c"
#include "audio_buffer_behavior.c"
#include "music_position_behavior.c"
#include "elapsed_time_behavior.c"
#include "ui_input_behavior.c"
#include "kernel_behavior.c"
#include "video_behavior.c"
#include "debug_behavior.c"
#include "directory_behavior.c"
#include "palette_resources_behavior.c"
#include "matrix_helpers_behavior.c"
#include "canvas_face_behavior.c"
#include "colormap_lifecycle_behavior.c"
#include "light_behavior.c"
#include "colormap_state_behavior.c"
#include "colormap_behavior.c"
#include "colormap_bridge_behavior.c"
#include "material_state_behavior.c"
#include "keyframe_lifecycle_behavior.c"
#include "world_readers_behavior.c"
#include "world_controls_behavior.c"
#include "world_names_behavior.c"
#include "face_keyframe_behavior.c"
#include "world_chunks_behavior.c"
#include "focused_accessors_behavior.c"
#include "font_state_behavior.c"
#include "font_measure_behavior.c"
#include "font_lifecycle_behavior.c"
#include "shield_lifecycle_behavior.c"
#include "effects_state_behavior.c"
#include "halo_overlay_behavior.c"
#include "zap_effects_behavior.c"
#include "ripple_effects_behavior.c"
#include "effects_save_behavior.c"
#include "effects_chain_behavior.c"
#include "ripple_spawn_behavior.h"
#include "projected_overlay_behavior.c"
#include "application_helpers_behavior.c"
#include "puppet_tracks_behavior.c"
#include "linked_list_behavior.c"

int main(void)
{
    op_viewport viewport;
    op_frustum frustum;
    op_camera_state camera;
    failures += op_test_gameplay_random();
    failures += op_test_gameplay_seed();
    failures += op_test_gameplay_roll();
    failures += op_test_gameplay_random_connected();
    failures += op_test_skills_new_game();
    failures += op_test_skills_easier();
    failures += op_test_skills_harder();
    failures += op_test_random_table_next();
    failures += op_test_random_table_shuffle_position();
    failures += op_test_random_table_at();
    failures += op_test_stream_open();
    failures += op_test_pack_stream_open();
    failures += op_test_stream_close();
    CHECK(sizeof(void *) == 4 && sizeof(int) == 4 && sizeof(float) == 4);
    CHECK(offsetof(op_viewport, width_bits) == 8);
    CHECK(offsetof(op_viewport, edge_18) == 0x18);
    CHECK(offsetof(op_viewport, edge_24) == 0x24);
    CHECK(offsetof(op_camera_state, viewport) == 4);
    CHECK(offsetof(op_camera_state, focal) == 0x3c);
    CHECK(offsetof(op_camera_state, frustum) == 0x48);
    CHECK(offsetof(op_frustum, near_bound) == 4 && offsetof(op_frustum, far_bound) == 8);
    CHECK(op_material_mode(1) == 0);
    CHECK(op_material_mode(0) == 2 && op_material_mode(-1) == 2);
    CHECK(op_material_mode(2) == 2 && op_material_mode(2147483647) == 2);
    CHECK(op_grid_cell(0, 0) == op_grid);
    CHECK(op_grid_cell(255, 1) == op_grid + 65408);
    plane_tests();
    clip_flag_tests();
    viewport.width_bits = 640; viewport.height_bits = 480;
    viewport.edge_18 = -5; viewport.edge_1c = 10;
    viewport.edge_20 = 635; viewport.edge_24 = 470;
    frustum.near_bound = 0.5f; frustum.far_bound = 8.0f;
    camera.viewport = &viewport; camera.focal = 320.0f; camera.frustum = &frustum;
    op_camera = &camera;
    op_update_projection();
    CHECK(op_width_bits == 640 && op_height_bits == 480);
    CHECK(op_focal == 320.0f && op_depth_scale == 0.125f);
    CHECK(op_edge_18 == -5.0f && op_edge_1c == 10.0f);
    CHECK(op_edge_20 == 635.0f && op_edge_24 == 470.0f);
    CHECK(op_near == 0.5f && op_far == 8.0f);
    projection_tests();
    mesh_context_tests();
    queue_tests();
    gathered_tests();
    frame_state_tests();
    failures += fp_main();
    surface_entry_tests();
    scan_tests();
    failures += op_test_bgl_state();
    failures += op_test_colormaps();
    failures += op_test_world_readers();
    failures += op_test_world_controls();
    failures += op_test_world_names();
    failures += op_test_world_chunks();
    failures += op_test_focused_accessors();
    failures += op_test_shield_lifecycle();
    failures += op_test_effects_state();
    failures += op_test_halo_overlay();
    failures += op_test_zap_effects();
    failures += op_test_ripple_effects();
    failures += op_test_effects_save();
    failures += op_test_effects_chain();
    failures += rs_main();
    failures += op_test_projected_overlay();
    failures += op_test_application_helpers();
    failures += op_test_puppet_tracks();
    failures += op_test_linked_list();
    failures += op_test_font_state();
    failures += op_test_font_measure();
    failures += op_test_font_lifecycle();
    failures += op_test_face_keyframe();
    failures += op_test_keyframe_lifecycle();
    failures += op_test_material_state();
    failures += op_test_colormap_state();
    failures += op_test_colormap_lifecycle();
    failures += op_test_light();
    failures += op_test_canvas();
    failures += op_test_colormap_reader();
    failures += op_test_colormap_writer();
    failures += op_test_point_plane_distance();
    failures += op_test_vector_angle();
    failures += op_test_directory_open();
    failures += op_test_directory_close();
    failures += op_test_directory_rewind();
    failures += op_test_directory_read();
    failures += op_test_directory_lifecycle();
    failures += op_test_save_stream();
    failures += op_test_save_slot_labels();
    failures += op_test_save_slot_chains();
    failures += op_test_module_initialize_once();
    failures += op_test_module_find_by_id();
    failures += op_test_module_find_by_name();
    failures += op_test_module_connected();
    failures += op_test_module_send_restore();
    failures += op_test_module_broadcast_save();
    failures += op_test_module_broadcast_event();
    failures += op_test_module_suspend_all();
    failures += op_test_module_resume_all();
    failures += op_test_module_stop_all();
    failures += op_test_module_start_all();
    failures += op_test_module_open_all();
    failures += op_test_module_close_all();
    failures += op_test_module_install();
    failures += op_test_module_shutdown_all();
    failures += op_test_module_broadcast();
    failures += op_test_path_drive();
    failures += op_test_path_first_directory_offset();
    failures += op_test_path_file_offset();
    failures += op_test_path_extension_offset();
    failures += op_test_path_separator_offset();
    failures += op_test_path_copy_extension();
    failures += op_test_path_copy_stem();
    failures += op_test_path_copy_directory();
    failures += op_test_path_copy_drive_directory();
    failures += op_test_path_directory_offset();
    failures += op_test_path_copy_nth_directory();
    failures += op_test_path_copy_filename();
    failures += op_test_conf_open_read();
    failures += op_test_conf_close_write();
    failures += op_test_conf_write_string();
    failures += op_test_conf_write_bytes();
    failures += op_test_conf_read_bytes();
    failures += op_test_conf_get_file();
    failures += op_test_conf_connected();
    failures += op_test_rect_overlap_inclusive();
    failures += op_test_rect_overlap_strict();
    failures += op_test_rect_union();
    failures += op_test_util_strdup();
    failures += op_test_util_snprintf();
    failures += op_test_util_strlwr();
    failures += op_test_util_file_exists();
    failures += op_test_utilities_connected();
    failures += op_test_util_tokenize();
    failures += op_test_util_quoted_string();
    failures += op_test_util_sort_range();
    failures += op_test_window_set_handle();
    failures += op_test_window_get_handle();
    failures += op_test_window_set_instance();
    failures += op_test_window_get_instance();
    failures += op_test_window_set_rect();
    failures += op_test_window_get_rect();
    failures += op_test_window_connected();
    failures += op_test_platform_alloc_debug();
    failures += op_test_platform_free_debug();
    failures += op_test_platform_realloc_debug();
    failures += op_test_platform_lock_identity();
    failures += op_test_platform_ticks();
    failures += op_test_platform_seconds();
    failures += op_test_sincos_degrees();
    failures += op_test_cosine_degrees();
    failures += op_test_asine_degrees();
    failures += op_test_tangent_degrees();
    failures += op_test_scalar_angle();
    failures += op_test_acos_degrees_copy2();
    failures += op_test_acos_degrees_copy3();
    failures += op_test_platform_heap_connected();
    failures += op_test_platform_timer_connected();
    failures += op_test_math_connected();
    failures += op_test_fast_length2_half();
    failures += op_test_fast_length2_quarter();
    failures += op_test_fast_length2_three_eighths();
    failures += op_test_fast_length2_best();
    failures += op_test_audio_com_initialize();
    failures += op_test_audio_com_uninitialize();
    failures += op_test_audio_set_cooperative();
    failures += op_test_audio_set_frequency();
    failures += op_test_audio3d_set_distance_bounds();
    failures += op_test_audio_commit_listener();
    failures += op_test_audio3d_set_mode();
    failures += op_test_audio_stop();
    failures += op_test_audio_release_buffer();
    failures += op_test_audio3d_release();
    failures += op_test_length_connected();
    failures += op_test_audio_connected();
    failures += op_test_audio_pause();
    failures += op_test_audio_play();
    failures += op_test_audio_unlock();
    failures += op_test_music_stop();
    failures += op_test_music_is_playing();
    failures += op_test_audio_get_play_position();
    failures += op_test_elapsed_ticks();
    failures += op_test_elapsed_seconds();
    failures += op_test_delta_ticks();
    failures += op_test_signed_ms_to_seconds();
    failures += op_test_ui_get_cursor();
    failures += op_test_ui_navigation_code_pointer();
    failures += op_test_ui_set_joy_navigation();
    failures += op_test_ui_clear_joy_latches();
    failures += op_test_elapsed_time_connected();
    failures += op_test_kernel_show();
    failures += op_test_kernel_hide();
    failures += op_test_kernel_set_field();
    failures += op_test_kernel_set_callback();
    failures += op_test_kernel_get_window();
    failures += op_test_kernel_add_hook();
    failures += op_test_kernel_remove_hook();
    failures += op_test_kernel_add_dialog();
    failures += op_test_kernel_remove_dialog();
    failures += op_test_kernel_connected();
    video_route=1; failures += op_test_video_open();
    video_route=2; failures += op_test_video_close();
    video_route=3; failures += op_test_video_set_volume();
    video_route=0;
    failures += op_test_video_connected();
    debug_route=1; failures += op_test_debug_toggle_visible();
    debug_route=2; failures += op_test_debug_next_page();
    debug_route=3; failures += op_test_debug_toggle_size();
    debug_route=0;
    failures += op_test_debug_connected();
    failures += op_test_normal_from_points();
    failures += op_test_palette_lifecycle();
    failures += op_test_palette_name_get();
    failures += op_test_palette_installation();
    failures += op_test_colormap_bridge();
    failures += op_test_matrix_helpers();
    failures += op_test_matrix_translation();
    failures += op_test_face_constructor();
    if (failures) return 1;
#ifdef OP_VC5_BEHAVIOR
    puts("VC5 behavioral fixture passed, including x87 unordered fallback.");
#else
    puts("Modern x86 behavior smoke passed; VC5 codegen and plane-solver unordered behavior NOT verified.");
#endif
    return 0;
}
