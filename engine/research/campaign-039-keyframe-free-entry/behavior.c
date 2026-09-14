#include "api.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
typedef struct OWNED_FRAME
{
    unsigned int before;
    OP_KEYFRAME value;
    unsigned int after;
} OWNED_FRAME;
static OWNED_FRAME frames[3], expected[3];
static int stage;
static void seed(void)
{
    memset(frames, 0x69, sizeof(frames));
    memcpy(expected, frames, sizeof(frames));
    stage = 0;
}
static void verify(void)
{
    CHECK(memcmp(frames, expected, sizeof(frames)) == 0);
}
typedef struct OWNED_NODES
{
    unsigned int before;
    OP_KEYFRAME_NODE rows[4];
    unsigned int after;
} OWNED_NODES;
static OWNED_NODES nodes[3], expected_nodes[3];
static unsigned int tokens[10];
static void *plan[5];
static int plan_node[5], plan_count, profile;
static void verify_all(void)
{
    verify();
    CHECK(memcmp(nodes, expected_nodes, sizeof(nodes)) == 0);
}
void op_release(void *p)
{
    int index;
    CHECK(stage < plan_count && p == plan[stage]);
    verify_all();
    index = plan_node[stage];
    if (index >= 0)
    {
        if (stage == 0)
        {
            if (profile == 1)
                frames[0].value.node_count = expected[0].value.node_count = 0;
            if (profile == 2)
                frames[0].value.node_count = expected[0].value.node_count = 4;
            if (profile == 3)
                frames[0].value.nodes = expected[0].value.nodes = nodes[1].rows;
            if (profile == 4)
            {
                nodes[0].rows[0].payload = &tokens[8];
                nodes[0].rows[1].payload = expected_nodes[0].rows[1].payload = &tokens[9];
            }
            if (profile == 5)
                nodes[0].rows[1].payload = expected_nodes[0].rows[1].payload = 0;
        }
        expected_nodes[0].rows[index].payload = 0;
    }
    else
    {
        frames[0].value.nodes = nodes[2].rows;
        expected[0].value.nodes = 0;
    }
    ++stage;
}
static void prepare(int count, int mask, int has_array)
{
    int i;
    seed();
    memset(nodes, 0x57, sizeof(nodes));
    for (i = 0; i < 4; ++i)
    {
        nodes[0].rows[i].payload = (mask & (1 << i)) ? &tokens[i] : 0;
        nodes[1].rows[i].payload = &tokens[i + 4];
        nodes[2].rows[i].payload = &tokens[i + 5];
    }
    memcpy(expected_nodes, nodes, sizeof(nodes));
    frames[0].value.node_count = (unsigned int)count;
    frames[0].value.nodes = has_array ? nodes[0].rows : 0;
    memcpy(expected, frames, sizeof(frames));
    plan_count = 0;
}
static void add(int index, void *p)
{
    plan_node[plan_count] = index;
    plan[plan_count++] = p;
}
static void run(void)
{
    op_keyframe_free_entry(&frames[0].value);
    CHECK(stage == plan_count);
    verify_all();
}
int main(void)
{
    int count, mask, i;
    profile = 0;
    for (count = 0; count <= 4; ++count)
        for (mask = 0; mask < 16; ++mask)
        {
            prepare(count, mask, 1);
            for (i = 0; i < count; ++i)
                if (mask & (1 << i))
                    add(i, &tokens[i]);
            add(-1, nodes[0].rows);
            run();
        }
    for (count = 0; count <= 4; ++count)
    {
        prepare(count, 15, 0);
        run();
    }
    for (profile = 1; profile <= 5; ++profile)
    {
        prepare(profile == 2 ? 1 : 3, 15, 1);
        add(0, &tokens[0]);
        if (profile != 1)
        {
            if (profile != 5)
                add(1, profile == 4 ? &tokens[9] : &tokens[1]);
            add(2, &tokens[2]);
            if (profile == 2)
                add(3, &tokens[3]);
        }
        add(-1, profile == 3 ? nodes[1].rows : nodes[0].rows);
        run();
    }
    printf("keyframe_free_entry: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
