/* Frame setup integration: matched projection plus explicit backend stubs. */
op_world_fog *op_active_world;
unsigned int op_frame_render_flags, op_frame_extra_render_flags;
int op_frame_option_89fde4, op_frame_option_89fdec, op_frame_option_59da00;
int op_frame_mode, op_vertex_fog_enabled;
float op_fog_start, op_fog_end, op_fog_range;
float op_fog_start_reciprocal, op_fog_end_reciprocal;
static op_world_fog frame_world;
static op_camera_state frame_camera;
static op_viewport frame_viewport;
static op_frustum frame_frustum;
static unsigned int frame_backend_flags;
static int frame_call_sequence, frame_capability;

void op_backend_begin_frame(void)
{
    CHECK(frame_call_sequence == 0);
    ++frame_call_sequence;
    CHECK(op_face_packet_count == 0 && op_projected_vertex_count == 0);
    CHECK(op_width_bits == 1234 && op_height_bits == 5678);
    CHECK(op_focal == 4 && op_far == 16 && op_near == 1);
    CHECK(op_depth_scale == 0.0625f);
    CHECK(op_frame_render_flags == 0x2468ace0u);
    CHECK(op_frame_extra_render_flags == 0xa1b2c3d4u);
}

unsigned int op_backend_render_flags(void)
{
    CHECK(frame_call_sequence == 1);
    ++frame_call_sequence;
    CHECK(op_frame_render_flags == (op_frame_option_89fde4 ? 0x9833u : 0x8033u));
    return frame_backend_flags;
}

int op_backend_fog_capability(void)
{
    CHECK(frame_call_sequence == 2);
    ++frame_call_sequence;
    CHECK(op_vertex_fog_enabled == 0);
    CHECK(op_frame_extra_render_flags == 0);
    CHECK((op_frame_render_flags & 0x40u) != 0);
    CHECK(op_frame_option_59da00 == (op_frame_option_89fdec == 0));
    return frame_capability;
}

static void frame_prepare(void)
{
    frame_viewport.width_bits = 1234;
    frame_viewport.height_bits = 5678;
    frame_viewport.edge_18 = -1; frame_viewport.edge_1c = -2;
    frame_viewport.edge_20 = 3; frame_viewport.edge_24 = 4;
    frame_frustum.near_bound = 1; frame_frustum.far_bound = 16;
    frame_camera.viewport = &frame_viewport;
    frame_camera.frustum = &frame_frustum; frame_camera.focal = 4;
    op_camera = &frame_camera;
    op_width_bits = 0; op_height_bits = 0; op_focal = -99;
    op_face_packet_count = 27; op_projected_vertex_count = 85;
    op_frame_render_flags = 0x2468ace0u;
    op_frame_extra_render_flags = 0xa1b2c3d4u;
    op_frame_option_89fde4 = 0; op_frame_option_89fdec = 0;
    op_frame_option_59da00 = 123; op_frame_mode = 0;
    op_vertex_fog_enabled = 7;
    op_fog_start = 17; op_fog_end = 19; op_fog_range = 31;
    op_fog_start_reciprocal = 23; op_fog_end_reciprocal = 29;
    frame_world.flags = 1; frame_world.fog_start = 2; frame_world.fog_end = 8;
    op_active_world = &frame_world;
    frame_backend_flags = 0xa55affbfu;
    frame_call_sequence = 0; frame_capability = 0;
}

static void frame_unchanged_fog(void)
{
    CHECK(op_fog_start == 17 && op_fog_end == 19 && op_fog_range == 31);
    CHECK(op_fog_start_reciprocal == 23 && op_fog_end_reciprocal == 29);
}

