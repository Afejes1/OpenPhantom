/* Fog values are synthetic. The original reference is never executed. */
static void fog_tests(void)
{
    unsigned int before_control = projection_control(0, 0);
    unsigned short before_top = projection_x87_top();
    unsigned int controlled;
    int i;
    static const float reciprocals[8] = {1.0f, 0.5f, 0.25f, 0.2f, 0.125f, 0.0625f, 0.0f, -0.25f};
    static const unsigned int expected[8] = {
        0xff000000u, 0xff000000u, 0xaa000000u, 0x80000000u, 0, 0, 0, 0
    };
    projection_control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    controlled = projection_control(0, 0);
    op_fog_start = 2.0f;
    op_fog_range = 6.0f;
    op_fog_start_reciprocal = 0.5f;
    op_fog_end_reciprocal = 0.125f;
    op_projected_vertex_count = 2;
    op_vertex_fog_enabled = 0;
    for (i = 0; i < 12; ++i)
        op_vertex_fog_alpha[i] = 0x12345678u;
    CHECK(op_commit_projected_vertices(3) == 1);
    CHECK(op_projected_vertex_count == 5);
    for (i = 0; i < 12; ++i)
        CHECK(op_vertex_fog_alpha[i] == 0x12345678u);
    op_projected_vertex_count = 0xffffffffu;
    CHECK(op_commit_projected_vertices(2) == 1);
    CHECK(op_projected_vertex_count == 1);

    op_vertex_fog_enabled = 1;
    CHECK(op_commit_projected_vertices(0) == 1);
    CHECK(op_projected_vertex_count == 1);
    CHECK(op_commit_projected_vertices(0xffffffffu) == 1);
    CHECK(op_projected_vertex_count == 0); /* Wrapped upper bound skips writes. */
    for (i = 0; i < 12; ++i)
        CHECK(op_vertex_fog_alpha[i] == 0x12345678u);

    op_projected_vertex_count = 2;
    for (i = 0; i < 8; ++i)
        op_projected_vertices[i + 2].reciprocal_depth = reciprocals[i];
    CHECK(op_commit_projected_vertices(8) == 1);
    CHECK(op_projected_vertex_count == 10);
    CHECK(op_vertex_fog_alpha[1] == 0x12345678u && op_vertex_fog_alpha[10] == 0x12345678u);
    for (i = 0; i < 8; ++i)
        CHECK(op_vertex_fog_alpha[i + 2] == expected[i]);

    /* No clamp: deliberately inconsistent fog settings retain low-byte wrap. */
    op_projected_vertex_count = 0;
    op_projected_vertices[0].reciprocal_depth = 0.25f;
    op_fog_range = 1.0f;
    CHECK(op_commit_projected_vertices(1) == 1);
    CHECK(op_vertex_fog_alpha[0] == 0x01000000u);
    op_fog_range = 6.0f;

    op_projected_vertex_count = 0;
    op_projected_vertices[0].reciprocal_depth = projection_float(0x80000000u);
    op_projected_vertices[1].reciprocal_depth = projection_float(0x7f800000u);
    op_projected_vertices[2].reciprocal_depth = projection_float(0xff800000u);
    CHECK(op_commit_projected_vertices(3) == 1);
    CHECK(op_vertex_fog_alpha[0] == 0 && op_vertex_fog_alpha[1] == 0xff000000u);
    CHECK(op_vertex_fog_alpha[2] == 0);
#ifdef OP_VC5_BEHAVIOR
    /* VC5 C0-only comparisons differ from ordered modern C comparisons. */
    op_projected_vertex_count = 0;
    op_projected_vertices[0].reciprocal_depth = projection_float(0x7fc00000u);
    CHECK(op_commit_projected_vertices(1) == 1);
    CHECK(op_vertex_fog_alpha[0] == 0);
#endif
    CHECK(projection_x87_top() == before_top);
    CHECK(projection_control(0, 0) == controlled);
    _clearfp();
    projection_control(before_control, _MCW_EM | _MCW_PC | _MCW_RC);
    op_vertex_fog_enabled = 0;
    op_projected_vertex_count = 0;
}
