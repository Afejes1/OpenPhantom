#include "../src/linked_list.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ls2_checks, ls2_failures;
static void ls2_check(int ok, int line)
{
    ++ls2_checks;
    if (!ok)
    {
        ++ls2_failures;
        printf("line %d failed\n", line);
    }
}
#define ls2_CHECK(x) ls2_check(!!(x), __LINE__)
typedef struct ls2_OWNED
{
    unsigned int before;
    OP_LIST_NODE value;
    unsigned char payload[12];
    unsigned int after;
} ls2_OWNED;
static ls2_OWNED ls2_nodes[6], ls2_expected[6];
static int ls2_rotation;
static OP_LIST_NODE *ls2_at(int index)
{
    return index < 0 ? 0 : &ls2_nodes[(index + ls2_rotation) % 6].value;
}
static void ls2_seed(int edges[6][2])
{
    int i;
    memset(ls2_nodes, 0x69, sizeof(ls2_nodes));
    for (i = 0; i < 6; ++i)
    {
        ls2_at(i)->prev = ls2_at(edges[i][0]);
        ls2_at(i)->next = ls2_at(edges[i][1]);
    }
    memcpy(ls2_expected, ls2_nodes, sizeof(ls2_nodes));
}
static void ls2_expect_edges(int edges[6][2])
{
    int i;
    for (i = 0; i < 6; ++i)
    {
        ls2_expected[(i + ls2_rotation) % 6].value.prev = ls2_at(edges[i][0]);
        ls2_expected[(i + ls2_rotation) % 6].value.next = ls2_at(edges[i][1]);
    }
}
int ls2_main(void)
{
    int i;
    static int input[2][6][2] = {{{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                 {{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}};
    static int output[2][6][2] = {{{-1, 1}, {0, 2}, {1, 3}, {2, -1}, {-1, -1}, {-1, -1}},
                                  {{-1, 1}, {0, 2}, {1, 3}, {2, -1}, {-1, -1}, {-1, -1}}};
    static int selected[2] = {0, 2};
    for (ls2_rotation = 0; ls2_rotation < 6; ++ls2_rotation)
        for (i = 0; i < 2; ++i)
        {
            ls2_seed(input[i]);
            ls2_expect_edges(output[i]);
            op_list_append(ls2_at(selected[i]), ls2_at(3));
            ls2_CHECK(memcmp(ls2_nodes, ls2_expected, sizeof(ls2_nodes)) == 0);
        }
    printf("list_append: %d checks, %d failures\n", ls2_checks, ls2_failures);
    return ls2_failures != 0;
}

#undef ls2_CHECK
