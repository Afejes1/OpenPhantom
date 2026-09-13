static int fp_active;
static int fp_draw_surface_buckets(void *, const void *, unsigned int);
/* Interaction tests for the wrapper, not a reconstructed surface renderer. */
unsigned int op_gathered_surface_count;
void *op_gathered_world;
int op_last_submitted_surface_count;
static unsigned int gathered_calls, gathered_seen_count;
static void *gathered_seen_world;
static const void *gathered_seen_camera;
static int gathered_return;

int op_draw_surface_buckets(void *world, const void *camera_data, unsigned int count)
{
    if (fp_active)
        return fp_draw_surface_buckets(world, camera_data, count);
    ++gathered_calls;
    gathered_seen_world = world;
    gathered_seen_camera = camera_data;
    gathered_seen_count = count;
    return gathered_return;
}

static void gathered_tests(void)
{
    op_camera_state camera;
    unsigned int counts[3] = {1u, 7u, 0xffffffffu};
    int results[3] = {3, 0, -1};
    int world, i;
    gathered_calls = 0;
    op_gathered_surface_count = 0;
    op_gathered_world = 0;
    op_camera = 0;
    op_last_submitted_surface_count = 123;
    op_submit_gathered_surfaces();
    CHECK(gathered_calls == 0);
    CHECK(op_last_submitted_surface_count == 123);
    op_camera = &camera;
    for (i = 0; i < 3; ++i) {
        op_gathered_world = i == 1 ? 0 : &world;
        op_gathered_surface_count = counts[i];
        gathered_return = results[i];
        op_submit_gathered_surfaces();
        CHECK(gathered_calls == (unsigned int)i + 1);
        CHECK(gathered_seen_world == op_gathered_world);
        CHECK(gathered_seen_camera == (unsigned char *)&camera + 8);
        CHECK(gathered_seen_count == counts[i]);
        CHECK(op_last_submitted_surface_count == results[i]);
        CHECK(op_gathered_surface_count == counts[i]);
    }
    op_gathered_surface_count = 0;
    op_submit_gathered_surfaces();
    CHECK(gathered_calls == 3);
    CHECK(op_last_submitted_surface_count == -1);
    op_camera = 0;
    op_gathered_world = 0;
}
