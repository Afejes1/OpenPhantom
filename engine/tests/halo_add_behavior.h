#include "../src/halo_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ha_checks, ha_failures;
static void ha_check(int condition, int line)
{
    ha_checks++;
    if (!condition)
    {
        ha_failures++;
        printf("line %d failed\n", line);
    }
}
#define ha_CHECK(x) ha_check(!!(x), __LINE__)
static OP_HALO ha_expected[32];
static OP_ATTACHED_ACTOR ha_actors[3], ha_expected_actors[3];
static char ha_name[] = "unit-halo";
static int ha_objects[3], ha_selected, ha_node, ha_acquired, ha_mutation, ha_stage;
static unsigned int ha_key;
static int ha_expected_count;
int ha_op_find_node_ordinal(OP_ATTACHED_ACTOR *actor, unsigned int node_key)
{
    ha_CHECK(ha_stage++ == 0);
    ha_CHECK(actor == &ha_actors[0]);
    ha_CHECK(node_key == ha_key);
    if (ha_mutation && ha_selected < 32)
    {
        op_halos[ha_selected].field8 ^= 0x55;
        ha_expected[ha_selected].field8 ^= 0x55;
    }
    return ha_node;
}
void *ha_op_acquire_sprite(char *resource_name)
{
    ha_CHECK(ha_stage++ == 1);
    ha_CHECK(resource_name == ha_name);
    ha_CHECK(ha_selected < 32);
    if (ha_mutation)
    {
        op_halos[ha_selected].owner = &ha_actors[2];
        ha_expected[ha_selected].owner = &ha_actors[2];
        op_halos[(ha_selected + 1) % 32].mode ^= 0x77;
        ha_expected[(ha_selected + 1) % 32].mode ^= 0x77;
        op_halo_count += 7;
        ha_expected_count += 7;
        ha_actors[0].flags ^= 0x101;
        ha_expected_actors[0].flags ^= 0x101;
    }
    return ha_acquired ? &ha_objects[1] : 0;
}
int ha_main(void)
{
    int i, n, nulls, nodes[5] = {INT_MIN, -1, 0, 1, INT_MAX};
    unsigned int colors[3] = {0, 0xffffffff, 0x12345678};
    for (ha_selected = 0; ha_selected <= 32; ha_selected++)
        for (n = 0; n < 5; n++)
            for (ha_acquired = 0; ha_acquired < 2; ha_acquired++)
                for (ha_mutation = 0; ha_mutation < 2; ha_mutation++)
                {
                    memset(ha_actors, 0, sizeof(ha_actors));
                    ha_actors[0].flags = 0x80000001;
                    for (i = 0; i < 32; i++)
                    {
                        memset(&op_halos[i], 0, sizeof(OP_HALO));
                        op_halos[i].owner = &ha_actors[1];
                        op_halos[i].sprite = &ha_objects[i % 3];
                        op_halos[i].field8 = i + 100;
                        op_halos[i].mode = -i;
                        op_halos[i].color = colors[i % 3];
                    }
                    if (ha_selected < 32)
                        op_halos[ha_selected].owner = 0;
                    memcpy(ha_expected, op_halos, sizeof(ha_expected));
                    memcpy(ha_expected_actors, ha_actors, sizeof(ha_actors));
                    op_halo_count = ha_expected_count = -19;
                    ha_stage = 0;
                    ha_node = nodes[n];
                    ha_key = 0x80000000u + (unsigned int)ha_selected;
                    op_halo_add(&ha_actors[0], ha_key, INT_MIN, INT_MAX, ha_name, colors[n % 3], -7);
                    if (ha_node > 0 && ha_selected < 32)
                    {
                        ha_expected[ha_selected].sprite = ha_acquired ? &ha_objects[1] : 0;
                        if (ha_acquired)
                        {
                            ha_expected[ha_selected].owner = &ha_actors[0];
                            ha_expected[ha_selected].node = ha_node;
                            ha_expected[ha_selected].field8 = INT_MIN;
                            ha_expected[ha_selected].fieldc = INT_MAX;
                            ha_expected[ha_selected].mode = -7;
                            ha_expected[ha_selected].color = colors[n % 3];
                            ha_expected_count++;
                            ha_expected_actors[0].flags |= 0x10;
                        }
                    }
                    ha_CHECK(ha_stage == (ha_node > 0 && ha_selected < 32 ? 2 : 1));
                    ha_CHECK(op_halo_count == ha_expected_count);
                    ha_CHECK(memcmp(op_halos, ha_expected, sizeof(ha_expected)) == 0);
                    ha_CHECK(memcmp(ha_actors, ha_expected_actors, sizeof(ha_actors)) == 0);
                }
    for (nulls = 0; nulls < 3; nulls++)
    {
        memcpy(ha_expected, op_halos, sizeof(ha_expected));
        memcpy(ha_expected_actors, ha_actors, sizeof(ha_actors));
        ha_expected_count = op_halo_count;
        ha_stage = 0;
        op_halo_add(nulls == 0 ? &ha_actors[0] : 0, 1, 0, 0, nulls == 1 ? ha_name : 0, 0, 0);
        ha_CHECK(ha_stage == 0);
        ha_CHECK(op_halo_count == ha_expected_count);
        ha_CHECK(memcmp(ha_actors, ha_expected_actors, sizeof(ha_actors)) == 0);
        ha_CHECK(memcmp(op_halos, ha_expected, sizeof(ha_expected)) == 0);
    }
    printf("halo add: %d checks, %d failures\n", ha_checks, ha_failures);
    return ha_failures != 0;
}

#undef ha_CHECK
