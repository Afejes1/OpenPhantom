/* Independently specified synthetic cases for the x87 projection contract. */
static unsigned int projection_bits(float value)
{
    union { float f; unsigned int u; } bits;
    bits.f = value;
    return bits.u;
}

static float projection_float(unsigned int value)
{
    union { float f; unsigned int u; } bits;
    bits.u = value;
    return bits.f;
}

static int projection_nan(float value)
{
    unsigned int bits = projection_bits(value);
    return (bits & 0x7f800000u) == 0x7f800000u && (bits & 0x007fffffu) != 0;
}

static unsigned short projection_x87_top(void)
{
    unsigned short status;
    __asm fnstsw status
    return (unsigned short)(status & 0x3800);
}

static void projection_identity(float *matrix)
{
    int i;
    for (i = 0; i < 12; ++i) matrix[i] = 0.0f;
    matrix[0] = matrix[4] = matrix[8] = 1.0f;
}

static unsigned int projection_control(unsigned int value, unsigned int mask)
{
#if _MSC_VER >= 1400
    unsigned int state;
    CHECK(_controlfp_s(&state, value, mask) == 0);
    return state;
#else
    return _controlfp(value, mask);
#endif
}

static void projection_tests(void)
{
    float matrix[12], vertices[24], output[32];
    int indices[4] = {2, 0, 1, 2};
    unsigned char codes[8];
    unsigned int before_control = projection_control(0, 0);
    unsigned short before_top = projection_x87_top();
    int i, mode;
    static const float clip_vertices[24] = {
        -4, 2, 0, 4, 2, 0, 0, 2, 4, 0, 2, -4,
        0, 0.5f, 0, 0, -1, 0, 2, 2, 2, 0, 1000, 0
    };
    static const unsigned char expected_codes[8] = {1, 2, 8, 4, 16, 128, 0, 0};

    projection_control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    /* Zero count must return before any argument or matrix dereference. */
    op_transform_project(0, 0, 0, 0, 0, 0);
    projection_identity(matrix);
    op_width_bits = projection_bits(320.0f);
    op_height_bits = projection_bits(240.0f);
    op_focal = 8.0f; op_depth_scale = 0.5f;
    op_edge_18 = 0; op_edge_20 = 640; op_edge_1c = 0; op_edge_24 = 480;
    op_near = 1.0f; op_depth_mode = 16;
    vertices[0] = 2; vertices[1] = 4; vertices[2] = 1;
    vertices[3] = -2; vertices[4] = 2; vertices[5] = 3;

    op_transform_project(vertices, output, 0, codes, 1, matrix);
    CHECK(output[0] == 324 && output[1] == 238);
    CHECK(output[2] == 0.25f && output[3] == 0.125f && codes[0] == 0);
    op_depth_mode = 0;
    op_transform_project(vertices, output, 0, 0, 1, matrix);
    CHECK(output[3] == 0.875f);
    /* The entry guard is equality to zero; negative counts perform one slot. */
    for (mode = 0; mode < 2; ++mode) {
        output[4] = -12345;
        op_transform_project(vertices, output, 0, codes,
                             mode ? (-2147483647 - 1) : -1, matrix);
        CHECK(output[0] == 324 && output[4] == -12345);
    }

    for (i = 0; i < 16; ++i) output[i] = -12345;
    for (i = 0; i < 4; ++i) codes[i] = 0x5a;
    op_transform_project(vertices, output, indices, codes, 4, matrix);
    CHECK(output[0] == 312 && output[1] == 228 && output[2] == 0.5f);
    for (i = 4; i < 8; ++i) CHECK(output[i] == -12345);
    CHECK(codes[1] == 0x5a);
    CHECK(output[8] == 324 && output[9] == 238 && output[10] == 0.25f);
    for (i = 0; i < 4; ++i) CHECK(output[i] == output[12 + i]);
    /* All-zero selections do not read vertex/matrix data or write output. */
    indices[0] = indices[1] = 0;
    output[0] = output[4] = -12345;
    codes[0] = codes[1] = 0x5a;
    op_transform_project(0, output, indices, codes, 2, 0);
    CHECK(output[0] == -12345 && output[4] == -12345);
    CHECK(codes[0] == 0x5a && codes[1] == 0x5a);

    op_width_bits = op_height_bits = projection_bits(0.0f);
    op_focal = 1; op_edge_18 = op_edge_1c = -1;
    op_edge_20 = op_edge_24 = 1; op_depth_mode = 16;
    op_transform_project(clip_vertices, output, 0, codes, 8, matrix);
    for (i = 0; i < 8; ++i) CHECK(codes[i] == expected_codes[i]);

    /* Stored reciprocal is rounded, but its retained x87 value feeds depth. */
    vertices[0] = 1; vertices[1] = 3; vertices[2] = 0;
    op_depth_scale = 10;
    op_transform_project(vertices, output, 0, 0, 1, matrix);
    CHECK(projection_bits(output[2]) == 0x3eaaaaabu);
    CHECK(projection_bits(output[3]) == 0x40555555u);

    /* Exact transform order: (large + -large) + small retains the small term. */
    vertices[0] = 1.0e20f; vertices[1] = -1.0e20f; vertices[2] = 3;
    for (i = 0; i < 12; ++i) matrix[i] = 0;
    matrix[0] = matrix[3] = matrix[6] = 1; matrix[10] = 2;
    op_transform_project(vertices, output, 0, 0, 1, matrix);
    CHECK(output[0] == 1.5f && output[2] == 0.5f);

    /* Distinguish +0 depth from -0 using their actual bits, as the kernel does. */
    vertices[0] = vertices[1] = vertices[2] = 1;
    for (mode = 0; mode < 2; ++mode) {
        op_depth_mode = mode ? 16 : 0;
        projection_identity(matrix);
        matrix[1] = matrix[4] = matrix[7] = matrix[10] = 0;
        op_transform_project(vertices, output, 0, codes, 1, matrix);
        CHECK(projection_bits(output[2]) == 0 && output[3] == (mode ? 0.0f : 1.0f));
        CHECK(codes[0] == 128);
        matrix[1] = matrix[4] = matrix[7] = matrix[10] = projection_float(0x80000000u);
        op_transform_project(vertices, output, 0, codes, 1, matrix);
        CHECK(projection_bits(output[2]) == 0xff800000u);
        CHECK(projection_bits(output[3]) == (mode ? 0xff800000u : 0x7f800000u));
        CHECK(codes[0] == 128);
    }

    projection_identity(matrix);
    vertices[1] = projection_float(0x7fc00000u);
    op_transform_project(vertices, output, 0, codes, 1, matrix);
    CHECK(projection_nan(output[0]) && projection_nan(output[1]));
    CHECK(projection_nan(output[2]) && projection_nan(output[3]));
    CHECK(codes[0] == 0x19);
    CHECK(projection_x87_top() == before_top);
    _clearfp();
    projection_control(before_control, _MCW_EM | _MCW_PC | _MCW_RC);
}
