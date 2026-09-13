#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static OP_VIEW_WORLD world;
static OP_VIEW_VERTEX vertices[65536];
static OP_VIEW_VERTEX expected[65536];
static int checks;
static int failures;

static void check(int condition, const char *message)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", message);
    }
}

static void reset_fixture(void)
{
    unsigned int i;

    memset(&world, 0x4c, sizeof(world));
    memset(vertices, 0x6d, sizeof(vertices));
    for (i = 0; i < 65536U; ++i)
        vertices[i].visited = (unsigned char)(1U + (i % 251U));
    world.vertices = vertices;
}

static void run_range(unsigned short first, unsigned short last)
{
    OP_VIEW_WORLD before_world;
    unsigned int i;
    unsigned int low = first;
    unsigned int high = last;

    reset_fixture();
    memcpy(expected, vertices, sizeof(vertices));
    memcpy(&before_world, &world, sizeof(world));
    if (low <= high) {
        for (i = low; i <= high; ++i)
            expected[i].visited = 0;
    }
    op_view_clear_visits(&world, first, last);
    check(memcmp(expected, vertices, sizeof(vertices)) == 0,
          "complete vertex array oracle");
    check(memcmp(&before_world, &world, sizeof(world)) == 0,
          "complete world record preserved");
}

int main(void)
{
    OP_VIEW_WORLD before_world;

    check(sizeof(OP_VIEW_VERTEX) == 32, "vertex size");
    check(offsetof(OP_VIEW_VERTEX, visited) == 0x0d, "visited offset");
    check(offsetof(OP_VIEW_WORLD, vertices) == 0x9c, "vertices offset");

    reset_fixture();
    memcpy(&before_world, &world, sizeof(world));
    memcpy(expected, vertices, sizeof(vertices));
    op_view_clear_visits(0, 0, 0);
    check(memcmp(&before_world, &world, sizeof(world)) == 0,
          "null world leaves world unchanged");
    check(memcmp(expected, vertices, sizeof(vertices)) == 0,
          "null world leaves vertices unchanged");

    reset_fixture();
    world.vertices = 0;
    memcpy(&before_world, &world, sizeof(world));
    memcpy(expected, vertices, sizeof(vertices));
    op_view_clear_visits(&world, 0, 0);
    check(memcmp(&before_world, &world, sizeof(world)) == 0,
          "null vertices leaves world unchanged");
    check(memcmp(expected, vertices, sizeof(vertices)) == 0,
          "null vertices leaves storage unchanged");

    run_range(1, 0);
    run_range(0xffff, 0xfffe);
    run_range(0, 0);
    run_range(7, 7);
    run_range(11, 19);
    run_range(0x8000, 0x8002);
    run_range(0xffff, 0xffff);
    run_range(0, 0xffff);

    printf("View clear visits: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
