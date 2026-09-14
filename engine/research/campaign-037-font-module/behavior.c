#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
OP_FONT_POOL op_font_pool;
OP_FONT3D *op_current_font;
int op_system_font_slot;
char op_system_font_name[] = "authored-default";
float op_font_screen_width, op_font_screen_height;
static OP_FONT_POOL expected;
static unsigned int tokens[32];
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
static void seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &tokens[i];
        op_font_pool.rows[i].sprite = &tokens[i + 1];
        op_font_pool.rows[i].backdrop = &tokens[i + 2];
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&expected, &op_font_pool, sizeof(expected));
}
static int stage, answer, wanted_slot;
int op_font_create(char *name)
{
    CHECK(stage == 0 && name == op_system_font_name);
    CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
    CHECK(op_system_font_slot == wanted_slot);
    op_font_pool.rows[5].word20 = expected.rows[5].word20 = 0x12345678;
    op_current_font = &op_font_pool.rows[5];
    op_system_font_slot = 44;
    stage = 1;
    return answer;
}
void op_font_destroy(int slot)
{
    CHECK(stage == 0 && slot == wanted_slot);
    CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
    op_font_pool.rows[7].word24 = expected.rows[7].word24 = 0x87654321;
    op_current_font = &op_font_pool.rows[7];
    op_system_font_slot = 99;
    stage = 2;
}
int main(void)
{
    int e, i, a, result;
    static int events[] = {INT_MIN, -1, 0, 2, 3, 4, 5, INT_MAX};
    static int slots[] = {INT_MIN, -1, 0, 1, 15, 16, INT_MAX};
    static int answers[] = {-1, 0, 15};
    for (e = 0; e < 8; ++e)
        for (i = 0; i < 7; ++i)
            for (a = 0; a < 3; ++a)
            {
                seed(3);
                stage = 0;
                wanted_slot = slots[i];
                op_system_font_slot = slots[i];
                answer = answers[a];
                if (events[e] == 3)
                    memset(&expected, 0, sizeof(expected));
                result = op_font_module(events[e]);
                CHECK(result == ((events[e] == 3 || events[e] == 4) ? 0 : 2));
                CHECK(op_system_font_slot ==
                      (events[e] == 4 ? -1 : (events[e] == 3 && slots[i] < 0 ? answer : slots[i])));
                CHECK(stage == (events[e] == 4 ? 2 : (events[e] == 3 && slots[i] < 0 ? 1 : 0)));
                CHECK(op_current_font == &op_font_pool.rows[stage == 1 ? 5 : (stage == 2 ? 7 : 3)]);
                CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
            }
    printf("font_module: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
