#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int ok, int line)
{
    checks++;
    if (!ok)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
void *op_active_world;
unsigned char *op_camera_packet;
static unsigned char worlds[5][64], expected_worlds[5][64], cameras[3][64], expected_cameras[3][64];
static int stage, rotation, mutation;
static void *first;
static void snapshots(void)
{
    CHECK(memcmp(worlds, expected_worlds, sizeof(worlds)) == 0);
    CHECK(memcmp(cameras, expected_cameras, sizeof(cameras)) == 0);
}
void op_prepare_world(void *world)
{
    CHECK(stage++ == 0);
    CHECK(world == first);
    snapshots();
    if (mutation)
    {
        op_active_world = worlds[(rotation + 1) % 5];
        worlds[0][0] = expected_worlds[0][0] = 10;
    }
}
void op_traverse_world(void *world)
{
    CHECK(stage++ == 1);
    CHECK(world == (mutation ? (void *)worlds[(rotation + 1) % 5] : first));
    snapshots();
    if (mutation)
    {
        op_active_world = worlds[(rotation + 2) % 5];
        op_camera_packet = cameras[1];
    }
}
void op_prepare_world_surfaces(void *world, void *transform, int flags)
{
    CHECK(stage++ == 2);
    CHECK(world == (mutation ? (void *)worlds[(rotation + 2) % 5] : first));
    CHECK(transform == (mutation ? cameras[1] + 8 : cameras[0] + 8));
    CHECK(flags == 0);
    snapshots();
    if (mutation)
    {
        op_active_world = worlds[(rotation + 3) % 5];
        op_camera_packet = cameras[2];
        cameras[1][8] = expected_cameras[1][8] = 21;
    }
}
void op_prepare_graphics_submission(void)
{
    CHECK(stage++ == 3);
    snapshots();
    CHECK(op_active_world == (mutation ? (void *)worlds[(rotation + 3) % 5] : first));
    if (mutation)
        op_active_world = worlds[(rotation + 4) % 5];
}
void op_submit_world_surfaces(void)
{
    CHECK(stage++ == 4);
    snapshots();
    CHECK(op_active_world == (mutation ? (void *)worlds[(rotation + 4) % 5] : first));
    if (mutation)
        worlds[4][63] = expected_worlds[4][63] = 33;
}
int main(void)
{
    int null_world;
    for (rotation = 0; rotation < 5; rotation++)
        for (mutation = 0; mutation < 2; mutation++)
            for (null_world = 0; null_world < 2; null_world++)
            {
                memset(worlds, 0x55, sizeof(worlds));
                memset(cameras, 0x66, sizeof(cameras));
                memcpy(expected_worlds, worlds, sizeof(worlds));
                memcpy(expected_cameras, cameras, sizeof(cameras));
                first = null_world ? 0 : (void *)worlds[rotation];
                op_active_world = first;
                op_camera_packet = cameras[0];
                stage = 0;
                op_frame_prepare();
                CHECK(stage == 5);
                snapshots();
                CHECK(op_active_world == (mutation ? (void *)worlds[(rotation + 4) % 5] : first));
                CHECK(op_camera_packet == (mutation ? cameras[2] : cameras[0]));
            }
    printf("frame prepare: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
