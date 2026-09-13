/* Authored connected frame preparation and real final surface submission. */
#include "../src/baseline.h"
#include "../src/b3d_chunk_readers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int fp_checks, fp_failures;
static void fp_check(int ok, int line)
{
    ++fp_checks;
    if (!ok)
    {
        ++fp_failures;
        printf("frame prepare failed at %d\n", line);
    }
}
#define FP_CHECK(x) fp_check(!!(x), __LINE__)
typedef struct FP_WORLD
{
    unsigned int before;
    OP_B3D_WORLD value;
    unsigned int after;
} FP_WORLD;
typedef struct FP_CAMERA
{
    unsigned int before;
    op_camera_state value;
    unsigned int after;
} FP_CAMERA;
static FP_WORLD fp_worlds[5], fp_expected_worlds[5];
static FP_CAMERA fp_cameras[3], fp_expected_cameras[3];
static OP_B3D_WORLD *fp_expected_active, *fp_first;
static op_camera_state *fp_expected_camera;
static void *fp_expected_gathered;
static unsigned int fp_expected_count, fp_final_count;
static int fp_expected_last, fp_stage, fp_rotation, fp_mutation, fp_return;
static void fp_snapshots(void)
{
    FP_CHECK(memcmp(fp_worlds, fp_expected_worlds, sizeof(fp_worlds)) == 0);
    FP_CHECK(memcmp(fp_cameras, fp_expected_cameras, sizeof(fp_cameras)) == 0);
    FP_CHECK(op_active_world == fp_expected_active);
    FP_CHECK(op_camera == fp_expected_camera);
    FP_CHECK(op_gathered_world == fp_expected_gathered);
    FP_CHECK(op_gathered_surface_count == fp_expected_count);
    FP_CHECK(op_last_submitted_surface_count == fp_expected_last);
}
void op_prepare_world(op_world_fog *world)
{
    FP_CHECK(fp_active && fp_stage++ == 0);
    FP_CHECK(world == fp_first);
    fp_snapshots();
    if (fp_mutation)
    {
        op_active_world = fp_expected_active = &fp_worlds[(fp_rotation + 1) % 5].value;
        fp_worlds[0].value.before_clock[0] = fp_expected_worlds[0].value.before_clock[0] = 10;
    }
}
void op_traverse_world(op_world_fog *world)
{
    FP_CHECK(fp_active && fp_stage++ == 1);
    FP_CHECK(world == (fp_mutation ? &fp_worlds[(fp_rotation + 1) % 5].value : fp_first));
    fp_snapshots();
    if (fp_mutation)
    {
        op_active_world = fp_expected_active = &fp_worlds[(fp_rotation + 2) % 5].value;
        op_camera = fp_expected_camera = &fp_cameras[1].value;
    }
}
void op_prepare_world_surfaces(op_world_fog *world, void *camera_data, int zero)
{
    FP_CHECK(fp_active && fp_stage++ == 2);
    FP_CHECK(world == (fp_mutation ? &fp_worlds[(fp_rotation + 2) % 5].value : fp_first));
    FP_CHECK(camera_data == (unsigned char *)&fp_cameras[fp_mutation ? 1 : 0].value + 8);
    FP_CHECK(zero == 0);
    fp_snapshots();
    op_gathered_world = fp_expected_gathered = world;
    op_gathered_surface_count = fp_expected_count = fp_mutation ? (fp_final_count ? 0 : 1) : fp_final_count;
    if (fp_mutation)
    {
        op_active_world = fp_expected_active = &fp_worlds[(fp_rotation + 3) % 5].value;
        op_camera = fp_expected_camera = &fp_cameras[2].value;
        fp_cameras[1].value.opaque_08[0] = fp_expected_cameras[1].value.opaque_08[0] = 21;
    }
}
void op_prepare_graphics_submission(void)
{
    FP_CHECK(fp_active && fp_stage++ == 3);
    fp_snapshots();
    if (fp_mutation)
    {
        op_active_world = fp_expected_active = &fp_worlds[(fp_rotation + 4) % 5].value;
        op_gathered_world = fp_expected_gathered = &fp_worlds[fp_rotation].value;
        op_gathered_surface_count = fp_expected_count = fp_final_count;
    }
}
static int fp_draw_surface_buckets(void *world, const void *camera_data, unsigned int count)
{
    FP_CHECK(fp_active && fp_stage++ == 4);
    FP_CHECK(world == fp_expected_gathered);
    FP_CHECK(camera_data == (unsigned char *)&fp_cameras[fp_mutation ? 2 : 0].value + 8);
    FP_CHECK(count == fp_final_count && count != 0);
    fp_snapshots();
    if (fp_mutation)
    {
        op_gathered_surface_count = fp_expected_count = fp_final_count ^ 1U;
        op_gathered_world = fp_expected_gathered = &fp_worlds[3].value;
        op_camera = fp_expected_camera = &fp_cameras[0].value;
        fp_worlds[4].value.before_clock[63] = fp_expected_worlds[4].value.before_clock[63] = 33;
        op_last_submitted_surface_count = -18;
    }
    /* Actual submit must overwrite any backend mutation with its returned int. */
    fp_expected_last = fp_return;
    return fp_return;
}
static int fp_main(void)
{
    static const unsigned int counts[4] = {0, 1, 7, 0xffffffffU};
    static const int results[3] = {0, -1, INT_MIN};
    int null_world, c, result;
    FP_CHECK(!fp_active);
    fp_active = 1;
    for (fp_rotation = 0; fp_rotation < 5; ++fp_rotation)
        for (fp_mutation = 0; fp_mutation < 2; ++fp_mutation)
            for (null_world = 0; null_world < 2; ++null_world)
                for (c = 0; c < 4; ++c)
                    for (result = 0; result < 3; ++result)
                    {
                        memset(fp_worlds, 0x55, sizeof(fp_worlds));
                        memset(fp_cameras, 0x66, sizeof(fp_cameras));
                        memcpy(fp_expected_worlds, fp_worlds, sizeof(fp_worlds));
                        memcpy(fp_expected_cameras, fp_cameras, sizeof(fp_cameras));
                        fp_first = null_world ? 0 : &fp_worlds[fp_rotation].value;
                        op_active_world = fp_expected_active = fp_first;
                        op_camera = fp_expected_camera = &fp_cameras[0].value;
                        op_gathered_world = fp_expected_gathered = &fp_worlds[4].value;
                        op_gathered_surface_count = fp_expected_count = 19;
                        op_last_submitted_surface_count = fp_expected_last = 123;
                        fp_final_count = counts[c];
                        fp_return = results[result];
                        fp_stage = 0;
                        op_frame_prepare();
                        FP_CHECK(fp_stage == (fp_final_count ? 5 : 4));
                        fp_snapshots();
                    }
    fp_active = 0;
    op_active_world = 0;
    op_camera = 0;
    op_gathered_world = 0;
    op_gathered_surface_count = 0;
    printf("frame prepare connected: %d checks, %d failures\n", fp_checks, fp_failures);
    return fp_failures != 0;
}
#undef FP_CHECK
