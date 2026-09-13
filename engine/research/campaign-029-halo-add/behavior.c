#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int condition, int line)
{
    checks++;
    if (!condition)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
OP_HALO op_halos[32];
int op_halo_count;
static OP_HALO expected[32];
static OP_ACTOR actors[3], expected_actors[3];
static char name[] = "unit-halo";
static int objects[3], selected, node, acquired, mutation, stage;
static unsigned int key;
static int expected_count;
int op_find_node_ordinal(OP_ACTOR *actor, unsigned int node_key)
{
    CHECK(stage++ == 0);
    CHECK(actor == &actors[0]);
    CHECK(node_key == key);
    if (mutation && selected < 32)
    {
        op_halos[selected].field8 ^= 0x55;
        expected[selected].field8 ^= 0x55;
    }
    return node;
}
void *op_acquire_sprite(char *resource_name)
{
    CHECK(stage++ == 1);
    CHECK(resource_name == name);
    CHECK(selected < 32);
    if (mutation)
    {
        op_halos[selected].owner = &actors[2];
        expected[selected].owner = &actors[2];
        op_halos[(selected + 1) % 32].mode ^= 0x77;
        expected[(selected + 1) % 32].mode ^= 0x77;
        op_halo_count += 7;
        expected_count += 7;
        actors[0].flags ^= 0x101;
        expected_actors[0].flags ^= 0x101;
    }
    return acquired ? &objects[1] : 0;
}
int main(void)
{
    int i, n, nulls, nodes[5] = {INT_MIN, -1, 0, 1, INT_MAX};
    unsigned int colors[3] = {0, 0xffffffff, 0x12345678};
    for (selected = 0; selected <= 32; selected++)
        for (n = 0; n < 5; n++)
            for (acquired = 0; acquired < 2; acquired++)
                for (mutation = 0; mutation < 2; mutation++)
                {
                    memset(actors, 0, sizeof(actors));
                    actors[0].flags = 0x80000001;
                    for (i = 0; i < 32; i++)
                    {
                        memset(&op_halos[i], 0, sizeof(OP_HALO));
                        op_halos[i].owner = &actors[1];
                        op_halos[i].sprite = &objects[i % 3];
                        op_halos[i].field8 = i + 100;
                        op_halos[i].mode = -i;
                        op_halos[i].color = colors[i % 3];
                    }
                    if (selected < 32)
                        op_halos[selected].owner = 0;
                    memcpy(expected, op_halos, sizeof(expected));
                    memcpy(expected_actors, actors, sizeof(actors));
                    op_halo_count = expected_count = -19;
                    stage = 0;
                    node = nodes[n];
                    key = 0x80000000u + (unsigned int)selected;
                    op_halo_add(&actors[0], key, INT_MIN, INT_MAX, name, colors[n % 3], -7);
                    if (node > 0 && selected < 32)
                    {
                        expected[selected].sprite = acquired ? &objects[1] : 0;
                        if (acquired)
                        {
                            expected[selected].owner = &actors[0];
                            expected[selected].node = node;
                            expected[selected].field8 = INT_MIN;
                            expected[selected].fieldc = INT_MAX;
                            expected[selected].mode = -7;
                            expected[selected].color = colors[n % 3];
                            expected_count++;
                            expected_actors[0].flags |= 0x10;
                        }
                    }
                    CHECK(stage == (node > 0 && selected < 32 ? 2 : 1));
                    CHECK(op_halo_count == expected_count);
                    CHECK(memcmp(op_halos, expected, sizeof(expected)) == 0);
                    CHECK(memcmp(actors, expected_actors, sizeof(actors)) == 0);
                }
    for (nulls = 0; nulls < 3; nulls++)
    {
        memcpy(expected, op_halos, sizeof(expected));
        memcpy(expected_actors, actors, sizeof(actors));
        expected_count = op_halo_count;
        stage = 0;
        op_halo_add(nulls == 0 ? &actors[0] : 0, 1, 0, 0, nulls == 1 ? name : 0, 0, 0);
        CHECK(stage == 0);
        CHECK(op_halo_count == expected_count);
        CHECK(memcmp(actors, expected_actors, sizeof(actors)) == 0);
        CHECK(memcmp(op_halos, expected, sizeof(expected)) == 0);
    }
    printf("halo add: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