static void frame_state_tests(void)
{
    unsigned int control = projection_control(0, 0);
    unsigned short top = projection_x87_top();
    int mode, option, i;
    int modes[6] = {-1, 0, 1, 2, 3, 4};
    unsigned int mode_bits[6] = {0, 0, 0x1000, 0x1800, 0x800, 0};
    int capabilities[3] = {1, 0x100, -1};
    CHECK(offsetof(op_world_fog, flags) == 0x210);
    CHECK(offsetof(op_world_fog, fog_start) == 0x218);
    CHECK(offsetof(op_world_fog, fog_end) == 0x21c);
    projection_control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (mode = 0; mode < 6; ++mode) {
        for (option = 0; option < 2; ++option) {
            frame_prepare();
            op_frame_mode = modes[mode];
            op_frame_option_89fde4 = option ? -1 : 0;
            op_frame_option_89fdec = option ? -1 : 0;
            op_active_world = 0;
            op_set_frame_state();
            CHECK(frame_call_sequence == 2 && op_vertex_fog_enabled == 0);
            CHECK(op_frame_render_flags == ((frame_backend_flags & ~0x1800u) | mode_bits[mode]));
            CHECK(op_frame_extra_render_flags == 0);
            CHECK(op_frame_option_59da00 == !option);
            frame_unchanged_fog();
        }
    }
    frame_prepare(); frame_world.flags = 2;
    op_set_frame_state();
    CHECK(frame_call_sequence == 2 && op_vertex_fog_enabled == 0);
    frame_unchanged_fog();
    for (i = 0; i < 3; ++i) {
        frame_prepare(); frame_capability = capabilities[i];
        op_set_frame_state();
        CHECK(frame_call_sequence == 3 && op_vertex_fog_enabled == 0);
        CHECK((op_frame_render_flags & 0x40u) != 0);
        frame_unchanged_fog();
    }
    frame_prepare(); op_set_frame_state();
    CHECK(frame_call_sequence == 3 && op_vertex_fog_enabled == 1);
    CHECK(op_fog_start == 2 && op_fog_end == 8 && op_fog_range == 6);
    CHECK(op_fog_start_reciprocal == 0.5f && op_fog_end_reciprocal == 0.125f);
    frame_prepare(); frame_world.fog_start = 8; frame_world.fog_end = 2;
    op_set_frame_state();
    CHECK(op_vertex_fog_enabled == 0 && op_fog_range == -6);
    CHECK(op_fog_start_reciprocal == 0.125f && op_fog_end_reciprocal == 0.5f);
    CHECK((op_frame_render_flags & 0x40u) != 0);
    frame_prepare(); frame_world.fog_start = 4; frame_world.fog_end = 4;
    op_set_frame_state();
    CHECK(op_vertex_fog_enabled == 1 && op_fog_range == 0);
    frame_prepare(); frame_world.fog_start = projection_float(0x80000000u); frame_world.fog_end = 0.0f;
    CHECK(projection_bits(frame_world.fog_start) == 0x80000000u);
    op_set_frame_state();
    CHECK(op_vertex_fog_enabled == 1 && op_fog_range == 0);
    CHECK(projection_bits(op_fog_start_reciprocal) == 0xff800000u);
    CHECK(projection_bits(op_fog_end_reciprocal) == 0x7f800000u);
    frame_prepare(); frame_world.fog_end = projection_float(0x7f800000u);
    op_set_frame_state();
    CHECK(op_vertex_fog_enabled == 1 && projection_bits(op_fog_range) == 0x7f800000u);
    CHECK(op_fog_end_reciprocal == 0);
    frame_prepare(); frame_world.fog_start = projection_float(0x7f800000u);
    op_set_frame_state();
    CHECK(op_vertex_fog_enabled == 0 && projection_bits(op_fog_range) == 0xff800000u);
#ifdef OP_VC5_BEHAVIOR
    /* C0-only original comparison treats unordered range as below zero. */
    frame_prepare(); frame_world.fog_start = projection_float(0x7fc12345u);
    op_set_frame_state();
    CHECK(op_vertex_fog_enabled == 0 && projection_nan(op_fog_range));
    frame_prepare(); frame_world.fog_start = projection_float(0x7f800000u);
    frame_world.fog_end = projection_float(0x7f800000u);
    op_set_frame_state();
    CHECK(op_vertex_fog_enabled == 0 && projection_nan(op_fog_range));
#endif
    CHECK(projection_x87_top() == top);
    CHECK((projection_control(0, 0) & (_MCW_EM | _MCW_PC | _MCW_RC)) ==
          (_MCW_EM | _PC_53 | _RC_NEAR));
    _clearfp();
    projection_control(control, _MCW_EM | _MCW_PC | _MCW_RC);
    op_active_world = 0; op_camera = 0;
}
