/* Synthetic coordinates only; no original world or camera data. */
float op_scan_step_x[3], op_scan_step_y[3];

static void scan_tests(void)
{
    static const int directions[7] = {(-2147483647 - 1), -2, -1, 0, 1, 2, 2147483647};
    static const float coefficients[7] = {0, 0, -1, 0, 1, 0, 0};
    static const float initial[3] = {10.0f, -4.0f, 0.25f};
    static const float x_basis[3] = {2.0f, 0.5f, -1.0f};
    static const float y_basis[3] = {-3.0f, 4.0f, 0.125f};
    float storage[5], expected;
    unsigned int before_control = projection_control(0, 0), active_control;
    unsigned short before_top = projection_x87_top();
    int x, y, coordinate;
    projection_control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    active_control = projection_control(0, 0);
    for (coordinate = 0; coordinate < 3; ++coordinate) {
        op_scan_step_x[coordinate] = x_basis[coordinate];
        op_scan_step_y[coordinate] = y_basis[coordinate];
    }
    /* Distinct fractional bases expose wrong signs, axes and coordinate order. */
    for (x = 0; x < 7; ++x) {
        for (y = 0; y < 7; ++y) {
            storage[0] = -12345.0f; storage[4] = 54321.0f;
            for (coordinate = 0; coordinate < 3; ++coordinate)
                storage[coordinate + 1] = initial[coordinate];
            op_step_scan(storage + 1, directions[x], directions[y]);
            for (coordinate = 0; coordinate < 3; ++coordinate) {
                expected = initial[coordinate] + coefficients[x] * x_basis[coordinate]
                           + coefficients[y] * y_basis[coordinate];
                CHECK(storage[coordinate + 1] == expected);
                CHECK(op_scan_step_x[coordinate] == x_basis[coordinate]);
                CHECK(op_scan_step_y[coordinate] == y_basis[coordinate]);
            }
            CHECK(storage[0] == -12345.0f && storage[4] == 54321.0f);
            if (coefficients[x] == 0 && coefficients[y] == 0)
                op_step_scan(0, directions[x], directions[y]);
        }
    }
    /* A float store between the axes loses the unit before cancellation.
     * Combining both axes, retaining extended precision, or swapping them fails.
     */
    for (coordinate = 0; coordinate < 3; ++coordinate) {
        storage[coordinate + 1] = 16777216.0f;
        op_scan_step_x[coordinate] = 1.0f;
        op_scan_step_y[coordinate] = -16777216.0f;
    }
    op_step_scan(storage + 1, 1, 1);
    for (coordinate = 0; coordinate < 3; ++coordinate)
        CHECK(projection_bits(storage[coordinate + 1]) == 0u);

    /* Valid full aliasing: Y must read the basis after X has updated it. */
    for (coordinate = 0; coordinate < 3; ++coordinate) {
        op_scan_step_x[coordinate] = x_basis[coordinate];
        op_scan_step_y[coordinate] = y_basis[coordinate];
    }
    op_step_scan(op_scan_step_y, 1, 1);
    CHECK(op_scan_step_y[0] == -2 && op_scan_step_y[1] == 9 && op_scan_step_y[2] == -1.75f);
    op_step_scan(op_scan_step_x, -1, 0);
    for (coordinate = 0; coordinate < 3; ++coordinate)
        CHECK(op_scan_step_x[coordinate] == 0);

    /* Ignored directions preserve signed zero and quiet-NaN payload bits. */
    storage[1] = projection_float(0x80000000u);
    storage[2] = projection_float(0x7fc12345u);
    storage[3] = projection_float(0x7f800000u);
    CHECK(projection_bits(storage[1]) == 0x80000000u);
    op_step_scan(storage + 1, 2, -2);
    CHECK(projection_bits(storage[1]) == 0x80000000u);
    CHECK(projection_bits(storage[2]) == 0x7fc12345u);
    CHECK(projection_bits(storage[3]) == 0x7f800000u);
    for (coordinate = 0; coordinate < 3; ++coordinate) {
        op_scan_step_x[coordinate] = 1.0f;
        op_scan_step_y[coordinate] = 0.0f;
    }
    op_step_scan(storage + 1, 1, 0);
    CHECK(storage[1] == 1 && projection_nan(storage[2]));
    CHECK(projection_bits(storage[3]) == 0x7f800000u);
    CHECK(projection_control(0, 0) == active_control);
    CHECK(projection_x87_top() == before_top);
    projection_control(before_control, _MCW_EM | _MCW_PC | _MCW_RC);
}
