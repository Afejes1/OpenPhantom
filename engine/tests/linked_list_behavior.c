#include "list_add_after_behavior.h"
#include "list_insert_before_behavior.h"
#include "list_append_behavior.h"
#include "list_unlink_behavior.h"
#include "list_cut_before_behavior.h"
#include "list_init_behavior.h"
#include "list_count_behavior.h"
#include "list_advance_behavior.h"
#include "list_last_behavior.h"
/* Connected operations share actual reconstructed functions and owned nodes. */
static int lg_checks, lg_failures;
static void lg_check(int ok, int line)
{
    ++lg_checks;
    if (!ok)
    {
        ++lg_failures;
        printf("list chain failed at %d\n", line);
    }
}
#define LG_CHECK(x) lg_check(!!(x), __LINE__)
typedef struct LG_OWNED
{
    unsigned int before;
    OP_LIST_NODE value;
    unsigned char payload[12];
    unsigned int after;
} LG_OWNED;
static LG_OWNED lg_nodes[5], lg_expected[5];
static int lg_rotation;
static OP_LIST_NODE *lg_at(int n)
{
    return n < 0 ? 0 : &lg_nodes[(n + lg_rotation) % 5].value;
}
static void lg_predict(const int edges[5][2])
{
    int i;
    for (i = 0; i < 5; ++i)
    {
        lg_expected[(i + lg_rotation) % 5].value.prev = lg_at(edges[i][0]);
        lg_expected[(i + lg_rotation) % 5].value.next = lg_at(edges[i][1]);
    }
}
static void lg_connected(void)
{
    static const int edges[6][5][2] = {
        {{-1, 1}, {0, -1}, {-1, -1}, {-1, -1}, {-1, -1}}, {{-1, 2}, {2, -1}, {0, 1}, {-1, -1}, {-1, -1}},
        {{-1, 2}, {2, 3}, {0, 1}, {1, -1}, {-1, -1}},     {{-1, 1}, {0, 3}, {-1, -1}, {1, -1}, {-1, -1}},
        {{-1, -1}, {-1, 3}, {-1, -1}, {1, -1}, {-1, -1}}, {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}};
    static const int counts[6] = {2, 3, 4, 3, 1, 1};
    static const int tails[6] = {1, 1, 3, 3, 0, 0};
    static const int seconds[6] = {1, 2, 2, 1, -1, -1};
    int i, stage;
    for (lg_rotation = 0; lg_rotation < 5; ++lg_rotation)
    {
        memset(lg_nodes, 0x59, sizeof(lg_nodes));
        for (i = 0; i < 5; ++i)
        {
            lg_at(i)->prev = lg_at(4);
            lg_at(i)->next = lg_at(4);
        }
        memcpy(lg_expected, lg_nodes, sizeof(lg_nodes));
        for (i = 0; i < 5; ++i)
        {
            lg_expected[(i + lg_rotation) % 5].value.prev = 0;
            lg_expected[(i + lg_rotation) % 5].value.next = 0;
            op_list_init(lg_at(i));
            LG_CHECK(memcmp(lg_nodes, lg_expected, sizeof(lg_nodes)) == 0);
        }
        for (stage = 0; stage < 6; ++stage)
        {
            lg_predict(edges[stage]);
            switch (stage)
            {
            case 0:
                op_list_add_after(lg_at(0), lg_at(1));
                break;
            case 1:
                op_list_insert_before(lg_at(1), lg_at(2));
                break;
            case 2:
                op_list_append(lg_at(0), lg_at(3));
                break;
            case 3:
                op_list_unlink(lg_at(2));
                break;
            case 4:
                op_list_cut_before(lg_at(1));
                break;
            default:
                op_list_unlink(lg_at(3));
                break;
            }
            LG_CHECK(memcmp(lg_nodes, lg_expected, sizeof(lg_nodes)) == 0);
            LG_CHECK(op_list_count(lg_at(0)) == counts[stage]);
            LG_CHECK(op_list_last(lg_at(0)) == lg_at(tails[stage]));
            LG_CHECK(op_list_advance(lg_at(0), 1) == lg_at(seconds[stage]));
            LG_CHECK(op_list_advance(lg_at(0), INT_MIN) == lg_at(0));
            LG_CHECK(op_list_advance(lg_at(0), INT_MAX) == 0);
            LG_CHECK(memcmp(lg_nodes, lg_expected, sizeof(lg_nodes)) == 0);
            if (stage == 4)
            {
                LG_CHECK(op_list_count(lg_at(1)) == 2);
                LG_CHECK(lg_at(3)->prev == lg_at(1));
            }
        }
    }
}
static int op_test_linked_list(void)
{
    int status = 0, total;
    status += ls0_main();
    status += ls1_main();
    status += ls2_main();
    status += ls3_main();
    status += ls4_main();
    status += ls5_main();
    status += ls6_main();
    status += ls7_main();
    status += ls8_main();
    lg_connected();
    total = lg_checks + ls0_checks + ls1_checks + ls2_checks + ls3_checks + ls4_checks + ls5_checks + ls6_checks +
            ls7_checks + ls8_checks;
    printf("linked list connected: %d checks, %d integration failures\n", total, lg_failures);
    return status + (lg_failures != 0);
}
#undef LG_CHECK
