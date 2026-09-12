/* Synthetic mesh-context setup cases; no original object or asset data. */
static void mesh_context_tests(void)
{
    op_render_thing thing;
    static const float opacity[5] = {0.0f, 0.5f, 1.0f, 2.0f, -1.0f};
    static const float scaled[5] = {0.0f, 127.5f, 255.0f, 510.0f, -255.0f};
    int i;
    CHECK(offsetof(op_render_thing, render_opacity) == 0x150);
    op_set_mesh_render_thing(0);
    CHECK(op_active_render_thing == 0 && op_mesh_opacity == 255.0f);
    for (i = 0; i < 5; ++i) {
        thing.render_opacity = opacity[i];
        op_set_mesh_render_thing(&thing);
        CHECK(op_active_render_thing == &thing);
        CHECK(op_mesh_opacity == scaled[i]);
    }
    thing.render_opacity = projection_float(0x80000000u);
    op_set_mesh_render_thing(&thing);
    CHECK(projection_bits(op_mesh_opacity) == 0x80000000u);
    thing.render_opacity = projection_float(0x7f800000u);
    op_set_mesh_render_thing(&thing);
    CHECK(projection_bits(op_mesh_opacity) == 0x7f800000u);
    thing.render_opacity = projection_float(0x7fc00000u);
    op_set_mesh_render_thing(&thing);
    CHECK(projection_nan(op_mesh_opacity));
    op_set_mesh_render_thing(0);
    CHECK(op_active_render_thing == 0 && op_mesh_opacity == 255.0f);
}
