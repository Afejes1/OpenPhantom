#include "../src/linked_list.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ls0_checks, ls0_failures;
static void ls0_check(int ok, int line)
{
    ++ls0_checks;
    if (!ok)
    {
        ++ls0_failures;
        printf("line %d failed\n", line);
    }
}
#define ls0_CHECK(x) ls0_check(!!(x), __LINE__)
typedef struct ls0_OWNED
{
    unsigned int before;
    OP_LIST_NODE value;
    unsigned char payload[12];
    unsigned int after;
} ls0_OWNED;
static ls0_OWNED ls0_nodes[6], ls0_expected[6];
static int ls0_rotation;
static OP_LIST_NODE *ls0_at(int index)
{
    return index < 0 ? 0 : &ls0_nodes[(index + ls0_rotation) % 6].value;
}
static void ls0_seed(int edges[6][2])
{
    int i;
    memset(ls0_nodes, 0x69, sizeof(ls0_nodes));
    for (i = 0; i < 6; ++i)
    {
        ls0_at(i)->prev = ls0_at(edges[i][0]);
        ls0_at(i)->next = ls0_at(edges[i][1]);
    }
    memcpy(ls0_expected, ls0_nodes, sizeof(ls0_nodes));
}
static void ls0_expect_edges(int edges[6][2])
{
    int i;
    for (i = 0; i < 6; ++i)
    {
        ls0_expected[(i + ls0_rotation) % 6].value.prev = ls0_at(edges[i][0]);
        ls0_expected[(i + ls0_rotation) % 6].value.next = ls0_at(edges[i][1]);
    }
}
int ls0_main(void)
{
    int i;
    static int input[2][6][2] = {{{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                 {{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}};
    static int output[2][6][2] = {{{-1, 1}, {0, 3}, {3, -1}, {1, 2}, {-1, -1}, {-1, -1}},
                                  {{-1, 1}, {0, 2}, {1, 3}, {2, -1}, {-1, -1}, {-1, -1}}};
    static int selected[2] = {1, 2};
    for (ls0_rotation = 0; ls0_rotation < 6; ++ls0_rotation)
        for (i = 0; i < 2; ++i)
        {
            ls0_seed(input[i]);
            ls0_expect_edges(output[i]);
            op_list_add_after(ls0_at(selected[i]), ls0_at(3));
            ls0_CHECK(memcmp(ls0_nodes, ls0_expected, sizeof(ls0_nodes)) == 0);
        }
    printf("list_add_after: %d checks, %d failures\n", ls0_checks, ls0_failures);
    return ls0_failures != 0;
}

#undef ls0_CHECK
