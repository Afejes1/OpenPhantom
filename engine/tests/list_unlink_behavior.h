#include "../src/linked_list.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ls3_checks, ls3_failures;
static void ls3_check(int ok, int line)
{
    ++ls3_checks;
    if (!ok)
    {
        ++ls3_failures;
        printf("line %d failed\n", line);
    }
}
#define ls3_CHECK(x) ls3_check(!!(x), __LINE__)
typedef struct ls3_OWNED
{
    unsigned int before;
    OP_LIST_NODE value;
    unsigned char payload[12];
    unsigned int after;
} ls3_OWNED;
static ls3_OWNED ls3_nodes[6], ls3_expected[6];
static int ls3_rotation;
static OP_LIST_NODE *ls3_at(int index)
{
    return index < 0 ? 0 : &ls3_nodes[(index + ls3_rotation) % 6].value;
}
static void ls3_seed(int edges[6][2])
{
    int i;
    memset(ls3_nodes, 0x69, sizeof(ls3_nodes));
    for (i = 0; i < 6; ++i)
    {
        ls3_at(i)->prev = ls3_at(edges[i][0]);
        ls3_at(i)->next = ls3_at(edges[i][1]);
    }
    memcpy(ls3_expected, ls3_nodes, sizeof(ls3_nodes));
}
static void ls3_expect_edges(int edges[6][2])
{
    int i;
    for (i = 0; i < 6; ++i)
    {
        ls3_expected[(i + ls3_rotation) % 6].value.prev = ls3_at(edges[i][0]);
        ls3_expected[(i + ls3_rotation) % 6].value.next = ls3_at(edges[i][1]);
    }
}
int ls3_main(void)
{
    int i;
    static int input[4][6][2] = {{{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                 {{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                 {{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                 {{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}};
    static int output[4][6][2] = {{{-1, 2}, {-1, -1}, {0, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                  {{-1, -1}, {-1, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                  {{-1, 1}, {0, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                  {{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}};
    static int selected[4] = {1, 0, 2, 3};
    for (ls3_rotation = 0; ls3_rotation < 6; ++ls3_rotation)
        for (i = 0; i < 4; ++i)
        {
            ls3_seed(input[i]);
            ls3_expect_edges(output[i]);
            op_list_unlink(ls3_at(selected[i]));
            ls3_CHECK(memcmp(ls3_nodes, ls3_expected, sizeof(ls3_nodes)) == 0);
        }
    printf("list_unlink: %d checks, %d failures\n", ls3_checks, ls3_failures);
    return ls3_failures != 0;
}

#undef ls3_CHECK
