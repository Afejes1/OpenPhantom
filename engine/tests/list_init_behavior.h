#include "../src/linked_list.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ls5_checks, ls5_failures;
static void ls5_check(int ok, int line)
{
    ++ls5_checks;
    if (!ok)
    {
        ++ls5_failures;
        printf("line %d failed\n", line);
    }
}
#define ls5_CHECK(x) ls5_check(!!(x), __LINE__)
typedef struct ls5_OWNED
{
    unsigned int before;
    OP_LIST_NODE value;
    unsigned char payload[12];
    unsigned int after;
} ls5_OWNED;
static ls5_OWNED ls5_nodes[6], ls5_expected[6];
static int ls5_rotation;
static OP_LIST_NODE *ls5_at(int index)
{
    return index < 0 ? 0 : &ls5_nodes[(index + ls5_rotation) % 6].value;
}
static void ls5_seed(int edges[6][2])
{
    int i;
    memset(ls5_nodes, 0x69, sizeof(ls5_nodes));
    for (i = 0; i < 6; ++i)
    {
        ls5_at(i)->prev = ls5_at(edges[i][0]);
        ls5_at(i)->next = ls5_at(edges[i][1]);
    }
    memcpy(ls5_expected, ls5_nodes, sizeof(ls5_nodes));
}
static void ls5_expect_edges(int edges[6][2])
{
    int i;
    for (i = 0; i < 6; ++i)
    {
        ls5_expected[(i + ls5_rotation) % 6].value.prev = ls5_at(edges[i][0]);
        ls5_expected[(i + ls5_rotation) % 6].value.next = ls5_at(edges[i][1]);
    }
}
int ls5_main(void)
{
    int i;
    static int input[2][6][2] = {{{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                 {{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}};
    static int output[2][6][2] = {{{-1, 1}, {-1, -1}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                  {{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}};
    static int selected[2] = {1, 3};
    for (ls5_rotation = 0; ls5_rotation < 6; ++ls5_rotation)
        for (i = 0; i < 2; ++i)
        {
            ls5_seed(input[i]);
            ls5_expect_edges(output[i]);
            op_list_init(ls5_at(selected[i]));
            ls5_CHECK(memcmp(ls5_nodes, ls5_expected, sizeof(ls5_nodes)) == 0);
        }
    printf("list_init: %d checks, %d failures\n", ls5_checks, ls5_failures);
    return ls5_failures != 0;
}

#undef ls5_CHECK
